#pragma once

#include <vector>
#include <list>
#include <fstream>

class Person;
class Location;

// Класс модель
// Содержит список всех агентов, список всех локаций
// Управляет моделированием
class Model
{
public:
    ~Model();

    // функция получения ссылки на инстанс
    static Model &instance();

    // Начать моделирование следующего дня
    void startNewDay();

    // Вывести в выходной файл статистику текущего дня моделирования
    void writeOutput();

    // Вывести в выходной файл
    void writeGraphvizFile();

    //Установить новую директорию проекта
    void SetProjectDir(const std::string& new_path);

public:
    bool isModellingStopped;                // признак остановки моделирования
    int m_currentDay;                       // номер текущего дня
    int personNumber;                       // количество агентов в моделировании
    std::vector<Person *> allPersons;       // вектор всех агентов (позиция в векторе = идентификатору агента, могут быть пустые записи)
    std::vector<Location *> allLocations;   // вектор всех локаций (позиция в векторе = идентификатору локации, могут быть пустые записи)

    std::list<Person *> exposedPersons;     // список контактных на текущий день моделирования
    std::list<Person *> infectedPersons;    // список инфицированных на текущий день моделирования
    std::list<Person *> recoveredPersons;   // список выздоровевших на текущий день моделирования
    std::list<Person *> deadPersons;        // список умерших на текущий день моделирования

private:
    Model();
    Model(const Model&);

private:
    Model *m_instance;              // Указатель на инстанс, для реализации синглтона
    std::ofstream *m_outputStream;  // Указатель на поток файла выходных данных
    std::string data_dir;           // Директория проекта
};


bool eventWithProbability(double probability);
