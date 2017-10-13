#include "OpenGLHost3D.h"

OpenGLHost3D::OpenGLHost3D(QWidget *parent) : QOpenGLWidget(parent)
{
	setFocus();
	for (int i = 0; i < 1024; i++)
		keys[i] = false;
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

	textureShader = new Shader("Resources/shaders/textureVS.glsl", "Resources/shaders/textureFS.glsl");
	phongShader = new Shader("Resources/shaders/phongVS.glsl", "Resources/shaders/phongFS.glsl");

	camera = new Camera(glm::vec3(0.0f, 1000.0f, 0.0));

	// Light attributes
	lightPos = glm::vec3(0.0f, 5.0f, 0.0f);

	if (visualization->GetCurrentSimulation() != NULL)
		initializeVisualization();

	myModel = new Model("Resources/models/plane/plane.obj");
	box = new Model("Resources/models/box/box.obj");
	car = new Model("Resources/models/car/Car.obj");
	car->Scale(glm::vec3(0.00001f, 0.00001f, 0.00001f));
}

void OpenGLHost3D::resizeGL(int w, int h)
{
	WIDTH = w;
	HEIGHT = h;
	glViewport(0, 0, WIDTH, HEIGHT);
}

void OpenGLHost3D::paintGL()
{
	if (visualization->GetVisualisationChanged())
	{
		initializeVisualization();
		visualization->SetVisualisationChanged(false);
	}

	// Set frame time
	GLfloat currentFrame = time.currentTime().msecsSinceStartOfDay() / 1000.0f;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	MoveCamera();

	// Clear the colorbuffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	textureShader->Use();

	// Use cooresponding shader when setting uniforms/drawing objects
	GLint lightPosLoc = glGetUniformLocation(textureShader->Program, "light.position");
	GLint viewPosLoc = glGetUniformLocation(textureShader->Program, "viewPos");
	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(viewPosLoc, camera->Position.x, camera->Position.y, camera->Position.z);

	GLint matAmbientLoc = glGetUniformLocation(textureShader->Program, "material.ambient");
	GLint matDiffuseLoc = glGetUniformLocation(textureShader->Program, "material.diffuse");
	GLint matSpecularLoc = glGetUniformLocation(textureShader->Program, "material.specular");
	GLint matShineLoc = glGetUniformLocation(textureShader->Program, "material.shininess");

	glUniform3f(matAmbientLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
	glUniform1f(matShineLoc, 32.0f);

	GLint lightAmbientLoc = glGetUniformLocation(textureShader->Program, "light.ambient");
	GLint lightDiffuseLoc = glGetUniformLocation(textureShader->Program, "light.diffuse");
	GLint lightSpecularLoc = glGetUniformLocation(textureShader->Program, "light.specular");

	glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
	glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f); // Let's darken the light a bit to fit the scene
	glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);


	//glm::mat4 projection = glm::ortho(-5.0f, 5.0f, 5.0f, -5.0f, 0.01f, 100.0f);
	glm::mat4 projection = glm::perspective(camera->Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 10000.0f);
	glm::mat4 view = camera->GetViewMatrix();
	//glm::mat4 view;
	glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 model;

	if (visualization->GetCurrentSimulation() != NULL)
	{
		// Draw map
		glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(mapModel->GetModelMatrix()));
		mapModel->Draw(*textureShader);

		Vehicle *vehicle = visualization->GetCurrentSimulation()->GetVehicle();
		vehicleModel->Translate(glm::vec3(vehicle->GetPosition().x, 0, vehicle->GetPosition().y));
		vehicleModel->RotateY(vehicle->GetRotation());
		glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(vehicleModel->GetModelMatrix()));
		vehicleModel->Draw(*textureShader);

		for (int i = 0; i < mapElementsModels.size(); i++)
		{
			glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(mapElementsModels[i]->GetModelMatrix()));
			mapElementsModels[i]->Draw(*textureShader);
		}
	}

	// Draw the loaded model
	/*model = glm::translate(model, glm::vec3(1.5f, 0.0f, 2.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
	glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	box->Draw(*textureShader);

	/*model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// It's a bit too big for our scene, so scale it down
	glUniformMatrix4fv(glGetUniformLocation(phongShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	car->Draw(*phongShader);*/

	/*for (int i = 0; i < models.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(phongShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(models[i].GetModelMatrix()));
		models[i].Draw(*phongShader);
	}*/

	/*model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// It's a bit too big for our scene, so scale it down
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	car->Draw(*shader);*/

	/*glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture2"), 1);

	shader->Use();

	// Create transformations
	// Camera/View transformation
	glm::mat4 view;
	view = view = camera->GetViewMatrix();
	// Projection
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
	// Get their uniform location
	GLint modelLoc = glGetUniformLocation(shader->Program, "model");
	GLint viewLoc = glGetUniformLocation(shader->Program, "view");
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(VAO);
	for (GLuint i = 0; i < 10; i++)
	{
	// Calculate the model matrix for each object and pass it to shader before drawing
	glm::mat4 model;
	model = glm::translate(model, cubePositions[i]);
	GLfloat angle = 20.0f * i;
	model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);*/

	/*glUniformMatrix4fv(glGetUniformLocation(phongShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));
	auto pathElements = simulation.path.GetElements();
	for (int i = 0; i < pathElements.size(); i++)
	{
		glBegin(GL_LINE_STRIP);
		if (pathElements[i].type == Line)
		{
			glVertex3f(pathElements[i].from.x, pathElements[i].from.y, pathElements[i].from.z);
			glVertex3f(pathElements[i].to.x, pathElements[i].to.y, pathElements[i].to.z);
			//glVertex3f(simulation.GetVehicle().GetState().x, 0, simulation.GetVehicle().GetState().y);
			//glVertex3f(simulation.GetVehicle().GetState().x + simulation.GetVehicle().GetDirWheelbase().x, 0, simulation.GetVehicle().GetState().y + +simulation.GetVehicle().GetDirWheelbase().y);
		}
		else if (pathElements[i].type == Circle)
		{
			for (double angle = pathElements[i].angleFrom; angle < pathElements[i].angleTo; angle += 0.001)
			{
				auto point = pathElements[i].GetCirclePoint(angle);
				glVertex3f(point.x, point.y, point.z);
			}

			for (double angle = pathElements[i].angleFrom; angle > pathElements[i].angleTo; angle -= 0.001)
			{
				auto point = pathElements[i].GetCirclePoint(angle);
				glVertex3f(point.x, point.y, point.z);
			}
		}
		glEnd();
	}*/

	// points drawing
	/*glBegin(GL_LINE_STRIP);
	for (int i = 0; i < simulation.path2.size(); i++)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(simulation.path2[i].GetX(), simulation.path2[i].GetY(), simulation.path2[i].GetZ());
	}
	glEnd();*/
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
	camera->Position = glm::vec3(visualization->GetCurrentSimulation()->GetMap()->GetWidth() / 2.0, camera->Position.y, visualization->GetCurrentSimulation()->GetMap()->GetHeight() / 2.0);

	mapModel = new Model("Resources/models/map/map.obj");
	mapModel->Translate(glm::vec3(visualization->GetCurrentSimulation()->GetMap()->GetWidth() / 2.0, 1.0, visualization->GetCurrentSimulation()->GetMap()->GetHeight() / 2.0));
	mapModel->Rotate(glm::vec3());
	mapModel->Scale(glm::vec3(visualization->GetCurrentSimulation()->GetMap()->GetWidth() / 100.0, 1.0, visualization->GetCurrentSimulation()->GetMap()->GetHeight() / 100.0));

	//car->Scale(glm::vec3(0.00001f, 0.00001f, 0.00001f));

	Vehicle *vehicle = visualization->GetCurrentSimulation()->GetVehicle();
	vehicleModel = new Model("Resources/models/vehicle/vehicle.obj");
	vehicleModel->Translate(glm::vec3());
	vehicleModel->Rotate(glm::vec3());
	vehicleModel->Scale(glm::vec3(vehicle->GetWheelbase(), 50, vehicle->GetTrack()));

	std::vector<MapElement*> mapElements = visualization->GetCurrentSimulation()->GetMap()->GetMapElements();
	for (int i = 0; i < mapElements.size(); i++)
	{
		mapElementsModels.push_back(new Model("Resources/models/box/box.obj"));
		mapElementsModels[i]->Translate(glm::vec3(mapElements[i]->GetPosition().x, 0, mapElements[i]->GetPosition().y));
		mapElementsModels[i]->Rotate(glm::vec3(0, mapElements[i]->GetRotation(), 0));
		mapElementsModels[i]->Scale(glm::vec3(mapElements[i]->GetSize().x, 100, mapElements[i]->GetSize().y));
	}
}
