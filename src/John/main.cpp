#include "Person.h"
#include "Location.h"
#include "Model.h"
#include "Generator.h"
#include "translate_data.h" //LEV 
#include "lib_migration.h"  //LEV


/*void main()
{
    Generator generator;
    // generator.setAgentFile()
    // generator.setModelFile()
    // generator.generate()
    

    //Model * model = generator.m_model;
    Model* model = &(Model::instance());

   // model->allLocations.push_back (new Location(102,)

    //model.allLocations.push_back(new Location(int id, ...))

    system("pause");
}*/

void main()
{
    Model& model = Model::instance();
    
    model.allLocations.resize(57000);           //LEV ���������� ������� (���� �������)
    model.allPersons.resize(100000);            //LEV ���������� ������� (���� ���������)
    const int TYPES_DAYS_AMOUNT = 2;            //LEV
    HumansMigration hm(TYPES_DAYS_AMOUNT);      //LEV
    hm.Init("c:/tmp/covid/humans_work.dat", 0); //���� ��� ������� ����   ����!!!!!     //LEV
    hm.Init("c:/tmp/covid/humans_holiday.dat", 1); //���� ��� �������� ���� ����!!!!!   //LEV
    //hm.ShowConfig();                          //LEV
    translate::translate_data( &model, &hm );   //������� ���������������� ������ �� ����� � ��������. //LEV
    int modelling_days_amount = 14; //������� ���� ���������� (������� ���� ����� ���������, ������ ���, ��� ���, ��� ���� �������)    //LEV

    {
        int days_cnt = 0;
        while (days_cnt < modelling_days_amount)
        {
            for (int i = 0; i < 5 && days_cnt < modelling_days_amount; ++i)
            {
                ++days_cnt;
                hm.MakeDayMigration(0); //����� ������� ���������� ������ �� ������� ���
                translate::translate_step(&model, &hm, 0);
                model.startNewDay(); // ������������� ���
            }
            for (int i = 0; i < 2 && days_cnt < modelling_days_amount; ++i)
            {
                ++days_cnt;
                hm.MakeDayMigration(1); //����� ������� ���������� ������ �� �������� ���
                translate::translate_step(&model, &hm, 1);
                model.startNewDay(); // ������������� ���
            }
        }
    }



    /*                                               ������ ����� ������ �� ���� - ����� �������
    // ��������� �������
    StayAtHome* stayAtHome1 = new StayAtHome();
    StayAtHome* stayAtHome2 = new StayAtHome();
    StayAtHome* stayAtHome3 = new StayAtHome();
    StayAtHome* stayAtHome4 = new StayAtHome();

    // ���������� ������� � ������
    model.allPersons.push_back(stayAtHome1);
    model.allPersons.push_back(stayAtHome2);
    model.allPersons.push_back(stayAtHome3);
    model.allPersons.push_back(stayAtHome4);

    //��������� �������
    Home* home1 = new Home();
    Home* home2 = new Home();
    Home* home3 = new Home();

    // ���������� ������� � ������
    model.allLocations.push_back(home1);
    model.allLocations.push_back(home2);
    model.allLocations.push_back(home3);

    // ����������� ����������
    stayAtHome1->m_shedule.addLocation(home1, 10, 30);
    stayAtHome2->m_shedule.addLocation(home1, 20, 40);
    stayAtHome3->m_shedule.addLocation(home1, 0, 50);
    stayAtHome4->m_shedule.addLocation(home1, 20, 50);

    stayAtHome1->setExposed(stayAtHome1, 0.0); // ��������� ������ stayAtHome1

    for (int i = 0; i < 50; ++i)
    {
        model.startNewDay(); // ������������� ���
    }
    */

    system("pause");
}