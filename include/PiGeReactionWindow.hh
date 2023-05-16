#ifndef PiGeReactionWindow_hh
#define PiGeReactionWindow_hh 1

#include <QMainWindow>

class PiGeMainWindow;

class PiGeReactionWindow : public QMainWindow {
public:
   PiGeReactionWindow(const PiGeMainWindow *mainWindow);

protected:
private:
   const PiGeMainWindow *MainWindow;
public slots:
private slots:
};

#endif