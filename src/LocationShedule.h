#pragma once

#include <list>
#include "Location.h"

// ����� ���������� � �������
// �������� ������ �������� �������, ������ ������� ������ ������� � ������� �� �������
// ������ ������� ������ ������� ���������� �������������
class LocationShedule
{
public:
    LocationShedule() {}
    ~LocationShedule() {}

    void addPerson(Person *person, double timeStart, double timeDuration);

    std::list<LocationSheduleSection> m_shedule; // <����� ������ �������, ����� ����� �������, ������>
};

class LocationSheduleSection
{
public:
    LocationSheduleSection() {}
    ~LocationSheduleSection() {}
    double m_timeStart;
    double m_timeEnd;
    std::list<Person *> m_personList;
};
