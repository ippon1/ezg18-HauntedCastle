#include <glm/gtc/type_ptr.hpp>

#include "Actor.hpp"
#include "../Resources/Const.hpp"

using namespace cgue;
using namespace glm;

Actor::Actor(Shader* shader, float sposx, float sposy, float sposz, float sroty)
{
	this->posx = 0;
	this->posy = 0;
	this->posz = 5;
	this->roty = 180;
	const std::string& displayFile = "knight1/knight1.dae";

	mat4& matrix = mat4(1.0f);

	mat4& initTrans = mat4(1.0f);
	/*
	initTrans = rotate(initTrans, PI / 2.0f, vec3(1, 0, 0));
	initTrans = rotate(initTrans, PI / 2.0f, vec3(0, 0, 1));
	initTrans = translate(initTrans, vec3(0, 0, 0));
	*/

	//matrix = rotate(matrix, sroty / 180.0f * PI, glm::vec3(0, 0, 1));

	init(displayFile, matrix, shader, initTrans);
}

Actor::~Actor(){
	// TODO delete data??
}

void Actor::resetPosition()
{
	modelMatrix = mat4(1.0f);
}

void Actor::initActor()
{
	//PxVec3 position(posx, posy, posz);

	mat4x4 matTrans = mat4x4(1.0);
	matTrans = translate(matTrans, changeAxis * vec3(posx, posy, posz));
	matTrans = rotate(matTrans, radians(roty), changeAxis * vec3(0, 0, 1));

	/*
	matTrans = rotate(matTrans, roty / 180.0f * PI, glm::vec3(0, 1, 0));
	matTrans = translate(matTrans, vec3(posx, posy, posz));
	*/

	PxTransform transform = PxTransform(glmToPxMat(matTrans));
	actor = Geometry::gPhysicsSDK->createRigidDynamic(transform);

	/*
	PxMaterial *aMaterial = gPhysicsSDK->createMaterial(2.0f, 2.0f, 0.0f);
	//PxMaterial *aMaterial = gPhysicsSDK->createMaterial(10.0f, 10.0f, 10.0f);
	mat4x4 rot = mat4x4(1.0f);// rotate(mat4x4(1.0f), roty / 180.0f * PI, vec3(0, 1, 0));
	vec4 positionSphere = rot * vec4(0, 0, -0.3, 0);
	PxShape* aCapsuleShape = actor->createShape(PxSphereGeometry(1), *aMaterial, PxTransform(PxVec3(positionSphere.x, positionSphere.y, positionSphere.z)));
	
	vec4 positionBasket = rot * vec4(0.0, 1.1, 0.0, 0);
	PxShape* xyz = actor->createShape(PxBoxGeometry(0.13, 0.6, 0.13), *aMaterial, PxTransform(PxVec3(positionBasket.x, positionBasket.y, positionBasket.z)));
	
	aCapsuleShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	*/
	actor->setMass(100);
	actor->setRigidDynamicFlag(physx::PxRigidBodyFlag::eENABLE_CCD, false);
	actor->setLinearDamping(5);
	actor->setAngularDamping(5);
	actor->setMaxAngularVelocity(1);
	actor->setMaxDepenetrationVelocity(1);

	gScene->addActor(*actor);

	PxD6Joint* mJoint = PxD6JointCreate(*gPhysicsSDK, actor, PxTransform::createIdentity(), nullptr, actor->getGlobalPose());
	
	mJoint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE); // Movement on X allowed
	mJoint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE); // Movement on Y allowed
	mJoint->setMotion(PxD6Axis::eZ, PxD6Motion::eFREE); // Movement on Z allowed
	
	//This will lock a rotation around the X axis :
	mJoint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
	//mJoint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);

	//For the Y rotation axis :
	mJoint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);

	//For the Z rotation axis :
	mJoint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	//mJoint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLOCKED);
}

mat4x4 Actor::getGlobalPose()
{
	return pxMatToGlm(PxMat44(actor->getGlobalPose()));
}

void Actor::excelerateSpeed(float factor) {
	if (factor > 0.5) {
		extraSpeed *= factor;
	}
}

float Actor::getExtraSpeed() {
	return extraSpeed;
}

void Actor::PxRotate(float rx, float ry, float rz)
{
	vec3 r = changeAxis * vec3(rx, ry, rz);
	actor->addTorque(actor->getGlobalPose().rotate(PxVec3(r.x, r.y, r.z)));
}

void Actor::PxTranslate(float tx, float ty, float tz)
{
	vec3 t = changeAxis * vec3(tx, ty, tz);
	actor->addForce(actor->getGlobalPose().rotate(PxVec3(t.x, t.y, t.z)));
}