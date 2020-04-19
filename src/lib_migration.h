#include "data_types.h"
#include <string>

class HumansConfigReader
{
public:
    HumansConfigReader() {}

    //
	bool readHumanConfig(const std::string &file_name, bool isWorkingDay);

    //
    bool readModelConfig(const std::string &file_name);

protected:
    LocationConfig* TryGetLocationConfigFromString(std::string &data_string);
};

