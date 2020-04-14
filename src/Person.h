#pragma once
#include "PersonShedule.h"
#include "SEIR_model.h"

class Location;
class Area;
class Group;
class Archive;
class Library;
class Canteen;

// Базовый класс агента, имеет расписание на день, текущее состояние
class Person
{
public:
	Person();
	virtual ~Person() {}

    // генерация расписания рабочего дня
    virtual void generateShedule();

    // генерация расписания выходного дня
	void generateDayOffShedule();

    // Сообщить локации о своем присутствии сегодня
    void notifyLocations();

    // Проверить состояние на сегодняшний день
    virtual void checkState();

    // изменить состояние на "Контактный"
    virtual void setExposed(Person *source, double time);

    // изменить состояние на "Инфицированный"
    virtual void setInfectious();

    // изменить состояние на "Выздоровевший"
    virtual void setRecovered(double time);

    // изменить состояние на "Умерший"
    virtual void setDead(double time);

    // установить карантин (пока нет мед помощи сидит дома)
    void setQuarantine();

public:
	SEIR_State m_state;			        // состояние по SEIR-модели
	Location *m_home;			        // локация по-умолчанию
	PersonShedule m_shedule;	        // расписание на день

    double m_timeExposed;
    Person *m_exposedSource;
    double m_timeSymptoms;
    double m_timeInfected;
    double m_timeRecovered;
    double m_timeDead;
    bool m_isOnQuarantine;
};

// Класс домосед - сидит дома 24/7
class StayAtHome : public Person
{
public:
	StayAtHome() {}
    ~StayAtHome() {}
};

// Класс абстрактный работник, имеет время прихода на работу и ухода с работы
class Employee : public Person
{
public:
	Employee() {}
    virtual ~Employee() {}

	double timeStartWorkingInSeconds;	// время прихода на работу в секундах с начала дня
	double timeEndWorkingInSeconds;		// время ухода с работы в секундах с начала дня
};

// Класс работник организации
// относится к своей группе (она относится к отделу и т.д.)
// может ходить к начальнику отдела, в архив, в библиотеку, в столовую
class OrganizationEmployee : public Employee
{
public:
	OrganizationEmployee() {}
    virtual ~OrganizationEmployee() {}

	Group *m_group;							// группа, к которой относится
	double m_headOfDepartmentProbability;	// вероятность пойти к начальнику отдела
	double m_headOfDepartmentTime;			// время посещения начальника отдела
	double m_archiveProbability;	        // вероятность пойти в архив
	double m_archiveTime;			        // время посещения архива
	double m_libraryProbability;		    // вероятность пойти в библиотеку
	double m_libraryTime;				    // время посещения библиотеки
	double m_canteenProbability;			// вероятность пойти в столовую
	double m_canteenTime;					// время посещения столовой
};

// Класс работник архива
// относится к своему первому отделу
// может ходить в столовую
// ходит в библиотеку?
class ArchiveEmployee : public OrganizationEmployee
{
public:
	ArchiveEmployee() {}
	~ArchiveEmployee() {}

	Archive *m_archive;	// архив, к которому относится
};

// Класс работник библиотеки
// относится к своей библиотеке
// может ходить в столовую
// ходит в архив?
class LibraryEmployee : public OrganizationEmployee
{
public:
	LibraryEmployee() {}
	~LibraryEmployee() {}

	Library *m_library;	// библиотека, к которому относится
};

// Класс работник столовой
// относится к своей столовой
class CanteenEmployee : public Employee
{
public:
	CanteenEmployee() {}
	~CanteenEmployee() {}

	Canteen *m_canteen;	// столовая, к которой относится
};
