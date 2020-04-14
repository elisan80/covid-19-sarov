#pragma once

enum SEIR_State
{
	Susceptible = 0,    // Восприимчивый
	Exposed,			// Контактный
	Infectious,			// Инфицированный
	Recovered,			// Выздоровевший
	Dead				// Умерший
};

const double EtoSProbability = 0.05;                // вероятность восприимчивому заразиться при встрече с контактным
const double ItoSProbability = 0.35;                // вероятность восприимчивому заразиться при встрече с инфицированным
const double RtoSProbability = 0.35;                // вероятность восприимчивому заразиться при встрече с выздоровевшим
const double ItoRProbabilityWithMedic = 0.10;       // вероятность выздоровления при больничном лечении
const double ItoRProbability = 0.03;                // вероятность выздоровления без больничного лечения
const double ItoDProbabilityWithMedic = 0.015;      // вероятность смерти при больничном лечении
const double ItoDProbability = 0.10;                // вероятность смерти без больничного лечения

const double HardSymptomsProbability = 0.16;        // Вероятность возникновения тяжелой симптоматики
const double EtoIDuration = 5.2;                    // Средняя длительность инкубационного периода
const double EtoSymptomsDuration = 8.0;             // Среднее время до проявления симптомов в днях
