#pragma once
#include <string>

class Model;
class HumansMigration;

namespace translate
{
	/* Структура для получения коэффициентов для SEIRD модели из конфигурационного файла.
	*/
	struct SEIRDcfg
	{
		double alpha;
        double betta;
	};
    
    void ParseCmdLine(int argc, char* argv[], std::string &data_dir, std::string& file_name1, std::string& file_name2, struct SEIRDcfg& seird_cfg);
};
