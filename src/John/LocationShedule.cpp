#include "LocationShedule.h"

#include <set>
#include <tuple>

#include "Person.h"

// добавить агента в расписание
void LocationShedule::addPerson(Person *person, double timeStart, double timeEnd)
{
    m_rawData.push_back(std::make_tuple(person, timeStart, timeEnd));
}

void LocationShedule::generate()
{
    // нужно скомпоновать расписание событийно
    // То есть с такого-то по такое-то время были эти агенты, с такого-то по такое-то другие
    // тогда  главном цикле шаг моделирования будет 1 день, цикл будет по локациям

    std::set<double> timeSet;

    for (size_t i = 0; i < m_rawData.size(); ++i)
    {
        timeSet.insert(std::get<1>(m_rawData.at(i)));
        timeSet.insert(std::get<2>(m_rawData.at(i)));
    }

    for (std::set<double>::iterator iter = timeSet.begin(); iter != timeSet.end(); ++iter)
    {
        LocationSheduleSection section;
        
        section.m_timeStart = *iter;
        if ((++iter) != timeSet.end())
        {
            section.m_timeEnd = *iter; --iter;
            if (section.m_timeEnd - section.m_timeStart > 0)
            {
                for (size_t i = 0; i < m_rawData.size(); ++i)
                {
                    double personStart = std::get<1>(m_rawData.at(i));
                    double personEnd = std::get<2>(m_rawData.at(i));

                    if (personStart <= section.m_timeStart && personEnd >= section.m_timeEnd)
                    {
                        section.m_personList.push_back(std::get<0>(m_rawData.at(i)));
                    }
                }
                m_sectionList.push_back(section);
            }
        }
        else
        {
            --iter;
        }
    }
}

void LocationShedule::clear()
{
    m_rawData.clear();
    m_sectionList.clear();
}
