#include <QCoreApplication>
#include "generation.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Generation generation("./examples/inputs", "./examples/templates", "./config.json", "./examples/outputs", "./examples");
    generation.generate();

    return 0;
}
