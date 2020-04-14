#ifndef _DATA_TYPES_MIGRATION_
#define _DATA_TYPES_MIGRATION_

#include <iostream>
#include <time.h>
#include <list>
/*
	 онфигураци€ одной локации (одной строки из конфигурационного файла).
*/

struct LocationConfig
{
	int location_id;	// уникальный идентификатор локации, обозначает конкретный кабинет, конкретный магазин, столовую и т.д. ÷елочисленный идентификатор.
	time_t start_time;	// врем€, начина€ с которого агент теоретически может находитьс€ в данной локации. 
	time_t end_time;	// врем€, начина€ с которого нахождение в данной локации более невозможно.
	int min_time;		// минимальное врем€, в течении которого можно находитьс€ в данной локации. «адаетс€ в секундах.
	int max_time;		// максимальное врем€, в течении которого можно находитьс€ в данной локации. «адаетс€ в секундах.
	float probability;	// веро€тность

	void Show ()
	{
		std::cout << "location_id=" << location_id <<
			" start_time=" << start_time <<
			" end_time=" << end_time <<
			" min_time=" << min_time <<
			" max_time=" << max_time <<
			" probability=" << probability << std::endl;
	}
};

/*
	—одержит все записи перемещени€ дл€ данного человека из конфигурационного файла.
*/

struct HumanConfig
{
	int human_id;
	std::list<LocationConfig> locations;
};

/*
	—труктура описывающа€ один период в таймлапсе.
*/
struct Period
{
	int location_id;
	time_t start_time;
	time_t end_time;
};

/*
	—труктура описывает таймлапс одного агента (расписание агента на сутки, совокупность всех его периодов).
*/
struct TimeLaps
{
	int human_id;
	std::list<Period> AllPeriods;
};

#endif