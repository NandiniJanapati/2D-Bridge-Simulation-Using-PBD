#pragma once
#ifndef Scene_H
#define Scene_H

#include <vector>
#include <memory>
#include <string>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

//class Cloth;
class Particle;
class MatrixStack;
class Program;
class Shape;
class Bridge; //nandini

class Scene
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	
	Scene();
	virtual ~Scene();
	
	void load(const std::string &RESOURCE_DIR, const std::string& DATA_DIR, const std::string& filename);
	void init();
	void tare();
	void reset();
	void step();
	
	void draw(std::shared_ptr<MatrixStack> MV, const std::shared_ptr<Program> prog) const;
	
	double getTime() const { return t; }

	void setShape(std::shared_ptr<Shape> shape) { linkshape = shape; }
	std::shared_ptr<Shape> getShape() { return linkshape; }
	
	std::shared_ptr<Bridge> bridge;
private:
	double t;
	double h;
	Eigen::Vector3d grav;
	
	//std::shared_ptr<Shape> sphereShape;
	//std::shared_ptr<Cloth> cloth;
	//std::vector< std::shared_ptr<Particle> > spheres;

	//nandini
	
	std::shared_ptr<Shape> linkshape; 
	//nandini
};

#endif
