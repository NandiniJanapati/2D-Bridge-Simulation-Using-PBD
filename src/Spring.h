#pragma once
#ifndef Spring_H
#define Spring_H

#include <memory>
#include <Eigen/Dense>

class Particle;
class Program; //nandini for draw fxn
class Shape; //nandini for draw fxn
class MatrixStack; //nandini for draw fxn

class Spring
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW //nandini,,, idk if this will help

	Spring(std::shared_ptr<Particle> p0, std::shared_ptr<Particle> p1, double alpha);
	Spring(std::shared_ptr<Particle> p0, std::shared_ptr<Particle> p1, double alpha, int _p0i, int _p1i);
	virtual ~Spring();

	void springConstraint(double& C, std::vector<std::vector<double>>& gradC);
	
	void draw(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> MV, const std::shared_ptr<Shape> shape) const;
	
	std::shared_ptr<Particle> p0;
	std::shared_ptr<Particle> p1;
	double L;
	double alpha;
	double lambda;
	int p0i; //index of particle 0
	int p1i; //index of particle 1 in the particles list for the beam

	bool isRoad;
};

#endif

