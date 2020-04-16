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
		float alpha;
		float betta;
	};

	/*
		Функция осуществляет перевод данных из библиотеки миграции в библиотеку заражения
	*/
	int translate_data(Model* model, HumansMigration *phm);//конвертация конфигурационных данных

	int translate_step (Model* model, HumansMigration* phm, int day_idx); // трансляция данных о ежедневном расписании
	void ParseCmdLine(int argc, char* argv[], std::string& file_name1, std::string& file_name2, struct SEIRDcfg& seird_cfg);
};
