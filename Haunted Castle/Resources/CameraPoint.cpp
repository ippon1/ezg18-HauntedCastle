#include "CameraPoint.hpp"

using namespace cgue;
using namespace glm;

CameraPoint::CameraPoint(vec3 point, vec3 derivative)
{
	//cout << "Point: " << point.x << " " << point.y << " " << point.z << " Derivative: " << derivative.x << " " << derivative.y << " " << derivative.z << endl;
	this->point = point;
	this->derivative = derivative;
	//cout << "Point: " << this->point.x << " " << this->point.y << " " << this->point.z << " Derivative: " << this->derivative.x << " " << this->derivative.y << " " << this->derivative.z << endl;
}

CameraPoint::~CameraPoint() {}

vec3 CameraPoint::getPoint()
{
	return point;
}

vec3 CameraPoint::getDerivative()
{
	return normalize(derivative);
}