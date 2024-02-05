#pragma once

#ifndef Bridge_H
#define Bridge_H

#include <vector>
#include <memory>
#include <mutex>
#include <string>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>
#include <Eigen/Sparse>


class Particle;
class Spring;
class MatrixStack;
class Program;
class Shape;
//class Car;

class Bridge 
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		//i'm not sure if using Eigen for the points is most efficent
	Bridge(std::vector<std::pair<Eigen::Vector2d, Eigen::Vector2d>>& points,
																double mass,
																double alpha,
																double damping,
																double pradius); 
	
	~Bridge();
	Bridge(const std::string& DATA_DIR, const std::string& filename);

	int addParticle(Eigen::Vector2d& point,
								double damping,
								double pradius);

	void tare();
	void reset();

	void draw(std::shared_ptr<MatrixStack> MV, const std::shared_ptr<Program> p, const std::shared_ptr<Shape> shape);

	void step(double h, const Eigen::Vector3d& grav);

	void loadDataInputFile(const std::string &DATA_DIR, const std::string& filename);

	float threshold;

	std::shared_ptr<Particle> car;
	Eigen::Vector3d endpt;
	Eigen::Vector3d startpt;

private:

	std::vector<std::shared_ptr<Particle>> particles; //set of vertices V
	std::vector<std::shared_ptr<Spring>> beams; //set of edges E
	std::mutex beamsMutex;
	

};

#endif