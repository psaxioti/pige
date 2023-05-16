#include "MyMainWindow.hh"

#include <QtWidgets>

MyMainWindow::MyMainWindow() {
}

QAction *MyMainWindow::CreateAction(const QString &ActionName, const QString &Signal, const QString &Slot,
                                    const QString &StatusTip, const QString &Shortcut,
                                    const bool &Checkable, const bool &Checked, const bool &Disabled) {
   QAction *action = new QAction(ActionName, this);
   action->setCheckable(Checkable);
   action->setChecked(Checked);
   action->setDisabled(Disabled);
   if (!StatusTip.isEmpty())
      action->setStatusTip(StatusTip);
   if (!Shortcut.isEmpty())
      action->setShortcut(Shortcut);

   if (Signal.isEmpty() || Slot.isEmpty())
      return action;

   QString LocalSignal = Signal;
   QString LocalSlot = Slot;
   LocalSignal.prepend("2");
   LocalSignal.append("()");
   LocalSlot.prepend("1");
   LocalSlot.append("()");
   connect(action, LocalSignal.toStdString().c_str(), this, LocalSlot.toStdString().c_str());

   return action;
}
