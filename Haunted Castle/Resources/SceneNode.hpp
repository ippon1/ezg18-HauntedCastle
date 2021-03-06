#pragma once

#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Mesh.hpp"
#include "Const.hpp"

using namespace glm;

namespace cgue
{
	class SceneNode
	{
	public:
		SceneNode(aiNode* aiNode, const aiScene* scene, string modelDir, Shader* shader);
		virtual ~SceneNode();

		void draw(Shader* drawShader, mat4x4 view, glm::mat4x4 proj, mat4x4 camera_model, bool cull);

		void translateLinear(string meshName, vec3 trans, float time_start, float duration, float time, float time_delta);
		void translateGravity(string meshName, float trans_y_end, float time_start, float time, float time_delta);
		void rotateLinear(string meshName, vec3 rotateAxis, float rotateValue, bool exactEndValue, float time_start, float duration, float time, float time_delta);

	private:

		int countVertices(aiNode* node, const aiScene* scene);

		mat4 convertMatrix(const aiMatrix4x4 m);

		int size;
		float* positions;
		float* normals;
		int* indices;
		float* uvs;

		int childNodeCount;
		SceneNode** childNode;

		int meshCount = 0;
		Mesh** mesh;

		string name;
		mat4x4 transform;
		mat4x4 transformOriginal;

		vec3 forces = vec3(0);

		bool init = false;
	};
}