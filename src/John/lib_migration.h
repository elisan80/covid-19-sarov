

#include "data_types.h"
#include <iostream>
#include <string>
#include <vector>

class HumansConfigReader
{
public:
	HumansConfigReader (const std::string &file_name, std::list <HumanConfig> &humans_config ) : _file_name  (file_name), _humans_config(humans_config)
	{
		 
	}
	bool TryAddData (std::string &data_string, HumanConfig &storage);
	bool ReadCfg();

private:
	const std::string &_file_name;
	std::list <HumanConfig> &_humans_config;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*
 Класс расчитывает миграцию людей между локациями на основании конфигурационного файла.
*/
class HumansMigration
{
public:
	HumansMigration(int days_amount = 7);
	bool Init (const std::string file_name, int day);
	void ShowConfig ();

	bool MakeAllMigrations (); //Осуществить все перемещения агентов
	bool MakeDayMigration (int day_index); // Осуществить перемещения агентов на указанный день
	void ShowMigration(std::string show_path); //по указанному пути создать файлы. по одному на каждого агента, с его таймлапсами

protected:
	bool MakeOneMigration (HumanConfig &human_cfg, TimeLaps &time_laps_); //Осуществить перемещение 1 агента (создать расписание по одному агенту)

public:
	//std::list <HumanConfig> _humans_config_0; //конфигурация по всем людям
	//std::list<TimeLaps> _humans_time_laps_0; // результирующие данные - расписание всех агентов за сутки
	std::vector<std::list <HumanConfig> > _humans_config; //конфигурация по всем людям по всем дням (0 - понедельник, 1 - вторник, ..., 6 - воскресенье)
	std::vector<std::list<TimeLaps> > _humans_time_laps; // результирующие данные - расписание всех агентов за сутки
};