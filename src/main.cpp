#include "Person.h"
#include "Location.h"
#include "Model.h"
#include "Generator.h"

int main()
{
    Generator generator;
    // generator.setAgentFile()
    // generator.setModelFile()
    // generator.generate()

    Model &model = Model::instance();
    model.writeOutput();

    while (!model.isModellingStopped)
    {
        model.startNewDay(); // моделирование дня
        model.writeOutput();
    }

    system("pause");
    return 0;
}