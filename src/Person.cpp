#include "Person.h"

#include "Location.h"
#include "Model.h"

Person::Person() :
    m_state(Susceptible),
    m_timeExposed(-1.0),
    m_exposedSource(nullptr),
    m_timeSymptoms(-1.0),
    m_timeInfected(-1.0),
    m_timeRecovered(-1.0),
    m_timeDead(-1.0),
    m_isOnQuarantine(false)
{
    Model &model = Model::instance();
    ++model.personNumber;
}

void Person::generateShedule()
{
}

void Person::generateDayOffShedule()
{
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
        double myItoRProbability = ItoRProbabilityWithMedic;
        double myItoDProbability = ItoDProbabilityWithMedic;
        double randValueItoR = (std::rand() % 100) / 100.0;
        double randValueItoD = (std::rand() % 100) / 100.0;

        if (randValueItoR < myItoRProbability) // с вероятностью myItoRProbability выздоровел
            setRecovered(model.m_currentDay + (std::rand() % 86400));
        else if (randValueItoD < myItoDProbability) // с вероятностью myItoDProbability умер
            setDead(model.m_currentDay + (std::rand() % 86400));
    }
}

// изменить состояние на "Контактный"
void Person::setExposed(Person *source, double time)
{
    m_state = Exposed;
    m_timeExposed = time;
    m_exposedSource = source;

    double myEtoIDuration = EtoIDuration; // todo add random
    m_timeInfected = time + myEtoIDuration;

    double myEtoSymptomsDuration = EtoSymptomsDuration; // todo add random
    m_timeSymptoms = time + myEtoSymptomsDuration;

    Model &model = Model::instance();
    model.exposedPersons.push_back(this);
}

// изменить состояние на "Инфицированный"
void Person::setInfectious()
{
    m_state = Infectious;

    Model &model = Model::instance();
    model.infectedPersons.push_back(this);
    model.exposedPersons.remove(this);
}

// изменить состояние на "Выздоровевший"
void Person::setRecovered(double time)
{
    m_state = Recovered;
    m_timeRecovered = time;

    Model &model = Model::instance();
    model.recoveredPersons.push_back(this);
    model.infectedPersons.remove(this);
}

// изменить состояние на "Умерший"
void Person::setDead(double time)
{
    m_state = Dead;
    m_timeDead = time;

    Model &model = Model::instance();
    model.deadPersons.push_back(this);
    model.infectedPersons.remove(this);
}

void Person::setQuarantine()
{
    m_isOnQuarantine = true;
}
