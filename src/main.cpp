#include "Person.h"
#include "Location.h"
#include "Model.h"
#include "Generator.h"
#include "translate_data.h" //LEV 
#include "lib_migration.h"  //LEV

#include <ctime>




void main(int argc, char *argv[])
{
    std::srand(std::time(nullptr));
    Model& model = Model::instance();
    
    model.allLocations.resize(57000);           //LEV магические чиселки (макс локаций)
    model.allPersons.resize(100000);            //LEV магические чиселки (макс населения)
    const int TYPES_DAYS_AMOUNT = 2;            //LEV
    HumansMigration hm(TYPES_DAYS_AMOUNT);      //LEV

    std::string data_dir;
    std::string file_name1;
    std::string file_name2;
    translate::SEIRDcfg seird_cfg;
    translate::ParseCmdLine(argc, argv, data_dir, file_name1, file_name2, seird_cfg);

    model.SetProjectDir(data_dir);
    std::cout << "reading first config file started" << std::endl;
    if (!hm.Init(file_name2, 0)) //выходные дни     //LEV
    {
        std::cout << "can't read holiday days file: " << file_name2 << std::endl;
        return;
    }

    std::cout << "reading second config file started" << std::endl;
    if (!hm.Init(file_name1, 1)) //рабочие дни //LEV
    {
        std::cout << "can't read work days file: " << file_name1 << std::endl;
        return;
    }
    translate::translate_data( &model, &hm );   //перевод конфигурационных данных по людям и локациям. //LEV

    {
        int days_cnt = 0;
        model.allPersons[0]->setExposed(model.allPersons[0], 0);
        model.writeOutput();
        translate::translate_step(&model, &hm, 0);
        while (!model.isModellingStopped)
        {
            ++days_cnt;
            model.startNewDay(); // моделирование дня
            model.writeOutput();
            std::cout << "day " << days_cnt << " ended" << std::endl;
        }
        model.writeGraphvizFile();
        std::cout << "Modelling stopped" << std::endl;
    }

    system("pause");
}
