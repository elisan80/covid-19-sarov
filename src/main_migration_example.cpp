// Migration.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <string>

#include "lib_migration.h"


int _tmain(int argc, _TCHAR* argv[])
{
	const int DAYS_AMOUNT = 2;
	HumansMigration hm (DAYS_AMOUNT);
	hm.Init("c:/tmp/covid/humans_work.dat", 0); //���� ��� ������� ����
	hm.Init("c:/tmp/covid/humans_holiday.dat", 1); //���� ��� �������� ����
	hm.ShowConfig();

	hm.MakeDayMigration (0);// ��������� ������� ����
	hm.ShowMigration("c:/tmp/covid/out0/");

	hm.MakeDayMigration (0);// ��������� ��� ������� ����
	hm.ShowMigration("c:/tmp/covid/out1/");

	hm.MakeDayMigration (0);// � ��� ����
	hm.ShowMigration("c:/tmp/covid/out2/");

	hm.MakeDayMigration (0);// � �.�.
	hm.ShowMigration("c:/tmp/covid/out3/");

	hm.MakeDayMigration (1);// ��������� ��������
	hm.ShowMigration("c:/tmp/covid/out4/");
	return 0;
}

