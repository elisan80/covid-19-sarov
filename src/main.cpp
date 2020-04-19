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
    
    model.allLocations.resize(100000, nullptr);         //LEV магические чиселки (макс локаций)
    model.allPersons.resize(100000, nullptr);           //LEV магические чиселки (макс населения)

    std::string data_dir;
    translate::ParseCmdLine(argc, argv, data_dir);
    model.SetProjectDir(data_dir);
    std::string humansWorkFileName = data_dir + "humans_work.dat";
    std::string humansHolidayFileName = data_dir + "humans_holiday.dat";
    std::string modelFileName = data_dir + "config.dat";

    {
        HumansConfigReader hcr;
        std::cout << "Reading model config file started : " << modelFileName << std::endl;
        if (!hcr.readModelConfig(modelFileName)) //конфигурационный файл модели     //LEV
        {
            std::cout << "can't read model config file: " << modelFileName << std::endl;
            std::cout << "using default params" << modelFileName << std::endl;
        }

        std::cout << "Reading holiday config file started : " << humansHolidayFileName << std::endl;
        if (!hcr.readHumanConfig(humansHolidayFileName, false)) //выходные дни     //LEV
        {
            std::cout << "can't read holiday file: " << humansHolidayFileName << std::endl;
            return;
        }

        std::cout << "Reading work config file started : " << humansWorkFileName << std::endl;
        if (!hcr.readHumanConfig(humansWorkFileName, true)) //рабочие дни //LEV
        {
            std::cout << "can't read work days file: " << humansWorkFileName << std::endl;
            return;
        }
    }

    int days_cnt = 0;

    model.applyModelParams();

    model.writeOutput();
    model.openGnuPlot(true);

    while (!model.isModellingStopped)
    {
        ++days_cnt;
        model.startNewDay(); // моделирование дня
        model.writeOutput();
        std::cout << "Day " << days_cnt << " ended" << std::endl;
    }
    model.writeGraphvizFile();
    model.writeStateFile();
    model.openGnuPlot(false);
    std::cout << "Modelling ended." << std::endl;
    std::cout << "Output directory : " << data_dir << std::endl;

    system("pause");
    model.clear();
}
