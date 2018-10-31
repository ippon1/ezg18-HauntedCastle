#pragma once

#include <GL\glew.h>
#include "SceneObject.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "SceneNode.hpp"
#include <iostream>

#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <PhysX\PxPhysicsAPI.h>

using namespace physx;

namespace cgue
{
	class Geometry : public SceneObject
	{
	public:
		Geometry();
		virtual ~Geometry();
		void init(const std::string& displayFile, glm::mat4& matrix, Shader* _shader, glm::mat4x4 initTrans);
		void update(float time_delta, float time_abs);
		void draw(Shader* shader, mat4x4 view, glm::mat4x4 proj, mat4x4 camera_model, bool cull);
		Shader* getShader();
		void setPhysX(PxPhysics* gPhysicsSDK, PxFoundation*	gFoundation, PxDefaultErrorCallback	gDefaultErrorCallback, PxDefaultAllocator gDefaultAllocatorCallback, PxScene* gScene);
		virtual void initActor();
		virtual mat4x4 getGlobalPose();

		PxCooking* mCooking;

		PxPhysics*				gPhysicsSDK;				//Instance of PhysX SDK
		PxFoundation*			gFoundation;				//Instance of singleton foundation SDK class
		PxDefaultErrorCallback	gDefaultErrorCallback;		//Instance of default implementation of the error callback
		PxDefaultAllocator		gDefaultAllocatorCallback;	//Instance of default implementation of the allocator interface required by the SDK
		PxScene*				gScene;

		// Positions
		float posx;
		float posy;
		float posz;

		float rotx;
		float roty;
		float rotz;

		float scale = 1.0;
	private:
		GLuint vao;
		GLuint positionBuffer, normalsBuffer, uvBuffer, indexBuffer;
		Shader *shader;

		SceneNode* sceneNode;

		PxRigidDynamic*	gRigid;

		PxRigidDynamic* actor;
	};
};