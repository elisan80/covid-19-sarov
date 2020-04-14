#ifndef _DATA_TYPES_MIGRATION_
#define _DATA_TYPES_MIGRATION_

#include <iostream>
#include <time.h>
#include <list>
/*
    Конфигурация одной локации (одной строки из конфигурационного файла).
*/

struct LocationConfig
{
    int location_id;    // уникальный идентификатор локации, обозначает конкретный кабинет, конкретный магазин, столовую и т.д. Целочисленный идентификатор.
    time_t start_time;  // время, начиная с которого агент теоретически может находиться в данной локации. 
    time_t end_time;    // время, начиная с которого нахождение в данной локации более невозможно.
    int min_time;       // минимальное время, в течении которого можно находиться в данной локации. Задается в секундах.
    int max_time;       // максимальное время, в течении которого можно находиться в данной локации. Задается в секундах.
    float probability;  // вероятность

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
    Содержит все записи перемещения для данного человека из конфигурационного файла.
*/

struct HumanConfig
{
    int human_id;
    std::list<LocationConfig> locations;
};

/*
Структура описывающая один период в таймлапсе.
*/
struct Period
{
    int location_id;
    time_t start_time;
    time_t end_time;
};

/*
Структура описывает таймлапс одного агента (расписание агента на сутки, совокупность всех его периодов).
*/
struct TimeLaps
{
    int human_id;
    std::list<Period> AllPeriods;
};

#endif
