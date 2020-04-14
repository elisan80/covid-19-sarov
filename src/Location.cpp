#include "Location.h"

#include "Person.h"
#include "Model.h"

Location::Location() :
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

void Location::seirModelling()
{
    for (std::list<LocationSheduleSection>::iterator iter = m_locationShedule.m_sectionList.begin(); iter != m_locationShedule.m_sectionList.end(); ++iter)
    {
        double interactionTime = iter->m_timeEnd - iter->m_timeStart;

        for (std::list<Person *>::iterator iterPerson = iter->m_personList.begin(); iterPerson != iter->m_personList.end(); ++iterPerson)
        {
            if ((*iterPerson)->m_state == Infectious || (*iterPerson)->m_state == Recovered)
            {
                for (std::list<Person *>::iterator iterTarget = iter->m_personList.begin(); iterTarget != iter->m_personList.end(); ++iterTarget)
                {
                    if ((*iterPerson)->m_state == Susceptible)
                    {
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
        target->setExposed(source, model.m_currentDay, (timeStartInSeconds + timeEndInSeconds)/2);
}

Home::Home()
{
    m_beta = EtoSProbability * 2.0; // Увеличенная вероятность заражения дома
}

void Group::meeting()
{
	// по вероятности планерки проверяем, будет ли сегодня планерка
	// если да - всем работникам добавляем локацию
}
