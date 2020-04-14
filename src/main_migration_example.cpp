// Migration.cpp: определ€ет точку входа дл€ консольного приложени€.
//

#include "stdafx.h"
#include <string>

#include "lib_migration.h"


int _tmain(int argc, _TCHAR* argv[])
{
	HumansMigration hm;
	hm.Init("c:/tmp/covid/humans.dat", 0); //здесь дл€ каждого дн€ должен указыватьс€ свой файл. ѕока везде стоит один и тот же.
	hm.Init("c:/tmp/covid/humans.dat", 1);
	hm.Init("c:/tmp/covid/humans.dat", 2);
	hm.Init("c:/tmp/covid/humans.dat", 3);
	hm.Init("c:/tmp/covid/humans.dat", 4);
	hm.Init("c:/tmp/covid/humans.dat", 5);
	hm.Init("c:/tmp/covid/humans.dat", 6);
	hm.ShowConfig();

	hm.MakeAllMigrations();
	hm.ShowMigration("c:/tmp/covid/out/");
	return 0;
}

