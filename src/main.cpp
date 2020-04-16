#include <ctime>

#include "Person.h"
#include "Location.h"
#include "Model.h"
#include "translate_data.h" //LEV 
#include "lib_migration.h"  //LEV

void main(int argc, char *argv[])
{
    std::srand(std::time(nullptr));
    Model& model = Model::instance();
    
    model.allLocations.resize(100000, nullptr);           //LEV магические чиселки (макс локаций)
    model.allPersons.resize(100000, nullptr);            //LEV магические чиселки (макс населения)

    std::string data_dir;
    std::string file_name1;
    std::string file_name2;
    translate::SEIRDcfg seird_cfg;
    translate::ParseCmdLine(argc, argv, data_dir, file_name1, file_name2, seird_cfg);

    model.SetProjectDir(data_dir);
    HumansConfigReader hcr;
    std::cout << "Reading holiday config file started : " << file_name2 << std::endl;
    if (!hcr.ReadCfg(file_name2, false)) //выходные дни     //LEV
    {
        std::cout << "can't read holiday file: " << file_name2 << std::endl;
        return;
    }

    std::cout << "Reading work config file started :    " << file_name1 << std::endl;
    if (!hcr.ReadCfg(file_name1, true)) //рабочие дни //LEV
    {
        std::cout << "can't read work days file: " << file_name1 << std::endl;
        return;
    }

    int days_cnt = 0;
    model.allPersons[0]->setExposed(model.allPersons[0], 0);
    model.writeOutput();
    while (!model.isModellingStopped)
    {
        ++days_cnt;
        model.startNewDay(); // моделирование дня
        model.writeOutput();
        std::cout << "Day " << days_cnt << " ended" << std::endl;
    }
    model.writeGraphvizFile();
    std::cout << "Modelling stopped." << std::endl;
    std::cout << "Output directory : " << data_dir << std::endl;

    system("pause");
}
