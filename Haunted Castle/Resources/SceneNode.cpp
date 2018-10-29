#include "SceneNode.hpp"

using namespace cgue;
using namespace std;

void printMatrix(string name, mat4x4 mat);

SceneNode::SceneNode(aiNode* aiNode, const aiScene* scene, string modelDir, Shader* shader)
{
	cout << "Node" << endl;

	name = aiNode->mName.data;
	transform = convertMatrix(aiNode->mTransformation);

	//printMatrix(name.data, transform);

	size = countVertices(aiNode, scene);
	positions = new float[size * 3];
	indices = new int[size];
	normals = new float[size * 3];
	uvs = new float[size * 2];

	cout << "size:" << size << endl;

	cout << "aiNode->mNumMeshes:" << aiNode->mNumMeshes << endl;

	meshCount = aiNode->mNumMeshes;
	mesh = new Mesh*[meshCount];

	for (int i = 0; i < aiNode->mNumMeshes; i++)
	{
		cout << "Mesh" << endl;

		int iMesh = aiNode->mMeshes[i];
		aiMesh* aiMesh = scene->mMeshes[iMesh];

		// MATERIAL
		int iMaterial = aiMesh->mMaterialIndex;
		const aiMaterial* material = scene->mMaterials[iMaterial];

		mesh[i] = new Mesh(modelDir, aiNode->mName.data, aiMesh, material, mat4x4(1), mat4x4(1), shader);
	}

	childNodeCount = aiNode->mNumChildren;
	childNode = new SceneNode*[childNodeCount];

	for (int i = 0; i < childNodeCount; i++)
	{
		childNode[i] = new SceneNode(aiNode->mChildren[i], scene, modelDir, shader);
	}
}

SceneNode::~SceneNode() {

	delete positions; positions = nullptr;
	delete normals; normals = nullptr;
	delete indices; indices = nullptr;
	delete uvs; uvs = nullptr;
	delete mesh; mesh = nullptr;
	delete childNode; childNode = nullptr;
	delete mesh; mesh = nullptr;
}

int SceneNode::countVertices(aiNode* node, const aiScene* scene)
{
	int count = 0;
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		int iMesh = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[iMesh];
		int iMeshFaces = mesh->mNumFaces;
		for (int j = 0; j < iMeshFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; k++)
			{
				count++;
			}
		}
	}
	return count;
}

mat4 SceneNode::convertMatrix(const aiMatrix4x4 m)
{
	mat4 Matri;

	Matri[0][0] = m.a1;
	Matri[0][1] = m.b1;
	Matri[0][2] = m.c1;
	Matri[0][3] = m.d1;
	Matri[1][0] = m.a2;
	Matri[1][1] = m.b2;
	Matri[1][2] = m.c2;
	Matri[1][3] = m.d2;
	Matri[2][0] = m.a3;
	Matri[2][1] = m.b3;
	Matri[2][2] = m.c3;
	Matri[2][3] = m.d3;
	Matri[3][0] = m.a4;
	Matri[3][1] = m.b4;
	Matri[3][2] = m.c4;
	Matri[3][3] = m.d4;

	return Matri;
}

void printMatrix(string name, mat4x4 mat)
{
	cout << name << endl;
	cout << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << " " << mat[3][0] << endl;
	cout << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << " " << mat[3][1] << endl;
	cout << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << " " << mat[3][2] << endl;
	cout << mat[0][3] << " " << mat[1][3] << " " << mat[2][3] << " " << mat[3][3] << endl;
}