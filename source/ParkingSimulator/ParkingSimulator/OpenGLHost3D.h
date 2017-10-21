#ifndef OPEN_GL_HOST_3D_H
#define OPEN_GL_HOST_3D_H

#define GLEW_STATIC
#include <glew.h>
#include <SOIL2.h>
#include <QTime>
#include <QEvent>
#include <qdesktopwidget.h>
#include <QMouseEvent> 
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Simulation.h"
#include "Visualisation.h"
#include <map.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace Qt;

class OpenGLHost3D : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	OpenGLHost3D(QWidget *parent);

	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent * event);

	void SetVisualisation(Visualisation *visualisation);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:
	Visualisation *visualization = nullptr;

	GLuint WIDTH = 800, HEIGHT = 600;
	GLuint VBO, VAO, EBO;
	Shader *textureShader, *phongShader;
	Camera *camera;
	Model *myModel, *box, *car;
	vector<Model> models;
	GLuint texture1, texture2;
	int width, height;
	QTime time;

	bool keys[1024];
	GLfloat mouseLastX;
	GLfloat mouseLastY;
	bool mouseFirstPressed = true;
	bool mouseLeftPressed = false;
	bool mouseRightPressed = false;
	bool mouseMiddlePressed = false;

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// Light attributes
	glm::vec3 lightPos;

	Model *mapModel;
	Model *vehicleModel;
	//std::vector<Model*> mapElementsModels;

	std::map<std::string, Model*> loadedModels;
	

	void MoveCamera();

	void initializeVisualization();
	void loadModel(std::string modelPath, std::vector<InstanceData> instances);
};

#endif