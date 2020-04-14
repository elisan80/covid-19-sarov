// Migration.cpp: определяет точку входа для консольного приложения.
//

#include <string>

#include "lib_migration.h"


int main()
{
	HumansMigration hm;
	hm.Init("c:/tmp/covid/humans.dat", 0); //здесь для каждого дня должен указываться свой файл. Пока везде стоит один и тот же.
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

