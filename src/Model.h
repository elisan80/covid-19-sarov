#pragma once

#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <random>
#include <Windows.h>

// перечесление типов состояний агентов
enum SEIR_State
{
    Susceptible = 0,    // Восприимчивый
    Exposed,			// Контактный
    Infectious,			// Инфицированный
    Recovered,			// Выздоровевший
    Dead				// Умерший
};

class Person;
class Location;

// используемые параметры
const std::string gnuPlotPath = "gnuPlotPath";                              // путь к исполняемому файлу gnuplot
const std::string workersPercentage = "workersPercentage";                  // процент работающих [0-100]
const std::string ECount_0day = "ECount_0day";                              // количество контактных на первом шаге
const std::string ICount_0day = "ICount_0day";                              // количество инфицированных на первом шаге
const std::string RCount_0day = "RCount_0day";                              // количество выздоровевших на первом шаге
const std::string DCount_0day = "DCount_0day";                              // количество умерших на первом шаге
const std::string EtoSTransferProbability = "EtoSTransferProbability";      // вероятность восприимчивому заразиться при контакте с контактным [0-1]
const std::string ItoSTransferProbability = "ItoSTransferProbability";      // вероятность восприимчивому заразиться при контакте с инфицированным [0-1]
const std::string RtoSTransferProbability = "RtoSTransferProbability";      // вероятность восприимчивому заразиться при контакте с выздоровевшим [0-1]
const std::string ItoRProbabilityWithMedic = "ItoRProbabilityWithMedic";    // вероятность выздоровления при больничном лечении [0-1]
const std::string ItoDProbabilityWithMedic = "ItoDProbabilityWithMedic";    // вероятность смерти при больничном лечении [0-1]
const std::string EtoIDuration = "EtoIDuration";                            // средняя длительность инкубационного периода в днях [0-∞)
const std::string EtoSymptomsDuration = "EtoSymptomsDuration";              // среднее время до проявления симптомов в днях [0-∞)
const std::string DefaultScale = "DefaultScale";                            // коэффициент, уменьшающий общее влияние
const std::string HomeScale = "HomeScale";                                  // коэффициент, уменьшающий влияние домов
const std::string ShopScale = "ShopScale";                                  // коэффициент, уменьшающий влияние магазинов

// не используемые параметры
const std::string RtoSDuration = "RtoSDuration";                            // длительность периода заразности выздоровевшим [0-∞)
const std::string ItoRProbability = "ItoRProbability";                      // вероятность выздоровления без больничного лечения [0-1]
const std::string ItoDProbability = "ItoDProbability";                      // вероятность смерти без больничного лечения [0-1]
const std::string HardSymptomsProbability = "HardSymptomsProbability";      // вероятность возникновения тяжелой симптоматики [0-1]

// Класс модель
// Содержит список всех агентов, список всех локаций
// Управляет моделированием
class Model
{
public:
    ~Model();

    // функция получения ссылки на инстанс
    static Model &instance();

    void init();

    // Начать моделирование следующего дня
    void startNewDay();

    // Вывести в выходной файл статистику текущего дня моделирования
    void writeOutput();

    // Вывести в выходной файл
    void writeGraphvizFile();

    // Вывести файл с состоянием агентов, по умолчанию выводятся состояния после моделирования в файл output_states.txt
    // при указании номера дня >=0 выводятся состояние текущего дня моделирования в файл output_states_<dayNumber>.txt
    void writeStateFile(int dayNumber = -1);

    //Установить новую директорию проекта
    void SetProjectDir(const std::string& new_path);

    // Установить процент работающих
    void applyModelParams();

    //
    void openGnuPlot(bool isAutoUpdate);

    //
    void closeGnuPlot();

    // Очистить после моделирования
    void clear();

public:
    bool isModellingStopped;                // признак остановки моделирования
    int m_currentDay;                       // номер текущего дня
    int personCount;                        // количество агентов в моделировании
    std::vector<Person *> allPersons;       // вектор всех агентов (позиция в векторе = идентификатору агента, могут быть пустые записи)
    std::vector<Location *> allLocations;   // вектор всех локаций (позиция в векторе = идентификатору локации, могут быть пустые записи)

    std::list<Person *> susceptiblePersons; // список восприимчивых на текущий день моделирования
    //std::list<Person *> exposedPersons;   // список контактных на текущий день моделирования
    int exposedPersonsCount;                // количество контактных на текущий день моделирования
    //std::list<Person *> infectedPersons;  // список инфицированных на текущий день моделирования
    int infectedPersonsCount;               // количество инфицированных на текущий день моделирования
    //std::list<Person *> recoveredPersons; // список выздоровевших на текущий день моделирования
    int recoveredPersonsCount;              // количество выздоровевших на текущий день моделирования
    //std::list<Person *> deadPersons;      // список умерших на текущий день моделирования
    int deadPersonsCount;                   // количество умерших на текущий день моделирования

    std::string m_gnuPlotPath;              // путь к исполняемому файлу gnuplot

    std::map<std::string, double> params;

    std::mt19937 *generator;
    std::uniform_real_distribution<> *uniformDistribution;

private:
    Model();
    Model(const Model&);

private:
    Model *m_instance;              // Указатель на инстанс, для реализации синглтона
    std::ofstream *m_outputStream;  // Указатель на поток файла выходных данных
    std::string data_dir;           // Директория проекта

    PROCESS_INFORMATION *m_gnuPlotProcessInformation;
};

bool eventWithProbability(double probability);
