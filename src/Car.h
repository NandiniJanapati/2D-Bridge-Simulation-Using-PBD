#pragma once
#ifndef Car_H
#define Car_H

#include <vector>
#include <memory>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

class Shape;
class Program;
class MatrixStack;
class Spring;
class Particle;
class Bridge;

class Car
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	
	Car();
	//Car(const std::shared_ptr<Shape> shape);
	virtual ~Car();
	void tare();
	void reset();
	void draw(std::shared_ptr<MatrixStack> MV, const std::shared_ptr<Program> p) const;
	
	//void step(double h, const Vector3d& grav, Bridge& road);
	

	std::shared_ptr<Particle> _car;
private:
	

};

#endif
