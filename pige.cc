#include <QApplication>

#include "PiGeMainWindow.hh"

int main(int argc, char *argv[]) {
   QApplication Application(argc, argv);

   Application.setApplicationName("PiGe Calculations");

   PiGeMainWindow MainWindow;

   MainWindow.setFixedSize(800, 550);

   MainWindow.show();

   return Application.exec();
}
