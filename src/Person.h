#pragma once
#include "PersonShedule.h"
#include "Model.h"

class Location;
class Area;
class Group;
class Archive;
class Library;
class Canteen;
struct HumanConfig;

// Базовый класс агента, имеет расписание на день, текущее состояние
class Person
{
public:
	Person();
    virtual ~Person();

    // генерация расписания рабочего дня
    virtual void generateShedule(bool isWorkingDay);

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

    // установить карантин (пока нет моделирования мед помощи сидит дома)
    void setQuarantine();

public:
    int index;
	SEIR_State m_state;			        // состояние по SEIR-модели
	Location *m_home;			        // локация по-умолчанию
	Location *m_work;			        // локация работа
	PersonShedule m_shedule;	        // расписание на день

    double m_EtoIDuration;
    double m_EtoSymptomsDuration;
    double m_timeExposed;               // день и время заражения
    Person *m_exposedSource;            // заразивший
    double m_timeSymptoms;              // день и время появления симптомов
    double m_timeInfected;              // день и время инфицирования
    double m_timeRecovered;             // день и время выздоровления
    double m_timeDead;                  // день и время смерти
    bool m_isOnQuarantine;              // признак нахождения на карантине

    HumanConfig *workingDayConfig;
    HumanConfig *dayOffConfig;
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
// относится к своему архиву
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
