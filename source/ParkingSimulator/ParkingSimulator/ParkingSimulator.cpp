#include "ParkingSimulator.h"

ParkingSimulator::ParkingSimulator(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	this->setStyleSheet("background-color: #2a2a2a;");

	Language::getInstance()->LoadLanguage(Settings::getInstance()->GetLanguage());

	((MapEditorGLHost*)ui.glMapEditor)->SetMapEditor(&mapEditor);
	((VehicleEditorGLHost*)ui.glVehicleEditor)->SetVehicleEditor(&vehicleEditor);
	((PathPlannerGLHost*)ui.glPathPlanner)->SetPathPlanner(&pathPlanner);
	((VisualisationGLHost*)ui.glVisualisation)->SetVisualisation(&visualisation);
	((Visualisation3DGLHost*)ui.glVisualisation3D)->SetVisualisation(&visualisation);

	renderTimer = new QTimer();
	renderTimer->setInterval(10);
	renderTimer->start();

	render3DTimer = new QTimer();
	render3DTimer->setInterval(10);
	render3DTimer->start();

	updateTimer = new QTimer();
	updateTimer->setInterval(20);
	updateTimer->start();

	connect(renderTimer, SIGNAL(timeout()), this, SLOT(renderTimerCall()));
	connect(render3DTimer, SIGNAL(timeout()), this, SLOT(render3DTimerCall()));
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateTimerCall()));

	// Start
	connect(ui.btnGoToMapEditor, SIGNAL(released()), this, SLOT(goToMapEditor()));
	connect(ui.btnGoToVehicleEditor, SIGNAL(released()), this, SLOT(goToVehicleEditor()));
	connect(ui.btnGoToPathPlanner, SIGNAL(released()), this, SLOT(goToPathPlanner()));
	connect(ui.btnGoToVisualisation, SIGNAL(released()), this, SLOT(goToVisualisation()));
	connect(ui.btnGoToSettings, SIGNAL(released()), this, SLOT(goToSettings()));
	connect(ui.btnClose, SIGNAL(released()), this, SLOT(close()));

	// Map Editor
	connect(ui.btnNewMap, SIGNAL(released()), this, SLOT(createMap()));
	connect(ui.btnSaveMap, SIGNAL(released()), this, SLOT(saveMap()));
	connect(ui.btnLoadMap, SIGNAL(released()), this, SLOT(loadMap()));
	connect(ui.btnClearMap, SIGNAL(released()), this, SLOT(clearMap()));
	connect(ui.btnAddBuilding, SIGNAL(released()), this, SLOT(addBuilding()));
	connect(ui.btnAddDecoration, SIGNAL(released()), this, SLOT(addDecoration()));
	connect(ui.btnAddParkPlace, SIGNAL(released()), this, SLOT(addParkPlace()));
	connect(ui.btnAddCar, SIGNAL(released()), this, SLOT(addCar()));	
	connect(ui.btnAddTerrain, SIGNAL(released()), this, SLOT(addTerrain()));
	connect(ui.btnMapElementRemove, SIGNAL(released()), this, SLOT(mapElementRemove()));
	connect(ui.btnMapElementSaveProperties, SIGNAL(released()), this, SLOT(mapElementSaveProperties()));
	connect(ui.treeMapElements, SIGNAL(itemSelectionChanged()), this, SLOT(treeMapElementsSelectionChanged()));
	ui.treeMapElements->setColumnCount(1);
	ui.treeMapElements->insertTopLevelItems(0, *mapEditor.GetMapElementsTreeItems());

	// Vehicle Editor
	connect(ui.btnNewVehicle, SIGNAL(released()), this, SLOT(newVehicle()));
	connect(ui.btnOpenVehicle, SIGNAL(released()), this, SLOT(openVehicle()));
	connect(ui.btnSaveVehicle, SIGNAL(released()), this, SLOT(saveVehicle()));
	connect(ui.btnApplyVehicleProperties, SIGNAL(released()), this, SLOT(applyVehicleProperties()));
	connect(ui.btnLoadVehicleModel, SIGNAL(released()), this, SLOT(loadVehicleModel()));
	connect(ui.btnLoadVehicleLeftFrontWheelModel, SIGNAL(released()), this, SLOT(loadVehicleLeftFrontWheelModel()));
	connect(ui.btnLoadVehicleRightFrontWheelModel, SIGNAL(released()), this, SLOT(loadVehicleRightFrontWheelModel()));
	connect(ui.btnLoadVehicleLeftRearWheelModel, SIGNAL(released()), this, SLOT(loadVehicleLeftRearWheelModel()));
	connect(ui.btnLoadRightRearWheelModel, SIGNAL(released()), this, SLOT(loadVehicleRightRearWheelModel()));
	connect(ui.vehicleFrontLeftWheelPositionX, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontLeftWheelPropertiesChanged()));
	connect(ui.vehicleFrontLeftWheelPositionY, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontLeftWheelPropertiesChanged()));
	connect(ui.vehicleFrontLeftWheelPositionZ, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontLeftWheelPropertiesChanged()));
	connect(ui.vehicleFrontLeftWheelRotationX, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontLeftWheelPropertiesChanged()));
	connect(ui.vehicleFrontLeftWheelRotationY, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontLeftWheelPropertiesChanged()));
	connect(ui.vehicleFrontLeftWheelRotationZ, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontLeftWheelPropertiesChanged()));
	connect(ui.vehicleFrontLeftWheelScaleX, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontLeftWheelPropertiesChanged()));
	connect(ui.vehicleFrontLeftWheelScaleY, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontLeftWheelPropertiesChanged()));
	connect(ui.vehicleFrontLeftWheelScaleZ, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontLeftWheelPropertiesChanged()));
	connect(ui.vehicleFrontRightWheelPositionX, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontRightWheelPropertiesChanged()));
	connect(ui.vehicleFrontRightWheelPositionY, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontRightWheelPropertiesChanged()));
	connect(ui.vehicleFrontRightWheelPositionZ, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontRightWheelPropertiesChanged()));
	connect(ui.vehicleFrontRightWheelRotationX, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontRightWheelPropertiesChanged()));
	connect(ui.vehicleFrontRightWheelRotationY, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontRightWheelPropertiesChanged()));
	connect(ui.vehicleFrontRightWheelRotationZ, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontRightWheelPropertiesChanged()));
	connect(ui.vehicleFrontRightWheelScaleX, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontRightWheelPropertiesChanged()));
	connect(ui.vehicleFrontRightWheelScaleY, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontRightWheelPropertiesChanged()));
	connect(ui.vehicleFrontRightWheelScaleZ, SIGNAL(valueChanged(double)), this, SLOT(vehicleFrontRightWheelPropertiesChanged()));
	connect(ui.vehicleRearLeftWheelPositionX, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearLeftWheelPropertiesChanged()));
	connect(ui.vehicleRearLeftWheelPositionY, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearLeftWheelPropertiesChanged()));
	connect(ui.vehicleRearLeftWheelPositionZ, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearLeftWheelPropertiesChanged()));
	connect(ui.vehicleRearLeftWheelRotationX, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearLeftWheelPropertiesChanged()));
	connect(ui.vehicleRearLeftWheelRotationY, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearLeftWheelPropertiesChanged()));
	connect(ui.vehicleRearLeftWheelRotationZ, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearLeftWheelPropertiesChanged()));
	connect(ui.vehicleRearLeftWheelScaleX, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearLeftWheelPropertiesChanged()));
	connect(ui.vehicleRearLeftWheelScaleY, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearLeftWheelPropertiesChanged()));
	connect(ui.vehicleRearLeftWheelScaleZ, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearLeftWheelPropertiesChanged()));
	connect(ui.vehicleRearRightWheelPositionX, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearRightWheelPropertiesChanged()));
	connect(ui.vehicleRearRightWheelPositionY, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearRightWheelPropertiesChanged()));
	connect(ui.vehicleRearRightWheelPositionZ, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearRightWheelPropertiesChanged()));
	connect(ui.vehicleRearRightWheelRotationX, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearRightWheelPropertiesChanged()));
	connect(ui.vehicleRearRightWheelRotationY, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearRightWheelPropertiesChanged()));
	connect(ui.vehicleRearRightWheelRotationZ, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearRightWheelPropertiesChanged()));
	connect(ui.vehicleRearRightWheelScaleX, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearRightWheelPropertiesChanged()));
	connect(ui.vehicleRearRightWheelScaleY, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearRightWheelPropertiesChanged()));
	connect(ui.vehicleRearRightWheelScaleZ, SIGNAL(valueChanged(double)), this, SLOT(vehicleRearRightWheelPropertiesChanged()));

	// Path Planner
	connect(ui.btnNewSimulation, SIGNAL(released()), this, SLOT(newSimulation()));
	connect(ui.btnOpenSimulation, SIGNAL(released()), this, SLOT(openSimulation()));
	connect(ui.btnSaveSimulation, SIGNAL(released()), this, SLOT(saveSimulation()));
	connect(ui.btnSetMap, SIGNAL(released()), this, SLOT(setMap()));
	connect(ui.btnSetVehicle, SIGNAL(released()), this, SLOT(setVehicle()));
	connect(ui.btnSetStart, SIGNAL(released()), this, SLOT(setStart()));
	connect(ui.btnSetEnd, SIGNAL(released()), this, SLOT(setEnd()));
	connect(ui.btnShowVoronoiGraph, SIGNAL(released()), this, SLOT(showVoronoiGraph()));
	connect(ui.btnShowFullVoronoiVisibilityGraph, SIGNAL(released()), this, SLOT(showFullVoronoiVisibilityGraph()));
	connect(ui.btnShowPolylinePath, SIGNAL(released()), this, SLOT(showPolylinePath()));
	connect(ui.btnShowParkingPath, SIGNAL(released()), this, SLOT(showParkingPath()));
	connect(ui.btnShowFinalPath, SIGNAL(released()), this, SLOT(showFinalPath()));
	connect(ui.btnShowExpandedObstacles, SIGNAL(released()), this, SLOT(showExpandedObstacles()));
	connect(ui.btnFindPath, SIGNAL(released()), this, SLOT(findPath()));
	connect(ui.btnCancelPathCalculation, SIGNAL(released()), this, SLOT(cancelPathCalculation()));
	connect(ui.pathElementsList, SIGNAL(itemSelectionChanged()), this, SLOT(pathElementSelectionChange()));
	ui.mapPropertiesForm->hide();
	ui.vehiclePropertiesForm->hide();
	ui.pathElementsList->hide();
	ui.calculationInProgress->hide();

	// Visualisation
	connect(ui.btnAddSimulation, SIGNAL(released()), this, SLOT(addSimulation()));
	connect(ui.btnRemoveSimulation, SIGNAL(released()), this, SLOT(removeSimulation()));
	connect(ui.btnInfoSimulation, SIGNAL(released()), this, SLOT(infoSimulation()));
	connect(ui.btnPlayPauseSimulation, SIGNAL(released()), this, SLOT(playPauseSimulation()));
	connect(ui.btnStopSimulation, SIGNAL(released()), this, SLOT(stopSimulation()));
	connect(ui.btnVisualisation2D, SIGNAL(released()), this, SLOT(enableVisualisation2D()));
	connect(ui.btnVisualisation3D, SIGNAL(released()), this, SLOT(enableVisualisation3D()));
	connect(ui.showPathElements, SIGNAL(stateChanged(int)), this, SLOT(showSimulationPath(int)));
	connect(ui.simulationPrograssBar, SIGNAL(sliderMoved(int)), this, SLOT(simulationProgressBarChange(int)));
	connect(ui.simulationPrograssBar, SIGNAL(sliderPressed()), this, SLOT(simulationProgressBarPressed()));
	connect(ui.simulationPrograssBar, SIGNAL(sliderReleased()), this, SLOT(simulationProgressBarReleased()));
	connect(ui.listSimulations, SIGNAL(itemSelectionChanged()), this, SLOT(simulationSelectionChange()));
	ui.simulationPrograssBar->setTracking(false);
	enableVisualisation2D();
	ui.glVisualisation3D->hide();
	ui.mapElementProperties->hide();
	ui.pathElementProperties->hide();

	// Settings
	connect(ui.btnSaveSettings, SIGNAL(released()), this, SLOT(saveSettings()));
	connect(ui.buildingBorderColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.buildingColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.decorationBorderColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.decorationColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.parkingPlaceBorderColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.parkingPlaceColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.vehicleBorderColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.vehicleColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.hoverMapElementBorderColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.hoverMapElementColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.selectedMapElementBorderColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.selectedMapElementColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.mapBorderColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.mapColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.lineColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.arcColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.splineColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.selectedPathElementColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.graphVertexColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.graphEdgeColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.admissibleElementBorderColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.admissibleElementColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.notAdmissibleElementBorderColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	connect(ui.notAdmissibleElementColor, SIGNAL(clicked(ColorContainer &)), this, SLOT(changeColor(ColorContainer &)));
	setLanguage();
	initializeLanguages();
	loadSettings();

	connect(&findPathThread, SIGNAL(finished()), this, SLOT(pathCalculationFinished()));	
}

#pragma region Common.

void ParkingSimulator::setLanguage()
{
	Language *language = Language::getInstance();
	std::map<std::string, std::string> dictionary = language->GetDictionary();

	this->setWindowTitle(QString::fromStdString(dictionary["Window_Title"]));
	ui.tabWidget->setTabText(0, QString::fromStdString(dictionary["Tab_Start"]));
	ui.tabWidget->setTabText(1, QString::fromStdString(dictionary["Tab_MapEditor"]));
	ui.tabWidget->setTabText(2, QString::fromStdString(dictionary["Tab_VehicleEditor"]));
	ui.tabWidget->setTabText(3, QString::fromStdString(dictionary["Tab_PathPlanner"]));
	ui.tabWidget->setTabText(4, QString::fromStdString(dictionary["Tab_Visualisation"]));
	ui.tabWidget->setTabText(5, QString::fromStdString(dictionary["Tab_Settings"]));
	ui.universityName->setText(QString::fromStdString(dictionary["Start_UniversityName"]));
	ui.facultyName->setText(QString::fromStdString(dictionary["Start_FacultyName"]));
	ui.mastersThesisLabel->setText(QString::fromStdString(dictionary["Start_MastersThesis"]));
	ui.thesisTitleLabel->setText(QString::fromStdString(dictionary["Start_ThesisTitle"]));
	ui.thesisAuthoLabel->setText(QString::fromStdString(dictionary["Start_ThesisAuthor"]));
	ui.thesisSupervisorLabel->setText(QString::fromStdString(dictionary["Start_ThesisSupervisor"]));
	ui.btnGoToMapEditor->setToolTip(QString::fromStdString(dictionary["Start_GoToMapEditor"]));
	ui.btnGoToVehicleEditor->setToolTip(QString::fromStdString(dictionary["Start_GoToVehicleEditor"]));
	ui.btnGoToPathPlanner->setToolTip(QString::fromStdString(dictionary["Start_GoToPathPlanner"]));
	ui.btnGoToVisualisation->setToolTip(QString::fromStdString(dictionary["Start_GoToVisualisation"]));
	ui.btnGoToSettings->setToolTip(QString::fromStdString(dictionary["Start_GoToSettings"]));
	ui.btnClose->setToolTip(QString::fromStdString(dictionary["Start_GoToExit"]));
	ui.btnNewMap->setToolTip(QString::fromStdString(dictionary["MapEditor_NewMap"]));
	ui.btnLoadMap->setToolTip(QString::fromStdString(dictionary["MapEditor_OpenMap"]));
	ui.btnSaveMap->setToolTip(QString::fromStdString(dictionary["MapEditor_SaveMap"]));
	ui.btnAddBuilding->setToolTip(QString::fromStdString(dictionary["MapEditor_AddBuilding"]));
	ui.btnAddDecoration->setToolTip(QString::fromStdString(dictionary["MapEditor_AddDecoration"]));
	ui.btnAddParkPlace->setToolTip(QString::fromStdString(dictionary["MapEditor_AddParkingPlace"]));
	ui.btnAddCar->setToolTip(QString::fromStdString(dictionary["MapEditor_AddVehicle"]));
	ui.btnAddTerrain->setToolTip(QString::fromStdString(dictionary["MapEditor_AddTerrain"]));
	ui.btnClearMap->setText(QString::fromStdString(dictionary["MapEditor_ClearMap"]));
	ui.mapElementPropertiesTitleLabel->setText(QString::fromStdString(dictionary["MapEditor_MapElementProperties"]));
	ui.mapElementPropertiesNameLabel->setText(QString::fromStdString(dictionary["Common_Name"]));
	ui.mapElementPropertiesWidthLabel->setText(QString::fromStdString(dictionary["Common_Width"]));
	ui.mapElementPropertiesHeightLabel->setText(QString::fromStdString(dictionary["Common_Height"]));
	ui.mapElementPropertiesPositionLabel->setText(QString::fromStdString(dictionary["Common_Position"]));
	ui.mapElementPropertiesRotationLabel->setText(QString::fromStdString(dictionary["Common_Rotation"]));
	ui.btnMapElementSaveProperties->setText(QString::fromStdString(dictionary["Common_Save"]));
	ui.btnMapElementRemove->setText(QString::fromStdString(dictionary["Common_Remove"]));
	ui.btnNewSimulation->setToolTip(QString::fromStdString(dictionary["PathPlanner_NewSimulation"]));
	ui.btnOpenSimulation->setToolTip(QString::fromStdString(dictionary["PathPlanner_OpenSimulation"]));
	ui.btnSaveSimulation->setToolTip(QString::fromStdString(dictionary["PathPlanner_SaveSimulation"]));
	ui.btnSetMap->setToolTip(QString::fromStdString(dictionary["PathPlanner_SetMap"]));
	ui.btnSetVehicle->setToolTip(QString::fromStdString(dictionary["PathPlanner_SetVehicle"]));
	ui.btnSetStart->setToolTip(QString::fromStdString(dictionary["PathPlanner_SetStart"]));
	ui.btnSetEnd->setToolTip(QString::fromStdString(dictionary["PathPlanner_SetEnd"]));
	ui.btnFindPath->setToolTip(QString::fromStdString(dictionary["PathPlanner_FindPath"]));
	ui.btnShowExpandedObstacles->setToolTip(QString::fromStdString(dictionary["PathPlanner_ShowExpandedObstacles"]));
	ui.btnShowFinalPath->setToolTip(QString::fromStdString(dictionary["PathPlanner_ShowFinalPath"]));
	ui.btnShowFullVoronoiVisibilityGraph->setToolTip(QString::fromStdString(dictionary["PathPlanner_ShowFullVoronoiVisibilityGraph"]));
	ui.btnShowParkingPath->setToolTip(QString::fromStdString(dictionary["PathPlanner_ShowParkingPath"]));
	ui.btnShowPolylinePath->setToolTip(QString::fromStdString(dictionary["PathPlanner_ShowPolylinePath"]));
	ui.btnShowVoronoiGraph->setToolTip(QString::fromStdString(dictionary["PathPlanner_ShowVoronoiGraph"]));
	ui.mapLabel->setText(QString::fromStdString(dictionary["PathPlanner_Map"]));
	ui.isMapSet->setText(QString::fromStdString(dictionary["PathPlanner_NotSet"]));
	ui.vehicleLabel->setText(QString::fromStdString(dictionary["PathPlanner_Vehicle"]));
	ui.isVehicleSet->setText(QString::fromStdString(dictionary["PathPlanner_NotSet"]));
	ui.pathLabel->setText(QString::fromStdString(dictionary["PathPlanner_Path"]));
	ui.isPathSet->setText(QString::fromStdString(dictionary["PathPlanner_NotCreated"]));
	ui.mapWidthLabel_2->setText(QString::fromStdString(dictionary["Common_Width"]));
	ui.mapHeightLabel_2->setText(QString::fromStdString(dictionary["Common_Height"]));
	ui.mapElementsCountLabel_2->setText(QString::fromStdString(dictionary["Common_ElementsCount"]));
	ui.vehicleNameLabel_2->setText(QString::fromStdString(dictionary["Common_Name"]));
	ui.vehicleWheelbaseLabel_2->setText(QString::fromStdString(dictionary["PathPlanner_VehicleWheelbase"]));
	ui.vehicleTrackLabel_2->setText(QString::fromStdString(dictionary["PathPlanner_VehicleTrack"]));
	ui.vehicleMaxAngleLabel_2->setText(QString::fromStdString(dictionary["PathPlanner_VehicleMaxAngle"]));
	ui.pathStartPositionLabel_2->setText(QString::fromStdString(dictionary["PathPlanner_StartPosition"]));
	ui.pathEndPositionLabel_2->setText(QString::fromStdString(dictionary["PathPlanner_EndPosition"]));
	ui.PathElementPropertiesTitleLabel->setText(QString::fromStdString(dictionary["PathPlanner_PathElementProperties"]));
	ui.pathElementPropertiesTypeLabel->setText(QString::fromStdString(dictionary["Common_Type"]));
	ui.pathElementPropertiesFromLabel->setText(QString::fromStdString(dictionary["Common_From"]));
	ui.pathElementPropertiesToLabel->setText(QString::fromStdString(dictionary["Common_To"]));
	ui.pathElementPropertiesLengthLabel->setText(QString::fromStdString(dictionary["Common_Length"]));
	if (pathPlanner.GetStartPoint() == NULL && pathPlanner.GetStartParkingSpace() == NULL)
		ui.pathStartPositionLabel->setText(QString::fromStdString(dictionary["PathPlanner_NotSet"]));
	if (pathPlanner.GetEndPoint() == NULL && pathPlanner.GetEndParkingSpace() == NULL)
		ui.pathEndPositionLabel->setText(QString::fromStdString(dictionary["PathPlanner_NotSet"]));
	ui.btnAddSimulation->setToolTip(QString::fromStdString(dictionary["Visualisation_AddSimulation"]));
	ui.btnRemoveSimulation->setToolTip(QString::fromStdString(dictionary["Visualization_RemoveSimulation"]));
	ui.btnInfoSimulation->setToolTip(QString::fromStdString(dictionary["Visualisation_InfoSimulation"]));
	ui.btnPlayPauseSimulation->setToolTip(QString::fromStdString(dictionary["Visualisation_PlayPause"]));
	ui.btnStopSimulation->setToolTip(QString::fromStdString(dictionary["Visualisation_Stop"]));
	ui.simulationDurationLabel->setText(QString::fromStdString(dictionary["Visualisation_SimulationTime"]));
	ui.showPathElements->setText(QString::fromStdString(dictionary["Visualisation_ShowPathElements"]));
	ui.settingsLanguageLabel->setText(QString::fromStdString(dictionary["Settings_Language"]));
	ui.settingsChooseLanguageLabel->setText(QString::fromStdString(dictionary["Settings_ChooseLanguage"]));
	ui.settingsMapLabel->setText(QString::fromStdString(dictionary["Settings_Map"]));
	ui.settingsMapBorderWidthLabel->setText(QString::fromStdString(dictionary["Settings_MapBorderWidth"]));
	ui.settingsMapBorderColorLabel->setText(QString::fromStdString(dictionary["Settings_MapBorderColor"]));
	ui.settingsMapColorLabel->setText(QString::fromStdString(dictionary["SettingsMapColor"]));
	ui.settingsGraphLabel->setText(QString::fromStdString(dictionary["Settings_Graph"]));
	ui.settingsGraphVertexLabel->setText(QString::fromStdString(dictionary["Settings_GraphVertex"]));
	ui.settingsGraphVertexRadiusLabel->setText(QString::fromStdString(dictionary["Settings_GraphVertexRadius"]));
	ui.settingsGraphVertexColorLabel->setText(QString::fromStdString(dictionary["Settings_GraphVertexColor"]));
	ui.settingsGraphEdge->setText(QString::fromStdString(dictionary["Settings_GraphEdge"]));
	ui.settingsGraphEdgeWidthLabel->setText(QString::fromStdString(dictionary["Settings_GraphEdgeWidth"]));
	ui.settingsGraphEdgeColorLabel->setText(QString::fromStdString(dictionary["Settings_GraphEdgeColor"]));
	ui.settingsPathElementsLabel->setText(QString::fromStdString(dictionary["Settings_PathElements"]));
	ui.settingsPathElementsLineLabel->setText(QString::fromStdString(dictionary["Settings_PathElementsLine"]));
	ui.settingsPathElementsLineWidthLabel->setText(QString::fromStdString(dictionary["Settings_Width"]));
	ui.settingsPathElementsLineColorLabel->setText(QString::fromStdString(dictionary["Settings_Color"]));
	ui.settingsPathElementsCircleLabel->setText(QString::fromStdString(dictionary["Settings_PathElementsCircle"]));
	ui.settingsPathElementsCircleWidthLabel->setText(QString::fromStdString(dictionary["Settings_Width"]));
	ui.settingsPathElementsCircleColorLabel->setText(QString::fromStdString(dictionary["Settings_Color"]));
	ui.settingsPathElementsBSplineLabel->setText(QString::fromStdString(dictionary["Settings_PathElementsBSpline"]));
	ui.settingsPathElementsBSplineWidthLabel->setText(QString::fromStdString(dictionary["Settings_Width"]));
	ui.settingsPathElementsBSplineColorLabel->setText(QString::fromStdString(dictionary["Settings_Color"]));
	ui.settingsPathElementsSelectedLabel->setText(QString::fromStdString(dictionary["Settings_PathElementsSelected"]));
	ui.settingsPathElementsSelectedWidthLabel->setText(QString::fromStdString(dictionary["Settings_Width"]));
	ui.settingsPathElementsSelectedColorLabel->setText(QString::fromStdString(dictionary["Settings_Color"]));
	ui.settingsMapElementsLabel->setText(QString::fromStdString(dictionary["Settings_MapElements"]));
	ui.settingsMapElementsBuildingLabel->setText(QString::fromStdString(dictionary["Settings_MapElementsBuilding"]));
	ui.settingsMapElementsBuildingBorderWidthLabel->setText(QString::fromStdString(dictionary["Settings_BorderWidth"]));
	ui.settingsMapElementsBuildingBorderColorLabel->setText(QString::fromStdString(dictionary["Settings_BorderColor"]));
	ui.settingsMapElementsBuildingColorLabel->setText(QString::fromStdString(dictionary["Settings_FillColor"]));
	ui.settingsMapElementsDecorationLabel->setText(QString::fromStdString(dictionary["Settings_MapElementsDecoration"]));
	ui.settingsMapElementsDecorationBorderWidthLabel->setText(QString::fromStdString(dictionary["Settings_BorderWidth"]));
	ui.settingsMapElementsDecorationBorderColorLabel->setText(QString::fromStdString(dictionary["Settings_BorderColor"]));
	ui.settingsMapElementsDecorationColorLabel->setText(QString::fromStdString(dictionary["Settings_FillColor"]));
	ui.settingsMapElementsParkingPlaceLabel->setText(QString::fromStdString(dictionary["Settings_MapElementsPrakingPlace"]));
	ui.settingsMapElementsParkingPlaceBorderWidthLabel->setText(QString::fromStdString(dictionary["Settings_BorderWidth"]));
	ui.settingsMapElementsParkingPlaceBorderColorLabel->setText(QString::fromStdString(dictionary["Settings_BorderColor"]));
	ui.settingsMapElementsParkingPlaceColorLabel->setText(QString::fromStdString(dictionary["Settings_FillColor"]));
	ui.settingsMapElementsVehicleLabel->setText(QString::fromStdString(dictionary["Settings_MapElementsVehicle"]));
	ui.settingsMapElementsVehicleBorderWidthLabel->setText(QString::fromStdString(dictionary["Settings_BorderWidth"]));
	ui.settingsMapElementsVehicleBorderColorLabel->setText(QString::fromStdString(dictionary["Settings_BorderColor"]));
	ui.settingsMapElementsVehicleColorLabel->setText(QString::fromStdString(dictionary["Settings_FillColor"]));
	ui.settingsMapElementsHoverLabel->setText(QString::fromStdString(dictionary["Settings_MapElementsHoverElement"]));
	ui.settingsMapElementsHoverBorderWidthLabel->setText(QString::fromStdString(dictionary["Settings_BorderWidth"]));
	ui.settingsMapElementsHoverBorderColorLabel->setText(QString::fromStdString(dictionary["Settings_BorderColor"]));
	ui.settingsMapElementsHoverColorLabel->setText(QString::fromStdString(dictionary["Settings_FillColor"]));
	ui.settingsMapElementsSelected->setText(QString::fromStdString(dictionary["Settings_MapElementsSelectedElement"]));
	ui.settingsMapElementsSelectedBorderWidthLabel->setText(QString::fromStdString(dictionary["Settings_BorderWidth"]));
	ui.settingsMapElementsSelectedBorderColorLabel->setText(QString::fromStdString(dictionary["Settings_BorderColor"]));
	ui.settingsMapElementsSelectedColorLabel->setText(QString::fromStdString(dictionary["Settings_FillColor"]));
	ui.settingsMapElementsTransformedLabel->setText(QString::fromStdString(dictionary["Settings_MapElementsTransformed"]));
	ui.settingsMapElementsAdmissibleLabel->setText(QString::fromStdString(dictionary["Settings_MapElementsTransformedAdmissible"]));
	ui.settingsMapElementsAdmissibleBorderWidthLabel->setText(QString::fromStdString(dictionary["Settings_BorderWidth"]));
	ui.settingsMapElementsAdmissibleBorderColorLabel->setText(QString::fromStdString(dictionary["Settings_BorderColor"]));
	ui.settingsMapElementsAdmissibleColorLabel->setText(QString::fromStdString(dictionary["Settings_FillColor"]));
	ui.settingsMapElementsNotAdmissibleLabel->setText(QString::fromStdString(dictionary["Settings_MapElementsTransformedNotAdmissible"]));
	ui.settingsMapElementsNotAdmissibleBorderWidthLabel->setText(QString::fromStdString(dictionary["Settings_BorderWidth"]));
	ui.settingsMapElementsNotAdmissibleBorderColorLabel->setText(QString::fromStdString(dictionary["Settings_BorderColor"]));
	ui.settingsMapElementsNotAdmissibleColorLabel->setText(QString::fromStdString(dictionary["Settings_FillColor"]));
	ui.btnSaveSettings->setText(QString::fromStdString(dictionary["Settings_SaveSettings"]));
	ui.btnNewVehicle->setToolTip(QString::fromStdString(dictionary["VehicleEditor_NewVehicle"]));
	ui.btnOpenVehicle->setToolTip(QString::fromStdString(dictionary["VehicleEditor_OpenVehicle"]));
	ui.btnSaveVehicle->setToolTip(QString::fromStdString(dictionary["VehicleEditor_SaveVehicle"]));
	ui.vehiclePropertiesTitleLabel->setText(QString::fromStdString(dictionary["VehicleEditor_VehicleProperties"]));
	ui.vehiclePropertiesName->setText(QString::fromStdString(dictionary["VehicleEditor_VehiclePropertiesName"]));
	ui.vehiclePropertiesWidth->setText(QString::fromStdString(dictionary["VehicleEditor_VehiclePropertiesWidth"]));
	ui.vehiclePropertiesLength->setText(QString::fromStdString(dictionary["VehicleEditor_VehiclePropertiesLength"]));
	ui.vehiclePropertiesTrack->setText(QString::fromStdString(dictionary["VehicleEditor_VehiclePropertiesTrack"]));
	ui.vehiclePropertiesWheelbase->setText(QString::fromStdString(dictionary["VehicleEditor_VehiclePropertiesWheelbase"]));
	ui.vehiclePropertiesMaxAngle->setText(QString::fromStdString(dictionary["VehicleEditor_MaxAngle"]));
	ui.btnApplyVehicleProperties->setText(QString::fromStdString(dictionary["VehicleEditor_VehiclePropertiesApply"]));
	ui.vehicleModelsTitleLabel->setText(QString::fromStdString(dictionary["VehicleEditor_VehicleModels"]));
	ui.vehicleModelLabel->setText(QString::fromStdString(dictionary["VehicleEditor_MainModel"]));
	ui.setVehicleSetMainModelLabel->setText(QString::fromStdString(dictionary["Common_Model"]));
	ui.btnLoadVehicleModel->setText(QString::fromStdString(dictionary["VehicleEditor_LoadModel"]));
	ui.vehicleFrontLeftWheelModelLabel->setText(QString::fromStdString(dictionary["VehicleEditor_FrontLeftWheelModel"]));
	ui.setVehicleFrontLeftWheelModelLabel->setText(QString::fromStdString(dictionary["Common_Model"]));
	ui.btnLoadVehicleLeftFrontWheelModel->setText(QString::fromStdString(dictionary["VehicleEditor_LoadModel"]));
	ui.setVehicleFrontLeftWheelPositionLabel->setText(QString::fromStdString(dictionary["Common_Position"]));
	ui.setVehicleFrontLeftWheelRotationLabel->setText(QString::fromStdString(dictionary["Common_Rotation"]));
	ui.setVehicleFrontLeftWheelScaleLabel->setText(QString::fromStdString(dictionary["Common_Scale"]));
	ui.vehicleFrontRightWheelModelLabel->setText(QString::fromStdString(dictionary["VehicleEditor_FrontRightWheelModel"]));
	ui.setVehicleFrontRightWheelModelLabel->setText(QString::fromStdString(dictionary["Common_Model"]));
	ui.btnLoadVehicleRightFrontWheelModel->setText(QString::fromStdString(dictionary["VehicleEditor_LoadModel"]));
	ui.setVehicleFrontRightWheelPositionLabel->setText(QString::fromStdString(dictionary["Common_Position"]));
	ui.setVehicleFrontRightWheelRotationLabel->setText(QString::fromStdString(dictionary["Common_Rotation"]));
	ui.setVehicleFrontRightWheelScaleLabel->setText(QString::fromStdString(dictionary["Common_Scale"]));
	ui.vehicleRearLeftWheelModelLabel->setText(QString::fromStdString(dictionary["VehicleEditor_RearLeftWheelModel"]));
	ui.setVehicleRearLeftWheelModelLabel->setText(QString::fromStdString(dictionary["Common_Model"]));
	ui.btnLoadVehicleLeftRearWheelModel->setText(QString::fromStdString(dictionary["VehicleEditor_LoadModel"]));
	ui.setVehicleRearLeftWheelPositionLabel->setText(QString::fromStdString(dictionary["Common_Position"]));
	ui.setVehicleRearLeftWheelRotationLabel->setText(QString::fromStdString(dictionary["Common_Rotation"]));
	ui.setVehicleRearLeftWheelScaleLabel->setText(QString::fromStdString(dictionary["Common_Scale"]));
	ui.vehicleRearRightWheelModelLabel->setText(QString::fromStdString(dictionary["VehicleEditor_RearRightWheelModel"]));
	ui.setVehicleRearRightWheelModelLabel->setText(QString::fromStdString(dictionary["Common_Model"]));
	ui.btnLoadRightRearWheelModel->setText(QString::fromStdString(dictionary["VehicleEditor_LoadModel"]));
	ui.setVehicleRearRightWheelPositionLabel->setText(QString::fromStdString(dictionary["Common_Position"]));
	ui.setVehicleRearRightWheelRotationLabel->setText(QString::fromStdString(dictionary["Common_Rotation"]));
	ui.setVehicleRearRightWheelScaleLabel->setText(QString::fromStdString(dictionary["Common_Scale"]));
	ui.pathCalculationInPrograssLabel->setText(QString::fromStdString(dictionary["PleaseWait_PathCalculation"]));
	ui.vehicleModelIsSet->setText(QString::fromStdString(dictionary["PathPlanner_NotSet"]));
	ui.vehicleFrontLeftWheelIsSet->setText(QString::fromStdString(dictionary["PathPlanner_NotSet"]));
	ui.vehicleFrontRightWheelIsSet->setText(QString::fromStdString(dictionary["PathPlanner_NotSet"]));
	ui.vehicleRearLeftWheelIsSet->setText(QString::fromStdString(dictionary["PathPlanner_NotSet"]));
	ui.vehicleRearRightWheelIsSet->setText(QString::fromStdString(dictionary["PathPlanner_NotSet"]));

	updateMapElementsTree();
}

void ParkingSimulator::renderTimerCall()
{
	ui.glMapEditor->repaint();
	ui.glPathPlanner->repaint();
	ui.glVisualisation->repaint();

	if (mapEditor.GetResetAddButtons())
	{
		clearMapEditorButtons();
		updateMapElementsTree();
	}
}

void ParkingSimulator::render3DTimerCall()
{
	ui.glVehicleEditor->repaint();
	ui.glVisualisation3D->repaint();
}

void ParkingSimulator::updateTimerCall()
{
	if (mapEditor.GetMapElementsChanged())
	{
		updateMapElementsTree();
		mapEditor.SetMapElementsChanged(false);
	}

	if (mapEditor.GetSelectedElement() != NULL && mapEditor.GetMapElementsPropertiesChanged())
	{
		MapElement *mapElement = mapEditor.GetSelectedElement();

		ui.mapElementName->setText(QString::fromStdString(mapElement->GetName()));
		ui.mapElementWidth->setValue(CommonHelper::ConvertPixelsToMeters(mapElement->GetSize().x));
		ui.mapElementHeight->setValue(CommonHelper::ConvertPixelsToMeters(mapElement->GetSize().y));
		ui.mapElementPositionX->setValue(CommonHelper::ConvertPixelsToMeters(mapElement->GetPosition().x));
		ui.mapElementPositionY->setValue(CommonHelper::ConvertPixelsToMeters(mapElement->GetPosition().y));
		ui.mapElementRotation->setValue(CommonHelper::ConvertRadiansToDegrees(mapElement->GetRotation()));

		mapEditor.SetMapElementsPropertiesChanged(false);
	}

	if (pathPlanner.GetSelectedPathElement() != NULL && pathPlanner.GetPathElementPropertiesChanged())
	{
		PathElement *pathElement = pathPlanner.GetSelectedPathElement();
		if (dynamic_cast<Line*>(pathElement) != NULL)
		{
			Line *line = dynamic_cast<Line*>(pathElement);

			ui.pathElementType->setText(QString("Line"));
			ui.pathElementFrom->setText(QString("X: %1, Y: %2").arg(QString::number(CommonHelper::ConvertPixelsToMeters(pathElement->GetFirstPoint().x), 'f', 2)).arg(QString::number(CommonHelper::ConvertPixelsToMeters(pathElement->GetFirstPoint().y), 'f', 2)));
			ui.pathElementTo->setText(QString("X: %1, Y: %2").arg(QString::number(CommonHelper::ConvertPixelsToMeters(pathElement->GetLastPoint().x), 'f', 2)).arg(QString::number(CommonHelper::ConvertPixelsToMeters(pathElement->GetLastPoint().y), 'f', 2)));
		}
		else if (dynamic_cast<Circle*>(pathElement) != NULL)
		{
			Circle *circle = dynamic_cast<Circle*>(pathElement);

			ui.pathElementType->setText(QString("Circle"));
			ui.pathElementFrom->setText(QString("%1 deg").arg(QString::number(glm::degrees(circle->GetAngleFrom()), 'f', 2)));
			ui.pathElementTo->setText(QString("%1 deg").arg(QString::number(glm::degrees(circle->GetAngleTo()), 'f', 2)));
		}
		else if (dynamic_cast<BSpline*>(pathElement) != NULL)
		{
			BSpline *bSpline = dynamic_cast<BSpline*>(pathElement);

			ui.pathElementType->setText(QString("B-Spline"));
			ui.pathElementFrom->setText(QString("X: %1, Y: %2").arg(QString::number(CommonHelper::ConvertPixelsToMeters(pathElement->GetFirstPoint().x), 'f', 2)).arg(QString::number(CommonHelper::ConvertPixelsToMeters(pathElement->GetFirstPoint().y), 'f', 2)));
			ui.pathElementTo->setText(QString("X: %1, Y: %2").arg(QString::number(CommonHelper::ConvertPixelsToMeters(pathElement->GetLastPoint().x), 'f', 2)).arg(QString::number(CommonHelper::ConvertPixelsToMeters(pathElement->GetLastPoint().y), 'f', 2)));
		}
		ui.pathElementLength->setText(QString("%1 m").arg(QString::number(CommonHelper::ConvertPixelsToMeters(pathElement->GetLength()), 'f', 2)));
		ui.pathElementProperties->show();

		pathPlanner.SetPathElementPropertiesChanged(false);
	}

	if (mapEditor.GetSelectedElementChanged())
	{
		if (mapEditor.GetSelectedElement() == NULL)
		{
			ui.mapElementProperties->hide();
		}
		else
		{
			ui.mapElementProperties->show();

			if (dynamic_cast<Obstacle*>(mapEditor.GetSelectedElement()) != NULL)
			{
				int i = 0;
				int buildingIndex = 0;
				int decorationIndex = 0;
				while (mapEditor.GetMap()->GetObstacles()[i] != mapEditor.GetSelectedElement())
				{
					if (mapEditor.GetMap()->GetObstacles()[i]->GetType() == ObstacleType::Building)
						buildingIndex++;
					if (mapEditor.GetMap()->GetObstacles()[i]->GetType() == ObstacleType::Decoration)
						decorationIndex++;
					i++;
				}

				QModelIndex root = ui.treeMapElements->indexAt(QPoint(0, 0));
				QModelIndex buildings = root.child(0, 0);
				QModelIndex decorations = root.child(1, 0);

				QModelIndex newIndex;
				if (dynamic_cast<Obstacle*>(mapEditor.GetSelectedElement())->GetType() == ObstacleType::Building)
				{
					newIndex = buildings.child(buildingIndex, 0);
				}
				else if (dynamic_cast<Obstacle*>(mapEditor.GetSelectedElement())->GetType() == ObstacleType::Decoration)
				{
					newIndex = decorations.child(decorationIndex, 0);
				}
				ui.treeMapElements->setCurrentIndex(newIndex);
			}
			else if (dynamic_cast<Vehicle*>(mapEditor.GetSelectedElement()) != NULL)
			{
				int index = 0;
				while (mapEditor.GetMap()->GetVehicles()[index] != mapEditor.GetSelectedElement())
					index++;
				QModelIndex root = ui.treeMapElements->indexAt(QPoint(0, 0));
				QModelIndex parkingPlaces = root.child(2, 0);
				QModelIndex newIndex = parkingPlaces.child(index, 0);
				ui.treeMapElements->setCurrentIndex(newIndex);
			}
			else if (dynamic_cast<ParkingSpace*>(mapEditor.GetSelectedElement()) != NULL)
			{
				int index = 0;
				while (mapEditor.GetMap()->GetParkingSpaces()[index] != mapEditor.GetSelectedElement())
					index++;
				QModelIndex root = ui.treeMapElements->indexAt(QPoint(0, 0));
				QModelIndex parkingPlaces = root.child(3, 0);
				QModelIndex newIndex = parkingPlaces.child(index, 0);
				ui.treeMapElements->setCurrentIndex(newIndex);
			}
		}
		mapEditor.SetSelectedElementChanged(false);
	}

	if (pathPlanner.GetStartPositionChanged() || pathPlanner.GetEndPositionChanged())
	{
		setPathProperties();
		clearPathPlannerButtons();
		pathPlanner.SetStartPositionChanged(false);
		pathPlanner.SetEndPositionChanged(false);
	}

	if (visualisation.GetCurrentSimulation() != NULL)
	{
		Simulation *simulation = visualisation.GetCurrentSimulation();

		float t = simulation->GetCurrentSimulationTime() / simulation->GetSimulationTime();

		ui.simulationPrograssBar->setValue(t * 1000);
	}
}

#pragma endregion

#pragma region Start.

void ParkingSimulator::goToMapEditor()
{
	ui.tabWidget->setCurrentIndex(1);
}

void ParkingSimulator::goToVehicleEditor()
{
	ui.tabWidget->setCurrentIndex(2);
}

void ParkingSimulator::goToPathPlanner()
{
	ui.tabWidget->setCurrentIndex(3);
}

void ParkingSimulator::goToVisualisation()
{
	ui.tabWidget->setCurrentIndex(4);
}

void ParkingSimulator::goToSettings()
{
	ui.tabWidget->setCurrentIndex(5);
}

#pragma endregion

#pragma region Map editor.

void ParkingSimulator::createMap()
{
	CreateMap createMapWindow;
	createMapWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	if (createMapWindow.exec())
	{
		int width = createMapWindow.GetMapWidth();
		int height = createMapWindow.GetMapHeight();

		mapEditor.CreateMap(width, height);
	}
}

void ParkingSimulator::saveMap()
{
	QString filter = tr("Map files (*.map)");
	QString filePath = QFileDialog::getSaveFileName(this, "Save map", QString(), tr("All files (*.*);;Map files (*.map)"), &filter);

	if(filePath != "")
		mapEditor.SaveMap(filePath.toStdString());
}

void ParkingSimulator::loadMap()
{
	QString filter = tr("Map files (*.map)");
	QString filePath = QFileDialog::getOpenFileName(this, "Open map", QString(), tr("All files (*.*);;Map files (*.map)"), &filter);

	if(filePath != "")
		mapEditor.LoadMap(filePath.toStdString());
}

void ParkingSimulator::clearMap()
{
	if (mapEditor.GetMap() != nullptr)
	{
		mapEditor.SetSelectedElement(nullptr);
		while (mapEditor.GetMap()->GetMapElements().size() > 0)
			mapEditor.GetMap()->RemoveMapElement(mapEditor.GetMap()->GetMapElements()[0]);
		mapEditor.SetMapElementsChanged(true);
		mapEditor.SetSelectedElementChanged(true);
	}
}

void ParkingSimulator::addBuilding()
{
	if (mapEditor.GetMap() == NULL)
		return;

	if (mapEditor.GetAddDecoration() || mapEditor.GetAddParkPlace() || mapEditor.GetAddCar() || mapEditor.GetAddTerrain())
		clearMapEditorButtons();

	if (!mapEditor.GetAddBuilding())
	{
		AddMapElement addMapElementWindow(AddMapElementType::B);
		addMapElementWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		if (addMapElementWindow.exec())
		{
			ui.btnAddBuilding->setStyleSheet("background-color: #d86a39;");
			mapEditor.SetAddBuilding(true);
			mapEditor.AddNewMapElement(addMapElementWindow.GetNewMapElement());
		}
	}
	else
	{
		ui.btnAddBuilding->setStyleSheet("");
		mapEditor.SetAddBuilding(false);
		mapEditor.SetNewElement(nullptr);
	}
}

void ParkingSimulator::addDecoration()
{
	if (mapEditor.GetMap() == NULL)
		return;

	if (mapEditor.GetAddBuilding() || mapEditor.GetAddParkPlace() || mapEditor.GetAddCar() || mapEditor.GetAddTerrain())
		clearMapEditorButtons();

	if (!mapEditor.GetAddDecoration())
	{
		AddMapElement addMapElementWindow(AddMapElementType::D);
		addMapElementWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		if (addMapElementWindow.exec())
		{
			ui.btnAddDecoration->setStyleSheet("background-color: #d86a39;");
			mapEditor.SetAddDecoration(true);
			mapEditor.AddNewMapElement(addMapElementWindow.GetNewMapElement());
		}
	}
	else
	{
		ui.btnAddDecoration->setStyleSheet("");
		mapEditor.SetAddDecoration(false);
		mapEditor.SetNewElement(nullptr);
	}
}

void ParkingSimulator::addCar()
{
	if (mapEditor.GetMap() == NULL)
		return;

	if (mapEditor.GetAddBuilding() || mapEditor.GetAddDecoration() || mapEditor.GetAddParkPlace() || mapEditor.GetAddTerrain())
		clearMapEditorButtons();

	if (!mapEditor.GetAddCar())
	{
		AddMapElement addMapElementWindow(AddMapElementType::C);
		addMapElementWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		if (addMapElementWindow.exec())
		{
			ui.btnAddCar->setStyleSheet("background-color: #d86a39;");
			mapEditor.SetAddCar(true);
			mapEditor.AddNewMapElement(addMapElementWindow.GetNewMapElement());
		}
	}
	else
	{
		ui.btnAddCar->setStyleSheet("");
		mapEditor.SetAddCar(false);
		mapEditor.SetNewElement(nullptr);
	}
}

void ParkingSimulator::addParkPlace()
{
	if (mapEditor.GetMap() == NULL)
		return;

	if (mapEditor.GetAddBuilding() || mapEditor.GetAddDecoration() || mapEditor.GetAddCar() || mapEditor.GetAddTerrain())
		clearMapEditorButtons();

	if (!mapEditor.GetAddParkPlace())
	{
		AddMapElement addMapElementWindow(AddMapElementType::P);
		addMapElementWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		if (addMapElementWindow.exec())
		{
			ui.btnAddParkPlace->setStyleSheet("background-color: #d86a39;");
			mapEditor.SetAddParkPlace(true);
			mapEditor.AddNewMapElement(addMapElementWindow.GetNewMapElement());
		}
	}
	else
	{
		ui.btnAddParkPlace->setStyleSheet("");
		mapEditor.SetAddParkPlace(false);
		mapEditor.SetNewElement(nullptr);
	}
}

void ParkingSimulator::addTerrain()
{
	if (mapEditor.GetMap() == NULL)
		return;

	if (mapEditor.GetAddBuilding() || mapEditor.GetAddDecoration() || mapEditor.GetAddCar() || mapEditor.GetAddParkPlace())
		clearMapEditorButtons();

	if (!mapEditor.GetAddTerrain())
	{
		AddMapElement addMapElementWindow(AddMapElementType::T);
		addMapElementWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		if (addMapElementWindow.exec())
		{
			ui.btnAddTerrain->setStyleSheet("background-color: #d86a39;");
			mapEditor.SetAddTerrain(true);
			mapEditor.SetNewTerrain(addMapElementWindow.GetNewTerrain());
			mapEditor.SetSelectedElement(nullptr);
		}
	}
	else
	{
		ui.btnAddTerrain->setStyleSheet("");
		mapEditor.SetAddTerrain(false);
		mapEditor.SetNewTerrain(nullptr);
	}
}

void ParkingSimulator::mapElementRemove()
{
	mapEditor.GetMap()->RemoveMapElement(mapEditor.GetSelectedElement());
	mapEditor.SetSelectedElement(nullptr);
	mapEditor.SetMapElementsChanged(true);

	ui.mapElementProperties->hide();
}

void ParkingSimulator::mapElementSaveProperties()
{
	MapElement *selectedElement = mapEditor.GetSelectedElement();

	string oldName = selectedElement->GetName();
	glm::vec2 oldSize = selectedElement->GetSize();
	glm::vec2 oldPosition = selectedElement->GetPosition();
	double oldRotation = selectedElement->GetRotation();

	selectedElement->SetName(ui.mapElementName->toPlainText().toStdString());
	selectedElement->SetSize(glm::vec2(CommonHelper::ConverMetersToPixeks(ui.mapElementWidth->value()), CommonHelper::ConverMetersToPixeks(ui.mapElementHeight->value())));
	selectedElement->SetPosition(glm::vec2(CommonHelper::ConverMetersToPixeks(ui.mapElementPositionX->value()), CommonHelper::ConverMetersToPixeks(ui.mapElementPositionY->value())));
	selectedElement->SetRotation(CommonHelper::ConvertDegreesToRadians(ui.mapElementRotation->value()));

	if (!mapEditor.IsMapElementAdmissible(selectedElement))
	{
		selectedElement->SetName(oldName);
		selectedElement->SetSize(oldSize);
		selectedElement->SetPosition(oldPosition);
		selectedElement->SetRotation(oldRotation);

		WarningErrorMsg warningWindow(Language::getInstance()->GetDictionary()["WarningError_MapElementNotAdmissible_Title"], Language::getInstance()->GetDictionary()["WarningError_MapElementNotAdmissible_Content"], MessageType::Error);
		warningWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		warningWindow.exec();
	}
}

void ParkingSimulator::updateMapElementsTree()
{
	auto mapElementsTree = *mapEditor.GetMapElementsTreeItems();
	auto mapElementsTreeExpanded = mapEditor.GetMapElementsTreeItemsExpanded();
	
	ui.treeMapElements->clear();
	ui.treeMapElements->insertTopLevelItems(0, mapElementsTree);

	for (int i = 0; i < 5; i++)
	{
		if (mapElementsTreeExpanded[i])
			ui.treeMapElements->expandItem(mapElementsTree[i]);
	}
}

void ParkingSimulator::treeMapElementsSelectionChanged()
{
	auto currentItem = ui.treeMapElements->currentItem();
	auto currentIndex = ui.treeMapElements->currentIndex();

	auto b = currentIndex.isValid();

	if (currentItem->parent() == NULL || mapEditor.SelectedElementChangedOnMap())
	{
		mapEditor.SetSelectedElementChangedOnMap(false);
		return;
	}

	QString parentItemTitle = currentItem->parent()->text(0);
	if (parentItemTitle == "Buildings")
	{
		int i = 0;
		int buildingIndex = 0;

		if (mapEditor.GetMap()->GetObstacles().size() > 0)
		{
			do
			{
				if (i >= mapEditor.GetMap()->GetObstacles().size()) return;
				if (mapEditor.GetMap()->GetObstacles()[i]->GetType() == ObstacleType::Building)
				{
					if (buildingIndex < currentIndex.row())
					{
						buildingIndex++;
						i++;
					}
					else
						break;
				}
				else
				{
					i++;
				}
			} while (buildingIndex <= currentIndex.row());

			mapEditor.SetSelectedElement(mapEditor.GetMap()->GetObstacles()[i]);
		}
		else
			mapEditor.SetSelectedElement(nullptr);
	}
	else if (parentItemTitle == "Decorations")
	{
		int i = 0;
		int decorationIndex = 0;

		if (mapEditor.GetMap()->GetObstacles().size() > 0)
		{
			do
			{
				if (i >= mapEditor.GetMap()->GetObstacles().size()) return;
				if (mapEditor.GetMap()->GetObstacles()[i]->GetType() == ObstacleType::Decoration)
				{
					if (decorationIndex < currentIndex.row())
					{
						decorationIndex++;
						i++;
					}
					else
						break;
				}
				else
				{
					i++;
				}
			} while (decorationIndex <= currentIndex.row());

			mapEditor.SetSelectedElement(mapEditor.GetMap()->GetObstacles()[i]);
		}
		else
			mapEditor.SetSelectedElement(nullptr);
	}
	else if (parentItemTitle == "Vehicles")
	{
		if (currentIndex.row() < mapEditor.GetMap()->GetVehicles().size())
			mapEditor.SetSelectedElement(mapEditor.GetMap()->GetVehicles()[currentIndex.row()]);
	}
	else if (parentItemTitle == "Park places")
	{
		if (currentIndex.row() < mapEditor.GetMap()->GetParkingSpaces().size())
			mapEditor.SetSelectedElement(mapEditor.GetMap()->GetParkingSpaces()[currentIndex.row()]);
	}
	else
		mapEditor.SetSelectedElement(nullptr);
}

void ParkingSimulator::clearMapEditorButtons()
{
	mapEditor.SetAddBuilding(false);
	mapEditor.SetAddDecoration(false);
	mapEditor.SetAddParkPlace(false);
	mapEditor.SetAddCar(false);
	mapEditor.SetAddTerrain(false);

	clearMapEditorButtonsStyle();
}

void ParkingSimulator::clearMapEditorButtonsStyle()
{
	if (!mapEditor.GetAddBuilding())
		ui.btnAddBuilding->setStyleSheet("");
	if (!mapEditor.GetAddDecoration())
		ui.btnAddDecoration->setStyleSheet("");
	if (!mapEditor.GetAddParkPlace())
		ui.btnAddParkPlace->setStyleSheet("");
	if (!mapEditor.GetAddCar())
		ui.btnAddCar->setStyleSheet("");
	if (!mapEditor.GetAddTerrain())
		ui.btnAddTerrain->setStyleSheet("");
}

#pragma endregion

#pragma region Vehicle Editor

void ParkingSimulator::newVehicle()
{
	vehicleEditor.CreateNewVehicle();

	applyVehicleProperties();
}

void ParkingSimulator::openVehicle()
{
	QString filter = tr("Vehicle files (*.vehicle)");
	QString filePath = QFileDialog::getOpenFileName(this, "Open vehicle", QString(), tr("All files (*.*);;Vehicle files (*.vehicle)"), &filter);

	if (filePath != "")
	{
		vehicleEditor.OpenVehicle(filePath.toStdString());

		updateVehicleProperties();

		if (vehicleEditor.GetVehicle()->GetFrontLeftWheelModel() != NULL)
		{
			enableVehicleFrontLeftWheelProperties();
			updateVehicleFrontLeftWheelProperties();
		}
		if (vehicleEditor.GetVehicle()->GetFrontRightWheelModel() != NULL)
		{
			enableVehicleFrontRightWheelProperties();
			updateVehicleFrontRightWheelProperties();
		}
		if (vehicleEditor.GetVehicle()->GetRearLeftWheelModel() != NULL)
		{
			enableVehicleRearLeftWheelProperties();
			updateVehicleRearLeftWheelProperties();
		}
		if (vehicleEditor.GetVehicle()->GetRearRightWheelModel() != NULL)
		{
			enableVehicleRearRightWheelProperties();
			updateVehicleRearRightWheelProperties();
		}
	}
}

void ParkingSimulator::saveVehicle()
{
	QString filter = tr("Vehicle files (*.vehicle)");
	QString filePath = QFileDialog::getSaveFileName(this, "Save vehicle", QString(), tr("All files (*.*);;Vehicle files (*.vehicle)"), &filter);

	if (filePath != "")
		vehicleEditor.SaveVehicle(filePath.toStdString());
}

void ParkingSimulator::applyVehicleProperties()
{
	if (vehicleEditor.GetVehicle() != NULL)
	{
		vehicleEditor.GetVehicle()->SetName(ui.vehicleName->text().toStdString());
		vehicleEditor.GetVehicle()->SetSize(glm::vec2(CommonHelper::ConverMetersToPixeks(ui.vehicleLength->value()), CommonHelper::ConverMetersToPixeks(ui.vehicleWidth->value())));
		vehicleEditor.GetVehicle()->SetWheelbase(CommonHelper::ConverMetersToPixeks(ui.vehicleWheelbase->value()));
		vehicleEditor.GetVehicle()->SetTrack(CommonHelper::ConverMetersToPixeks(ui.vehicleTrack->value()));
		vehicleEditor.GetVehicle()->SetMaxInsideAngle(CommonHelper::ConvertDegreesToRadians(ui.vehicleMaxAngle->value()));
	}
}

void ParkingSimulator::loadVehicleModel()
{
	QString filter = tr("OBJ model files (*.obj)");
	QString filePath = QFileDialog::getOpenFileName(this, "OBJ model files", QString(), tr("All files (*.*);;OBJ model files (*.obj)"), &filter);

	if (filePath != "")
	{
		vehicleEditor.LoadVehicleModel(filePath.toStdString());
		ui.vehicleModelIsSet->setText(QString("OK"));
		ui.vehicleModelIsSet->setStyleSheet(QString("color: green;"));
	}
}

void ParkingSimulator::loadVehicleLeftFrontWheelModel()
{
	QString filter = tr("OBJ model files (*.obj)");
	QString filePath = QFileDialog::getOpenFileName(this, "OBJ model files", QString(), tr("All files (*.*);;OBJ model files (*.obj)"), &filter);

	if (filePath != "")
	{
		vehicleEditor.LoadFrontLeftWheelModel(filePath.toStdString());
		enableVehicleFrontLeftWheelProperties();
		updateVehicleFrontLeftWheelProperties();
	}
}

void ParkingSimulator::loadVehicleRightFrontWheelModel()
{
	QString filter = tr("OBJ model files (*.obj)");
	QString filePath = QFileDialog::getOpenFileName(this, "OBJ model files", QString(), tr("All files (*.*);;OBJ model files (*.obj)"), &filter);

	if (filePath != "")
	{
		vehicleEditor.LoadFrontRightWheelModel(filePath.toStdString());
		enableVehicleFrontRightWheelProperties();
		updateVehicleFrontRightWheelProperties();
	}
}

void ParkingSimulator::loadVehicleLeftRearWheelModel()
{
	QString filter = tr("OBJ model files (*.obj)");
	QString filePath = QFileDialog::getOpenFileName(this, "OBJ model files", QString(), tr("All files (*.*);;OBJ model files (*.obj)"), &filter);

	if (filePath != "")
	{
		vehicleEditor.LoadRearLeftWheelModel(filePath.toStdString());
		enableVehicleRearLeftWheelProperties();
		updateVehicleRearLeftWheelProperties();
	}
}

void ParkingSimulator::loadVehicleRightRearWheelModel()
{
	QString filter = tr("OBJ model files (*.obj)");
	QString filePath = QFileDialog::getOpenFileName(this, "OBJ model files", QString(), tr("All files (*.*);;OBJ model files (*.obj)"), &filter);

	if (filePath != "")
	{
		vehicleEditor.LoadRearRightWheelModel(filePath.toStdString());
		enableVehicleRearRightWheelProperties();
		updateVehicleRearRightWheelProperties();
	}
}

void ParkingSimulator::enableVehicleFrontLeftWheelProperties()
{
	ui.vehicleFrontLeftWheelIsSet->setText(QString("OK"));
	ui.vehicleFrontLeftWheelIsSet->setStyleSheet(QString("color: green;"));
	ui.vehicleFrontLeftWheelPositionX->setEnabled(true);
	ui.vehicleFrontLeftWheelPositionY->setEnabled(true);
	ui.vehicleFrontLeftWheelPositionZ->setEnabled(true);
	ui.vehicleFrontLeftWheelRotationX->setEnabled(true);
	ui.vehicleFrontLeftWheelRotationY->setEnabled(true);
	ui.vehicleFrontLeftWheelRotationZ->setEnabled(true);
	ui.vehicleFrontLeftWheelScaleX->setEnabled(true);
	ui.vehicleFrontLeftWheelScaleY->setEnabled(true);
	ui.vehicleFrontLeftWheelScaleZ->setEnabled(true);
}

void ParkingSimulator::enableVehicleFrontRightWheelProperties()
{
	ui.vehicleFrontRightWheelIsSet->setText(QString("OK"));
	ui.vehicleFrontRightWheelIsSet->setStyleSheet(QString("color: green;"));
	ui.vehicleFrontRightWheelPositionX->setEnabled(true);
	ui.vehicleFrontRightWheelPositionY->setEnabled(true);
	ui.vehicleFrontRightWheelPositionZ->setEnabled(true);
	ui.vehicleFrontRightWheelRotationX->setEnabled(true);
	ui.vehicleFrontRightWheelRotationY->setEnabled(true);
	ui.vehicleFrontRightWheelRotationZ->setEnabled(true);
	ui.vehicleFrontRightWheelScaleX->setEnabled(true);
	ui.vehicleFrontRightWheelScaleY->setEnabled(true);
	ui.vehicleFrontRightWheelScaleZ->setEnabled(true);
}

void ParkingSimulator::enableVehicleRearLeftWheelProperties()
{
	ui.vehicleRearLeftWheelIsSet->setText(QString("OK"));
	ui.vehicleRearLeftWheelIsSet->setStyleSheet(QString("color: green;"));
	ui.vehicleRearLeftWheelPositionX->setEnabled(true);
	ui.vehicleRearLeftWheelPositionY->setEnabled(true);
	ui.vehicleRearLeftWheelPositionZ->setEnabled(true);
	ui.vehicleRearLeftWheelRotationX->setEnabled(true);
	ui.vehicleRearLeftWheelRotationY->setEnabled(true);
	ui.vehicleRearLeftWheelRotationZ->setEnabled(true);
	ui.vehicleRearLeftWheelScaleX->setEnabled(true);
	ui.vehicleRearLeftWheelScaleY->setEnabled(true);
	ui.vehicleRearLeftWheelScaleZ->setEnabled(true);
}

void ParkingSimulator::enableVehicleRearRightWheelProperties()
{
	ui.vehicleRearRightWheelIsSet->setText(QString("OK"));
	ui.vehicleRearRightWheelIsSet->setStyleSheet(QString("color: green;"));
	ui.vehicleRearRightWheelPositionX->setEnabled(true);
	ui.vehicleRearRightWheelPositionY->setEnabled(true);
	ui.vehicleRearRightWheelPositionZ->setEnabled(true);
	ui.vehicleRearRightWheelRotationX->setEnabled(true);
	ui.vehicleRearRightWheelRotationY->setEnabled(true);
	ui.vehicleRearRightWheelRotationZ->setEnabled(true);
	ui.vehicleRearRightWheelScaleX->setEnabled(true);
	ui.vehicleRearRightWheelScaleY->setEnabled(true);
	ui.vehicleRearRightWheelScaleZ->setEnabled(true);
}

void ParkingSimulator::updateVehicleProperties()
{
	ui.vehicleName->setText(QString::fromStdString(vehicleEditor.GetVehicle()->GetName()));
	ui.vehicleLength->setValue(CommonHelper::ConvertPixelsToMeters(vehicleEditor.GetVehicle()->GetSize().x));
	ui.vehicleWidth->setValue(CommonHelper::ConvertPixelsToMeters(vehicleEditor.GetVehicle()->GetSize().y));
	ui.vehicleTrack->setValue(CommonHelper::ConvertPixelsToMeters(vehicleEditor.GetVehicle()->GetTrack()));
	ui.vehicleWheelbase->setValue(CommonHelper::ConvertPixelsToMeters(vehicleEditor.GetVehicle()->GetWheelbase()));
	ui.vehicleMaxAngle->setValue(CommonHelper::ConvertRadiansToDegrees(vehicleEditor.GetVehicle()->GetMaxInsideAngle()));

	if (vehicleEditor.GetVehicle()->GetVehicleModel() != NULL)
	{
		ui.vehicleModelIsSet->setText(QString("OK"));
		ui.vehicleModelIsSet->setStyleSheet(QString("color: green;"));
	}
	else
	{
		ui.vehicleModelIsSet->setText(QString("NOT SET"));
		ui.vehicleModelIsSet->setStyleSheet(QString("color: red;"));
	}
}

void ParkingSimulator::updateVehicleFrontLeftWheelProperties()
{
	glm::vec3 translation = vehicleEditor.GetVehicle()->GetFrontLeftWheelModel()->GetTranslation();
	glm::vec3 rotation = vehicleEditor.GetVehicle()->GetFrontLeftWheelModel()->GetRotation();
	glm::vec3 scale = vehicleEditor.GetVehicle()->GetFrontLeftWheelModel()->GetScale();

	ui.vehicleFrontLeftWheelPositionX->setValue(CommonHelper::ConvertPixelsToMeters(translation.x));
	ui.vehicleFrontLeftWheelPositionY->setValue(CommonHelper::ConvertPixelsToMeters(translation.y));
	ui.vehicleFrontLeftWheelPositionZ->setValue(CommonHelper::ConvertPixelsToMeters(translation.z));
	ui.vehicleFrontLeftWheelRotationX->setValue(CommonHelper::ConvertRadiansToDegrees(rotation.x));
	ui.vehicleFrontLeftWheelRotationY->setValue(CommonHelper::ConvertRadiansToDegrees(rotation.y));
	ui.vehicleFrontLeftWheelRotationZ->setValue(CommonHelper::ConvertRadiansToDegrees(rotation.z));
	ui.vehicleFrontLeftWheelScaleX->setValue(scale.x);
	ui.vehicleFrontLeftWheelScaleY->setValue(scale.y);
	ui.vehicleFrontLeftWheelScaleZ->setValue(scale.z);
}

void ParkingSimulator::updateVehicleFrontRightWheelProperties()
{
	glm::vec3 translation = vehicleEditor.GetVehicle()->GetFrontRightWheelModel()->GetTranslation();
	glm::vec3 rotation = vehicleEditor.GetVehicle()->GetFrontRightWheelModel()->GetRotation();
	glm::vec3 scale = vehicleEditor.GetVehicle()->GetFrontRightWheelModel()->GetScale();

	ui.vehicleFrontRightWheelPositionX->setValue(CommonHelper::ConvertPixelsToMeters(translation.x));
	ui.vehicleFrontRightWheelPositionY->setValue(CommonHelper::ConvertPixelsToMeters(translation.y));
	ui.vehicleFrontRightWheelPositionZ->setValue(CommonHelper::ConvertPixelsToMeters(translation.z));
	ui.vehicleFrontRightWheelRotationX->setValue(CommonHelper::ConvertRadiansToDegrees(rotation.x));
	ui.vehicleFrontRightWheelRotationY->setValue(CommonHelper::ConvertRadiansToDegrees(rotation.y));
	ui.vehicleFrontRightWheelRotationZ->setValue(CommonHelper::ConvertRadiansToDegrees(rotation.z));
	ui.vehicleFrontRightWheelScaleX->setValue(scale.x);
	ui.vehicleFrontRightWheelScaleY->setValue(scale.y);
	ui.vehicleFrontRightWheelScaleZ->setValue(scale.z);
}

void ParkingSimulator::updateVehicleRearLeftWheelProperties()
{
	glm::vec3 translation = vehicleEditor.GetVehicle()->GetRearLeftWheelModel()->GetTranslation();
	glm::vec3 rotation = vehicleEditor.GetVehicle()->GetRearLeftWheelModel()->GetRotation();
	glm::vec3 scale = vehicleEditor.GetVehicle()->GetRearLeftWheelModel()->GetScale();

	ui.vehicleRearLeftWheelPositionX->setValue(CommonHelper::ConvertPixelsToMeters(translation.x));
	ui.vehicleRearLeftWheelPositionY->setValue(CommonHelper::ConvertPixelsToMeters(translation.y));
	ui.vehicleRearLeftWheelPositionZ->setValue(CommonHelper::ConvertPixelsToMeters(translation.z));
	ui.vehicleRearLeftWheelRotationX->setValue(CommonHelper::ConvertRadiansToDegrees(rotation.x));
	ui.vehicleRearLeftWheelRotationY->setValue(CommonHelper::ConvertRadiansToDegrees(rotation.y));
	ui.vehicleRearLeftWheelRotationZ->setValue(CommonHelper::ConvertRadiansToDegrees(rotation.z));
	ui.vehicleRearLeftWheelScaleX->setValue(scale.x);
	ui.vehicleRearLeftWheelScaleY->setValue(scale.y);
	ui.vehicleRearLeftWheelScaleZ->setValue(scale.z);
}

void ParkingSimulator::updateVehicleRearRightWheelProperties()
{
	glm::vec3 translation = vehicleEditor.GetVehicle()->GetRearRightWheelModel()->GetTranslation();
	glm::vec3 rotation = vehicleEditor.GetVehicle()->GetRearRightWheelModel()->GetRotation();
	glm::vec3 scale = vehicleEditor.GetVehicle()->GetRearRightWheelModel()->GetScale();

	ui.vehicleRearRightWheelPositionX->setValue(CommonHelper::ConvertPixelsToMeters(translation.x));
	ui.vehicleRearRightWheelPositionY->setValue(CommonHelper::ConvertPixelsToMeters(translation.y));
	ui.vehicleRearRightWheelPositionZ->setValue(CommonHelper::ConvertPixelsToMeters(translation.z));
	ui.vehicleRearRightWheelRotationX->setValue(CommonHelper::ConvertRadiansToDegrees(rotation.x));
	ui.vehicleRearRightWheelRotationY->setValue(CommonHelper::ConvertRadiansToDegrees(rotation.y));
	ui.vehicleRearRightWheelRotationZ->setValue(CommonHelper::ConvertRadiansToDegrees(rotation.z));
	ui.vehicleRearRightWheelScaleX->setValue(scale.x);
	ui.vehicleRearRightWheelScaleY->setValue(scale.y);
	ui.vehicleRearRightWheelScaleZ->setValue(scale.z);
}

void ParkingSimulator::vehicleFrontLeftWheelPropertiesChanged()
{
	glm::vec3 translation = glm::vec3(CommonHelper::ConverMetersToPixeks(ui.vehicleFrontLeftWheelPositionX->value()), CommonHelper::ConverMetersToPixeks(ui.vehicleFrontLeftWheelPositionY->value()), CommonHelper::ConverMetersToPixeks(ui.vehicleFrontLeftWheelPositionZ->value()));
	glm::vec3 rotation = glm::vec3(CommonHelper::ConvertDegreesToRadians(ui.vehicleFrontLeftWheelRotationX->value()), CommonHelper::ConvertDegreesToRadians(ui.vehicleFrontLeftWheelRotationY->value()), CommonHelper::ConvertDegreesToRadians(ui.vehicleFrontLeftWheelRotationZ->value()));
	glm::vec3 scale = glm::vec3(ui.vehicleFrontLeftWheelScaleX->value(), ui.vehicleFrontLeftWheelScaleY->value(), ui.vehicleFrontLeftWheelScaleZ->value());

	vehicleEditor.UpdateFrontLeftWheelProperties(translation, rotation, scale);
}

void ParkingSimulator::vehicleFrontRightWheelPropertiesChanged()
{
	glm::vec3 translation = glm::vec3(CommonHelper::ConverMetersToPixeks(ui.vehicleFrontRightWheelPositionX->value()), CommonHelper::ConverMetersToPixeks(ui.vehicleFrontRightWheelPositionY->value()), CommonHelper::ConverMetersToPixeks(ui.vehicleFrontRightWheelPositionZ->value()));
	glm::vec3 rotation = glm::vec3(CommonHelper::ConvertDegreesToRadians(ui.vehicleFrontRightWheelRotationX->value()), CommonHelper::ConvertDegreesToRadians(ui.vehicleFrontRightWheelRotationY->value()), CommonHelper::ConvertDegreesToRadians(ui.vehicleFrontRightWheelRotationZ->value()));
	glm::vec3 scale = glm::vec3(ui.vehicleFrontRightWheelScaleX->value(), ui.vehicleFrontRightWheelScaleY->value(), ui.vehicleFrontRightWheelScaleZ->value());

	vehicleEditor.UpdateFrontRightWheelProperties(translation, rotation, scale);
}

void ParkingSimulator::vehicleRearLeftWheelPropertiesChanged()
{
	glm::vec3 translation = glm::vec3(CommonHelper::ConverMetersToPixeks(ui.vehicleRearLeftWheelPositionX->value()), CommonHelper::ConverMetersToPixeks(ui.vehicleRearLeftWheelPositionY->value()), CommonHelper::ConverMetersToPixeks(ui.vehicleRearLeftWheelPositionZ->value()));
	glm::vec3 rotation = glm::vec3(CommonHelper::ConvertDegreesToRadians(ui.vehicleRearLeftWheelRotationX->value()), CommonHelper::ConvertDegreesToRadians(ui.vehicleRearLeftWheelRotationY->value()), CommonHelper::ConvertDegreesToRadians(ui.vehicleRearLeftWheelRotationZ->value()));
	glm::vec3 scale = glm::vec3(ui.vehicleRearLeftWheelScaleX->value(), ui.vehicleRearLeftWheelScaleY->value(), ui.vehicleRearLeftWheelScaleZ->value());

	vehicleEditor.UpdateRearLeftWheelProperties(translation, rotation, scale);
}

void ParkingSimulator::vehicleRearRightWheelPropertiesChanged()
{
	glm::vec3 translation = glm::vec3(CommonHelper::ConverMetersToPixeks(ui.vehicleRearRightWheelPositionX->value()), CommonHelper::ConverMetersToPixeks(ui.vehicleRearRightWheelPositionY->value()), CommonHelper::ConverMetersToPixeks(ui.vehicleRearRightWheelPositionZ->value()));
	glm::vec3 rotation = glm::vec3(CommonHelper::ConvertDegreesToRadians(ui.vehicleRearRightWheelRotationX->value()), CommonHelper::ConvertDegreesToRadians(ui.vehicleRearRightWheelRotationY->value()), CommonHelper::ConvertDegreesToRadians(ui.vehicleRearRightWheelRotationZ->value()));
	glm::vec3 scale = glm::vec3(ui.vehicleRearRightWheelScaleX->value(), ui.vehicleRearRightWheelScaleY->value(), ui.vehicleRearRightWheelScaleZ->value());

	vehicleEditor.UpdateRearRightWheelProperties(translation, rotation, scale);
}

#pragma endregion 

#pragma region Path Planner.

void ParkingSimulator::newSimulation()
{
	pathPlanner.NewSimulation();
	setMapProperties();
	setVehicleProperties();
	setPathProperties();
	ui.pathElementProperties->hide();
}

void ParkingSimulator::openSimulation()
{
	QString filter = tr("Simulation files (*.simulation)");
	QString filePath = QFileDialog::getOpenFileName(this, "Open simulation", QString(), tr("All files (*.*);;Simulation files (*.simulation)"), &filter);

	if (filePath != "")
	{
		pathPlanner.OpenSimulation(filePath.toStdString());
		setMapProperties();
		setVehicleProperties();
		setPathProperties();
		ui.pathElementProperties->hide();
	}
}

void ParkingSimulator::saveSimulation()
{
	QString filter = tr("Simulation files (*.simulation)");
	QString filePath = QFileDialog::getSaveFileName(this, "Save simulation", QString(), tr("All files (*.*);;Simulation files (*.simulation)"), &filter);

	if (filePath != "")
		pathPlanner.SaveSimulation(filePath.toStdString());
}

void ParkingSimulator::setMap()
{
	SelectMap selectMapWindow(&mapEditor);
	selectMapWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	if (selectMapWindow.exec())
	{
		pathPlanner.SetMap(selectMapWindow.GetMap());
		pathPlanner.SetExpandedMap(nullptr);
		pathPlanner.SetVoronoiGraph(nullptr);
		pathPlanner.SetFullVoronoiVisibilityGraph(nullptr);
		pathPlanner.SetStartPoint(nullptr);
		pathPlanner.SetStartDirection(nullptr);
		pathPlanner.SetStartParkingSpace(nullptr);
		pathPlanner.SetEndPoint(nullptr);
		pathPlanner.SetEndDrection(nullptr);
		pathPlanner.SetEndParkingSpace(nullptr);
		pathPlanner.SetPolylinePath(nullptr);
		pathPlanner.SetParkingPathStart(nullptr);
		pathPlanner.SetParkingPathEnd(nullptr);
		pathPlanner.SetFinalPath(nullptr);
		setMapProperties();
		setPathProperties();
	}
}

void ParkingSimulator::setVehicle()
{
	SelectVehicle selectVehicleWindow(&vehicleEditor);
	selectVehicleWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	if (selectVehicleWindow.exec())
	{
		pathPlanner.SetVehicle(selectVehicleWindow.GetVehicle());
		pathPlanner.SetVehicleStart(new Vehicle(*selectVehicleWindow.GetVehicle()));
		pathPlanner.SetVehicleEnd(new Vehicle(*selectVehicleWindow.GetVehicle()));
		pathPlanner.SetStartPoint(nullptr);
		pathPlanner.SetStartDirection(nullptr);
		pathPlanner.SetStartParkingSpace(nullptr);
		pathPlanner.SetEndPoint(nullptr);
		pathPlanner.SetEndDrection(nullptr);
		pathPlanner.SetEndParkingSpace(nullptr);
		pathPlanner.SetPolylinePath(nullptr);
		pathPlanner.SetParkingPathStart(nullptr);
		pathPlanner.SetParkingPathEnd(nullptr);
		pathPlanner.SetFinalPath(nullptr);
		setVehicleProperties();
		setPathProperties();
	}
}

void ParkingSimulator::setStart()
{
	if (pathPlanner.GetMap() == nullptr || pathPlanner.GetVehicle() == nullptr)
	{
		WarningErrorMsg warningWindow(Language::getInstance()->GetDictionary()["WarningError_SetStartPosition_Title"], Language::getInstance()->GetDictionary()["WarningError_SetStartPosition_Content"], MessageType::Error);
		warningWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		warningWindow.exec();
		return;
	}

	if (pathPlanner.GetSetEndPosition())
	{
		pathPlanner.SetEndPoint(nullptr);
		clearPathPlannerButtons();
	}

	if (pathPlanner.GetSetStartPosition())
	{
		pathPlanner.SetSetStartPosition(false);
		ui.btnSetStart->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetSetStartPosition(true);
		ui.btnSetStart->setStyleSheet("background-color: #d86a39;");
	}
}

void ParkingSimulator::setEnd()
{
	if (pathPlanner.GetMap() == nullptr || pathPlanner.GetVehicle() == nullptr)
	{
		WarningErrorMsg warningWindow(Language::getInstance()->GetDictionary()["WarningError_SetEndPosition_Title"], Language::getInstance()->GetDictionary()["WarningError_SetEndPosition_Content"], MessageType::Error);
		warningWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		warningWindow.exec();
		return;
	}

	if (pathPlanner.GetSetStartPosition())
	{
		pathPlanner.SetStartPoint(nullptr);
		clearPathPlannerButtons();
	}

	if (pathPlanner.GetSetEndPosition())
	{
		pathPlanner.SetSetEndPosition(false);
		ui.btnSetEnd->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetSetEndPosition(true);
		ui.btnSetEnd->setStyleSheet("background-color: #d86a39;");
	}
}

void ParkingSimulator::showVoronoiGraph()
{
	if (pathPlanner.GetShowVoronoiGraph())
	{
		pathPlanner.SetShowVoronoiGraph(false);
		ui.btnShowVoronoiGraph->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetShowVoronoiGraph(true);
		ui.btnShowVoronoiGraph->setStyleSheet("border: 2px solid #d86a39;");
	}
}

void ParkingSimulator::showFullVoronoiVisibilityGraph()
{
	if (pathPlanner.GetShowFullVoronoiVisibilityGraph())
	{
		pathPlanner.SetShowFullVoronoiVisibilityGraph(false);
		ui.btnShowFullVoronoiVisibilityGraph->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetShowFullVoronoiVisibilityGraph(true);
		ui.btnShowFullVoronoiVisibilityGraph->setStyleSheet("border: 2px solid #d86a39;");
	}
}

void ParkingSimulator::showPolylinePath()
{
	if (pathPlanner.GetShowPolylinePath())
	{
		pathPlanner.SetShowPolylinePath(false);
		ui.btnShowPolylinePath->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetShowPolylinePath(true);
		ui.btnShowPolylinePath->setStyleSheet("border: 2px solid #d86a39;");
	}
}

void ParkingSimulator::showParkingPath()
{
	if (pathPlanner.GetShowParkingPath())
	{
		pathPlanner.SetShowParkingPath(false);
		ui.btnShowParkingPath->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetShowParkingPath(true);
		ui.btnShowParkingPath->setStyleSheet("border: 2px solid #d86a39;");
	}
}

void ParkingSimulator::showFinalPath()
{
	if (pathPlanner.GetShowFinalPath())
	{
		pathPlanner.SetShowFinalPath(false);
		ui.btnShowFinalPath->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetShowFinalPath(true);
		ui.btnShowFinalPath->setStyleSheet("border: 2px solid #d86a39;");
	}
}

void ParkingSimulator::showExpandedObstacles()
{
	if (pathPlanner.GetShowExpandedObstacles())
	{
		pathPlanner.SetShowExpandedObstacles(false);
		ui.btnShowExpandedObstacles->setStyleSheet("");
	}
	else
	{
		pathPlanner.SetShowExpandedObstacles(true);
		ui.btnShowExpandedObstacles->setStyleSheet("border: 2px solid #d86a39;");
	}
}

void ParkingSimulator::findPath()
{
	FindPath findPathWindow;
	findPathWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

	if (pathPlanner.GetMap() == NULL || pathPlanner.GetVehicle() == NULL 
		|| (pathPlanner.GetStartPoint() != NULL && pathPlanner.GetStartDirection() != NULL && pathPlanner.GetStartParkingSpace() != NULL)
		|| (pathPlanner.GetEndPoint() != NULL && pathPlanner.GetEndDirection() != NULL && pathPlanner.GetEndParkingSpace() != NULL))
	{
		WarningErrorMsg warningWindow(Language::getInstance()->GetDictionary()["WarningError_FindPathNoParameters_Title"], Language::getInstance()->GetDictionary()["WarningError_FindPathNoParameters_Content"], MessageType::Error);
		warningWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		warningWindow.exec();
		return;
	}

	if (findPathWindow.exec())
	{
		pathPlanner.SetSelectedPathElement(nullptr);
		pathPlanner.SetExpandSizePercent(findPathWindow.GetExpandSizePercent());
		pathPlanner.SetCollisionDetectionDensity(findPathWindow.GetCollisionDetectionQuality());
		pathPlanner.SetAlgorithm(findPathWindow.GetPathPlanningAlgorithm());
		pathPlanner.SetUseAdmissibleArcsOnly(findPathWindow.UseOnlyAdmissibleArcs());
		pathPlanner.SetGraphExtraVerticesAlong(findPathWindow.GetGraphExtraVerticesAlong());
		pathPlanner.SetGraphExtraVerticesAcross(findPathWindow.GetGraphExtraVerticesAcross());

		findPathThread.SetPathPlanner(new PathPlanner(pathPlanner));
		findPathThread.start();

		QMovie *movie = new QMovie(QString("%1/Resources/icons/common/loading.gif").arg(QDir::currentPath()));
		ui.pathCalculationInPrograssMovie->setMovie(movie);
		movie->start();

		ui.calculationInProgress->show();
		disablePathPlannerButtons();
	}
}

void ParkingSimulator::clearPathPlannerButtons()
{
	pathPlanner.SetSetStartPosition(false);
	pathPlanner.SetSetStartDirection(false);
	pathPlanner.SetSetEndPosition(false);
	pathPlanner.SetSetEndDirection(false);

	clearPathPlannerButtonsStyle();
}

void ParkingSimulator::clearPathPlannerButtonsStyle()
{
	if (!pathPlanner.GetSetStartPosition())
		ui.btnSetStart->setStyleSheet("");
	if (!pathPlanner.GetSetEndPosition())
		ui.btnSetEnd->setStyleSheet("");
}

void ParkingSimulator::pathElementSelectionChange()
{
	int selectedIndex = ui.pathElementsList->currentRow();
	if (selectedIndex >= 0 && selectedIndex < pathPlanner.GetFinalPath()->GetElements().size())
	{
		int index = 0;
		auto pathElements = pathPlanner.GetFinalPath()->GetElements();
		for(int i = 0; i < pathElements.size(); i++)
		{
			if(dynamic_cast<Turn*>(pathElements[i]) == NULL)
				index++;
			if (index - 1 == selectedIndex)
			{
				pathPlanner.SetSelectedPathElement(pathElements[i]);
				break;
			}
		}
	}
}

void ParkingSimulator::enablePathPlannerButtons()
{
	ui.btnNewSimulation->setEnabled(true);
	ui.btnOpenSimulation->setEnabled(true);
	ui.btnSaveSimulation->setEnabled(true);
	ui.btnSetMap->setEnabled(true);
	ui.btnSetVehicle->setEnabled(true);
	ui.btnSetStart->setEnabled(true);
	ui.btnSetEnd->setEnabled(true);
	ui.btnFindPath->setEnabled(true);
	ui.btnShowExpandedObstacles->setEnabled(true);
	ui.btnShowVoronoiGraph->setEnabled(true);
	ui.btnShowFullVoronoiVisibilityGraph->setEnabled(true);
	ui.btnShowPolylinePath->setEnabled(true);
	ui.btnShowParkingPath->setEnabled(true);
	ui.btnShowFinalPath->setEnabled(true);
}

void ParkingSimulator::disablePathPlannerButtons()
{
	ui.btnNewSimulation->setEnabled(false);
	ui.btnOpenSimulation->setEnabled(false);
	ui.btnSaveSimulation->setEnabled(false);
	ui.btnSetMap->setEnabled(false);
	ui.btnSetVehicle->setEnabled(false);
	ui.btnSetStart->setEnabled(false);
	ui.btnSetEnd->setEnabled(false);
	ui.btnFindPath->setEnabled(false);
	ui.btnShowExpandedObstacles->setEnabled(false);
	ui.btnShowVoronoiGraph->setEnabled(false);
	ui.btnShowFullVoronoiVisibilityGraph->setEnabled(false);
	ui.btnShowPolylinePath->setEnabled(false);
	ui.btnShowParkingPath->setEnabled(false);
	ui.btnShowFinalPath->setEnabled(false);
}

void ParkingSimulator::cancelPathCalculation()
{
	while (!findPathThread.GetPathPlanner()->GetCanCancelCalculation());
	
	findPathThread.terminate();
}

void ParkingSimulator::pathCalculationFinished()
{
	if (findPathThread.GetPathPlanner()->GetIsCalculationCompleted())
	{
		pathPlanner.SetExpandedMap(findPathThread.GetPathPlanner()->GetExpandedMap());
		pathPlanner.SetVoronoiGraph(findPathThread.GetPathPlanner()->GetVoronoiGraph());
		pathPlanner.SetFullVoronoiVisibilityGraph(findPathThread.GetPathPlanner()->GetFullVoronoiVisibilityGraph());
		pathPlanner.SetPolylinePath(findPathThread.GetPathPlanner()->GetPolylinePath());
		pathPlanner.SetParkingPathStart(findPathThread.GetPathPlanner()->GetParkingPathStart());
		pathPlanner.SetParkingPathEnd(findPathThread.GetPathPlanner()->GetParkingPathEnd());
		pathPlanner.SetFinalPath(findPathThread.GetPathPlanner()->GetFinalPath());
	}

	ui.calculationInProgress->hide();
	enablePathPlannerButtons();

	if (pathPlanner.GetFinalPath() == NULL)
	{
		WarningErrorMsg warningWindow(Language::getInstance()->GetDictionary()["WarningError_PathNotFound_Title"], Language::getInstance()->GetDictionary()["WarningError_PathNotFound_Content"], MessageType::Error);
		warningWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		warningWindow.exec();
	}

	setPathProperties();
}

void ParkingSimulator::setMapProperties()
{
	if (pathPlanner.GetMap() != NULL)
	{
		Map *map = pathPlanner.GetMap();

		ui.isMapSet->hide();
		ui.mapPropertiesForm->show();
		ui.mapWidthLabel->setText(QString("%1 m").arg(QString::number(CommonHelper::ConvertPixelsToMeters(map->GetWidth()), 'f', 2)));
		ui.mapHeightLabel->setText(QString("%1 m").arg(QString::number(CommonHelper::ConvertPixelsToMeters(map->GetHeight()), 'f', 2)));
		ui.mapElementsCountLabel->setText(QString("%1").arg(map->GetMapElements().size()));
	}
	else
	{
		ui.isMapSet->show();
		ui.mapPropertiesForm->hide();
	}
}

void ParkingSimulator::setVehicleProperties()
{
	if (pathPlanner.GetVehicle() != NULL)
	{
		Vehicle *vehicle = pathPlanner.GetVehicle();

		ui.isVehicleSet->hide();
		ui.vehiclePropertiesForm->show();
		ui.vehicleNameLabel->setText(QString::fromStdString(vehicle->GetName()));
		ui.vehicleWheelbaseLabel->setText(QString("%1 m").arg(QString::number(CommonHelper::ConvertPixelsToMeters(vehicle->GetWheelbase()), 'f', 2)));
		ui.vehicleTrackLabel->setText(QString("%1 m").arg(QString::number(CommonHelper::ConvertPixelsToMeters(vehicle->GetTrack()), 'f', 2)));
		ui.vehicleMaxAngleLabel->setText(QString("%1 deg").arg(QString::number(CommonHelper::ConvertRadiansToDegrees(vehicle->GetMaxInsideAngle()), 'f', 2)));
	}
	else
	{
		ui.isVehicleSet->show();
		ui.vehiclePropertiesForm->hide();
	}
}

void ParkingSimulator::setPathProperties()
{
	if (pathPlanner.GetStartPoint() != NULL)
	{
		glm::vec2 *startPoint = pathPlanner.GetStartPoint();
		glm::vec2 *startDirection = pathPlanner.GetStartDirection();
		float angle = CommonHelper::ConvertRadiansToDegrees(GeometryHelper::GetAngleBetweenVectors(glm::vec2(1, 0), *startDirection));

		ui.pathStartPositionLabel->setText(QString("X: %1, Y: %2\nAngle: %3 deg").arg(QString::number(startPoint->x, 'f', 2)).arg(QString::number(startPoint->y, 'f', 2)).arg(QString::number(angle, 'f', 2)));
	}

	if (pathPlanner.GetEndPoint() != NULL)
	{
		glm::vec2 *endPoint = pathPlanner.GetEndPoint();
		glm::vec2 *endDirection = pathPlanner.GetEndDirection();
		float angle = CommonHelper::ConvertRadiansToDegrees(GeometryHelper::GetAngleBetweenVectors(glm::vec2(1, 0), *endDirection));

		ui.pathEndPositionLabel->setText(QString("X: %1, Y: %2\nAngle: %3 deg").arg(QString::number(endPoint->x, 'f', 2)).arg(QString::number(endPoint->y, 'f', 2)).arg(QString::number(angle, 'f', 2)));
	}

	if (pathPlanner.GetStartParkingSpace() != NULL)
	{
		ParkingSpace *startParkingSpace = pathPlanner.GetStartParkingSpace();

		ui.pathStartPositionLabel->setText(QString("X: %1, Y: %2\nAngle: %3 deg").arg(QString::number(CommonHelper::ConvertPixelsToMeters(startParkingSpace->GetPosition().x), 'f', 2)).arg(QString::number(CommonHelper::ConvertPixelsToMeters(startParkingSpace->GetPosition().y), 'f', 2)).arg(QString::number(CommonHelper::ConvertRadiansToDegrees(startParkingSpace->GetRotation()), 'f', 2)));
	}

	if (pathPlanner.GetEndParkingSpace() != NULL)
	{
		ParkingSpace *endParkingSpace = pathPlanner.GetEndParkingSpace();

		ui.pathEndPositionLabel->setText(QString("X: %1, Y: %2\nAngle: %3 deg").arg(QString::number(CommonHelper::ConvertPixelsToMeters(endParkingSpace->GetPosition().x), 'f', 2)).arg(QString::number(CommonHelper::ConvertPixelsToMeters(endParkingSpace->GetPosition().y), 'f', 2)).arg(QString::number(CommonHelper::ConvertRadiansToDegrees(endParkingSpace->GetRotation()), 'f', 2)));
	}

	if (pathPlanner.GetStartPoint() == NULL && pathPlanner.GetStartParkingSpace() == NULL)
	{
		ui.pathStartPositionLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_NotSet"]));
	}

	if (pathPlanner.GetEndPoint() == NULL && pathPlanner.GetEndParkingSpace() == NULL)
	{
		ui.pathEndPositionLabel->setText(QString::fromStdString(Language::getInstance()->GetDictionary()["PathPlanner_NotSet"]));
	}

	if (pathPlanner.GetFinalPath() != NULL && pathPlanner.GetFinalPath()->GetElements().size() > 0)
	{
		int pathElementsCount = pathPlanner.GetFinalPath()->GetElements().size();

		ui.isPathSet->hide();
		ui.pathElementsList->clear();
		int index = 0;
		for (int i = 0; i < pathElementsCount; i++)
			if (dynamic_cast<Turn*>(pathPlanner.GetFinalPath()->GetElements()[i]) == NULL)
			{
				ui.pathElementsList->addItem(new QListWidgetItem(QString("%1 %2").arg(QString::fromStdString(Language::getInstance()->GetDictionary()["Common_PathElement"])).arg(index + 1)));
				index++;
			}
		ui.pathElementsList->show();
	}
	else
	{
		ui.isPathSet->show();
		ui.pathElementsList->hide();
	}
}

#pragma endregion

#pragma region Visualisation.

void ParkingSimulator::addSimulation()
{
	AddSimulation addSimulationWindow(&pathPlanner);
	addSimulationWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	if (addSimulationWindow.exec())
	{
		Simulation *simulation = addSimulationWindow.GetSimulation();

		visualisation.AddSimulation(simulation);
		visualisation.SetCurrentSimulation(simulation);
		ui.listSimulations->addItem(new QListWidgetItem(QString::fromStdString(simulation->GetName())));
		ui.listSimulations->setCurrentRow(visualisation.GetSimulations().size() - 1);
	}
}

void ParkingSimulator::removeSimulation()
{
	int selectedIndex = ui.listSimulations->currentRow();
	if (selectedIndex >= 0 && selectedIndex < visualisation.GetSimulations().size())
	{
		Simulation *selectedSimulation = visualisation.GetSimulations()[selectedIndex];
		visualisation.RemoveSimulation(selectedSimulation);
		
		ui.listSimulations->clear();
		std::vector<Simulation*> simulations = visualisation.GetSimulations();
		for (int i = 0; i < simulations.size(); i++)
			ui.listSimulations->addItem(new QListWidgetItem(QString::fromStdString(simulations[i]->GetName())));
		if (simulations.size() > 0)
		{
			if (selectedIndex < simulations.size())
				ui.listSimulations->setCurrentRow(selectedIndex);
			else
				ui.listSimulations->setCurrentRow(selectedIndex - 1);
		}
	}
}

void ParkingSimulator::infoSimulation()
{
	int selectedIndex = ui.listSimulations->currentRow();
	if (selectedIndex >= 0 && selectedIndex < visualisation.GetSimulations().size())
	{
		Simulation *selectedSimulation = visualisation.GetSimulations()[selectedIndex];
		SimulationInfo simulationInfoWindow(selectedSimulation);
		simulationInfoWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		simulationInfoWindow.exec();
	}
}

void ParkingSimulator::playPauseSimulation()
{
	if (visualisation.GetCurrentSimulation() != NULL)
	{
		Simulation *simulation = visualisation.GetCurrentSimulation();
		if (simulation->IsStarted())
			simulation->Pause();
		else
		{
			float t = ui.simulationPrograssBar->value() / 1000.0f;
			simulation->SetSimulationTime(ui.simulationLength->value());
			simulation->SetCurrentSimulationTime(t * simulation->GetSimulationTime());
			simulation->Start();
		}
	}
}

void ParkingSimulator::stopSimulation()
{
	if (visualisation.GetCurrentSimulation() != NULL)
	{
		Simulation *simulation = visualisation.GetCurrentSimulation();

		simulation->Stop();
	}
}

void ParkingSimulator::enableVisualisation2D()
{
	visualisation.SetVisualisation2D(true);
	visualisation.SetVisualisation3D(false);
	ui.btnVisualisation2D->setStyleSheet("border: 2px solid #d86a39; color: #d86a39;");
	ui.btnVisualisation3D->setStyleSheet("");
	ui.glVisualisation->show();
	ui.glVisualisation3D->hide();
}

void ParkingSimulator::enableVisualisation3D()
{
	visualisation.SetVisualisation2D(false);
	visualisation.SetVisualisation3D(true);
	ui.btnVisualisation2D->setStyleSheet("");
	ui.btnVisualisation3D->setStyleSheet("border: 2px solid #d86a39; color: #d86a39;");
	ui.glVisualisation->hide();
	ui.glVisualisation3D->show();
}

void ParkingSimulator::showSimulationPath(int checked)
{
	visualisation.SetShowPath(checked == Qt::Checked);
}

void ParkingSimulator::simulationProgressBarChange(int time)
{
	float t = time / 1000.0f;

	if (t == 1.0f)
		t = 0.999f;

	if (visualisation.GetCurrentSimulation() != NULL)
	{
		Simulation *simulation = visualisation.GetCurrentSimulation();

		simulation->SetCurrentSimulationTime(t * simulation->GetSimulationTime());

		visualisation.GetCurrentSimulation()->UpdateSimulationState();
	}
}

void ParkingSimulator::simulationSelectionChange()
{
	int selectedIndex = ui.listSimulations->currentRow();
	if (selectedIndex >= 0 && selectedIndex < visualisation.GetSimulations().size())
		visualisation.SetCurrentSimulation(visualisation.GetSimulations()[selectedIndex]);
	else
		visualisation.SetCurrentSimulation(nullptr);
}

#pragma endregion

#pragma region Settings.

void ParkingSimulator::loadSettings()
{
	Settings::getInstance()->LoadSettings();

	ui.buildingBorderWidthSpinBox->setValue(Settings::getInstance()->GetInt("BUILDING_BORDER_WIDTH"));
	ui.decorationBorderWidthSpinBox->setValue(Settings::getInstance()->GetInt("DECORATION_BORDER_WIDTH"));
	ui.parkingPlaceBorderWidthSpinBox->setValue(Settings::getInstance()->GetInt("PARKING_SPACE_BORDER_WIDTH"));
	ui.vehicleBorderWidthSpinBox->setValue(Settings::getInstance()->GetInt("VEHICLE_BORDER_WIDTH"));
	ui.hoverMapElementBorderWidthSpinBox->setValue(Settings::getInstance()->GetInt("MAP_ELEMENT_HOVER_BORDER_WIDTH"));
	ui.selectedMapElementBorderWidthSpinBox->setValue(Settings::getInstance()->GetInt("MAP_ELEMENT_SELECTED_BORDER_WIDTH"));
	ui.mapBorderWidthSpinBox->setValue(Settings::getInstance()->GetInt("MAP_BORDER_WIDTH"));
	ui.lineWidthSpinBox->setValue(Settings::getInstance()->GetInt("PATH_LINE_WIDTH"));
	ui.circleWidthSpinBox->setValue(Settings::getInstance()->GetInt("PATH_CIRCLE_WIDTH"));
	ui.splineWidthSpinBox->setValue(Settings::getInstance()->GetInt("PATH_BSPLINE_WIDTH"));
	ui.selectedPathElementWidthSpinBox->setValue(Settings::getInstance()->GetInt("PATH_ELEMENT_SELECTED_WIDTH"));
	ui.vertexRadiusSpinBox->setValue(Settings::getInstance()->GetInt("GRAPH_VERTEX_RADIUS"));
	ui.edgeWidthSpinBox->setValue(Settings::getInstance()->GetInt("GRAPH_EDGE_WIDTH"));
	ui.admissibleElementBorderWidthSpinBox->setValue(Settings::getInstance()->GetInt("ACTIVE_GOOD_BORDER_WIDTH"));
	ui.notAdmissibleElementBorderWidthSpinBox->setValue(Settings::getInstance()->GetInt("ACTIVE_BAD_BORDER_WIDTH"));
	ui.buildingBorderColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("BUILDING_BORDER_COLOR").name()));
	ui.buildingColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("BUILDING_COLOR").name()));
	ui.decorationBorderColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("DECORATION_BORDER_COLOR").name()));
	ui.decorationColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("DECORATION_COLOR").name()));
	ui.parkingPlaceBorderColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("PARKING_SPACE_BORDER_COLOR").name()));
	ui.parkingPlaceColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("PARKING_SPACE_COLOR").name()));
	ui.vehicleBorderColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("VEHICLE_BORDER_COLOR").name()));
	ui.vehicleColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("VEHICLE_COLOR").name()));
	ui.hoverMapElementBorderColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("MAP_ELEMENT_HOVER_BORDER_COLOR").name()));
	ui.hoverMapElementColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("MAP_ELEMENT_HOVER_COLOR").name()));
	ui.selectedMapElementBorderColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("MAP_ELEMENT_SELECTED_BORDER_COLOR").name()));
	ui.selectedMapElementColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("MAP_ELEMENT_SELECTED_COLOR").name()));
	ui.mapBorderColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("MAP_BORDER_COLOR").name()));
	ui.mapColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("MAP_COLOR").name()));
	ui.lineColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("PATH_LINE_COLOR").name()));
	ui.arcColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("PATH_CIRCLE_COLOR").name()));
	ui.splineColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("PATH_BSPLINE_COLOR").name()));
	ui.selectedPathElementColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("PATH_ELEMENT_SELECTED_COLOR").name()));
	ui.graphVertexColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("GRAPH_VERTEX_COLOR").name()));
	ui.graphEdgeColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("GRAPH_EDGE_COLOR").name()));
	ui.admissibleElementBorderColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("ACTIVE_GOOD_BORDER_COLOR").name()));
	ui.admissibleElementColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("ACTIVE_GOOD_COLOR").name()));
	ui.notAdmissibleElementBorderColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("ACTIVE_BAD_BORDER_COLOR").name()));
	ui.notAdmissibleElementColor->setStyleSheet(QString("background-color: %1;").arg(Settings::getInstance()->GetQColor("ACTIVE_BAD_COLOR").name()));
}

void ParkingSimulator::saveSettings()
{
	Settings::getInstance()->SetInt("BUILDING_BORDER_WIDTH", ui.buildingBorderWidthSpinBox->value());
	Settings::getInstance()->SetInt("DECORATION_BORDER_WIDTH", ui.decorationBorderWidthSpinBox->value());
	Settings::getInstance()->SetInt("PARKING_SPACE_BORDER_WIDTH", ui.parkingPlaceBorderWidthSpinBox->value());
	Settings::getInstance()->SetInt("VEHICLE_BORDER_WIDTH", ui.vehicleBorderWidthSpinBox->value());
	Settings::getInstance()->SetInt("MAP_ELEMENT_HOVER_BORDER_WIDTH", ui.hoverMapElementBorderWidthSpinBox->value());
	Settings::getInstance()->SetInt("MAP_ELEMENT_SELECTED_BORDER_WIDTH", ui.selectedMapElementBorderWidthSpinBox->value());
	Settings::getInstance()->SetInt("MAP_BORDER_WIDTH", ui.mapBorderWidthSpinBox->value());
	Settings::getInstance()->SetInt("PATH_LINE_WIDTH", ui.lineWidthSpinBox->value());
	Settings::getInstance()->SetInt("PATH_CIRCLE_WIDTH", ui.circleWidthSpinBox->value());
	Settings::getInstance()->SetInt("PATH_BSPLINE_WIDTH", ui.splineWidthSpinBox->value());
	Settings::getInstance()->SetInt("PATH_ELEMENT_SELECTED_WIDTH", ui.selectedPathElementWidthSpinBox->value());
	Settings::getInstance()->SetInt("GRAPH_VERTEX_RADIUS", ui.vertexRadiusSpinBox->value());
	Settings::getInstance()->SetInt("GRAPH_EDGE_WIDTH", ui.edgeWidthSpinBox->value());
	Settings::getInstance()->SetInt("ACTIVE_GOOD_BORDER_WIDTH", ui.admissibleElementBorderWidthSpinBox->value());
	Settings::getInstance()->SetInt("ACTIVE_BAD_BORDER_WIDTH", ui.notAdmissibleElementBorderWidthSpinBox->value());
	Settings::getInstance()->SetColor("BUILDING_BORDER_COLOR", ui.buildingBorderColor->palette().button().color());
	Settings::getInstance()->SetColor("BUILDING_COLOR", ui.buildingColor->palette().button().color());
	Settings::getInstance()->SetColor("DECORATION_BORDER_COLOR", ui.decorationBorderColor->palette().button().color());
	Settings::getInstance()->SetColor("DECORATION_COLOR", ui.decorationColor->palette().button().color());
	Settings::getInstance()->SetColor("PARKING_SPACE_BORDER_COLOR", ui.parkingPlaceBorderColor->palette().button().color());
	Settings::getInstance()->SetColor("PARKING_SPACE_COLOR", ui.parkingPlaceColor->palette().button().color());
	Settings::getInstance()->SetColor("VEHICLE_BORDER_COLOR", ui.vehicleBorderColor->palette().button().color());
	Settings::getInstance()->SetColor("VEHICLE_COLOR", ui.vehicleColor->palette().button().color());
	Settings::getInstance()->SetColor("MAP_ELEMENT_HOVER_BORDER_COLOR", ui.hoverMapElementBorderColor->palette().button().color());
	Settings::getInstance()->SetColor("MAP_ELEMENT_HOVER_COLOR", ui.hoverMapElementColor->palette().button().color());
	Settings::getInstance()->SetColor("MAP_ELEMENT_SELECTED_COLOR", ui.selectedMapElementBorderColor->palette().button().color());
	Settings::getInstance()->SetColor("MAP_ELEMENT_SELECTED_COLOR", ui.selectedMapElementColor->palette().button().color());
	Settings::getInstance()->SetColor("MAP_BORDER_COLOR", ui.mapBorderColor->palette().button().color());
	Settings::getInstance()->SetColor("MAP_COLOR", ui.mapColor->palette().button().color());
	Settings::getInstance()->SetColor("PATH_LINE_COLOR", ui.lineColor->palette().button().color());
	Settings::getInstance()->SetColor("PATH_CIRCLE_COLOR", ui.arcColor->palette().button().color());
	Settings::getInstance()->SetColor("PATH_BSPLINE_COLOR", ui.splineColor->palette().button().color());
	Settings::getInstance()->SetColor("PATH_ELEMENT_SELECTED_COLOR", ui.selectedPathElementColor->palette().button().color());
	Settings::getInstance()->SetColor("GRAPH_VERTEX_COLOR", ui.graphVertexColor->palette().button().color());
	Settings::getInstance()->SetColor("GRAPH_EDGE_COLOR", ui.graphEdgeColor->palette().button().color());
	Settings::getInstance()->SetColor("ACTIVE_GOOD_BORDER_COLOR", ui.admissibleElementBorderColor->palette().button().color());
	Settings::getInstance()->SetColor("ACTIVE_GOOD_COLOR", ui.admissibleElementColor->palette().button().color());
	Settings::getInstance()->SetColor("ACTIVE_BAD_BORDER_COLOR", ui.notAdmissibleElementBorderColor->palette().button().color());
	Settings::getInstance()->SetColor("ACTIVE_BAD_COLOR", ui.notAdmissibleElementColor->palette().button().color());
	Settings::getInstance()->SetLanguage(ui.languagesList->currentText().toStdString());
	Settings::getInstance()->SaveSettings();
	Language::getInstance()->LoadLanguage(Settings::getInstance()->GetLanguage());

	setLanguage();
	((MapEditorGLHost*)ui.glMapEditor)->UpdateSettings();
	((PathPlannerGLHost*)ui.glPathPlanner)->UpdateSettings();
	((VisualisationGLHost*)ui.glVisualisation)->UpdateSettings();
}

void ParkingSimulator::initializeLanguages()
{
	path p(".");
	std::vector<directory_entry> v;

	if (is_directory(p))
	{
		copy(directory_iterator(p), directory_iterator(), back_inserter(v));
		for (std::vector<directory_entry>::const_iterator it = v.begin(); it != v.end(); ++it)
		{
			std::string path = (*it).path().string();
			if (path.find("lang") != std::string::npos)
			{
				int start = path.find("lang.") + 5;
				int end = path.find(".ini");
				ui.languagesList->addItem(QString::fromStdString(path.substr(start, end-start)));
			}
		}
	}
}

void ParkingSimulator::changeColor(ColorContainer & pos)
{
	QColorDialog colorDialog;
	
	auto name = colorDialog.getColor().name();
	
	pos.setStyleSheet(QString("background-color: %1;").arg(name));
}

#pragma endregion