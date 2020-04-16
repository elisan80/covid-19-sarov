#include "translate_data.h"
#include <iostream> 
#include "lib_migration.h" 
#include "Model.h"
#include "Person.h"
#include "Location.h"

namespace translate
{
    void ParseCmdLine(int argc, char* argv[], std::string& data_dir, std::string& file_name1, std::string& file_name2, struct SEIRDcfg& seird_cfg)
    {
        std::string default_path = "./config/";
        std::string default_filename1 = "humans_work.dat";
        std::string default_filename2 = "humans_holiday.dat";

        SEIRDcfg seird_default;
        seird_default.alpha = 0.1; //заполнить здесь правильными значениями
        seird_default.betta = 0.2;

        for (int i = 1; i < argc; ++i)
        {
            std::string s(argv[i]);
            if (s == "-h" || s == "/?" || s == "--help")
            {
                std::cout << "Usage: [-p Path] [-h] [--help] [/?]" << std::endl;
                std::cout << "Path - this is path to configuration files" << std::endl;
            }
            else if (s == "-p")
            {//получить путь до конфигурационных файлов
                ++i;
                if (i >= argc)
                {
                    std::cout << "Usage: [-p Path] [-h] [--help] [/?]" << std::endl;
                    std::cout << "There is no value for Path." << std::endl;
                    exit(0);
                }
               
                default_path = argv[i];
                if (default_path[default_path.length() - 1] != '/' && default_path[default_path.length() - 1] != '\\')
                {
                    default_path += "/";
                }
            }
            else if (s == "-d")
            {
                // для отладки
                std::cout << "Enter path: " << std::endl;
                std::cin >> default_path;
                default_path += "/";
            }
            else
            {
                std::cout << "Unknown parameter: '" << argv[i] << "'. Use '/?' for help. " << std::endl;
                //exit(0);
            }
        }

        data_dir = default_path;
        file_name1 = default_path + default_filename1;
        file_name2 = default_path + default_filename2;
        seird_cfg = seird_default;
    }
};
