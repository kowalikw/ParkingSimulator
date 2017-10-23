#include "AddSimulation.h"
#include "Language.h"

AddSimulation::AddSimulation(PathPlanner *pathPlanner, QDialog *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnLoadFromPathPlanner, SIGNAL(released()), this, SLOT(loadSimulationFromPathPlanner()));
	connect(ui.btnLoadFromFile, SIGNAL(released()), this, SLOT(loadSimulationFromFile()));
	connect(ui.btnCancel, SIGNAL(released()), this, SLOT(reject()));

	ui.btnCancel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Visualisation_AddSimulation_FromPathPlanner"]));
	ui.btnCancel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Visualisation_AddSimulation_FromFile"]));
	ui.btnCancel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Cancel"]));

	this->pathPlanner = pathPlanner;
}

AddSimulation::~AddSimulation()
{
}

Simulation * AddSimulation::GetSimulation()
{
	return this->simulation;
}

void AddSimulation::loadSimulationFromPathPlanner()
{
	this->simulation = pathPlanner->GetSimulation();
	this->simulation->SetSimulationTime(10);
	this->simulation->SetName("Simulation from path planner");

	if (simulation != NULL && simulation->GetMap() != NULL && simulation->GetVehicle() != NULL && simulation->GetFinalPath() != NULL) //TODO:
		accept();
	else
	{
		WarningErrorMsg warningWindow(Language::getInstance()->GetDictionary()["WarningError_NoSimulation_Title"], Language::getInstance()->GetDictionary()["WarningError_NoSimulation_Content"], MessageType::Warning);
		warningWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		warningWindow.exec();
	}
}

void AddSimulation::loadSimulationFromFile()
{
	QString filter = tr("Simulation files (*.simulation)");
	QString filePath = QFileDialog::getOpenFileName(this, "Open simulation", QString(), tr("All files (*.*);;Simulation files (*.simulation)"), &filter);

	if (filePath != "")
	{
		this->simulation = new Simulation();

		std::ifstream ifs(filePath.toStdString());

		boost::archive::text_iarchive ia(ifs);

		ia >> *simulation;

		this->simulation->SetSimulationTime(10);
		this->simulation->SetName(filePath.toStdString());

		accept();
	}
}
