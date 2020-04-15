#include "Location.h"

#include "Person.h"
#include "Model.h"

Location::Location() :
    index(-1),
    m_beta(EtoSProbability),
    m_gamma(1.0 / (EtoSymptomsDuration - EtoIDuration)),
    m_alpha(ItoRProbability)
{
}

bool Location::hasEIRPerson()
{
    for (std::list<LocationSheduleSection>::iterator iter = m_locationShedule.m_sectionList.begin(); iter != m_locationShedule.m_sectionList.end(); ++iter)
    {
        for (std::list<Person *>::iterator iterPerson = iter->m_personList.begin(); iterPerson != iter->m_personList.end(); ++iterPerson)
        {
            if ((*iterPerson)->m_state != SEIR_State::Susceptible)
                return true;
        }
    }
    return false;
}

bool Location::hasSPerson()
{
    for (std::list<LocationSheduleSection>::iterator iter = m_locationShedule.m_sectionList.begin(); iter != m_locationShedule.m_sectionList.end(); ++iter)
    {
        for (std::list<Person *>::iterator iterPerson = iter->m_personList.begin(); iterPerson != iter->m_personList.end(); ++iterPerson)
        {
            if ((*iterPerson)->m_state == SEIR_State::Susceptible)
                return true;
        }
    }
    return false;
}

void Location::seirModelling()
{
    if (!hasEIRPerson() || !hasSPerson())
        return;

    for (std::list<LocationSheduleSection>::iterator iter = m_locationShedule.m_sectionList.begin(); iter != m_locationShedule.m_sectionList.end(); ++iter)
    {
        Model &model = Model::instance();

        double interactionTime = iter->m_timeEnd - iter->m_timeStart;

        for (std::list<Person *>::iterator iterPerson = iter->m_personList.begin(); iterPerson != iter->m_personList.end(); ++iterPerson)
        {
            if (((*iterPerson)->m_state == Exposed && ((*iterPerson)->m_timeExposed < model.m_currentDay)) ||
                ((*iterPerson)->m_state == Infectious && ((*iterPerson)->m_timeInfected < model.m_currentDay)) ||
                ((*iterPerson)->m_state == Recovered && ((*iterPerson)->m_timeRecovered < model.m_currentDay)))
            {
                for (std::list<Person *>::iterator iterTarget = iter->m_personList.begin(); iterTarget != iter->m_personList.end(); ++iterTarget)
                {
                    if ((*iterTarget)->m_state == Susceptible)
                    {
                        if (eventWithProbability((iter->m_timeEnd - iter->m_timeStart) / 14400.0))
                            modelContact(*iterPerson, *iterTarget, iter->m_timeStart, iter->m_timeEnd);
                    }
                }
            }
        }
    }
}

void Location::modelContact(Person *source, Person *target, double timeStartInSeconds, double timeEndInSeconds)
{
    double probability = 0.0;
    switch (source->m_state)
    {
    case Exposed:
        probability = EtoSProbability;
        break;
    case Infectious:
        probability = ItoSProbability;
        break;
    case Recovered:
        probability = RtoSProbability;
        break;
    default:
        break;
    }

    Model &model = Model::instance();
    double randValue = (std::rand() % 100) / 100.0;
    if (randValue < probability)
        target->setExposed(source, model.m_currentDay + ((timeStartInSeconds + timeEndInSeconds)/2.0)/86400.0);
}

Home::Home()
{
    m_beta = EtoSProbability * 2.0; // Увеличенная вероятность заражения дома
}
