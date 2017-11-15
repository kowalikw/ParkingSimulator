#include "OpenGLHost3D.h"
#include "Settings.h"
#include <QThread>

class LoadModelsThread : public QThread
{

public:
	LoadModelsThread() 
	{
		
	}
	void SetVisualisation(Visualisation *visualisation)
	{
		this->visualisation = visualisation;
	}

	std::map<std::string, Model*> * GetLoadedModels()
	{
		return this->loadedModels;
	}

	Model * GetVehicleModel()
	{
		return this->vehicleModel;
	}

protected:
	void run()
	{
		this->loadedModels = new std::map<std::string, Model*>();

		loadedModels->clear();

		std::map<std::string, std::vector<InstanceData>> instances;
		Map *map = visualisation->GetCurrentSimulation()->GetMap();

		for (int i = 0; i < map->GetTerrainWidthCount(); i++)
		{
			for (int j = 0; j < map->GetTerrainHeightCount(); j++)
			{
				Terrain *terrain = map->GetTerrainSlice(i, j);

				InstanceData instance;
				instance.Position = glm::vec3(glm::vec3(terrain->GetPosition().x, -0.5, terrain->GetPosition().y));
				instance.Rotation = glm::vec3();
				instance.Scale = glm::vec3(1, 1, 1);

				if (instances.count(terrain->GetModelPath()) > 0)
					instances[terrain->GetModelPath()].push_back(instance);
				else
				{
					std::vector<InstanceData> instancesVector;
					instancesVector.push_back(instance);
					instances.insert(std::pair<std::string, std::vector<InstanceData>>(terrain->GetModelPath(), instancesVector));
				}
			}
		}

		std::vector<MapElement*> mapElements = map->GetMapElements();
		for (int i = 0; i < mapElements.size(); i++)
		{
			Model *model = new Model(mapElements[i]->GetModelPath());
			model->MeasureModel();

			double scaleRatioX = mapElements[i]->GetSize().x / model->GetMeasure().x;
			double scaleRatioZ = mapElements[i]->GetSize().y / model->GetMeasure().z;
			double scaleRatioY = (scaleRatioX + scaleRatioZ) / 2.0f;

			delete model;

			InstanceData instance;
			instance.Position = glm::vec3(glm::vec3(mapElements[i]->GetPosition().x, 0, mapElements[i]->GetPosition().y));
			instance.Rotation = glm::vec3(0, mapElements[i]->GetRotation(), 0);
			instance.Scale = glm::vec3(scaleRatioX, scaleRatioY, scaleRatioZ);

			if (instances.count(mapElements[i]->GetModelPath()) > 0)
				instances[mapElements[i]->GetModelPath()].push_back(instance);
			else
			{
				std::vector<InstanceData> instancesVector;
				instancesVector.push_back(instance);
				instances.insert(std::pair<std::string, std::vector<InstanceData>>(mapElements[i]->GetModelPath(), instancesVector));
			}
		}

		std::vector<MapElementModel> terrainsModels = Settings::getInstance()->GetTerrains();
		std::vector<MapElementModel> buildingsModels = Settings::getInstance()->GetBuildings();
		std::vector<MapElementModel> decorationsModels = Settings::getInstance()->GetDecorations();
		std::vector<MapElementModel> parkingPlacesModels = Settings::getInstance()->GetParkingPlaces();
		std::vector<MapElementModel> vehiclesModels = Settings::getInstance()->GetVehicles();

		for (int i = 0; i < terrainsModels.size(); i++)
		{
			if (instances.count(terrainsModels[i].model) > 0)
				loadModel(terrainsModels[i].model, instances[terrainsModels[i].model]);
		}
		for (int i = 0; i < buildingsModels.size(); i++)
		{
			if (instances.count(buildingsModels[i].model) > 0)
				loadModel(buildingsModels[i].model, instances[buildingsModels[i].model]);
		}
		for (int i = 0; i < decorationsModels.size(); i++)
		{
			if (instances.count(decorationsModels[i].model) > 0)
				loadModel(decorationsModels[i].model, instances[decorationsModels[i].model]);
		}
		for (int i = 0; i < parkingPlacesModels.size(); i++)
		{
			if (instances.count(parkingPlacesModels[i].model) > 0)
				loadModel(parkingPlacesModels[i].model, instances[parkingPlacesModels[i].model]);
		}
		for (int i = 0; i < vehiclesModels.size(); i++)
		{
			if (instances.count(vehiclesModels[i].model) > 0)
				loadModel(vehiclesModels[i].model, instances[vehiclesModels[i].model]);
		}

		Vehicle *vehicle = visualisation->GetCurrentSimulation()->GetVehicle();
		vehicleModel = new Model(vehicle->GetVehicleModel()->path);

		quit();
	}
private:
	Visualisation *visualisation;
	std::map<std::string, Model*> *loadedModels;
	Model *vehicleModel;

	void loadModel(std::string modelPath, std::vector<InstanceData> instances)
	{
		Model *model = new Model(modelPath, instances);
		model->Translate(glm::vec3(0, 0, 0));
		model->Rotate(glm::vec3(0, 0, 0));
		model->Scale(glm::vec3(1, 1, 1));
		model->MeasureModel();

		loadedModels->insert(std::pair<std::string, Model*>(modelPath, model));
	}
};

LoadModelsThread loadModelsThread;

OpenGLHost3D::OpenGLHost3D(QWidget *parent) : QOpenGLWidget(parent)
{
	setFocus();
	for (int i = 0; i < 1024; i++)
		keys[i] = false;

	connect(&loadModelsThread, SIGNAL(finished()), this, SLOT(loadModelsFinished()));
}

#pragma region User input events.

void OpenGLHost3D::mousePressEvent(QMouseEvent *event)
{
	mouseLastX = event->x();
	mouseLastY = event->y();

	if (event->button() == MouseButton::LeftButton)
		mouseLeftPressed = true;
	if (event->button() == MouseButton::RightButton)
		mouseRightPressed = true;
	if (event->button() == MouseButton::MiddleButton)
		mouseMiddlePressed = true;
	setFocus();
}

void OpenGLHost3D::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == MouseButton::LeftButton)
		mouseLeftPressed = false;
	if (event->button() == MouseButton::RightButton)
		mouseRightPressed = false;
	if (event->button() == MouseButton::MiddleButton)
		mouseMiddlePressed = false;
}

void OpenGLHost3D::mouseMoveEvent(QMouseEvent * event)
{
	if (mouseLeftPressed)
	{
		GLfloat xoffset = event->x() - mouseLastX;
		GLfloat yoffset = mouseLastY - event->y();

		mouseLastX = event->x();
		mouseLastY = event->y();

		camera->ProcessMouseMovement(xoffset, yoffset);
	}
}

void OpenGLHost3D::keyPressEvent(QKeyEvent * event)
{
	int key = event->key();
	if (key >= 0 && key < 1024)
		keys[key] = true;
}

void OpenGLHost3D::keyReleaseEvent(QKeyEvent * event)
{
	int key = event->key();
	if (key >= 0 && key < 1024)
		keys[key] = false;
}

void OpenGLHost3D::SetVisualisation(Visualisation * visualisation)
{
	this->visualization = visualisation;
}

#pragma endregion

#pragma region OpenGL methods.

void OpenGLHost3D::initializeGL()
{
	time.start();
	glewInit();
	initializeOpenGLFunctions();

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	loadedModels = new std::map<std::string, Model*>();

	textureInstancedShader = new Shader("Resources/shaders/textureInstancedVS.glsl", "Resources/shaders/textureInstancedFS.glsl");
	textureShader = new Shader("Resources/shaders/textureVS.glsl", "Resources/shaders/textureFS.glsl");
	phongShader = new Shader("Resources/shaders/phongVS.glsl", "Resources/shaders/phongFS.glsl");

	camera = new Camera(glm::vec3(0.0f, 500.0f, 0.0));

	// Light attributes
	lightPos = glm::vec3(0.0f, 1000.0f, 0.0);
}

void OpenGLHost3D::resizeGL(int w, int h)
{
	WIDTH = w;
	HEIGHT = h;
	glViewport(0, 0, WIDTH, HEIGHT);
}

void OpenGLHost3D::paintGL()
{
	if (loadModelsInProgress) return;

	if (visualization->GetVisualisationChanged())
	{
		initializeVisualization();
		visualization->SetVisualisationChanged(false);
		return;
	}

	if (reloadModels)
	{
		reloadModels = false;
		
		std::map<std::string, Model*> *modelss = loadModelsThread.GetLoadedModels();

		loadedModels->clear();
		std::map<std::string, std::vector<InstanceData>> instances;
		Map *map = visualization->GetCurrentSimulation()->GetMap();

		std::vector<MapElementModel> terrainsModels = Settings::getInstance()->GetTerrains();
		std::vector<MapElementModel> buildingsModels = Settings::getInstance()->GetBuildings();
		std::vector<MapElementModel> decorationsModels = Settings::getInstance()->GetDecorations();
		std::vector<MapElementModel> parkingPlacesModels = Settings::getInstance()->GetParkingPlaces();
		std::vector<MapElementModel> vehiclesModels = Settings::getInstance()->GetVehicles();

		for (int i = 0; i < terrainsModels.size(); i++)
		{
			if ((*modelss).count(terrainsModels[i].model) > 0)
				loadModel(terrainsModels[i].model, instances[terrainsModels[i].model], (*modelss)[terrainsModels[i].model]);
		}
		for (int i = 0; i < buildingsModels.size(); i++)
		{
			if ((*modelss).count(buildingsModels[i].model) > 0)
				loadModel(buildingsModels[i].model, instances[buildingsModels[i].model], (*modelss)[buildingsModels[i].model]);
		}
		for (int i = 0; i < decorationsModels.size(); i++)
		{
			if ((*modelss).count(decorationsModels[i].model) > 0)
				loadModel(decorationsModels[i].model, instances[decorationsModels[i].model], (*modelss)[decorationsModels[i].model]);
		}
		for (int i = 0; i < parkingPlacesModels.size(); i++)
		{
			if ((*modelss).count(parkingPlacesModels[i].model) > 0)
				loadModel(parkingPlacesModels[i].model, instances[parkingPlacesModels[i].model], (*modelss)[parkingPlacesModels[i].model]);
		}
		for (int i = 0; i < vehiclesModels.size(); i++)
		{
			if ((*modelss).count(vehiclesModels[i].model) > 0)
				loadModel(vehiclesModels[i].model, instances[vehiclesModels[i].model], (*modelss)[vehiclesModels[i].model]);
		}

		Vehicle *vehicle = visualization->GetCurrentSimulation()->GetVehicle();
		this->vehicleModel = new Model(vehicle->GetVehicleModel()->path, std::vector<InstanceData>(), &loadModelsThread.GetVehicleModel()->meshes);
		this->vehicleModel->Translate(vehicle->GetVehicleModel()->GetTranslation());
		this->vehicleModel->Rotate(vehicle->GetVehicleModel()->GetRotation());
		this->vehicleModel->Scale(vehicle->GetVehicleModel()->GetScale());

		pleaseWaitWindow->close();
	}

	// Set frame time
	GLfloat currentFrame = time.currentTime().msecsSinceStartOfDay() / 1000.0f;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	MoveCamera();

	// Clear the colorbuffer
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	textureInstancedShader->Use();

	// Use cooresponding shader when setting uniforms/drawing objects
	GLint lightPosLoc = glGetUniformLocation(textureInstancedShader->Program, "light.position");
	GLint viewPosLoc = glGetUniformLocation(textureInstancedShader->Program, "viewPos");
	glUniform3f(lightPosLoc, camera->Position.x, camera->Position.y, camera->Position.z);
	glUniform3f(viewPosLoc, camera->Position.x, camera->Position.y, camera->Position.z);

	GLint matAmbientLoc = glGetUniformLocation(textureInstancedShader->Program, "material.ambient");
	GLint matDiffuseLoc = glGetUniformLocation(textureInstancedShader->Program, "material.diffuse");
	GLint matSpecularLoc = glGetUniformLocation(textureInstancedShader->Program, "material.specular");
	GLint matShineLoc = glGetUniformLocation(textureInstancedShader->Program, "material.shininess");

	glUniform3f(matAmbientLoc, 0.5f, 0.5f, 0.5f);
	glUniform3f(matDiffuseLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(matSpecularLoc, 0.6f, 0.6f, 0.6f);
	glUniform1f(matShineLoc, 2.0f);

	GLint lightAmbientLoc = glGetUniformLocation(textureInstancedShader->Program, "light.ambient");
	GLint lightDiffuseLoc = glGetUniformLocation(textureInstancedShader->Program, "light.diffuse");
	GLint lightSpecularLoc = glGetUniformLocation(textureInstancedShader->Program, "light.specular");

	glUniform3f(lightAmbientLoc, 0.6f, 0.6f, 0.6f);
	glUniform3f(lightDiffuseLoc, 0.7f, 0.7f, 0.7f); // Let's darken the light a bit to fit the scene
	glUniform3f(lightSpecularLoc, 0.6f, 0.6f, 0.6f);

	glm::mat4 projection = glm::perspective(camera->Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 10000.0f);
	glm::mat4 view = camera->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(textureInstancedShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(textureInstancedShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	if (visualization->GetCurrentSimulation() != NULL)
	{
		Vehicle *vehicle = visualization->GetCurrentSimulation()->GetVehicle();

		for (std::map<std::string, Model*>::iterator iterator = loadedModels->begin(); iterator != loadedModels->end(); iterator++) 
		{
			glUniformMatrix4fv(glGetUniformLocation(textureInstancedShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(iterator->second->GetModelMatrix()));
			iterator->second->Draw(*textureInstancedShader);
		}

		textureShader->Use();

		// Use cooresponding shader when setting uniforms/drawing objects
		GLint lightPosLoc = glGetUniformLocation(textureShader->Program, "light.position");
		GLint viewPosLoc = glGetUniformLocation(textureShader->Program, "viewPos");
		glUniform3f(lightPosLoc, camera->Position.x, camera->Position.y, camera->Position.z);
		glUniform3f(viewPosLoc, camera->Position.x, camera->Position.y, camera->Position.z);

		GLint matAmbientLoc = glGetUniformLocation(textureShader->Program, "material.ambient");
		GLint matDiffuseLoc = glGetUniformLocation(textureShader->Program, "material.diffuse");
		GLint matSpecularLoc = glGetUniformLocation(textureShader->Program, "material.specular");
		GLint matShineLoc = glGetUniformLocation(textureShader->Program, "material.shininess");

		glUniform3f(matAmbientLoc, 0.5f, 0.5f, 0.5f);
		glUniform3f(matDiffuseLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(matSpecularLoc, 0.6f, 0.6f, 0.6f);
		glUniform1f(matShineLoc, 2.0f);

		GLint lightAmbientLoc = glGetUniformLocation(textureShader->Program, "light.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(textureShader->Program, "light.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(textureShader->Program, "light.specular");

		glUniform3f(lightAmbientLoc, 0.6f, 0.6f, 0.6f);
		glUniform3f(lightDiffuseLoc, 0.7f, 0.7f, 0.7f); // Let's darken the light a bit to fit the scene
		glUniform3f(lightSpecularLoc, 0.6f, 0.6f, 0.6f);

		glm::mat4 projection = glm::perspective(camera->Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera->GetViewMatrix();
		//glm::mat4 view;
		glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		vehicleModel->Translate(glm::vec3(vehicle->GetPosition().x, 0, vehicle->GetPosition().y));
		vehicleModel->Rotate(glm::vec3(0, vehicle->GetRotation() - M_PI / 2.0f, 0));
		glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(vehicleModel->GetModelMatrix()));
		vehicleModel->Draw(*textureShader, false);
	}
}

#pragma endregion

void OpenGLHost3D::MoveCamera()
{
	if (keys[Key::Key_W])
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (keys[Key::Key_S])
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[Key::Key_A])
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (keys[Key::Key_D])
		camera->ProcessKeyboard(RIGHT, deltaTime);
}

void OpenGLHost3D::initializeVisualization()
{
	if (visualization->GetCurrentSimulation() == NULL) return;

	this->loadModelsInProgress = true;
	loadModelsThread.SetVisualisation(visualization);
	loadModelsThread.start();

	camera->Position = glm::vec3(visualization->GetCurrentSimulation()->GetMap()->GetWidth() / 2.0, camera->Position.y, visualization->GetCurrentSimulation()->GetMap()->GetHeight() / 2.0);

	pleaseWaitWindow = new PleaseWaitWindow();
	pleaseWaitWindow->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	pleaseWaitWindow->show();
}

void OpenGLHost3D::loadModel(std::string modelPath, std::vector<InstanceData> instances, Model *m)
{
	Model *model = new Model(modelPath, instances, m != nullptr ? &m->meshes : nullptr);
	model->Translate(glm::vec3(0, 0, 0));
	model->Rotate(glm::vec3(0, 0, 0));
	model->Scale(glm::vec3(1, 1, 1));
	model->SetMeasure(m->GetMeasure());

	loadedModels->insert(std::pair<std::string, Model*>(modelPath, model));
}

void OpenGLHost3D::loadModelsFinished()
{
	this->loadModelsInProgress = false;
	this->reloadModels = true;
}