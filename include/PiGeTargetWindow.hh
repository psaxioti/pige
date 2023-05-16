#ifndef PiGeTargetWindow_hh
#define PiGeTargetWindow_hh 1

#include "MyMainWindow.hh"

class PiGeMainWindow;
class Target;
class Element;

class QGridLayout;
class QLabel;

class PiGeTargetWindow : public MyMainWindow {
   Q_OBJECT
public:
   PiGeTargetWindow(const PiGeMainWindow *mainWindow);

protected:
private:
   const PiGeMainWindow *MainWindow;
   std::vector<Element *> Elements;
   Target *MyTarget;

   bool GetThickTargetFlag();
   bool GetYieldCalculationFlag();

   void CreateMainLayout();
   void RefreshMainLayout();
   QGridLayout *MainWindowArea;
   QLabel *TargetThickness;
   QLabel *NbOfElements;
   QLabel *AtomicPercentLabel;
   void CreateTargetLayout();
   void RefreshTargetLayout();
   QGridLayout *TargetWindowArea;

   void CreateActions();
   QAction *ThickTargetAction;
   QAction *ThinTargetAction;
   QAction *LoadTargetAction;
   QAction *SaveTargetAction;

   void CreateMenu();

public slots:

private slots:
   void SetThickTarget();
   void SetThinTarget();
   void LoadTarget();
   void SaveTarget();

   void SetElement(int Index, QString ElementSymbol);
   void SetElementFitState(int Index, QString FitState);
   void SetElementAtomicPercent(int Index, double value);
   void FillLayerAtomicPercent(int Index);
   void RemoveElement(int Index);
};

#endif