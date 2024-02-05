#pragma once
#ifndef Particle_H
#define Particle_H

#include <vector>
#include <memory>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

class Shape;
class Program;
class MatrixStack;
class Spring;

class Particle
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	
	Particle();
	Particle(const std::shared_ptr<Shape> shape);
	virtual ~Particle();
	void tare();
	void reset();
	void draw(std::shared_ptr<MatrixStack> MV, const std::shared_ptr<Program> p) const;
	
	double r; // radius
	double m; // mass
	double d; // damping
	Eigen::Vector3d x0; // initial position
	Eigen::Vector3d v0; // initial velocity
	Eigen::Vector3d x;  // position
	Eigen::Vector3d p;  // previous position
	Eigen::Vector3d v;  // velocity
	bool fixed;

	//std::vector<std::shared_ptr<Spring>> edges;
	int edgecount;
	//vector<int> edges; //list of the indexes of the beams it's connected to
private:
	const std::shared_ptr<Shape> sphere;

};

#endif
