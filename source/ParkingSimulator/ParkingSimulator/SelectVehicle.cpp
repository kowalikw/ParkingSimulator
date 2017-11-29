#include "SelectVehicle.h"

SelectVehicle::SelectVehicle(VehicleEditor *vehicleEditor, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnLoadFromVehicleEditor, SIGNAL(released()), this, SLOT(loadVehicleFromVehicleEditor()));
	connect(ui.btnLoadFromFile, SIGNAL(released()), this, SLOT(loadVehicleFromFile()));
	connect(ui.btnCancel, SIGNAL(released()), this, SLOT(reject()));

	ui.btnLoadFromVehicleEditor->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_SetVehicle_FromVehicleEditor"]));
	ui.btnLoadFromFile->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_SetVehicle_FromFile"]));
	ui.btnCancel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_Cancel"]));

	this->vehicleEditor = vehicleEditor;
}

SelectVehicle::~SelectVehicle()
{
}

Vehicle * SelectVehicle::GetVehicle()
{
	return this->vehicle;
}

void SelectVehicle::loadVehicleFromVehicleEditor()
{
	this->vehicle = vehicleEditor->GetVehicle();

	if (vehicle != NULL && vehicle->GetWheelbase() > 0 && vehicle->GetTrack() > 0 && vehicle->GetVehicleModel() != NULL)
		accept();
	else
	{
		WarningErrorMsg warningWindow(Language::getInstance()->GetDictionary()["WarningError_NoVehicleInVehicleEditor_Title"], Language::getInstance()->GetDictionary()["WarningError_NoVehicleInVehicleEditor_Content"], MessageType::Warning);
		warningWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		warningWindow.exec();
	}
}

void SelectVehicle::loadVehicleFromFile()
{
	QString filter = tr("Vehicle files (*.vehicle)");
	QString filePath = QFileDialog::getOpenFileName(this, "Open vehicle", QString(), tr("All files (*.*);;Vehicle files (*.vehicle)"), &filter);

	if (filePath != "")
	{
		this->vehicle = new Vehicle();

		std::ifstream ifs(filePath.toStdString());

		boost::archive::text_iarchive ia(ifs);

		ia >> *vehicle;

		accept();
	}
}