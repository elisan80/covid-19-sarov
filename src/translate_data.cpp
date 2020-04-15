#include "translate_data.h"
#include <iostream> 
#include "lib_migration.h" 
#include "Model.h"
#include "Person.h"
#include "Location.h"

namespace translate
{
    //Magic их растак numbers
    const int LOCATIONS_OFFSET = 100000; //данное число вычитается из идентификатора локации
    /*
        Функция переводит и создает всех людей
    */
    bool translate_create_humans_and_locations(Model* model, HumansMigration *phm)
    {
        for (auto itr = phm->_humans_config[0].begin(); itr != phm->_humans_config[0].end(); ++itr)
        {
            //Проверка, создание и заполнение агента
            int id = itr->human_id;
            if (!model->allPersons[id])
            {//человек еще не заполнен - заполним его 
                model->allPersons[id] = new Person();
                model->allPersons[id]->index = id;
                model->allPersons[id]->dayOffConfig = &(*itr);
            }

            //Проверка/создание локации

            //раз уж мы здесь и у нас уже есть итератор на агента и его возможные локации, то создадим и их в целях ускорения процесса. Хотя красивей это было бы сделать в отдельной функци.
            for (auto itr2 = itr->locations.begin(); itr2 != itr->locations.end(); ++itr2)
            {
                int loc_id = itr2->location_id - LOCATIONS_OFFSET;
                if (!model->allLocations[loc_id])
                {//локация еще не создана - значит креэйтим
                    model->allLocations[loc_id] = new Location();
                    model->allLocations[loc_id]->index = loc_id;
                }
            }
        }
        for (auto itr = phm->_humans_config[1].begin(); itr != phm->_humans_config[1].end(); ++itr)
        {
            int id = itr->human_id;
            if (model->allPersons[id])
            {
                model->allPersons[id]->workingDayConfig = &(*itr);
            }
            for (auto itr2 = itr->locations.begin(); itr2 != itr->locations.end(); ++itr2)
            {
                int loc_id = itr2->location_id - LOCATIONS_OFFSET;
                if (!model->allLocations[loc_id])
                {//локация еще не создана - значит креэйтим
                    model->allLocations[loc_id] = new Location();
                    model->allLocations[loc_id]->index = loc_id;
                }
            }
        }
        return true;
    }





    int translate_data(Model* model, HumansMigration* phm)
    {
        
        

        translate_create_humans_and_locations(model, phm);
        return 0;
    }

    int translate_step(Model* model, HumansMigration* phm, int day_idx)
    {// трансляция данных о ежедневном расписании
        for (auto itr = phm->_humans_time_laps[day_idx].begin(); itr != phm->_humans_time_laps[day_idx].end(); ++itr)
        {
            int human_id = itr->human_id;
            if (model->allPersons[human_id] == NULL)
            {//Это какая то ошибка - так быть не должно! но отловим эту ситуацию и исправим ее.
                model->allPersons[human_id] = new Person();
            }

            for (auto itr2 = itr->AllPeriods.begin(); itr2 != itr->AllPeriods.end(); ++itr2)
            {//идем по всем временнЫм периодам данного агента и перекидываем их в Модельные структуры
                int loc_id = itr2->location_id - LOCATIONS_OFFSET;
                if (model->allLocations[loc_id] == NULL)
                {//Это какая то ошибка - так быть не должно! но отловим эту ситуацию и исправим ее.
                    model->allLocations[loc_id] = new Location();
                }

                //stayAtHome1->m_shedule.addLocation(home1, 10, 30); - аналогичное заполнение расписания
                model->allPersons[human_id]->m_shedule.addLocation(model->allLocations[loc_id], itr2->start_time, itr2->end_time);
            }
        }
        return 0;
    }
};
