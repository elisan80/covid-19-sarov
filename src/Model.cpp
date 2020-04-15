#include "Model.h"

#include "Person.h"
#include "Location.h"
#include <fstream>

Model::Model() :
    isModellingStopped(false),
    m_currentDay(0),
    personNumber(0)
{
    m_outputStream = new std::ofstream("output.txt", std::ofstream::trunc);
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

void Model::startNewDay()
{
    if (exposedPersons.size() == 0 && infectedPersons.size() == 0) // заканчиваем моделирование, если нет контактных и инфицированных
        isModellingStopped = true;
    if (m_currentDay > 180) // заканчиваем моделирование черз полгода, возможно зациклились
        isModellingStopped = true;

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
}

void Model::writeOutput()
{
    if (m_outputStream->is_open())
    {
        *m_outputStream
            << m_currentDay << " "
            << personNumber - exposedPersons.size() - infectedPersons.size() - recoveredPersons.size() - deadPersons.size() << " "
            << exposedPersons.size() << " "
            << infectedPersons.size() << " "
            << recoveredPersons.size() << " "
            << deadPersons.size() << std::endl;
        m_outputStream->flush();
    }
}


// Вывести в выходной файл статистику текущего дня моделирования
void Model::writeGraphvizFile()
{
    std::ofstream outputStream("graph.txt", std::ofstream::trunc);
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

bool eventWithProbability(double probability)
{
    double randValue = (std::rand() % 1000) / 1000.0;
    return randValue < probability;
}
