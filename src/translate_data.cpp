#include "translate_data.h"

#include <iostream>
#include <filesystem>
#include <direct.h>

#include "lib_migration.h"

namespace translate
{
    void ParseCmdLine(int argc, char* argv[], std::string& data_dir)
    {
        std::string default_path = "\\config\\";

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
                if (default_path[0] != '/' && default_path[0] != '\\')
                {
                    default_path = "\\" + default_path;
                }
                if (default_path[default_path.length() - 1] != '/' && default_path[default_path.length() - 1] != '\\')
                {
                    default_path += "\\";
                }
            }
            else if (s == "-d")
            {
                // для отладки
                std::cout << "Enter path: " << std::endl;
                std::cin >> default_path;
                default_path += "\\";
            }
            else
            {
                std::cout << "Unknown parameter: '" << argv[i] << "'. Use '/?' for help. " << std::endl;
                //exit(0);
            }
        }


        char buff[FILENAME_MAX];
        _getcwd(buff, FILENAME_MAX);
        std::string currentDir(buff);

        data_dir = currentDir + default_path;
    }
};
