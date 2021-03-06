#include "VehicleEditorGLHost.h"

VehicleEditorGLHost::VehicleEditorGLHost(QWidget *parent) : QOpenGLWidget(parent)
{
	setFocus();
	for (int i = 0; i < 1024; i++)
		keys[i] = false;
}

#pragma region User input events.

void VehicleEditorGLHost::mousePressEvent(QMouseEvent *event)
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

void VehicleEditorGLHost::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == MouseButton::LeftButton)
		mouseLeftPressed = false;
	if (event->button() == MouseButton::RightButton)
		mouseRightPressed = false;
	if (event->button() == MouseButton::MiddleButton)
		mouseMiddlePressed = false;
}

void VehicleEditorGLHost::mouseMoveEvent(QMouseEvent * event)
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

void VehicleEditorGLHost::keyPressEvent(QKeyEvent * event)
{
	int key = event->key();
	if (key >= 0 && key < 1024)
		keys[key] = true;
}

void VehicleEditorGLHost::keyReleaseEvent(QKeyEvent * event)
{
	int key = event->key();
	if (key >= 0 && key < 1024)
		keys[key] = false;
}

void VehicleEditorGLHost::SetVehicleEditor(VehicleEditor * vehicleEditor)
{
	this->vehicleEditor = vehicleEditor;
}

#pragma endregion

#pragma region OpenGL methods.

void VehicleEditorGLHost::initializeGL()
{
	time.start();
	glewInit();
	initializeOpenGLFunctions();

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	textureShader = new Shader("Resources/shaders/textureVS.glsl", "Resources/shaders/textureFS.glsl");
	phongShader = new Shader("Resources/shaders/phongVS.glsl", "Resources/shaders/phongFS.glsl");

	camera = new Camera(glm::vec3(0.0f, 100.0f, 0.0));

	// Light attributes
	lightPos = glm::vec3(-30.0f, 100.0f, 0.0f);

	skyboxModel = new Model("Resources/models/skyboxes/vehicleEditor/skybox.obj");
}

void VehicleEditorGLHost::resizeGL(int w, int h)
{
	WIDTH = w;
	HEIGHT = h;
	glViewport(0, 0, WIDTH, HEIGHT);
}

void VehicleEditorGLHost::paintGL()
{
	// Set frame time
	GLfloat currentFrame = time.currentTime().msecsSinceStartOfDay() / 1000.0f;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	MoveCamera();

	// Clear the colorbuffer
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	glUniform1f(matShineLoc, 32.0f);

	GLint lightAmbientLoc = glGetUniformLocation(textureShader->Program, "light.ambient");
	GLint lightDiffuseLoc = glGetUniformLocation(textureShader->Program, "light.diffuse");
	GLint lightSpecularLoc = glGetUniformLocation(textureShader->Program, "light.specular");

	glUniform3f(lightAmbientLoc, 0.7f, 0.7f, 0.7f);
	glUniform3f(lightDiffuseLoc, 0.8f, 0.8f, 0.8f); // Let's darken the light a bit to fit the scene
	glUniform3f(lightSpecularLoc, 0.8f, 0.8f, 0.8f);

	glm::mat4 projection = glm::perspective(camera->Zoom, (float)WIDTH / (float)HEIGHT, 1.0f, 10000.0f);
	glm::mat4 view = camera->GetViewMatrix();
	
	glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	if (vehicleEditor->GetVehicleModelChanged())
		initializeVehicleModel();
	if (vehicleEditor->GetVehicleFrontLeftWheelModelChanged())
		initializeVehicleFrontLeftWheelModel();
	if (vehicleEditor->GetVehicleFrontRightWheelModelChanged())
		initializeVehicleFrontRightWheelModel();
	if (vehicleEditor->GetVehicleRearLeftWheelModelChanged())
		initializeVehicleRearLeftWheelModel();
	if (vehicleEditor->GetVehicleRearRightWheelModelChanged())
		initializeVehicleRearRightWheelModel();

	skyboxModel->Scale(glm::vec3(5000, 5000, 5000));
	skyboxModel->Translate(camera->Position);
	glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(skyboxModel->GetModelMatrix()));
	skyboxModel->Draw(*textureShader, false);

	if (vehicleEditor->GetVehicle() != NULL)
	{
		Vehicle *vehicle = vehicleEditor->GetVehicle();

		if (vehicle->GetVehicleModel() != NULL)
		{
			Model *model = vehicleEditor->GetVehicle()->GetVehicleModel();

			double scaleRatioX = vehicle->GetSize().y / model->GetMeasure().x;
			double scaleRatioZ = vehicle->GetSize().x / model->GetMeasure().z;
			double scaleRatioY = (scaleRatioX + scaleRatioZ) / 2.0f;

			model->Scale(glm::vec3(scaleRatioX, scaleRatioY, scaleRatioZ));
			glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model->GetModelMatrix()));
			model->Draw(*textureShader, false);
		}

		if (vehicle->GetFrontLeftWheelModel() != NULL)
		{
			Model *model = vehicleEditor->GetVehicle()->GetFrontLeftWheelModel();
			glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model->GetModelMatrix()));
			model->Draw(*textureShader, false);
		}

		if (vehicle->GetFrontRightWheelModel() != NULL)
		{
			Model *model = vehicleEditor->GetVehicle()->GetFrontRightWheelModel();
			glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model->GetModelMatrix()));
			model->Draw(*textureShader, false);
		}

		if (vehicle->GetRearLeftWheelModel() != NULL)
		{
			Model *model = vehicleEditor->GetVehicle()->GetRearLeftWheelModel();
			glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model->GetModelMatrix()));
			model->Draw(*textureShader, false);
		}

		if (vehicle->GetRearRightWheelModel() != NULL)
		{
			Model *model = vehicleEditor->GetVehicle()->GetRearRightWheelModel();
			glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model->GetModelMatrix()));
			model->Draw(*textureShader, false);
		}
	}
}

#pragma endregion

void VehicleEditorGLHost::MoveCamera()
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

void VehicleEditorGLHost::initializeVehicleModel()
{
	std::string path = vehicleEditor->GetVehicle()->GetVehicleModel()->path;
	Model *model = new Model(path);
	model->MeasureModel();
	model->Translate(vehicleEditor->GetVehicle()->GetVehicleModel()->GetTranslation());
	model->Rotate(vehicleEditor->GetVehicle()->GetVehicleModel()->GetRotation());
	model->Scale(vehicleEditor->GetVehicle()->GetVehicleModel()->GetScale());
	vehicleEditor->GetVehicle()->SetVehicleModel(model);
	vehicleEditor->SetVehicleModelChanged(false);
}

void VehicleEditorGLHost::initializeVehicleFrontLeftWheelModel()
{
	std::string path = vehicleEditor->GetVehicle()->GetFrontLeftWheelModel()->path;
	Model *model = new Model(path);
	model->Translate(vehicleEditor->GetVehicle()->GetFrontLeftWheelModel()->GetTranslation());
	model->Rotate(vehicleEditor->GetVehicle()->GetFrontLeftWheelModel()->GetRotation());
	model->Scale(vehicleEditor->GetVehicle()->GetFrontLeftWheelModel()->GetScale());
	vehicleEditor->GetVehicle()->SetFrontLeftWheelModel(model);
	vehicleEditor->SetVehicleFrontLeftWheelModelChanged(false);
}

void VehicleEditorGLHost::initializeVehicleFrontRightWheelModel()
{
	std::string path = vehicleEditor->GetVehicle()->GetFrontRightWheelModel()->path;
	Model *model = new Model(path);
	model->Translate(vehicleEditor->GetVehicle()->GetFrontRightWheelModel()->GetTranslation());
	model->Rotate(vehicleEditor->GetVehicle()->GetFrontRightWheelModel()->GetRotation());
	model->Scale(vehicleEditor->GetVehicle()->GetFrontRightWheelModel()->GetScale());
	vehicleEditor->GetVehicle()->SetFrontRightWheelModel(model);
	vehicleEditor->SetVehicleFrontRightWheelModelChanged(false);
}

void VehicleEditorGLHost::initializeVehicleRearLeftWheelModel()
{
	std::string path = vehicleEditor->GetVehicle()->GetRearLeftWheelModel()->path;
	Model *model = new Model(path);
	model->Translate(vehicleEditor->GetVehicle()->GetRearLeftWheelModel()->GetTranslation());
	model->Rotate(vehicleEditor->GetVehicle()->GetRearLeftWheelModel()->GetRotation());
	model->Scale(vehicleEditor->GetVehicle()->GetRearLeftWheelModel()->GetScale());
	vehicleEditor->GetVehicle()->SetRearLeftWheelModel(model);
	vehicleEditor->SetVehicleRearLeftWheelModelChanged(false);
}

void VehicleEditorGLHost::initializeVehicleRearRightWheelModel()
{
	std::string path = vehicleEditor->GetVehicle()->GetRearRightWheelModel()->path;
	Model *model = new Model(path);
	model->Translate(vehicleEditor->GetVehicle()->GetRearRightWheelModel()->GetTranslation());
	model->Rotate(vehicleEditor->GetVehicle()->GetRearRightWheelModel()->GetRotation());
	model->Scale(vehicleEditor->GetVehicle()->GetRearRightWheelModel()->GetScale());
	vehicleEditor->GetVehicle()->SetRearRightWheelModel(model);
	vehicleEditor->SetVehicleRearRightWheelModelChanged(false);
}