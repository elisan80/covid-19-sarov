#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>

#include "lib_migration.h"
#include "Model.h"
#include "Person.h"
#include "Location.h"

LocationConfig* HumansConfigReader::TryGetLocationConfigFromString(std::string &data_string)
{
    Model &model = Model::instance();
	std::istringstream iss(data_string);
    LocationConfig *config = nullptr;
	std::string s;
	try
	{
		iss >> s;
		int location_id = std::stoi(s) - LOCATIONS_OFFSET;
        Location *currentLocation = model.allLocations[location_id];
        if (!currentLocation)
        {
            currentLocation = new Location();
            currentLocation->index = location_id;
            model.allLocations[location_id] = currentLocation;
        }

        time_t start_time;
		iss >> s;
		if (s.find(":") == std::string::npos)
			return false;
		else
		{
			s[2] = ' ';
			std::istringstream iss2 (s);
			int hour_1, min_1;
			iss2 >> hour_1 >> min_1;
			start_time = hour_1 * 3600 + min_1 * 60;
		}
	
        time_t end_time;
		iss >> s; //time end
		if (s.find(":") == std::string::npos)
			return false;
		else
		{
			s[2] = ' ';
			std::istringstream iss2 (s);
			int hour_2, min_2;
			iss2 >> hour_2 >> min_2;
			end_time = hour_2 * 3600 + min_2 * 60;
		}

        int min_time;
		iss >> min_time;
        int max_time;
		iss >> max_time;
        double probability;
		iss >> probability;

        config = new LocationConfig();
        config->location = currentLocation;
        config->start_time = start_time;
        config->end_time = end_time;
        config->min_time = min_time;
        config->max_time = max_time;
        config->probability = probability;
	}
    catch (...)
	{
		return nullptr;
	}
	
	return config;
}

bool HumansConfigReader::ReadCfg(const std::string &file_name, bool isWorkingDay)
{//Return value:
//true - все хорошо
//false - все плохо

    std::ifstream ifs (file_name, std::ifstream::in);
    if (!ifs.is_open())
        return false;

    Model &model = Model::instance();

	std::string buf;
	std::string pattern = "[human_id=";
	HumanConfig humanConfig;
	
	while (ifs)
	{
        int currentHumanID = -1;
        Person *currentPerson;
		std::getline(ifs, buf);

        if (buf[0] == '[')
        {
            //найдено начало новой секции по человеку
            int pos1 = buf.find_first_of('=') + 1;
            int pos2 = buf.find_first_of(' ');
            if (pos1 != std::string::npos && pos2 != std::string::npos)
            {
                //найти ИД человека
                std::string s1 = buf.substr(pos1, pos2 - pos1);
                currentHumanID = std::stoi(s1);
                currentPerson = model.allPersons[currentHumanID];
                if (!currentPerson)
                {
                    model.allPersons[currentHumanID] = new Person();
                    currentPerson = model.allPersons[currentHumanID];
                    currentPerson->index = currentHumanID;
                    //найти ИД дома
                    int pos3 = buf.find_last_of('=') + 1;
                    int pos4 = buf.find_first_of(']');
                    if (pos3 != std::string::npos && pos4 != std::string::npos)
                    {
                        std::string s2 = buf.substr(pos3, pos4 - pos3);
                        int home_id = std::stoi(s2) - LOCATIONS_OFFSET;
                        Location *currentLocation = model.allLocations.at(home_id);
                        if (!currentLocation)
                        {
                            Home *currentHome = new Home();
                            currentHome->index = home_id;
                            currentHome->m_personList.push_back(currentPerson);
                            currentPerson->m_home = currentHome;
                            model.allLocations[home_id] = currentHome;
                        }
                        else
                        {
                            Home *currentHome = dynamic_cast<Home *>(currentLocation);
                            currentHome->m_personList.push_back(currentPerson);
                            currentPerson->m_home = currentHome;
                        }
                    }
                }
            }
        }
		else if (buf[0] != '{' && buf[0] != '}')
		{
            LocationConfig *config = TryGetLocationConfigFromString(buf);
            if (config)
            {
                if (config->probability == -1)
                {
                    // это работа
                    currentPerson->m_work = config->location;
                    config->min_time = config->max_time;
                    config->probability = 1.0;
                }

                if (isWorkingDay)
                {
                    if (!currentPerson->workingDayConfig)
                        currentPerson->workingDayConfig = new HumanConfig();
                    currentPerson->workingDayConfig->locations.push_back(config);
                }
                else
                {
                    if (!currentPerson->dayOffConfig)
                        currentPerson->dayOffConfig = new HumanConfig();
                    currentPerson->dayOffConfig->locations.push_back(config);
                }
            }
		}
        buf.clear();
	}
	
	return true;
}
