#pragma once
#include <vector>

class Location;

class PersonSheduleSection;

// Класс расписание агента
// Содержит список локаций, которые посетит сегодня и время в секундах начала и конца их посещения
class PersonShedule
{
public:
	PersonShedule() {}
	~PersonShedule() {}

    // Добавить локацию в которой агент находился с timeStart по timeEnd, уже готовое расписание, без пересечений
	void addLocation(Location *location, double timeStartInSeconds, double timeEndInSeconds);

    // очистить расписание
    void clear();

public:
	std::vector<PersonSheduleSection> m_sectionList; // список отрезков
};

// Класс отрезок расписания агента
// внутри отрезка агент находится в одной локации
class PersonSheduleSection
{
public:
    PersonSheduleSection(Location *location, double timeStartInSeconds, double timeEndInSeconds);
    ~PersonSheduleSection() {}

    double m_timeStartInSeconds;    // Время начала отрезка в секундах с начала дня
    double m_timeEndInSeconds;      // Время конца отрезка в секундах с начала дня
    Location *m_location;           // Посещаемая локация
};
