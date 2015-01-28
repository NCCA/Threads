#include <QApplication>
#include "MyThread.h"
int main(int argc, char **argv)
{
QApplication a(argc, argv);
MyThread t;
t.start();
MyThread v(99);
t.start();
return a.exec();
}
