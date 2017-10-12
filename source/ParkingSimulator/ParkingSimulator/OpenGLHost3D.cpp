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

	camera = new Camera(glm::vec3(0.0f, 7.0f, 3.0f));

	// Light attributes
	lightPos = glm::vec3(0.0f, 5.0f, 0.0f);

	myModel = new Model("Resources/models/plane/plane.obj");
	box = new Model("Resources/models/box/box.obj");
	car = new Model("Resources/models/car/Car.obj");
	car->Scale(glm::vec3(0.00001f, 0.00001f, 0.00001f));

	/*for (int i = 0; i < simulation.GetMap().GetMapElements().size(); i++)
	{
		MapElement *mapElement = simulation.GetMap().GetMapElements().at(i);
		if (dynamic_cast<Obstacle*>(mapElement) != nullptr)
		{
			Obstacle *obstacle = dynamic_cast<Obstacle*>(mapElement);
			switch (obstacle->GetType())
			{
			case ObstacleType::Box:
				models.push_back(Model("Resources/models/box/box.obj", glm::vec3(obstacle->GetPosX(), 0.5f, obstacle->GetPosY())));
				break;
			case ObstacleType::Lala:
				models.push_back(Model("Resources/models/box/box.obj", glm::vec3(obstacle->GetPosX(), 0.5f, obstacle->GetPosY()), glm::vec3(), glm::vec3(2.0f, 1.0f, 0.5f)));
				break;
			}

		}
		else if (dynamic_cast<ParkingSpace*>(mapElement) != nullptr)
		{
			ParkingSpace *parkingSpace = dynamic_cast<ParkingSpace*>(mapElement);
			//models.push_back(Model("Resources/models/ParkingSpace/ParkingSpace.obj", parkingSpace->GetPosition(), parkingSpace->GetRotation(), parkingSpace->GetSize()));
		}
	}*/

	int lala = 0;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/*
	GLfloat vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLuint indices[] = {
	0, 1, 3, // First Triangle
	1, 2, 3  // Second Triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	glBindVertexArray(VAO); // Bind VAO

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	glBindTexture(GL_TEXTURE_2D, texture1); // Bind texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* image1 = SOIL_load_image("Resources/textures/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture


	glBindTexture(GL_TEXTURE_2D, texture2); // Bind texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* image2 = SOIL_load_image("Resources/textures/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture */
}

void OpenGLHost3D::resizeGL(int w, int h)
{
	WIDTH = w;
	HEIGHT = h;
	glViewport(0, 0, WIDTH, HEIGHT);
}

void OpenGLHost3D::paintGL()
{
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
	glm::mat4 projection = glm::perspective(camera->Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	//glm::mat4 view;
	glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(textureShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	// Draw the loaded model
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(1.5f, 0.0f, 2.0f)); // Translate it down a bit so it's at the center of the scene
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
