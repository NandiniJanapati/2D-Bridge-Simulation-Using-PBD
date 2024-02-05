#include <iostream>
#include "GLSL.h"
#include "Scene.h"
#include "Particle.h"
//#include "Cloth.h"
#include "Shape.h"
#include "Program.h"
#include "Bridge.h" //nandini

using namespace std;
using namespace Eigen;

Scene::Scene() :
	t(0.0),
	h(1e-2),
	grav(0.0, 0.0, 0.0)
{
}

Scene::~Scene()
{
}

void Scene::load(const string &RESOURCE_DIR, const string &DATA_DIR, const string& filename)
{
	// Units: meters, kilograms, seconds
	h = 1e-3;
	
	grav << 0.0, -9.8, 0.0;
	
	int rows = 2;
	int cols = 2;
	double mass = 0.1;
	double alpha = 0.01; //0.005; //0e-1;
	double damping = 1e-5;
	double pradius = 0.01; // Particle radius, used for collisions

	//hard coded square 
	/*  *-*
	*	|/|
	*	*-*
	*/

	/*
	Vector2d x0(0, 1);
	Vector2d x1(1, 1);
	Vector2d x2(0, 0);
	Vector2d x3(1, 0);

	pair<Vector2d, Vector2d> beam0(x0, x1); //top horizontal
	pair<Vector2d, Vector2d> beam1(x0, x2); //left vertical
	//pair<Vector2d, Vector2d> beam2(x1, x2); //diagonal
	pair<Vector2d, Vector2d> beam3(x1, x3); //right vertical
	pair<Vector2d, Vector2d> beam4(x2, x3); //bottom horizontal

	vector<pair<Vector2d, Vector2d>> beams;
	beams.push_back(beam0);
	beams.push_back(beam1);
	//beams.push_back(beam2);
	beams.push_back(beam3);
	beams.push_back(beam4);
	*/

	//*-*-*
	/*
	Vector2d x0(0, 0);
	Vector2d x2(2, 0);
	Vector2d x1(1, 0);

	pair<Vector2d, Vector2d> beam0(x0, x1);
	pair<Vector2d, Vector2d> beam1(x1, x2);
	vector<pair<Vector2d, Vector2d>> beams;
	beams.push_back(beam0);
	beams.push_back(beam1);
	*/

	//hard coded bridge from c++ video
	/*
	Vector2d x0(0, 0);
	Vector2d x1(1, 0);
	Vector2d x2(2, 0);
	Vector2d x3(3, 0);
	Vector2d x4(4, 0);
	Vector2d x5(1, 1);
	Vector2d x6(2, 1);
	Vector2d x7(3, 1);
	pair<Vector2d, Vector2d> beam0(x0, x1);
	pair<Vector2d, Vector2d> beam1(x1, x2);
	pair<Vector2d, Vector2d> beam2(x2, x3);
	pair<Vector2d, Vector2d> beam3(x3, x4);
	pair<Vector2d, Vector2d> beam4(x5, x6);
	pair<Vector2d, Vector2d> beam5(x6, x7);
	pair<Vector2d, Vector2d> beam6(x0, x5);
	pair<Vector2d, Vector2d> beam7(x1, x5);
	pair<Vector2d, Vector2d> beam8(x1, x6);
	pair<Vector2d, Vector2d> beam9(x2, x6);
	pair<Vector2d, Vector2d> beam10(x3, x6);
	pair<Vector2d, Vector2d> beam11(x3, x7);
	pair<Vector2d, Vector2d> beam12(x4, x7);
	vector<pair<Vector2d, Vector2d>> beams;
	beams.push_back(beam0);
	beams.push_back(beam1);
	beams.push_back(beam2);
	beams.push_back(beam3);
	beams.push_back(beam4);
	beams.push_back(beam5);
	beams.push_back(beam6);
	beams.push_back(beam7);
	beams.push_back(beam8);
	beams.push_back(beam9);
	beams.push_back(beam10);
	beams.push_back(beam11);
	beams.push_back(beam12);
	bridge = make_shared<Bridge>(beams, mass, alpha, damping, pradius);
	*/

	bridge = make_shared<Bridge>(DATA_DIR, filename);
	

}

void Scene::init()
{
	//sphereShape->init();
	//cloth->init();
}

void Scene::tare()
{
	//for(auto s : spheres) {
	//	s->tare();
	//}
	//cloth->tare();
}

void Scene::reset()
{
	t = 0.0;
	//for(auto s : spheres) {
	//	s->reset();
	//}
	//cloth->reset();
}

void Scene::step()
{
	t += h;
	
	// Move the sphere
	//if(!spheres.empty()) {
	//	auto s = spheres.front();
	//	s->x(2) = 0.5 * sin(0.5*t);
	//}
	
	// Simulate the cloth
	//cloth->step(h, grav, spheres);

	bridge->step(h, grav);
}

void Scene::draw(shared_ptr<MatrixStack> MV, const shared_ptr<Program> prog) const
{
	//glUniform3fv(prog->getUniform("kdFront"), 1, Vector3f(1.0, 1.0, 1.0).data());  //nandini - what does this line do?
	//for(auto s : spheres) {
	//	s->draw(MV, prog);
	//}
	//cloth->draw(MV, prog);

	bridge->draw(MV, prog, linkshape);

	
	//left grass
	glLineWidth(4.0f);
	glColor3f(91.0f / 255.0f, 110.0f / 255.0f, 18.0f / 255.0f);
	glBegin(GL_POLYGON);
	glVertex3f(bridge->startpt(0), bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 3, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 3, bridge->startpt(1) - 0.75, 0.0f);
	glVertex3f(bridge->startpt(0), bridge->startpt(1) - 0.75, 0.0f);
	glEnd();

	//left ground
	glLineWidth(4.0f);
	glColor3f(89.0f / 255.0f, 56.0f / 255.0f, 21.0f / 255.0f);
	glBegin(GL_POLYGON);
	glVertex3f(bridge->startpt(0), bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 3, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 3, bridge->startpt(1) - 3, 0.0f);
	glVertex3f(bridge->startpt(0), bridge->startpt(1) - 3, 0.0f);
	glEnd();

	/*
	glBegin(GL_TRIANGLES);

	glColor3f(92.0f / 255.0f, 133.0f / 255.0f, 34.0f / 255.0f);
	glVertex3f(bridge->startpt(0) - 2.75, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 2.40, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 2.575, bridge->startpt(1) + 1.3, 0.0f);

	glColor3f(79.0f / 255.0f, 117.0f / 255.0f, 26.0f / 255.0f);
	glVertex3f(bridge->startpt(0) - 2.6, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 2.5, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 2.55, bridge->startpt(1) + 0.18, 0.0f);

	glColor3f(86.0f / 255.0f, 135.0f / 255.0f, 38.0f / 255.0f);
	glVertex3f(bridge->startpt(0) - 2.3, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 1.95, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 2.775, bridge->startpt(1) + 0.8, 0.0f);

	glColor3f(77.0f / 255.0f, 130.0f / 255.0f, 48.0f / 255.0f);
	glVertex3f(bridge->startpt(0) - 2, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 1.6, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 1.8, bridge->startpt(1) + 0.25, 0.0f);

	glColor3f(95.0f / 255.0f, 163.0f / 255.0f, 59.0f / 255.0f);
	glVertex3f(bridge->startpt(0) - 1.75, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 1.5, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 1.625, bridge->startpt(1) + 0.28, 0.0f);

	glColor3f(88.0f / 255.0f, 135.0f / 255.0f, 54.0f / 255.0f);
	glVertex3f(bridge->startpt(0) - 1.45, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 1.15, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 1.30, bridge->startpt(1) + 1.2, 0.0f);

	glColor3f(73.0f / 255.0f, 135.0f / 255.0f, 54.0f / 255.0f);
	glVertex3f(bridge->startpt(0) - 1.25, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 0.75, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 1.0, bridge->startpt(1) + 0.65, 0.0f);
	
	glColor3f(53.0f / 255.0f, 115.0f / 255.0f, 34.0f / 255.0f);
	glVertex3f(bridge->startpt(0) - 0.6, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 0.45, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 0.525, bridge->startpt(1) + 1, 0.0f);
	 
	glColor3f(84.0f / 255.0f, 138.0f / 255.0f, 51.0f / 255.0f);
	glVertex3f(bridge->startpt(0) - 0.8, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 0.7, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 0.75, bridge->startpt(1) + 0.2, 0.0f);

	glColor3f(94.0f / 255.0f, 133.0f / 255.0f, 65.0f / 255.0f);
	glVertex3f(bridge->startpt(0) - 0.3, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 0.15, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 0.225, bridge->startpt(1) + 0.7, 0.0f);

	glEnd();
	*/

	//left mountain
	glColor3f(120.0f / 255.0f, 107.0f / 255.0f, 81.0f / 255.0f);
	glBegin(GL_POLYGON);
	glVertex3f(bridge->startpt(0) - 3.0f, bridge->startpt(1), 0.0f);
	glVertex3f(bridge->startpt(0) - 3.0f, bridge->startpt(1) + 2.0f, 0.0f);
	glVertex3f(bridge->startpt(0) - 2.65f, bridge->startpt(1) + 1.85f, 0.0f);
	glVertex3f(bridge->startpt(0) - 2.5f, bridge->startpt(1) + 1.5f, 0.0f);
	glVertex3f(bridge->startpt(0) - 2.0f, bridge->startpt(1) + 1.15f, 0.0f);
	glVertex3f(bridge->startpt(0) - 1.75f, bridge->startpt(1) + 0.75f, 0.0f);
	glVertex3f(bridge->startpt(0) - 1.5f, bridge->startpt(1) + 0.6f, 0.0f);
	glVertex3f(bridge->startpt(0) - 1.0f, bridge->startpt(1) + 0.0f, 0.0f);
	glEnd();


	//right grass
	glLineWidth(4.0f);
	glColor3f(91.0f / 255.0f, 110.0f / 255.0f, 18.0f / 255.0f);
	glBegin(GL_POLYGON);
	glVertex3f(bridge->endpt(0), bridge->endpt(1), 0.0f);
	glVertex3f(bridge->endpt(0) + 3, bridge->endpt(1), 0.0f);
	glVertex3f(bridge->endpt(0) + 3, bridge->endpt(1) - 0.75, 0.0f);
	glVertex3f(bridge->endpt(0), bridge->endpt(1) - 0.75, 0.0f);
	glEnd();
	 

	//right ground
	glLineWidth(4.0f);
	glColor3f(77.0f / 255.0f, 49.0f / 255.0f, 15.0f / 255.0f);
	glBegin(GL_POLYGON);
	glVertex3f(bridge->endpt(0), bridge->endpt(1), 0.0f);
	glVertex3f(bridge->endpt(0) + 3, bridge->endpt(1), 0.0f);
	glVertex3f(bridge->endpt(0) + 3, bridge->endpt(1) - 3, 0.0f);
	glVertex3f(bridge->endpt(0), bridge->endpt(1) - 3, 0.0f);
	glEnd();
	
	//right mountain
	glColor3f(120.0f / 255.0f, 107.0f / 255.0f, 81.0f / 255.0f);
	glBegin(GL_POLYGON);
	glVertex3f(bridge->endpt(0) + 3.0f, bridge->endpt(1), 0.0f);
	glVertex3f(bridge->endpt(0) + 0.6f, bridge->endpt(1), 0.0f);
	glVertex3f(bridge->endpt(0) + 1.15f, bridge->endpt(1) + 0.5f, 0.0f);
	glVertex3f(bridge->endpt(0) + 1.5f, bridge->endpt(1) + 0.6f, 0.0f);
	glVertex3f(bridge->endpt(0) + 1.65f, bridge->endpt(1) + 1.0f, 0.0f);
	glVertex3f(bridge->endpt(0) + 2.0f, bridge->endpt(1) + 1.20f, 0.0f);
	glVertex3f(bridge->endpt(0) + 2.5f, bridge->endpt(1) + 1.5f, 0.0f);
	glVertex3f(bridge->endpt(0) + 2.6f, bridge->endpt(1) + 1.8f, 0.0f);
	glVertex3f(bridge->endpt(0) + 3.0f, bridge->endpt(1) + 2.0f, 0.0f);
	glEnd();

}
