//nandini
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
//nandini

#include "Spring.h"
#include "Particle.h"

//nandini
#include "Shape.h"
#include "MatrixStack.h"
#include "Program.h"
//nandini

using namespace std;
using namespace Eigen;

Spring::Spring(shared_ptr<Particle> p0, shared_ptr<Particle> p1, double alpha)
{
	assert(p0);
	assert(p1);
	assert(p0 != p1);
	this->p0 = p0;
	this->p1 = p1;
	this->alpha = alpha;
	
	// TODO: Compute L
	L = (p1->x0 - p0->x0).norm();
	lambda = 0;
	p0i = 0;
	p1i = 0;
}

Spring::Spring(shared_ptr<Particle> p0, shared_ptr<Particle> p1, double alpha, int _p0i, int _p1i) {
	
	assert(p0);
	assert(p1);
	assert(p0 != p1);
	this->p0 = p0;
	this->p1 = p1;
	this->alpha = alpha;
	this->p0i = _p0i;
	this->p1i = _p1i;

	// TODO: Compute L
	L = (p1->x0 - p0->x0).norm();
	lambda = 0;
	
}

Spring::~Spring()
{
	
}


void Spring::springConstraint(double& C, std::vector<std::vector<double>>& gradC) {

	Vector3d deltax = p1->x - p0->x;
	double l = deltax.norm();
	C = l - L;

	Vector3d gradC0 = -(deltax / l);
	Vector3d gradC1 = (deltax / l);

	vector<double> C0 = { gradC0(0), gradC0(1), gradC0(2) };
	vector<double> C1 = { gradC1(0), gradC1(1), gradC1(2) };

	gradC.push_back(C0);
	gradC.push_back(C1);  //2x3

	return;
}

void Spring::draw(const shared_ptr<Program> prog, shared_ptr<MatrixStack> MV, const shared_ptr<Shape> shape) const
{
	assert(prog);
	assert(MV);
	assert(shape);

	MV->pushMatrix();
	MV->translate(p0->x(0), p0->x(1), 0.0);
	
	Vector3d beam = (p1->x - p0->x);
	beam.normalize();
	double x = beam(0);
	double y = beam(1);
	double angle = atan2(y, x);

	MV->rotate(angle, 0.0, 0.0, 1.0);
	MV->scale(L, 1, 1);
	MV->translate(0.5, 0, 0); //translate an extra 0.5 to move the origin to p0.

	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	shape->draw(prog); //should be progTex

	MV->popMatrix();
}