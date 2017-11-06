#include "Language.h"

Language* Language::s_instance = NULL;

void Language::LoadLanguage(std::string lang)
{
	QString languageFilePath = QString("C:\\Users\\Wojtek\\Source\\Repos\\ParkingSimulator-NEW\\source\\ParkingSimulator\\x64\\Debug\\lang.%1.ini").arg(QString::fromStdString(lang));

	QSettings language(languageFilePath, QSettings::IniFormat);

	dictionary.clear();

	dictionary.insert(std::pair<std::string, std::string>("Title", language.value("Title").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Tab_Start", language.value("Tab_Start").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Tab_MapEditor", language.value("Tab_MapEditor").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Tab_VehicleEditor", language.value("Tab_VehicleEditor").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Tab_PathPlanner", language.value("Tab_PathPlanner").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Tab_Visualisation", language.value("Tab_Visualisation").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Tab_Settings", language.value("Tab_Settings").toString().toStdString()));

	dictionary.insert(std::pair<std::string, std::string>("Start_UniversityName", language.value("Start_UniversityName").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Start_FacultyName", language.value("Start_FacultyName").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Start_MastersThesis", language.value("Start_MastersThesis").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Start_ThesisTitle", language.value("Start_ThesisTitle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Start_ThesisAuthor", language.value("Start_ThesisAuthor").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Start_ThesisSupervisor", language.value("Start_ThesisSupervisor").toString().toStdString()));

	dictionary.insert(std::pair<std::string, std::string>("Start_GoToMapEditor", language.value("Start_GoToMapEditor").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Start_GoToVehicleEditor", language.value("Start_GoToMapEditor").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Start_GoToPathPlanner", language.value("Start_GoToPathPlanner").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Start_GoToVisualisation", language.value("Start_GoToVisualisation").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Start_GoToSettings", language.value("Start_GoToSettings").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Start_GoToExit", language.value("Start_GoToExit").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_NewMap", language.value("MapEditor_NewMap").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_OpenMap", language.value("MapEditor_OpenMap").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_SaveMap", language.value("MapEditor_SaveMap").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_ClearMap", language.value("MapEditor_ClearMap").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_CreateNewMap", language.value("MapEditor_CreateNewMap").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_AddMapElement", language.value("MapEditor_AddMapElement").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_AddBuilding", language.value("MapEditor_AddBuilding").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_AddDecoration", language.value("MapEditor_AddDecoration").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_AddParkingPlace", language.value("MapEditor_AddParkingPlace").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_AddVehicle", language.value("MapEditor_AddVehicle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_AddTerrain", language.value("MapEditor_AddTerrain").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_MapElements", language.value("MapEditor_MapElements").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_Buildings", language.value("MapEditor_Buildings").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_Decorations", language.value("MapEditor_Decorations").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_ParkingPlaces", language.value("MapEditor_ParkingPlaces").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_Vehicles", language.value("MapEditor_Vehicles").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("MapEditor_MapElementProperties", language.value("MapEditor_MapElementProperties").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_NewSimulation", language.value("PathPlanner_NewSimulation").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_OpenSimulation", language.value("PathPlanner_OpenSimulation").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_SaveSimulation", language.value("PathPlanner_SaveSimulation").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_SetMap", language.value("PathPlanner_SetMap").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_SetMap_FromMapEditor", language.value("PathPlanner_SetMap_FromMapEditor").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_SetMap_FromFile", language.value("PathPlanner_SetMap_FromFile").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_SetVehicle", language.value("PathPlanner_SetVehicle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_SetStart", language.value("PathPlanner_SetStart").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_SetEnd", language.value("PathPlanner_SetEnd").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_FindPath", language.value("PathPlanner_FindPath").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_PathElementProperties", language.value("PathPlanner_PathElementProperties").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_Algorithm", language.value("PathPlanner_Algorithm").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_Algorithm_Arcs", language.value("PathPlanner_Algorithm_Arcs").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_Algorithm_BSpline", language.value("PathPlanner_Algorithm_BSpline").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_UseAdmissibleArcsOnly", language.value("PathPlanner_UseAdmissibleArcsOnly").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_CollisionDetectionQuality", language.value("PathPlanner_CollisionDetectionQuality").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Visualisation_AddSimulation", language.value("Visualisation_AddSimulation").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Visualisation_AddSimulation_FromPathPlanner", language.value("Visualisation_AddSimulation_FromPathPlanner").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Visualisation_AddSimulation_FromFile", language.value("Visualisation_AddSimulation_FromFile").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Visualization_RemoveSimulation", language.value("Visualization_RemoveSimulation").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Visualisation_InfoSimulation", language.value("Visualisation_InfoSimulation").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Visualisation_Play", language.value("Visualisation_Play").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Visualisation_Pause", language.value("Visualisation_Pause").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Visualisation_Stop", language.value("Visualisation_Stop").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Visualisation_SimulationTime", language.value("Visualisation_SimulationTime").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Visualisation_SimulationFromPathPlanner", language.value("Visualisation_SimulationFromPathPlanner").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Name", language.value("Common_Name").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Position", language.value("Common_Position").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Rotation", language.value("Common_Rotation").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Width", language.value("Common_Width").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Height", language.value("Common_Height").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Save", language.value("Common_Save").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Remove", language.value("Common_Remove").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Cancel", language.value("Common_Cancel").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Close", language.value("Common_Close").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_CloseWindow", language.value("Common_CloseWindow").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_NoSimulation_Title", language.value("WarningError_NoSimulation_Title").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_NoSimulation_Content", language.value("WarningError_NoSimulation_Content").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_PathNotFound_Title", language.value("WarningError_PathNotFound_Title").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_PathNotFound_Content", language.value("WarningError_PathNotFound_Content").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_FindPathNoParameters_Title", language.value("WarningError_FindPathNoParameters_Title").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_FindPathNoParameters_Content", language.value("WarningError_FindPathNoParameters_Content").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_NoMapInMapEditor_Title", language.value("WarningError_NoMapInMapEditor_Title").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_NoMapInMapEditor_Content", language.value("WarningError_NoMapInMapEditor_Content").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_MapElementNotAdmissible_Title", language.value("WarningError_MapElementNotAdmissible_Title").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_MapElementNotAdmissible_Content", language.value("WarningError_MapElementNotAdmissible_Content").toString().toStdString()));

	dictionary.insert(std::pair<std::string, std::string>("Common_Create", language.value("Common_Create").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_Map", language.value("PathPlanner_Map").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_Vehicle", language.value("PathPlanner_Vehicle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_Path", language.value("PathPlanner_Path").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_NotSet", language.value("PathPlanner_NotSet").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_NotCreated", language.value("PathPlanner_NotCreated").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_StartPosition", language.value("PathPlanner_StartPosition").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_EndPosition", language.value("PathPlanner_EndPosition").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Visualisation_PlayPause", language.value("Visualisation_PlayPause").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_ShowExpandedObstacles", language.value("PathPlanner_ShowExpandedObstacles").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_ShowFinalPath", language.value("PathPlanner_ShowFinalPath").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_ShowFullVoronoiVisibilityGraph", language.value("PathPlanner_ShowFullVoronoiVisibilityGraph").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_ShowParkingPath", language.value("PathPlanner_ShowParkingPath").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_ShowPolylinePath", language.value("PathPlanner_ShowPolylinePath").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_ShowVoronoiGraph", language.value("PathPlanner_ShowVoronoiGraph").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_MapElementsCount", language.value("PathPlanner_MapElementsCount").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_VehicleWheelbase", language.value("PathPlanner_VehicleWheelbase").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_VehicleTrack", language.value("PathPlanner_VehicleTrack").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_VehicleMaxAngle", language.value("PathPlanner_VehicleMaxAngle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Type", language.value("Common_Type").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_From", language.value("Common_From").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_To", language.value("Common_To").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Length", language.value("Common_Length").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Visualisation_ShowPathElements", language.value("Visualisation_ShowPathElements").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("SimulationInfo_VehicleName", language.value("SimulationInfo_VehicleName").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("SimulationInfo_PathElements", language.value("SimulationInfo_PathElements").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("SimulationInfo_PathLength", language.value("SimulationInfo_PathLength").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("SimulationInfo_ObstaclesExpandSizePercent", language.value("SimulationInfo_ObstaclesExpandSizePercent").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("SimulationInfo_PathPlanningAlgorithm", language.value("SimulationInfo_PathPlanningAlgorithm").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("SimulationInfo_UseAdmissibleArcsOnly", language.value("SimulationInfo_UseAdmissibleArcsOnly").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("SimulationInfo_CollisionDetectionQuality", language.value("SimulationInfo_CollisionDetectionQuality").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("AddMapElement_Title", language.value("AddMapElement_Title").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("AddMapElement_SelectBuilding", language.value("AddMapElement_SelectBuilding").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("AddMapElement_SelectDecoration", language.value("AddMapElement_SelectDecoration").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("AddMapElement_SelectParkingPlace", language.value("AddMapElement_SelectParkingPlace").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("AddMapElement_SelectVehicle", language.value("AddMapElement_SelectVehicle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("AddMapElement_SelectTerrain", language.value("AddMapElement_SelectTerrain").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Add", language.value("Common_Add").toString().toStdString()));

	dictionary.insert(std::pair<std::string, std::string>("Settings_Language", language.value("Settings_Language").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_ChooseLanguage", language.value("Settings_ChooseLanguage").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_MapElements", language.value("Settings_MapElements").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_MapElementsBuilding", language.value("Settings_MapElementsBuilding").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_MapElementsDecoration", language.value("Settings_MapElementsDecoration").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_MapElementsPrakingPlace", language.value("Settings_MapElementsPrakingPlace").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_MapElementsVehicle", language.value("Settings_MapElementsVehicle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_MapElementsHoverElement", language.value("Settings_MapElementsHoverElement").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_MapElementsSelectedElement", language.value("Settings_MapElementsSelectedElement").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_MapElementsTransformed", language.value("Settings_MapElementsTransformed").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_MapElementsTransformedAdmissible", language.value("Settings_MapElementsTransformedAdmissible").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_MapElementsTransformedNotAdmissible", language.value("Settings_MapElementsTransformedNotAdmissible").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_Graph", language.value("Settings_Graph").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_GraphEdge", language.value("Settings_GraphEdge").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_GraphEdgeWidth", language.value("Settings_GraphEdgeWidth").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_GraphEdgeColor", language.value("Settings_GraphEdgeColor").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_GraphVertex", language.value("Settings_GraphVertex").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_GraphVertexRadius", language.value("Settings_GraphVertexRadius").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_GraphVertexColor", language.value("Settings_GraphVertexColor").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_PathElements", language.value("Settings_PathElements").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_PathElementsLine", language.value("Settings_PathElementsLine").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_PathElementsCircle", language.value("Settings_PathElementsCircle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_PathElementsBSpline", language.value("Settings_PathElementsBSpline").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_PathElementsSelected", language.value("Settings_PathElementsSelected").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_Map", language.value("Settings_Map").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_MapBorderWidth", language.value("Settings_MapBorderWidth").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_MapBorderColor", language.value("Settings_MapBorderColor").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("SettingsMapColor", language.value("SettingsMapColor").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_BorderWidth", language.value("Settings_BorderWidth").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_BorderColor", language.value("Settings_BorderColor").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_FillColor", language.value("Settings_FillColor").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_Width", language.value("Settings_Width").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_Radius", language.value("Settings_Radius").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_Color", language.value("Settings_Color").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Settings_SaveSettings", language.value("Settings_SaveSettings").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_ElementsCount", language.value("Common_ElementsCount").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_PathElement", language.value("Common_PathElement").toString().toStdString()));

	dictionary.insert(std::pair<std::string, std::string>("WarningError_NoVehicleInVehicleEditor_Title", language.value("WarningError_NoVehicleInVehicleEditor_Title").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_NoVehicleInVehicleEditor_Content", language.value("WarningError_NoVehicleInVehicleEditor_Content").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_NewVehicle", language.value("VehicleEditor_NewVehicle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_OpenVehicle", language.value("VehicleEditor_OpenVehicle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_SaveVehicle", language.value("VehicleEditor_SaveVehicle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_VehicleProperties", language.value("VehicleEditor_VehicleProperties").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_VehiclePropertiesName", language.value("VehicleEditor_VehiclePropertiesName").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_VehiclePropertiesWidth", language.value("VehicleEditor_VehiclePropertiesWidth").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_VehiclePropertiesLength", language.value("VehicleEditor_VehiclePropertiesLength").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_VehiclePropertiesTrack", language.value("VehicleEditor_VehiclePropertiesTrack").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_VehiclePropertiesWheelbase", language.value("VehicleEditor_VehiclePropertiesWheelbase").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_MaxAngle", language.value("VehicleEditor_MaxAngle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_VehiclePropertiesApply", language.value("VehicleEditor_VehiclePropertiesApply").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_VehicleModels", language.value("VehicleEditor_VehicleModels").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_MainModel", language.value("VehicleEditor_MainModel").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_FrontLeftWheelModel", language.value("VehicleEditor_FrontLeftWheelModel").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_FrontRightWheelModel", language.value("VehicleEditor_FrontRightWheelModel").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_RearLeftWheelModel", language.value("VehicleEditor_RearLeftWheelModel").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_RearRightWheelModel", language.value("VehicleEditor_RearRightWheelModel").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("VehicleEditor_LoadModel", language.value("VehicleEditor_LoadModel").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Model", language.value("Common_Model").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_Scale", language.value("Common_Scale").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_NotSet", language.value("Common_NotSet").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Common_OK", language.value("Common_OK").toString().toStdString()));

	dictionary.insert(std::pair<std::string, std::string>("WarningError_SetStartPosition_Title", language.value("WarningError_SetStartPosition_Title").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_SetStartPosition_Content", language.value("WarningError_SetStartPosition_Content").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_SetEndPosition_Title", language.value("WarningError_SetEndPosition_Title").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("WarningError_SetEndPosition_Content", language.value("WarningError_SetEndPosition_Content").toString().toStdString()));

	dictionary.insert(std::pair<std::string, std::string>("Quality_Disabled", language.value("Quality_Disabled").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Quality_VeryLow", language.value("Quality_VeryLow").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Quality_Low", language.value("Quality_Low").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Quality_Medium", language.value("Quality_Medium").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Quality_Good", language.value("Quality_Good").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("Quality_VeryGood", language.value("Quality_VeryGood").toString().toStdString()));

	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_GraphExtraVerticesAlong", language.value("PathPlanner_GraphExtraVerticesAlong").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_GraphExtraVerticesAcross", language.value("PathPlanner_GraphExtraVerticesAcross").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_FindPathTitle", language.value("PathPlanner_FindPathTitle").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PathPlanner_ExpandObstaclesPercent", language.value("PathPlanner_ExpandObstaclesPercent").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PleaseWait_LoadingModels", language.value("PleaseWait_LoadingModels").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("PleaseWait_PathCalculation", language.value("PleaseWait_PathCalculation").toString().toStdString()));

	dictionary.insert(std::pair<std::string, std::string>("SimulationInfo_ExtraVerticesAlong", language.value("SimulationInfo_ExtraVerticesAlong").toString().toStdString()));
	dictionary.insert(std::pair<std::string, std::string>("SimulationInfo_ExtraVerticesAcross", language.value("SimulationInfo_ExtraVerticesAcross").toString().toStdString()));
}

std::map<std::string, std::string> Language::GetDictionary()
{
	return this->dictionary;
}