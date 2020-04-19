#include "Person.h"

#include "Location.h"
#include "Model.h"
#include "data_types.h"

Person::Person() :
    index(-1),
    m_state(Susceptible),
    m_home(nullptr),
    m_work(nullptr),
    m_timeExposed(-1.0),
    m_exposedSource(nullptr),
    m_timeSymptoms(-1.0),
    m_timeInfected(-1.0),
    m_timeRecovered(-1.0),
    m_timeDead(-1.0),
    m_isOnQuarantine(false),
    workingDayConfig(nullptr),
    dayOffConfig(nullptr)
{
    Model &model = Model::instance();
    //model.susceptiblePersons.push_back(this);
    ++model.personCount;
}

Person::~Person()
{
    if (workingDayConfig)
    {
        for (std::list<LocationConfig *>::iterator iter = workingDayConfig->locations.begin(); iter != workingDayConfig->locations.end(); ++iter)
        {
            delete *iter;
        }
        delete workingDayConfig;
        workingDayConfig = nullptr;
    }

    if (dayOffConfig)
    {
        for (std::list<LocationConfig *>::iterator iter = dayOffConfig->locations.begin(); iter != dayOffConfig->locations.end(); ++iter)
        {
            delete *iter;
        }
        delete dayOffConfig;
        dayOffConfig = nullptr;
    }
}

void Person::generateShedule(bool isWorkingDay)
{
    if (!dayOffConfig && !workingDayConfig)
        return;

    Model &model = Model::instance();
    HumanConfig *currentConfig;
    if (!isWorkingDay || !workingDayConfig)
        currentConfig = dayOffConfig;
    else
        currentConfig = workingDayConfig;

    m_shedule.addLocation(m_home, 0, 86400);
    for (std::list<LocationConfig *>::iterator iter = currentConfig->locations.begin(); iter != currentConfig->locations.end(); ++iter)
    {
        if (eventWithProbability((*iter)->probability))
        {
            // идем сегодня в эту локацию
            int duration = (*iter)->min_time;
            int durationDelta = ((*iter)->max_time - (*iter)->min_time);
            if (durationDelta > 0)
                duration += (std::rand() % durationDelta);
            int startTime = (*iter)->start_time;
            int startTimeDelta = (((*iter)->end_time - (*iter)->start_time) - duration);
            if (startTimeDelta > 0)
                startTime += (std::rand() % startTimeDelta);
            m_shedule.addLocation((*iter)->location, startTime, startTime + duration);
        }
    }
}

// Сообщить локации о своем присутствии сегодня
void Person::notifyLocations()
{
    for (size_t i = 0; i < m_shedule.m_sectionList.size(); ++i)
    {
        PersonSheduleSection currentSection = m_shedule.m_sectionList.at(i);
        currentSection.m_location->m_locationShedule.addPerson(this, currentSection.m_timeStartInSeconds, currentSection.m_timeEndInSeconds);
    }
}

void Person::checkState()
{
    Model &model = Model::instance();
    if (m_state == Exposed && model.m_currentDay > m_timeInfected)
    {
        setInfectious();
    }
    if (m_state == Infectious && model.m_currentDay > m_timeSymptoms)
    {
        setQuarantine();
    }

    if (m_isOnQuarantine && m_state == Infectious) // на карантине
    {
        double myItoRProbability = model.params[ItoRProbabilityWithMedic];
        double myItoDProbability = model.params[ItoDProbabilityWithMedic];

        if (eventWithProbability(myItoRProbability)) // с вероятностью myItoRProbability выздоровел
            setRecovered(model.m_currentDay + (std::rand() % 86400));
        else if (eventWithProbability(myItoDProbability)) // с вероятностью myItoDProbability умер
            setDead(model.m_currentDay + (std::rand() % 86400));
    }
}

// изменить состояние на "Контактный"
void Person::setExposed(Person *source, double time)
{
    m_state = Exposed;

    Model &model = Model::instance();
    m_EtoIDuration = model.params[EtoIDuration]; // todo add random

    if (time >= 0.0) // зараженный во время моделирования
    {
        m_timeExposed = time;
        m_exposedSource = source;

        m_timeInfected = time + m_EtoIDuration;
    }
    else // зараженный до моделирования
    {
        m_timeExposed = -m_EtoIDuration;
        m_exposedSource = source;

        m_timeInfected = 0.0;
    }

    ++model.exposedPersonsCount;
    //model.exposedPersons.push_back(this);
    //model.susceptiblePersons.remove(this);
}

// изменить состояние на "Инфицированный"
void Person::setInfectious()
{
    m_state = Infectious;

    Model &model = Model::instance();
    m_EtoSymptomsDuration = model.params[EtoSymptomsDuration]; // todo add random
    m_timeSymptoms = m_timeExposed + m_EtoSymptomsDuration;

    ++model.infectedPersonsCount;
    --model.exposedPersonsCount;
    //model.infectedPersons.push_back(this);
    //model.exposedPersons.remove(this);
}

// изменить состояние на "Выздоровевший"
void Person::setRecovered(double time)
{
    m_state = Recovered;
    m_timeRecovered = time;

    Model &model = Model::instance();
    ++model.recoveredPersonsCount;
    --model.infectedPersonsCount;

    //model.recoveredPersons.push_back(this);
    //model.infectedPersons.remove(this);
}

// изменить состояние на "Умерший"
void Person::setDead(double time)
{
    m_state = Dead;
    m_timeDead = time;

    Model &model = Model::instance();
    ++model.deadPersonsCount;
    --model.infectedPersonsCount;

    //model.deadPersons.push_back(this);
    //model.infectedPersons.remove(this);
}

void Person::setQuarantine()
{
    m_isOnQuarantine = true;
    
    if (workingDayConfig)
    {
        for (std::list<LocationConfig *>::iterator iter = workingDayConfig->locations.begin(); iter != workingDayConfig->locations.end(); ++iter)
        {
            delete *iter;
        }
        delete workingDayConfig;
        workingDayConfig = nullptr;
    }

    if (dayOffConfig)
    {
        for (std::list<LocationConfig *>::iterator iter = dayOffConfig->locations.begin(); iter != dayOffConfig->locations.end(); ++iter)
        {
            delete *iter;
        }
        delete dayOffConfig;
        dayOffConfig = nullptr;
    }
}
