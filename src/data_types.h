#ifndef _DATA_TYPES_MIGRATION_
#define _DATA_TYPES_MIGRATION_

#include <iostream>
#include <list>

const int LOCATIONS_OFFSET = 100000; //данное число вычитается из идентификатора локации

class Location;

/*
	Конфигурация одной локации (одной строки из конфигурационного файла).
*/
struct LocationConfig
{
	Location* location;	// указатель на локацию, обозначает конкретный кабинет, конкретный магазин, столовую и т.д.
	time_t start_time;	// время, начиная с которого агент теоретически может находиться в данной локации. 
	time_t end_time;	// время, начиная с которого нахождение в данной локации более невозможно.
	int min_time;		// минимальное время, в течении которого можно находиться в данной локации. Задается в секундах.
	int max_time;		// максимальное время, в течении которого можно находиться в данной локации. Задается в секундах.
	double probability;	// вероятность

	void Show ()
	{
		std::cout << "location_id=" << location <<
			" start_time=" << start_time <<
			" end_time=" << end_time <<
			" min_time=" << min_time <<
			" max_time=" << max_time <<
			" probability=" << probability << std::endl;
	}
};

/*
	Содержит все записи перемещения для данного человека из конфигурационного файла.
*/
struct HumanConfig
{
	std::list<LocationConfig *> locations;
};

#endif
