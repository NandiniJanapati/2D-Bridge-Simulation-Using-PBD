#include <iostream>
#include <fstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <mutex>

#include "Bridge.h"
#include "Particle.h"
//#include "Car.h"
#include "Spring.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"
#include "GLSL.h"


using namespace std;
using namespace Eigen;


double sciToDub(const string& str) {

	stringstream ss(str);
	double d = 0;
	ss >> d;

	if (ss.fail()) {
		string s = "Unable to format ";
		s += str;
		s += " as a number!";
		throw (s);
	}

	return (d);
}

void Bridge::loadDataInputFile(const string &DATA_DIR, const string& filename)
{
	string thefile = DATA_DIR + filename;
	ifstream in;
	in.open(thefile);
	if (!in.good()) {
		cout << "Cannot read " << thefile << endl;
		return;
	}
	cout << "Loading " << thefile << endl;

	double mass;
	double _alpha;
	double damping;
	double pradius;

	string line;
	while (1) {
		getline(in, line);
		if (in.eof()) {
			break;
		}
		if (line.empty()) {
			continue;
		}
		// Skip comments
		if (line.at(0) == '#') {
			continue;
		}
		// Parse lines
		string key, value;
		stringstream ss(line);
		// key
		ss >> key;
		if (key.compare("GLOBAL") == 0) {
			ss >> value;
			mass = sciToDub(value);

			ss >> value;
			_alpha = sciToDub(value);

			ss >> value;
			damping = sciToDub(value);

			ss >> value;
			pradius = sciToDub(value);

			ss >> value;
			this->threshold = sciToDub(value);
		}
		else if (key.compare("PARTICLE") == 0) {
			ss >> value;
			double x = stod(value);

			ss >> value;
			double y = stod(value);

			ss >> value;
			bool f = (bool) stoi(value);
			
			auto p = make_shared<Particle>();
			particles.push_back(p);
			p->r = pradius;
			p->d = damping;
			Vector3d newpoint;
			newpoint << x, y, 0.0; //set the z to 0
			p->x0 = newpoint;
			p->x = p->x0;
			p->v0.setZero();
			p->v.setZero();
			p->p = p->x;
			p->fixed = f;
		}
		else if (key.compare("BEAM") == 0) {
			ss >> value;
			int P0idx = stoi(value);

			ss >> value;
			int P1idx = stoi(value);

			ss >> value;
			bool f = (bool)stoi(value);

			auto b = make_shared<Spring>(particles[P0idx], particles[P1idx], _alpha, P0idx, P1idx);
			beams.push_back(b);
			/*b->p0 = particles[P0idx];
			b->p1 = particles[P1idx];
			b->alpha = _alpha;
			b->L = (b->p0->x0 - b->p1->x0).norm();
			b->lambda = 0;
			b->p0i = P0idx;
			b->p1i = P1idx;*/
			b->isRoad = f;
			particles[P0idx]->edgecount++;
			particles[P1idx]->edgecount++;
		}
		else if (key.compare("CAR") == 0) {
			car = make_shared<Particle>();
			
			ss >> value;
			double x = stod(value);

			ss >> value;
			double y = stod(value);

			Vector3d newpoint;
			newpoint << x, y, 0.0; //set the z to 0
			car->x0 = newpoint;
			car->x = car->x0;
			car->p = car->x;
			car->fixed = false;

			this->startpt = newpoint;

			ss >> value;
			car->m = sciToDub(value);

			ss >> value;
			car->d = sciToDub(value);

			ss >> value;
			car->r = sciToDub(value);

			ss >> value;
			x = stod(value);

			ss >> value;
			y = stod(value);

			newpoint << x, y, 0;
			endpt = newpoint;

			ss >> value;
			double initvel = sciToDub(value);
			newpoint << initvel, 0, 0;
			car->v0 = newpoint;
			car->v = car->v0;

		}
		else {
			cout << "Unkown key word: " << key << endl;
		}
	}
	in.close();

	for (int i = 0; i < particles.size(); i++) {
		particles[i]->m = mass / particles.size();
	}

	double minx = 100000000;
	double maxx = -100000000;
	for (int i = 0; i < beams.size(); i++) {
		if (beams[i]->isRoad) {
			if (beams[i]->p0->x0(0) < minx) {
				startpt = beams[i]->p0->x0;
				minx = startpt(0);
			}
			if (beams[i]->p0->x0(0) > maxx) {
				endpt = beams[i]->p0->x0;
				maxx = endpt(0);
			}
			if (beams[i]->p1->x0(0) < minx) {
				startpt = beams[i]->p1->x0;
				minx = startpt(0);
			}
			if (beams[i]->p1->x0(0) > maxx) {
				endpt = beams[i]->p1->x0;
				maxx = endpt(0);
			}
		}
		
	}

}

Bridge::Bridge(const std::string& DATA_DIR, const std::string& filename) {
	loadDataInputFile(DATA_DIR, filename);
}

int Bridge::addParticle(Eigen::Vector2d& point, 
	double damping,
	double pradius) {

	for (int i = 0; i < particles.size(); i++) {
		Vector3d pos0 = particles[i]->x0;
		double x = point(0);
		double y = point(1);
		if (abs(x - pos0(0)) < 0.001) {
			if (abs(y - pos0(1)) < 0.001) {
				//both the x and y of the point we are checking match the x&y of the particle's x0
				return i; //dont add point to the list, return the particle's index
			}
		}
	}

	auto p = make_shared<Particle>(); //we'll set all position's z coord to 0;
	particles.push_back(p);
	p->r = pradius;
	p->d = damping;
	Vector3d newpoint;
	newpoint << point, 0.0; //set the z to 0
	p->x0 = newpoint;
	p->x = p->x0;
	p->v0.setZero();
	p->v.setZero();
	p->p = p->x;
	p->fixed = false;
	//p->mass;   //TODO: figure out the mass of each particle;
	return particles.size() - 1; //return last index
}

Bridge::Bridge(std::vector<std::pair<Eigen::Vector2d, Eigen::Vector2d>>& points,
	double mass,
	double alpha,
	double damping,
	double pradius) 
{
	assert(mass > 0.0);
	assert(alpha >= 0.0);
	assert(damping >= 0.0);
	assert(pradius >= 0.0);

	threshold = 10e-7; //TODO: change this later 

	int totalNumberBeams = points.size();
	for (int i = 0; i < points.size(); i++) {
		
		int p1 = addParticle(points[i].first, damping, pradius);
		int p2 = addParticle(points[i].second, damping, pradius);

		beams.push_back(make_shared<Spring>(particles[p1], particles[p2], alpha));
		int c = beams.size() - 1;
		//particles[p1]->edges.push_back(beams[c]);
		//particles[p2]->edges.push_back(beams[c]);
		particles[p1]->edgecount++;
		particles[p2]->edgecount++;
		beams[c]->p0i = p1;
		beams[c]->p1i = p2;

		if (i < 4) {
			beams[c]->isRoad = true;
		}
		else {
			beams[c]->isRoad = false;
		}
	}
	
	int totalParticles = particles.size();
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->m = mass / totalParticles; //set mass of all particles
	}

	//TODO: somehow sort beams
	particles[0]->fixed = true;
	particles[4]->fixed = true;

	
	car = make_shared<Particle>();
	car->fixed = false;
	car->m = 0;
	car->d = damping;
	car->r = 0.05;
	car->x0.setZero();
	car->x = car->x0;
	car->v0.setZero();
	car->v.setZero();

}

Bridge::~Bridge() {

}

void Bridge::tare() //nandini - not sure what tare does tbh
{
	for (auto p : particles) {
		p->tare();
	}
}

void Bridge::reset()
{
	for (auto p : particles) {
		p->reset();
	}
}

void Bridge::draw(shared_ptr<MatrixStack> MV, const shared_ptr<Program> p, const shared_ptr<Shape> shape) {

	//draw bridge with old OpenGL
	glLineWidth(4.0f);
	glColor3f(1.0f, 0.0f, 0.0f);  //should be red
	glBegin(GL_LINES);
	
	std::lock_guard<std::mutex> lock(beamsMutex);

	for (int i = 0; i < beams.size(); i++) {
		if (beams[i] != nullptr) {
			float r;
			if (abs(beams[i]->lambda - threshold) < 1e-8) {
				int temp = 0;
			}
			if (beams[i]->lambda >= 0) {
				r = (1 / threshold) * beams[i]->lambda;
			}
			else {
				r = -1 * (1 / threshold) * beams[i]->lambda;
			}

			//cout << beams[i]->lambda << endl;
			glColor3f(r, 0.0f, 0.0f);
			float x0 = beams[i]->p0->x(0);
			float y0 = beams[i]->p0->x(1);
			float x1 = beams[i]->p1->x(0);
			float y1 = beams[i]->p1->x(1);
			glVertex3f(x0, y0, 0.0f);
			glVertex3f(x1, y1, 0.0f);
		}
	}
	glEnd();

	if (car != nullptr) {
		glPointSize(15.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		/*glBegin(GL_POINTS);
		glVertex3f(car->x(0), car->x(1), 0.0f);
		glEnd();*/

		glBegin(GL_POLYGON);
		for (float i = 0; i <= 6.283185; i += 0.6283185) {
			float x = car->r * cos(i) + car->x(0);
			float y = car->r * sin(i) + car->x(1);
			glVertex3f(x, y, 0.0f);
		}
			//Gl.glVertex3dv(vertices[i]);
		glEnd();
	}
	

	//draw the bridge with current OpenGL and 3D links
	/*std::lock_guard<std::mutex> lock(beamsMutex);
	for (int i = 0; i < beams.size(); i++) {
		beams[i]->draw(p, MV, shape);
	}*/
	
}



bool BetweenP0andP1(Vector3d& p0, Vector3d& p1, Vector3d& c) {
	double left, right, bottom, top;
	left = min(p0(0), p1(0));
	right = max(p0(0), p1(0));
	bottom = min(p0(1), p1(1));
	top = max(p0(1), p1(1));

	if ( (left <= c(0)) && (c(0) <= right) && 
		(bottom <= c(1)) && (c(1) <= top)) {
		return true;
	}
	return false;
}

void Bridge::step(double h, const Vector3d& grav) {

	//move car
	int roadbeam = -1;
	if (car != nullptr) {
		//car->x += Vector3d(0.0005, 0, 0);
		Vector3d zero;
		zero.setZero();
		Vector3d carforce = car->m * grav - car->d * car->v;
		car->v += (h / car->m) * carforce;
		car->p = car->x;
		car->x += h * car->v;
		
		float mindist = 1000000.0f;
		for (int j = 0; j < beams.size(); j++) {
			if (beams[j] != nullptr) {
				if (beams[j]->isRoad) {
					Vector3d midpoint = (beams[j]->p0->x + beams[j]->p1->x) / 2;
					float dist = (midpoint - car->x).norm(); //distance between car's pos and middle of the spring
					if (dist < mindist) {
						roadbeam = j; //gets set to beam car is closest to
						mindist = dist;
					}
				}
			}
		}

		Vector3d firstpt = particles[beams[roadbeam]->p0i]->x;
		Vector3d secondpt = particles[beams[roadbeam]->p1i]->x;
		float distance = (car->x - firstpt).norm();
		float length = (secondpt - firstpt).norm();
		float s = distance / length;

		//move beam positions
		for (int i = 0; i < particles.size(); i++) {
			if (particles[i] != nullptr) {
				if (!particles[i]->fixed) {
					Vector3d fi;
					if (beams[roadbeam]->p0i == i) {
						fi = (particles[i]->m + ((1 - s) * car->m)) * grav - particles[i]->d * particles[i]->v;
					}
					else if (beams[roadbeam]->p1i == i) {
						fi = (particles[i]->m + (s * car->m)) * grav - particles[i]->d * particles[i]->v;
					}
					else {
						fi = particles[i]->m * grav - particles[i]->d * particles[i]->v;
					}
					particles[i]->v += (h / particles[i]->m) * fi;
					particles[i]->p = particles[i]->x;
					particles[i]->x += h * particles[i]->v;
				}
			}
		}
	}
	else {
		//move beam positions
		for (int i = 0; i < particles.size(); i++) {
			if (particles[i] != nullptr) {
				if (!particles[i]->fixed) {
					Vector3d fi = particles[i]->m * grav - particles[i]->d * particles[i]->v;
					particles[i]->v += (h / particles[i]->m) * fi;
					particles[i]->p = particles[i]->x;
					particles[i]->x += h * particles[i]->v;
				}
			}
		}
	}


	std::lock_guard<std::mutex> lock(beamsMutex);
	//apply the spring constraints
	for (int i = 0; i < beams.size(); i++) {
		if (beams[i] != nullptr) {
			double Cj;
			vector<vector<double>> gradCj;

			beams[i]->springConstraint(Cj, gradCj);

			Vector3d gradCj0;
			gradCj0 << gradCj[0][0], gradCj[0][1], gradCj[0][2];

			Vector3d gradCj1;
			gradCj1 << gradCj[1][0], gradCj[1][1], gradCj[1][2];

			double w0 = 1 / beams[i]->p0->m;
			double w1 = 1 / beams[i]->p1->m;

			double _lambda = -Cj / ((w0 * pow(gradCj0.norm(), 2)) + (w1 * pow(gradCj1.norm(), 2)) + (beams[i]->alpha / (h * h)));
			beams[i]->lambda = _lambda;

			float r;
			if (beams[i]->lambda >= 0) {
				r = (1 / threshold) * beams[i]->lambda;
			}
			else {
				r = -1 * (1 / threshold) * beams[i]->lambda;
			}
			if (r > 1) { //lambda is too large, delete beam
				int point0 = beams[i]->p0i;
				int point1 = beams[i]->p1i;
				beams[i]->p0->edgecount--;
				beams[i]->p1->edgecount--;
				//beams.erase(beams.begin() + i);
				beams[i] = nullptr;
				if (car != nullptr) {
					if (roadbeam == i) {
						car = nullptr; //destroy car
					}
				}
				if (particles[point0]->edgecount <= 0) particles[point0] = nullptr;
				if (particles[point1]->edgecount <= 0) particles[point1] = nullptr;
				continue;
			}
			else {
				//beams[i]->lambda = _lambda;
				//double lambda = (-1 * Cj) / (w0 + w1 + (beams[i]->alpha / (h * h)));
				if (!beams[i]->p0->fixed) {
					beams[i]->p0->x += _lambda * w0 * gradCj0;
				}
				if (!beams[i]->p1->fixed) {
					beams[i]->p1->x += _lambda * w1 * gradCj1;
				}
			}
		}
	}


	//apply car constraints -> get line perpendicular to beam. project car position on beam.  draw point radius distance above beam in direction of norma.
	if (car != nullptr) {
		
		
		/*Vector3d normal = (Vector3d(0, 0, 1).cross(beams[roadbeam]->p1->x - beams[roadbeam]->p0->x)).normalized();
		Vector3d a = (beams[roadbeam]->p1->x - beams[roadbeam]->p0->x);
		Vector3d b = car->x - beams[roadbeam]->p0->x;
		Vector3d c = (a.dot(b) / a.norm()) * a.normalized();
		c += (car->r) * normal;
		car->x = c;*/

		for (int i = 0; i < beams.size(); i++) {
			if ((beams[i] != nullptr) && beams[i]->isRoad) {
				Vector3d point0 = beams[i]->p0->x;
				Vector3d point1 = beams[i]->p1->x;
				Vector3d AB = point1 - point0; //beam vector
				Vector3d AC = car->x - point0; //vector to circle from beam
				Vector3d projection = (AC.dot(AB) / AB.norm()) * AB.normalized();
				Vector3d closestPtOnLine = point0 + projection; //closest point on line to the circle
				if (BetweenP0andP1(point0, point1, closestPtOnLine)) {
					float distanceFromLine = (car->x - closestPtOnLine).norm(); //distance from the closest point to the circle
					if (distanceFromLine <= car->r) { //there should be an intersection
						//float displace = abs(distanceFromLine - car->r);
						Vector3d aaaaa = (car->x - closestPtOnLine).normalized();
						car->x = closestPtOnLine + aaaaa * car->r;
						//car->x = closestPtOnLine;
					}
				}
				
			}
		}

		if ((car->x(0) - endpt(0)) > 0.001) {
			car = nullptr; //car reached the finish line, delete car
		}
	}


	//update velocities
	for (int i = 0; i < particles.size(); i++) {
		if (particles[i] != nullptr) {
			if (!particles[i]->fixed) {
				particles[i]->v = (1 / h) * (particles[i]->x - particles[i]->p);
			}
		}
	}
	if (car != nullptr) {
		car->v = (1 / h) * (car->x - car->p);
	}
}