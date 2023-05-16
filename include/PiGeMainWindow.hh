#ifndef PiGeMainWindow_hh
#define PiGeMainWindow_hh 1

#include "MyMainWindow.hh"

class Stopping;
class Element;
class Isotope;
class Target;

class PiGeTargetWindow;

class PiGeMainWindow : public MyMainWindow {
   Q_OBJECT

public:
   PiGeMainWindow();
   ~PiGeMainWindow();

   Target *GetTarget() { return MyTarget; }

   void SetThickTargetFlag(bool NewThickTargetFlag) { ThickTargetFlag = NewThickTargetFlag; }
   bool GetThickTargetFlag() { return ThickTargetFlag; }
   bool GetYieldCalculationFlag() { return YieldCalculationFlag; }

protected:
private:
   Stopping *MyStopping;
   Target *MyTarget;
   Isotope *BeamIsotope;

   bool YieldCalculationFlag = true;
   bool ThickTargetFlag = true;

   PiGeTargetWindow *TargetWindow = nullptr;

   void CreateActions();
   QAction *AboutInfoAction;
   QAction *OpenTargetWindowAction;
   QAction *YieldCalculationAction;
   QAction *TargetCompositionCalculationAction;
   QAction *ProtonBeamAction;
   QAction *DeutronBeamAction;
   QAction *ZBLStoppingAction;
   QAction *SRIMStoppingAction;
   QAction *StragglingAction;

   void CreateMenu();

public slots:

private slots:
   void ShowAboutInfo();
   void OpenTargetWindow();
   void SetYieldCalculation();
   void SetTargetCompositionCalculation();
   void SetProtonBeam();
   void SetDeutronBeam();
   void SetZBLStopping();
   void SetSRIMStopping();
};

#endif
