#include "PersonShedule.h"

#include "Location.h"

// добавить локацию в расписание
void PersonShedule::addLocation(Location *location, double timeStartInSeconds, double timeEndInSeconds)
{
    if (m_sectionList.empty())
    {
        m_sectionList.push_back(PersonSheduleSection(location, timeStartInSeconds, timeEndInSeconds));
    }
    else
    {
        for (std::vector<PersonSheduleSection>::iterator iter = m_sectionList.begin(); iter != m_sectionList.end(); ++iter)
        {
            if (iter->m_timeStartInSeconds <= timeStartInSeconds && timeStartInSeconds <= iter->m_timeEndInSeconds)
            {
                // время нахождения в локациях пересеклось, нужно корректировать отрезки

                if (iter->m_timeStartInSeconds == timeStartInSeconds)
                {
                    // совпало начало нахождения в локациях
                    if (timeEndInSeconds == iter->m_timeEndInSeconds)
                    {
                        // время нахождения в локациях полностью совпало, идем вместо первой локации во вторую

                        //  (________)
                        //      +
                        //  [########] 
                        //      =
                        //  [########]
                        iter->m_location = location;
                    }
                    else
                    {
                        // новая локация, потом остатки старой

                        //  (________)
                        //      +
                        //  [########] 
                        //      =
                        //  [####](____)
                        iter->m_timeStartInSeconds = timeEndInSeconds;
                        PersonSheduleSection newSection(location, timeStartInSeconds, timeEndInSeconds);
                        m_sectionList.push_back(newSection);
                    }
                }
                else if (timeEndInSeconds > iter->m_timeEndInSeconds)
                {
                    // кусочек старой, потом вся новая

                    //  (________)
                    //      +
                    //      [########] 
                    //      =
                    //  (__)[########]
                    iter->m_timeEndInSeconds = timeStartInSeconds;
                    PersonSheduleSection newSection(location, timeStartInSeconds, timeEndInSeconds);
                    m_sectionList.push_back(newSection);
                }
                else if (timeEndInSeconds < iter->m_timeEndInSeconds)
                {
                    // кусочек старой, потом вся новая, потом еще кусочек старой

                    //  (________)
                    //      +
                    //    [####] 
                    //      =
                    //  (__)[####](__)
                    PersonSheduleSection newSection1(location, timeStartInSeconds, timeEndInSeconds);
                    PersonSheduleSection newSection2(iter->m_location, timeEndInSeconds, iter->m_timeEndInSeconds);
                    iter->m_timeEndInSeconds = timeStartInSeconds;
                    m_sectionList.push_back(newSection1);
                    m_sectionList.push_back(newSection2);
                }
                else
                {
                    // кусочек старой, потом вся новая

                    //  (________)
                    //      +
                    //      [####] 
                    //      =
                    //  (__)[####]
                    iter->m_timeEndInSeconds = timeStartInSeconds;
                    PersonSheduleSection newSection(location, timeStartInSeconds, timeEndInSeconds);
                    m_sectionList.push_back(newSection);
                }
                break;
            }
        }
    }
}

void PersonShedule::clear()
{
    m_sectionList.clear();
}

PersonSheduleSection::PersonSheduleSection(Location *location, double timeStartInSeconds, double timeEndInSeconds)
{
    m_location = location;
    m_timeStartInSeconds = timeStartInSeconds;
    m_timeEndInSeconds = timeEndInSeconds;
}
