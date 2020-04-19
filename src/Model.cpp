#include "Model.h"

#include <fstream>
#include <string>
#include <algorithm>
#include <set>

#include "Person.h"
#include "Location.h"
#include "data_types.h"

Model::Model() :
    isModellingStopped(false),
    m_currentDay(0),
    personCount(0),
    allPersons(),
    allLocations(),
    exposedPersonsCount(0),
    infectedPersonsCount(0),
    recoveredPersonsCount(0),
    deadPersonsCount(0),
    m_gnuPlotPath(),
    m_gnuPlotProcessInformation(nullptr)
{
    data_dir = "config/";
    m_outputStream = nullptr;
    SetProjectDir(data_dir);
    init();
}

Model::~Model()
{
    m_outputStream->flush();
    m_outputStream->close();
    delete m_outputStream;
}

Model& Model::instance()
{
    static Model instance;

    return instance;
}

void Model::init()
{
    params[workersPercentage] = 100.0;
    params[ECount_0day] = 0.0;
    params[ICount_0day] = 1.0;
    params[RCount_0day] = 0.0;
    params[DCount_0day] = 0.0;
    params[EtoSTransferProbability] = 0.02;
    params[ItoSTransferProbability] = 0.35;
    params[RtoSTransferProbability] = 0.05;
    params[ItoRProbabilityWithMedic] = 0.20;
    params[ItoDProbabilityWithMedic] = 0.005;
    params[EtoIDuration] = 3.0;
    params[EtoSymptomsDuration] = 5.1;
    params[DefaultScale] = 0.5;
    params[ShopScale] = 0.5;
    params[HomeScale] = 0.05;

    std::random_device rd;
    generator = new std::mt19937(rd());
    uniformDistribution = new std::uniform_real_distribution<>(0.0, 1.0);
}

void Model::SetProjectDir(const std::string& new_path)
{
    data_dir = new_path;
    if (m_outputStream != nullptr)
    {
        m_outputStream->close();
        delete m_outputStream;
    }
    m_outputStream = new std::ofstream(data_dir + "output.txt", std::ofstream::trunc);
}

void Model::startNewDay()
{
    ++m_currentDay;
    bool isWorkingDay = m_currentDay % 7 < 5; // рабочий ли день

    for (std::vector<Location *>::iterator iter = allLocations.begin(); iter != allLocations.end(); ++iter)
    {
        if (!*iter)
            continue;

        // чистим расписание локаций
        (*iter)->m_locationShedule.clear();
    }

    for (std::vector<Person *>::iterator iter = allPersons.begin(); iter != allPersons.end(); ++iter)
    {
        if (!*iter)
            continue;

        // чистим расписание агентов
        (*iter)->m_shedule.clear();

        // проверяем состояние агентов
        (*iter)->checkState();

        // генерируем новое расписание
        (*iter)->generateShedule(isWorkingDay);

        // отправляем сгенерированное расписание в локации
        (*iter)->notifyLocations();
    }

    Model &model = Model::instance();
    for (std::vector<Location *>::iterator iter = allLocations.begin(); iter != allLocations.end(); ++iter)
    {
        if (!*iter)
            continue;

        // генерируем расписание локации из кусочков, полученных от агентов
        (*iter)->m_locationShedule.generate();
        (*iter)->seirModelling();
    }

    if (exposedPersonsCount == 0 && infectedPersonsCount == 0) // заканчиваем моделирование, если нет контактных и инфицированных
    {
        isModellingStopped = true;
        return;
    }

    if (m_currentDay > 360) // заканчиваем моделирование черз полгода, возможно зациклились
    {
        isModellingStopped = true;
        return;
    }
}

void Model::writeOutput()
{
    if (m_outputStream->is_open())
    {
        *m_outputStream
            << m_currentDay << " "
            << personCount - exposedPersonsCount - infectedPersonsCount - recoveredPersonsCount - deadPersonsCount << " "
            << exposedPersonsCount << " "
            << infectedPersonsCount << " "
            << recoveredPersonsCount << " "
            << deadPersonsCount << std::endl;
        m_outputStream->flush();
    }
}

// Вывести в выходной файл статистику текущего дня моделирования
void Model::writeGraphvizFile()
{
    std::ofstream outputStream(data_dir + "graph.txt", std::ofstream::trunc);
    outputStream << "digraph JOB_HIERARHY {" << std::endl;

    for (std::vector<Person *>::iterator iter = allPersons.begin(); iter != allPersons.end(); ++iter)
    {
        if (*iter)
            outputStream << (*iter)->index << "[label=\"" << (*iter)->index << "\"]" << std::endl;
    }

    for (std::vector<Person *>::iterator iter = allPersons.begin(); iter != allPersons.end(); ++iter)
    {
        if (*iter && (*iter)->m_exposedSource && (*iter)->index != (*iter)->m_exposedSource->index)
            outputStream << (*iter)->m_exposedSource->index << "->" << (*iter)->index << ";" << std::endl;
    }
    outputStream << "}" << std::endl;

    outputStream.flush();
    outputStream.close();
}

void Model::writeStateFile(int dayNumber)
{
    std::string fileName = "states/output_states.txt";
    if (dayNumber >= 0)
    {
        fileName = "output_states_" + std::to_string(dayNumber) + ".txt";
    }
    std::ofstream outputStream(data_dir + "output_states.txt", std::ofstream::trunc);

    for (std::vector<Person *>::iterator iter = allPersons.begin(); iter != allPersons.end(); ++iter)
    {
        if (*iter)
            outputStream << (*iter)->m_state << std::endl;
    }

    outputStream.flush();
    outputStream.close();
}

void Model::applyModelParams()
{
    // процент работающих
    int dayOffCount = 0;
    for (std::vector<Person *>::iterator iter = allPersons.begin(); iter != allPersons.end(); ++iter)
    {
        if ((*iter) && (*iter)->m_work && (*iter)->m_work->index > 50000)
        {
            if (eventWithProbability(1.0 - params[workersPercentage] / 100.0))
            {
                for (std::list<LocationConfig *>::iterator iterConfig = (*iter)->workingDayConfig->locations.begin(); iterConfig != (*iter)->workingDayConfig->locations.end(); ++iterConfig)
                {
                    delete *iterConfig;
                }
                delete (*iter)->workingDayConfig;
                (*iter)->workingDayConfig = nullptr;
                (*iter)->workingDayConfig = nullptr;
                ++dayOffCount;
            }
        }
    }
    std::cout << "Day off count : " << dayOffCount << std::endl;


    std::vector<int> indexes;
    for (int i = 0; i < personCount; ++i)
        indexes.push_back(i);

    std::random_shuffle(indexes.begin(), indexes.end());

    std::vector<int>::iterator indexesIter = indexes.begin();
    Person *currentPerson = nullptr;

    // количество контактных на 0 день
    int eCount = params[ECount_0day];
    for (int i = 0; i < eCount; ++i)
    {
        currentPerson = allPersons[*indexesIter];
        currentPerson->setExposed(currentPerson, 0.0);

        ++indexesIter;
    }

    // количество инфицированных на 0 день
    int iCount = params[ICount_0day];
    for (int i = 0; i < iCount; ++i)
    {
        currentPerson = allPersons[*indexesIter];

        currentPerson->setExposed(currentPerson, -1.0);
        currentPerson->setInfectious();

        ++indexesIter;
    }

    // количество выздоровевших на 0 день
    int rCount = params[RCount_0day];
    for (int i = 0; i < params[RCount_0day]; ++i)
    {
        currentPerson = allPersons[*indexesIter];

        currentPerson->setExposed(currentPerson, -1.0);
        currentPerson->setInfectious();
        currentPerson->setRecovered(0.0);

        ++indexesIter;
    }

    // количество умерших на 0 день
    int dCount = params[ECount_0day];
    for (int i = 0; i < params[DCount_0day]; ++i)
    {
        currentPerson = allPersons[*indexesIter];

        currentPerson->setExposed(currentPerson, -1.0);
        currentPerson->setInfectious();
        currentPerson->setDead(0.0);

        ++indexesIter;
    }
}

void Model::openGnuPlot(bool isAutoUpdate)
{
    closeGnuPlot();
    m_gnuPlotProcessInformation = new PROCESS_INFORMATION();

    if (!m_gnuPlotPath.empty())
    {
        std::string gnuWorkPath = data_dir;
        std::replace(gnuWorkPath.begin(), gnuWorkPath.end(), '\\', '/');
        std::string gpFileName = "Sarov.gp";
        if (isAutoUpdate)
            gpFileName = "Sarov_autoupdate.gp";

        LPSTR commandLPSTR = const_cast<char *>(m_gnuPlotPath.c_str());
        std::string arguments = " -p -e \"cd '" + gnuWorkPath + "'; load '" + gpFileName + "';\"";

        LPSTR argumentsLPSTR = const_cast<char *>(arguments.c_str());
        STARTUPINFO info = { sizeof(info) };
        CreateProcess(commandLPSTR, argumentsLPSTR, NULL, NULL, TRUE, 0, NULL, NULL, &info, m_gnuPlotProcessInformation);
    }
}

void Model::closeGnuPlot()
{
    if (m_gnuPlotProcessInformation)
    {
        TerminateProcess(m_gnuPlotProcessInformation->hProcess, 0);
        CloseHandle(m_gnuPlotProcessInformation->hProcess);
        CloseHandle(m_gnuPlotProcessInformation->hThread);

        delete m_gnuPlotProcessInformation;
        m_gnuPlotProcessInformation = nullptr;
    }
}

void Model::clear()
{
    for (int i = 0; i < allPersons.size(); ++i)
    {
        delete allPersons[i];
    }
    allPersons.clear();

    for (int i = 0; i < allLocations.size(); ++i)
    {
        delete allLocations[i];
    }
    allLocations.clear();

    closeGnuPlot();
}

bool eventWithProbability(double probability)
{
    if (probability >= 1.0)
        return true;
    else if (probability <= 0)
        return false;
    Model &model = Model::instance();
    return (*model.uniformDistribution)(*model.generator) < probability;
}
