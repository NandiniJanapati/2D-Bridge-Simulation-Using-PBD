#include <iostream>

//#define GLEW_STATIC
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Particle.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Bridge.h"
#include "Car.h"

using namespace std;
using namespace Eigen;

Car::Car()
{
	_car = make_shared<Particle>();
	_car->fixed = false;
}


Car::~Car()
{
}

void Car::tare()
{
	_car->tare();
}

void Car::reset()
{
	_car->reset();
}

void Car::draw(shared_ptr<MatrixStack> MV, const shared_ptr<Program> prog) const
{
	//draw a circle with old openGL i guess.
}

//void Car::step(double h, const Vector3d& grav, Bridge& road) {
//	
//	//move position (take step forward)
//	if (car != nullptr) {
//		Vector3d fi = car->m * grav - car->d * car->v;
//		car->v += (h / car->m) * fi;
//		car->p = car->x;
//		car->x += h * car->v;
//	}
//
//	//apply constraints (are there any??)
//
//	//bridge collisions
//
//
//
//	if (car != nullptr) {
//		car->v = (1 / h) * (car->x - car->p);
//	}
//
//}
