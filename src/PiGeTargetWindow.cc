#include "PiGeTargetWindow.hh"

#include "PiGeMainWindow.hh"

#include "MassFunctions.hh"
#include "Target.hh"

#include <QtWidgets>

PiGeTargetWindow::PiGeTargetWindow(const PiGeMainWindow *mainWindow) : MainWindow(mainWindow) {
   Elements = GetElements();
   MyTarget = const_cast<PiGeMainWindow *>(MainWindow)->GetTarget();

   CreateActions();
   CreateMenu();

   this->setGeometry(300, 300, 500, 500);

   CreateMainLayout();
   CreateTargetLayout();

   QWidget *window = new QWidget();
   window->setLayout(MainWindowArea);
   setCentralWidget(window);
}

void PiGeTargetWindow::CreateMainLayout() {
   MainWindowArea = new QGridLayout;

   int Line = 0;

   auto label = new QLabel();
   auto font = label->font();
   font.setPointSize(14);
   font.setWeight(QFont::Bold);
   label->setFont(font);
   label->setText("Target Information");
   label->setHidden(false);
   label->setAlignment(Qt::AlignLeft);
   MainWindowArea->addWidget(label, Line++, 0, 1, 3);

   label = new QLabel();
   font.setPointSize(12);
   font.setWeight(QFont::Medium);
   label->setFont(font);
   label->setText("Target Thickness: ");
   label->setHidden(false);
   label->setAlignment(Qt::AlignLeft);
   MainWindowArea->addWidget(label, Line, 1, 1, 3);
   TargetThickness = new QLabel();
   TargetThickness->setNum(0);
   TargetThickness->setHidden(false);
   TargetThickness->setAlignment(Qt::AlignLeft);
   MainWindowArea->addWidget(TargetThickness, Line++, 4, 1, 3);

   label = new QLabel();
   label->setFont(font);
   label->setText("Target Number of elements: ");
   label->setHidden(false);
   label->setAlignment(Qt::AlignLeft);
   MainWindowArea->addWidget(label, Line, 1, 1, 3);
   NbOfElements = new QLabel();
   NbOfElements->setNum(0);
   NbOfElements->setHidden(false);
   NbOfElements->setAlignment(Qt::AlignLeft);
   MainWindowArea->addWidget(NbOfElements, Line++, 4, 1, 3);

   label = new QLabel();
   label->setFont(font);
   label->setText("Target Atomic percent: ");
   label->setHidden(false);
   label->setAlignment(Qt::AlignLeft);
   MainWindowArea->addWidget(label, Line, 1, 1, 3);
   AtomicPercentLabel = new QLabel();
   AtomicPercentLabel->setNum(1000);
   AtomicPercentLabel->setHidden(false);
   AtomicPercentLabel->setAlignment(Qt::AlignLeft);
   MainWindowArea->addWidget(AtomicPercentLabel, Line++, 4, 1, 3);

   label = new QLabel();
   font.setPointSize(14);
   font.setWeight(QFont::Bold);
   label->setFont(font);
   label->setText("Target Composition");
   label->setHidden(false);
   label->setAlignment(Qt::AlignLeft);
   MainWindowArea->addWidget(label, Line++, 0, 1, 3);

   RefreshMainLayout();
}

void PiGeTargetWindow::RefreshMainLayout() {
   if (GetThickTargetFlag())
      TargetThickness->setText("Infinite thickness");
   else
      TargetThickness->setNum(MyTarget->GetLayerThickness());
   NbOfElements->setNum(static_cast<int>(MyTarget->GetElementsInLayer().size()));
   AtomicPercentLabel->setNum(MyTarget->GetLayerAtomicPerCent());
}

void PiGeTargetWindow::CreateTargetLayout() {
   TargetWindowArea = new QGridLayout;

   RefreshTargetLayout();

   auto container = new QWidget();
   auto scrollArea = new QScrollArea;
   scrollArea->setWidgetResizable(true);
   scrollArea->setWidget(container);

   container->setLayout(TargetWindowArea);

   MainWindowArea->addWidget(scrollArea, 5, 0, 10, 20);
}

void PiGeTargetWindow::RefreshTargetLayout() {
   if (TargetWindowArea) {
      delete TargetWindowArea;
      TargetWindowArea = new QGridLayout;
      auto container = new QWidget();
      auto scrollArea = new QScrollArea;
      scrollArea->setWidgetResizable(true);
      scrollArea->setWidget(container);

      container->setLayout(TargetWindowArea);

      MainWindowArea->addWidget(scrollArea, 5, 0, 10, 20);
   }
   auto TargetElements = MyTarget->GetElementsInLayer();
   auto MyAlignment = Qt::AlignTop;

   int Column = 0;
   auto label = new QLabel();
   label->setText("Element");
   TargetWindowArea->addWidget(label, 0, Column++, MyAlignment);
   if (!GetYieldCalculationFlag()) {
      label = new QLabel();
      label->setText("Fit State");
      TargetWindowArea->addWidget(label, 0, Column++, MyAlignment);
   }
   label = new QLabel();
   label->setText("Atomic Percent");
   TargetWindowArea->addWidget(label, 0, Column++, MyAlignment);
   label = new QLabel();
   label->setText("Fill Atomic Percent");
   TargetWindowArea->addWidget(label, 0, Column++, MyAlignment);
   label = new QLabel();
   label->setText("Remove Element");
   TargetWindowArea->addWidget(label, 0, Column++, MyAlignment);

   for (int i = 0; i <= TargetElements.size(); ++i) {
      auto SelectElement = new QComboBox();
      SelectElement->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
      SelectElement->setFocusPolicy(Qt::ClickFocus);
      SelectElement->setFixedSize(80, 30);
      SelectElement->addItem(" ");
      for (const auto &el : Elements)
         if (!MyTarget->ElementExistsInLayer(el) || (i < TargetElements.size() && el == TargetElements[i]))
            SelectElement->addItem(QString::fromStdString(el->GetSymbol()));
      if (i < TargetElements.size()) {
         auto symbol = TargetElements[i]->GetSymbol();
         SelectElement->setCurrentText(QString::fromStdString(symbol));
      } else
         SelectElement->setCurrentText(" ");
      connect(SelectElement, &QComboBox::currentIndexChanged, this, [this, i, SelectElement] { SetElement(i, SelectElement->currentText()); });

      QComboBox *ElementFitStatus = nullptr;
      if (!GetYieldCalculationFlag()) {
         ElementFitStatus = new QComboBox();
         ElementFitStatus->addItem("Free");
         ElementFitStatus->addItem("Fit");
         ElementFitStatus->addItem("Fixed");
         ElementFitStatus->setFixedSize(80, 30);
         if (i < TargetElements.size()) {
            auto state = GetFitStateString(MyTarget->GetElementFitStateInLayer(TargetElements[i]));
            ElementFitStatus->setCurrentText(QString::fromStdString(state));
         } else
            ElementFitStatus->setCurrentText("Free");
         //         connect(ElementFitStatus, SIGNAL(activated(int)), this, SLOT(check_linked()));
      }

      auto AtomicPercent = new QDoubleSpinBox();
      AtomicPercent->setRange(0, 100);
      AtomicPercent->setDecimals(2);
      AtomicPercent->setKeyboardTracking(false);
      AtomicPercent->setButtonSymbols(QAbstractSpinBox::NoButtons);
      AtomicPercent->setFixedSize(80, 30);
      if (i < TargetElements.size()) {
         auto atomicPercent = MyTarget->GetElementAtomicPercentInLayer(TargetElements[i]);
         AtomicPercent->setValue(atomicPercent);
      } else
         AtomicPercent->setValue(0.);
      connect(AtomicPercent, &QDoubleSpinBox::valueChanged, this, [this, i, AtomicPercent] { SetElementAtomicPercent(i, AtomicPercent->value()); });

      auto FillButton = new QPushButton("Fill");
      FillButton->setDisabled(false);
      FillButton->setFocusPolicy(Qt::ClickFocus);
      FillButton->setFixedSize(50, 30);
      connect(FillButton, &QPushButton::released, this, [this, i] { FillLayerAtomicPercent(i); });

      auto RemoveButton = new QPushButton("Remove");
      RemoveButton->setDisabled(false);
      RemoveButton->setFocusPolicy(Qt::ClickFocus);
      RemoveButton->setFixedSize(150, 30);
      connect(RemoveButton, &QPushButton::released, this, [this, i] { RemoveElement(i); });

      Column = 0;
      TargetWindowArea->addWidget(SelectElement, i + 1, Column++, MyAlignment);
      if (ElementFitStatus)
         TargetWindowArea->addWidget(ElementFitStatus, i + 1, Column++, MyAlignment);
      TargetWindowArea->addWidget(AtomicPercent, i + 1, Column++, MyAlignment);
      TargetWindowArea->addWidget(FillButton, i + 1, Column++, MyAlignment);
      TargetWindowArea->addWidget(RemoveButton, i + 1, Column++, MyAlignment);
   }
}

void PiGeTargetWindow::CreateActions() {
   ThickTargetAction = CreateAction("Thi&ck Target", "triggered", "SetThickTarget", "Infinite Monolayer Target", "", true, GetThickTargetFlag(), false);
   ThinTargetAction = CreateAction("Thi&n Target", "triggered", "SetThinTarget", "Finite Thickness Target", "", true, !GetThickTargetFlag(), false);
   LoadTargetAction = CreateAction("&Load", "triggered", "LoadTarget", "Load a Target from file", "Ctrl+P", false, false, false);
   SaveTargetAction = CreateAction("&Save", "triggered", "SaveTarget", "Save the Target to file", "Ctrl+S", false, false, false);
}

void PiGeTargetWindow::CreateMenu() {
   auto TargetType = menuBar()->addMenu("Target Type");
   {
      TargetType->addAction(ThickTargetAction);
      TargetType->addAction(ThinTargetAction);
   }
   menuBar()->addAction(LoadTargetAction);
   menuBar()->addAction(SaveTargetAction);
}

bool PiGeTargetWindow::GetThickTargetFlag() {
   return const_cast<PiGeMainWindow *>(MainWindow)->GetThickTargetFlag();
}

bool PiGeTargetWindow::GetYieldCalculationFlag() {
   return const_cast<PiGeMainWindow *>(MainWindow)->GetYieldCalculationFlag();
}

void PiGeTargetWindow::SetThickTarget() {
   const_cast<PiGeMainWindow *>(MainWindow)->SetThickTargetFlag(true);
   ThickTargetAction->setChecked(GetThickTargetFlag());
   ThinTargetAction->setChecked(!GetThickTargetFlag());
   RefreshMainLayout();
}

void PiGeTargetWindow::SetThinTarget() {
   const_cast<PiGeMainWindow *>(MainWindow)->SetThickTargetFlag(false);
   ThickTargetAction->setChecked(GetThickTargetFlag());
   ThinTargetAction->setChecked(!GetThickTargetFlag());
   RefreshMainLayout();
}

void PiGeTargetWindow::LoadTarget() {
}

void PiGeTargetWindow::SaveTarget() {
}

void PiGeTargetWindow::SetElement(int Index, QString ElementSymbol) {
   if (Index < MyTarget->GetElementsInLayer().size())
      MyTarget->ChangeElementInLayer(Index, GetElement(ElementSymbol.toStdString()));
   else
      MyTarget->AddElementToLayer(GetElement(ElementSymbol.toStdString()));
   RefreshMainLayout();
   RefreshTargetLayout();
}

void PiGeTargetWindow::SetElementFitState(int Index, QString FitState) {
   if (Index >= MyTarget->GetElementsInLayer().size())
      return;
   auto el = MyTarget->GetElementsInLayer()[Index];
   auto fitstate = GetFitStateFromString(FitState.toStdString());
   MyTarget->SetElementFitStateInLayer(el, fitstate);
   RefreshMainLayout();
   RefreshTargetLayout();
}

void PiGeTargetWindow::SetElementAtomicPercent(int Index, double value) {
   if (Index >= MyTarget->GetElementsInLayer().size())
      return;
   auto el = MyTarget->GetElementsInLayer()[Index];
   MyTarget->SetElementAtomicPercentInLayer(el, value);
   RefreshMainLayout();
   RefreshTargetLayout();
}

void PiGeTargetWindow::FillLayerAtomicPercent(int Index) {
   if (Index >= MyTarget->GetElementsInLayer().size())
      return;
   auto curAtomic = MyTarget->GetLayerAtomicPerCent();
   auto el = MyTarget->GetElementsInLayer()[Index];
   curAtomic -= MyTarget->GetElementAtomicPercentInLayer(el);
   SetElementAtomicPercent(Index, 100 - curAtomic);
}

void PiGeTargetWindow::RemoveElement(int Index) {
   if (Index >= MyTarget->GetElementsInLayer().size())
      return;
   auto el = MyTarget->GetElementsInLayer()[Index];
   MyTarget->RemoveElementFromLayer(el);
   RefreshMainLayout();
   RefreshTargetLayout();
}
