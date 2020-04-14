#pragma once

class Person;
class Location;
class Model;

// класс генератор структуры общества по заданным параметрам
// класс считывает входные файлы с описанием агентов и локаций и создает соответствующих агентов в модели
// входные параметры: общее число агентов, семей, площадок, отделов на площадке, групп в отделе,
// столовых, работников архива, магазинов и т.д.
class Generator
{
public:
    Generator() {}
    ~Generator() {}

    Model *m_model;   // Полная модель
};
