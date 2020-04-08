#pragma once
#include "Location.h"
#include "PersonShedule.h"
#include "SEIR_model.h"

// ������� ����� ������, ����� ���������� �� ����, ������� ���������
class Person
{
public:
	Person();
	~Person() {}

	virtual void generateShedule() {}	// ��������� ���������� �������� ���
	void generateDayOffShedule();		// ��������� ���������� ��������� ���

	SEIR_State m_state;			// ��������� �� SEIR-������
	Location *m_home;			// ������� ��-���������
	PersonShedule m_shedule;	// ���������� �� ����
};

// ����� ������� - ����� ���� 24/7
class StayAtHome : public Person
{
public:
	StayAtHome() {}
	~StayAtHome() {}

	void generateShedule();
};

// ����� ��������, ����� ����� ������� �� ������ � ����� � ������
class Employee : public Person
{
public:
	Employee() {}
	~Employee() {}

	virtual void generateShedule() {}

	double timeStartWorking;	// ����� ������� �� ������
	double timeEndWorking;		// ����� ����� � ������
};

// ����� �������� ������
// ��������� � ����� ������ (��� ��������� � ������ � �.�.)
// ����� ������ � ���������� ������, � ������ �����, � ����� �����-������, � ��������
class VNIIEFEmployee : public Employee
{
public:
	VNIIEFEmployee() {}
	~VNIIEFEmployee() {}

	virtual void generateShedule();

	Group *m_group;							// ������, � ������� ���������
	double m_headOfDepartmentProbability;	// ����������� ����� � ���������� ������
	double m_headOfDepartmentTime;			// ����� ��������� ���������� ������
	double m_FirstDepartmentProbability;	// ����������� ����� � ������ �����
	double m_FirstDepartmentTime;			// ����� ��������� ������� ������
	double m_inputOutputProbability;		// ����������� ����� � ����� �����-������
	double m_inputOutputTime;				// ����� ��������� ������ �����-������
	double m_canteenProbability;			// ����������� ����� � ��������
	double m_canteenTime;					// ����� ��������� ��������
};

// ����� �������� ������� ������
// ��������� � ������ ������� ������
// ����� ������ � ��������
// ����� � ����� �����-������?
class FirstDepartmentEmployee : public VNIIEFEmployee
{
public:
	FirstDepartmentEmployee() {}
	~FirstDepartmentEmployee() {}

	virtual void generateShedule();

	FirstDepartment *m_firstDepartment;	// ������ �����, � �������� ���������
};

// ����� �������� ������ �����-������
// ��������� � ������ ������ �����-������
// ����� ������ � ��������
// ����� � ������ �����?
class InputOutputEmployee : public VNIIEFEmployee
{
public:
	InputOutputEmployee() {}
	~InputOutputEmployee() {}

	virtual void generateShedule();

	InputOutput *m_inputOutput;	// ����� �����-������, � �������� ���������
};

// ����� �������� ��������
// ��������� � ����� ��������
class CanteenEmployee : public Employee
{
public:
	CanteenEmployee() {}
	~CanteenEmployee() {}

	virtual void generateShedule();

	Canteen *m_canteen;	// ��������, � ������� ���������
};
