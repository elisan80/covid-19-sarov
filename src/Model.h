#pragma once

#include <vector>

class Person;
class Location;

// Класс модель
// Содержит список всех агентов, список всех локаций
// Управляет моделированием
class Model
{
public:
    static Model &instance();

    // Начать моделирование следующего дня
    void startNewDay();

public:
    int m_currentDay;
    std::vector<Person *> allPersons;
    std::vector<Location *> allLocations;

private:
    Model();
    Model(const Model&);

private:
    Model *m_instance;
};
