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
    
    model.allLocations.resize(57000);           //LEV магические чиселки (макс локаций)
    model.allPersons.resize(100000);            //LEV магические чиселки (макс населени€)
    const int TYPES_DAYS_AMOUNT = 2;            //LEV
    HumansMigration hm(TYPES_DAYS_AMOUNT);      //LEV
    hm.Init("c:/tmp/covid/humans_work.dat", 0); //файл дл€ рабочих дней   ѕ”“»!!!!!     //LEV
    hm.Init("c:/tmp/covid/humans_holiday.dat", 1); //файл дл€ выходных дней ѕ”“»!!!!!   //LEV
    //hm.ShowConfig();                          //LEV
    translate::translate_data( &model, &hm );   //перевод конфигурационных данных по люд€м и локаци€м. //LEV
    int modelling_days_amount = 14; //—колько дней моделируем (сколько дней хотим насчитать, неделю там, или год, или пару мес€цев)    //LEV

    {
        int days_cnt = 0;
        while (days_cnt < modelling_days_amount)
        {
            for (int i = 0; i < 5 && days_cnt < modelling_days_amount; ++i)
            {
                ++days_cnt;
                hm.MakeDayMigration(0); //здесь создаем расписание только на рабочие дни
                translate::translate_step(&model, &hm, 0);
                model.startNewDay(); // моделирование дн€
            }
            for (int i = 0; i < 2 && days_cnt < modelling_days_amount; ++i)
            {
                ++days_cnt;
                hm.MakeDayMigration(1); //здесь создаем расписание только на выходные дни
                translate::translate_step(&model, &hm, 1);
                model.startNewDay(); // моделирование дн€
            }
        }
    }



    /*                                               ничего этого больше не надо - можно стирать
    // генераци€ агентов
    StayAtHome* stayAtHome1 = new StayAtHome();
    StayAtHome* stayAtHome2 = new StayAtHome();
    StayAtHome* stayAtHome3 = new StayAtHome();
    StayAtHome* stayAtHome4 = new StayAtHome();

    // добавление агентов в модель
    model.allPersons.push_back(stayAtHome1);
    model.allPersons.push_back(stayAtHome2);
    model.allPersons.push_back(stayAtHome3);
    model.allPersons.push_back(stayAtHome4);

    //генераци€ локаций
    Home* home1 = new Home();
    Home* home2 = new Home();
    Home* home3 = new Home();

    // добавление локаций в модель
    model.allLocations.push_back(home1);
    model.allLocations.push_back(home2);
    model.allLocations.push_back(home3);

    // составление расписани€
    stayAtHome1->m_shedule.addLocation(home1, 10, 30);
    stayAtHome2->m_shedule.addLocation(home1, 20, 40);
    stayAtHome3->m_shedule.addLocation(home1, 0, 50);
    stayAtHome4->m_shedule.addLocation(home1, 20, 50);

    stayAtHome1->setExposed(stayAtHome1, 0.0); // заражение агента stayAtHome1

    for (int i = 0; i < 50; ++i)
    {
        model.startNewDay(); // моделирование дн€
    }
    */

    system("pause");
}