#include "PiGeMainWindow.hh"

#include "PiGeTargetWindow.hh"

#include "MassFunctions.hh"
#include "Stopping.hh"

#include <QtWidgets>

#define XSTR(x) STR(x)
#define STR(x) #x

PiGeMainWindow::PiGeMainWindow() {
   MyStopping = Stopping::GetInstance();
   MyStopping->SetStopping("SRIM");
   MyTarget = new Target();
   BeamIsotope = GetIsotope("1H");

   TargetWindow = new PiGeTargetWindow(this);

   setWindowTitle(XSTR(PROJECTNAME) " v" XSTR(VERSION));

   CreateActions();
   CreateMenu();

   QGridLayout *layout = new QGridLayout;

   QPushButton *open_button = new QPushButton();
   open_button->setDisabled(false);
   open_button->setFixedSize(120, 30);
   open_button->setSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding);
   open_button->setText(tr("Select Reaction"));
   layout->addWidget(open_button, 1, 2);
   layout->setAlignment(open_button, Qt::AlignCenter);
   QWidget *window = new QWidget();
   window->setLayout(layout);
   setCentralWidget(window);
}

PiGeMainWindow::~PiGeMainWindow() {
   delete TargetWindow;
   delete MyTarget;
   delete MyStopping;
}

void PiGeMainWindow::CreateActions() {
   OpenTargetWindowAction = CreateAction("&Target", "triggered", "OpenTargetWindow", "Build Target", "Ctrl+T", false);

   YieldCalculationAction = CreateAction("&Yield Calculation", "triggered", "SetYieldCalculation", "Yield Calculation", "", true, true, false);
   TargetCompositionCalculationAction = CreateAction("&Target Composition Calculation", "triggered", "SetTargetCompositionCalculation", "Target Composition Calculation", "", true, false, false);

   ProtonBeamAction = CreateAction("&Proton beam", "triggered", "SetProtonBeam", "Proton Incident Beam", "", true, true, false);
   DeutronBeamAction = CreateAction("&Deuteron beam", "triggered", "SetDeutronBeam", "Deuteron Incident Beam", "", true, false, false);

   ZBLStoppingAction = CreateAction("&ZBL", "triggered", "SetZBLStopping", "ZBL stopping power", "", true, false, false);
   SRIMStoppingAction = CreateAction("&SRIM", "triggered", "SetSRIMStopping", "SRIM stopping power", "", true, true, false);

   StragglingAction = CreateAction("St&raggling", "", "", "Include straggling calculation", "", true, true, false);

   AboutInfoAction = CreateAction("&About", "triggered", "ShowAboutInfo", "About the program", "Ctrl+A", false, false, false);
}

void PiGeMainWindow::CreateMenu() {
   menuBar()->addAction(OpenTargetWindowAction);

   auto OptionsMenu = menuBar()->addMenu("&Options");
   {
      auto BeamMenu = OptionsMenu->addMenu("&Incident Beam");
      BeamMenu->addAction(ProtonBeamAction);
      BeamMenu->addAction(DeutronBeamAction);

      auto CalculationType = OptionsMenu->addMenu("&Calculation Type");
      CalculationType->addAction(YieldCalculationAction);
      CalculationType->addAction(TargetCompositionCalculationAction);

      auto StoppingMenu = OptionsMenu->addMenu("&Stopping Power");
      StoppingMenu->addAction(ZBLStoppingAction);
      StoppingMenu->addAction(SRIMStoppingAction);

      OptionsMenu->addAction(StragglingAction);
   }

   auto helpMenu = menuBar()->addMenu("&Help");
   {
      helpMenu->addAction(AboutInfoAction);
   }
}

void PiGeMainWindow::ShowAboutInfo() {
   QMessageBox msgBox;
   msgBox.setWindowTitle("About");
   msgBox.setIcon(QMessageBox::Information);
   msgBox.setText(XSTR(PROJECTNAME) " program version " XSTR(VERSION) "\nA program for PiGe calculations");
   msgBox.exec();
}

void PiGeMainWindow::OpenTargetWindow() {
   if (!TargetWindow->isVisible())
      TargetWindow->show();
   else
      TargetWindow->activateWindow();
}

void PiGeMainWindow::SetYieldCalculation() {
   YieldCalculationFlag = true;
   YieldCalculationAction->setChecked(YieldCalculationFlag);
   TargetCompositionCalculationAction->setChecked(!YieldCalculationFlag);
}

void PiGeMainWindow::SetTargetCompositionCalculation() {
   YieldCalculationFlag = false;
   YieldCalculationAction->setChecked(YieldCalculationFlag);
   TargetCompositionCalculationAction->setChecked(!YieldCalculationFlag);
}

void PiGeMainWindow::SetProtonBeam() {
   ProtonBeamAction->setChecked(true);
   DeutronBeamAction->setChecked(false);
   BeamIsotope = GetIsotope("1H");
}

void PiGeMainWindow::SetDeutronBeam() {
   ProtonBeamAction->setChecked(false);
   DeutronBeamAction->setChecked(true);
   BeamIsotope = GetIsotope("2H");
}

void PiGeMainWindow::SetZBLStopping() {
   SRIMStoppingAction->setChecked(false);
   ZBLStoppingAction->setChecked(true);
   MyStopping->SetStopping("ZBL");
}

void PiGeMainWindow::SetSRIMStopping() {
   SRIMStoppingAction->setChecked(true);
   ZBLStoppingAction->setChecked(false);
   MyStopping->SetStopping("SRIM");
}
