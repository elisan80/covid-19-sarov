#include "data_types.h"
#include <iostream>
#include <string>
#include <vector>

class HumansConfigReader
{
public:
    HumansConfigReader() {}
	bool ReadCfg(const std::string &file_name, bool isWorkingDay);

    LocationConfig* TryGetLocationConfigFromString(std::string &data_string);
};

