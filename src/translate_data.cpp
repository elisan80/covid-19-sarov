#include "translate_data.h"
#include <iostream> 
#include "lib_migration.h" 
#include "Model.h"
#include "Person.h"
#include "Location.h"

namespace translate
{
    //Magic �� ������ numbers
    const int LOCATIONS_OFFSET = 100000; //������ ����� ���������� �� �������������� �������
    /*
        ������� ��������� � ������� ���� �����
    */
    bool translate_create_humans_and_locations(Model* model, HumansMigration *phm)
    {
        for (auto itr = phm->_humans_config[0].begin(); itr != phm->_humans_config[0].end(); ++itr)
        {
            //��������, �������� � ���������� ������
            int id = itr->human_id;
            if (!model->allPersons[id])
            {//������� ��� �� �������� - �������� ��� 
                model->allPersons[id] = new Person();
                model->allPersons[id]->index = id;
                model->allPersons[id]->dayOffConfig = &(*itr);
            }

            //��������/�������� �������

            //��� �� �� ����� � � ��� ��� ���� �������� �� ������ � ��� ��������� �������, �� �������� � �� � ����� ��������� ��������. ���� �������� ��� ���� �� ������� � ��������� ������.
            for (auto itr2 = itr->locations.begin(); itr2 != itr->locations.end(); ++itr2)
            {
                int loc_id = itr2->location_id - LOCATIONS_OFFSET;
                if (!model->allLocations[loc_id])
                {//������� ��� �� ������� - ������ ��������
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
                {//������� ��� �� ������� - ������ ��������
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
    {// ���������� ������ � ���������� ����������
        for (auto itr = phm->_humans_time_laps[day_idx].begin(); itr != phm->_humans_time_laps[day_idx].end(); ++itr)
        {
            int human_id = itr->human_id;
            if (model->allPersons[human_id] == NULL)
            {//��� ����� �� ������ - ��� ���� �� ������! �� ������� ��� �������� � �������� ��.
                model->allPersons[human_id] = new Person();
            }

            for (auto itr2 = itr->AllPeriods.begin(); itr2 != itr->AllPeriods.end(); ++itr2)
            {//���� �� ���� ��������� �������� ������� ������ � ������������ �� � ��������� ���������
                int loc_id = itr2->location_id - LOCATIONS_OFFSET;
                if (model->allLocations[loc_id] == NULL)
                {//��� ����� �� ������ - ��� ���� �� ������! �� ������� ��� �������� � �������� ��.
                    model->allLocations[loc_id] = new Location();
                }

                //stayAtHome1->m_shedule.addLocation(home1, 10, 30); - ����������� ���������� ����������
                model->allPersons[human_id]->m_shedule.addLocation(model->allLocations[loc_id], itr2->start_time, itr2->end_time);
            }
        }
        return 0;
    }
};
