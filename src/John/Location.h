#pragma once

#include <list>

#include "LocationShedule.h"

class Person;
class Employee;

class Area;
class Group;
class Archive;
class Library;
class Canteen;


// Базовый класс Локаций
class Location
{
public:
    Location();
    virtual ~Location() {}

    // имеет ли среди агентов распространителей
    bool hasEIRPerson();

    // Моделирование распространения в данной локации на сегодняшний день
    void seirModelling();

    // Моделирование контакта source-target
    void modelContact(Person *source, Person *target, double timeStartInSeconds, double timeEndInSeconds);

public:
    double m_beta;		// коэффициент перехода от восприимчивого к инкубационному периоду
    double m_alpha;		// коэффициент перехода от инкубационного периода к инфицированию
    double m_gamma;		// коэффициент выздоровления

    LocationShedule m_locationShedule;
};

// Локация дом - локация по умолчанию
class Home : public Location
{
public:
    Home();
    ~Home() {}

    std::list<Person *> m_personList;	// список членов семьи
};

// Класс работа - базовый класс рабочего места
class Work : public Location
{
public:
    Work() {}
    virtual ~Work() {}

    std::list<Employee *> m_employeeList;// список работников
};

class Organization : public Location
{
public:
    Organization() {}
    ~Organization() {}

    void addArea() {}

    std::list<Area *> m_areaList; // список площадок
};

// Класс площадка
class Area : public Location
{
public:
    Area() {}
    ~Area() {}

    void addArchive(Archive *Archive) {}
    void addLibrary(Library *library) {}
    void addLibrary(Canteen *canteen) {}

    Archive *m_archive;			// архив
    std::list<Library *> m_libraryArray;	// список библиотек
    Canteen *m_canteen;							// столовая
};

// Класс Архив
class Archive : public Work
{
public:
    Archive() {}
    ~Archive() {}

    Area *m_area; // площадка, к которой относится
};

// Класс библиотека
class Library : public Work
{
public:
    Library() {}
    ~Library() {}

    Area *m_area; // площадка, к которой относится
};

// Класс Столовая
class Canteen : public Work
{
public:
    Canteen() {}
    ~Canteen() {}

    Area *m_area; // площадка, к которой относится
};

// Класс отдела
class Department : public Work
{
public:
    Department() {}
    ~Department() {}

    Area *m_area;					    // площадка, к которой относится
    Employee *m_headOfDepartment;	    // начальник отдела
    std::list<Group *> m_groupList;       // список групп
};

// Класс группы
class Group : public Work
{
public:
    Group() {}
    ~Group() {}

    // планерка
    //void meeting();

    Department *m_department;           // отдел, к которому относится
    Employee *m_headOfGroup;            // начальник
    std::list<Employee *> m_employeeList; // список подчиненных
    double m_meetingProbability;        // вероятность планерки
    double m_meetingDuration;           // длительность планерки
};
