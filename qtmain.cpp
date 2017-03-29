#include <QApplication>
#include <QTimer>
#include "qtdialog.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  qtdialog d;
  if (argc > 1)
  {
    //Profile for 10 secs
    QTimer::singleShot(10000, &d, SLOT(close()));
  }
  d.show();

  return a.exec();
}
