#ifndef MyMainWindow_hh
#define MyMainWindow_hh 1

#include <QMainWindow>

class MyMainWindow : public QMainWindow {
   Q_OBJECT
public:
   MyMainWindow();

protected:
   QAction *CreateAction(const QString &ActionName, const QString &Signal, const QString &Slot,
                         const QString &StatusTip = "", const QString &Shortcut = "",
                         const bool &Checkable = true, const bool &Checked = true, const bool &Disabled = false);
};

#endif