#pragma once

#include <list>
#include <vector>
#include <tuple>

class Person;

// Класс отрезок времени, внутри которых состав агентов в локации не менялся
class LocationSheduleSection
{
public:
    LocationSheduleSection() {}
    ~LocationSheduleSection() {}
    double m_timeStart;
    double m_timeEnd;
    std::list<Person *> m_personList;
};

// Класс расписание в локации на текущий день моделирования
// содержит список отрезков времени, внутри которых состав агентов в локации не менялся
// внутри каждого такого отрезка происходит моделирование
class LocationShedule
{
public:
    LocationShedule() {}
    ~LocationShedule() {}

    // Добавить агента в расписание
    void addPerson(Person *person, double timeStart, double timeEnd);

    // Составить расписание, после добавления всех агентов
    void generate();

    // очистить расписание
    void clear();

public:
    std::list<LocationSheduleSection> m_sectionList; // список отрезков времени, внутри которых состав агентов в локации не менялся

private:
    std::vector<std::tuple<Person *, double, double>> m_rawData;
};
