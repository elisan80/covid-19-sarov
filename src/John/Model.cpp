#include "Model.h"

#include "Person.h"
#include "Location.h"

Model::Model() :
    m_currentDay(0)
{}

Model& Model::instance()
{
    static Model instance;
    return instance;
}

void Model::startNewDay()
{
    ++m_currentDay;
    bool isWorkingDay = m_currentDay % 7 < 5; // рабочий ли день

    for (std::vector<Location *>::iterator iter = allLocations.begin(); iter != allLocations.end(); ++iter)
    {
        // чистим расписание локаций
        if (*iter)
        {
            (*iter)->m_locationShedule.clear();
        }
    }

    for (std::vector<Person *>::iterator iter = allPersons.begin(); iter != allPersons.end(); ++iter)
    {
        if (*iter)
        {
            // чистим расписание агентов
            (*iter)->m_shedule.clear();

            // проверяем состояние агентов
            (*iter)->checkState();

            // генерируем новое расписание
            if (isWorkingDay)
                (*iter)->generateShedule();
            else
                (*iter)->generateDayOffShedule();

            // отправляем сгенерированное расписание в локации
            (*iter)->notifyLocations();
        }
    }

    for (std::vector<Location *>::iterator iter = allLocations.begin(); iter != allLocations.end(); ++iter)
    {
        // генерируем расписание локации из кусочков, полученных от агентов
        if (*iter)
        {
            (*iter)->m_locationShedule.generate();
        }
    }


}
