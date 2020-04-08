#pragma once
#include "Person.h"
#include <list>

// ������� ����� �������
class Location
{
public:
    Location() {}
    ~Location() {}

    double m_beta;		// ����������� �������� �� �������������� � �������������� �������
    double m_alpha;		// ����������� �������� �� �������������� ������� � �������������
    double m_gamma;		// ����������� �������������
};

// ������� ��� - ������� �� ���������
class Home : public Location
{
public:
    Home() {}
    ~Home() {}

    std::list<Person> m_personList;	// ������ ������ �����
};

// ����� ������ - ������� ����� �������� �����
class Work : public Location
{
public:
    Work() {}
    ~Work() {}

    std::list<Employee> m_employeeList;// ������ ����������
};

class VNIIEF : public Location
{
public:
    VNIIEF() {}
    ~VNIIEF() {}

    void addArea() {}

    std::list<Area> m_areaList; // ������ ��������
};

// ����� ��������
class Area : public Location
{
public:
    Area() {}
    ~Area() {}

    void addFirstDepartment(FirstDepartment *firstDepartment) {}
    void addInputOutput(InputOutput *inputOutput) {}
    void addInputOutput(Canteen *canteen) {}

    FirstDepartment *m_firstDepartment;			// ������ �����
    std::list<InputOutput> m_inputOutputArray;	// ������ ������� �����-������
    Canteen *m_canteen;							// ��������
};

// ����� ������ �����
class FirstDepartment : public Work
{
public:
    FirstDepartment() {}
    ~FirstDepartment() {}

    Area *m_area; // ��������, � ������� ���������
};

// ����� ����� �����-������
class InputOutput : public Work
{
public:
    InputOutput() {}
    ~InputOutput() {}

    Area *m_area; // ��������, � ������� ���������
};

// ����� ��������
class Canteen : public Work
{
public:
    Canteen() {}
    ~Canteen() {}

    Area *m_area; // ��������, � ������� ���������
};

// ����� ������
class Department : public Work
{
public:
    Department() {}
    ~Department() {}

    Area *m_area;					    // ��������, � ������� ���������
    Employee *m_headOfDepartment;	    // ��������� ������
    std::list<Group> m_groupList;       // ������ �����
};

// ����� ������
class Group : public Work
{
public:
    Group() {}
    ~Group() {}

    // ��������
    void meeting();

    Department *m_department;           // �����, � �������� ���������
    Employee *m_headOfGroup;            // ���������
    std::list<Employee> m_employeeList; // ������ �����������
    double m_meetingProbability;        // ����������� ��������
    double m_meetingDuration;           // ������������ ��������
};
