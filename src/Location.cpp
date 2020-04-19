#include "Location.h"

#include "Person.h"
#include "Model.h"

Location::Location() :
    index(-1)
{
    m_scale = Model::instance().params[DefaultScale];
}

Location* Location::createLocationByIndex(int index)
{
    if (index >= 130050 && index <= 130100)
        return new Shop();
    
    return new Location();
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

    Model &model = Model::instance();
    for (std::list<LocationSheduleSection>::iterator iter = m_locationShedule.m_sectionList.begin(); iter != m_locationShedule.m_sectionList.end(); ++iter)
    {
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
                        if (eventWithProbability((iter->m_timeEnd - iter->m_timeStart) * m_scale / 3600.0))
                            modelContact(*iterPerson, *iterTarget, iter->m_timeStart, iter->m_timeEnd);
                    }
                }
            }
        }
    }
}

void Location::modelContact(Person *source, Person *target, double timeStartInSeconds, double timeEndInSeconds)
{
    Model &model = Model::instance();

    double probability = 0.0;
    switch (source->m_state)
    {
    case Exposed:
        probability = model.params[EtoSTransferProbability];
        break;
    case Infectious:
        probability = model.params[ItoSTransferProbability];
        break;
    case Recovered:
        probability = model.params[RtoSTransferProbability];
        break;
    default:
        break;
    }

    if (eventWithProbability(probability))
        target->setExposed(source, model.m_currentDay + ((timeStartInSeconds + timeEndInSeconds)/2.0)/86400.0);
}

Home::Home()
{
    m_scale = Model::instance().params[HomeScale];
}

Shop::Shop()
{
    m_scale = Model::instance().params[ShopScale];
}