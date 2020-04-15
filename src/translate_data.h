#pragma once

class Model;
class HumansMigration;

/*
	‘ункци€ осуществл€ет перевод данных из библиотеки миграции в библиотеку заражени€
*/
namespace translate
{
	int translate_data(Model* model, HumansMigration *phm);//конвертаци€ конфигурационных данных
	int translate_step (Model* model, HumansMigration* phm, int day_idx); // трансл€ци€ данных о ежедневном расписании
};
