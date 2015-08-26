#include "Model.h"
#include "../Core/Logger.h"

Model::Model()
{
	modelMatUpdated = false;
}


Model::~Model()
{
}

bool Model::loadFromFile(string filepath, string filename)
{
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath + filename,
		aiProcess_Triangulate
		| aiProcess_JoinIdenticalVertices
		| aiProcess_OptimizeMeshes
		| aiProcess_GenSmoothNormals
		| aiProcess_GenUVCoords
		| aiProcess_ValidateDataStructure
		| aiProcess_CalcTangentSpace
		);

	if (!scene)
	{
		LOG_ERR("Could not load model ", filename);
		return false;
	}

	// For each mesh
	vector<int> matIndices(scene->mNumMeshes);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		meshes.push_back(std::move(Mesh()));

		// Create shaders
		meshes[i].compileShaders("", Constants::BASIC_SHADER);
		meshes[i].getProgram().use();

		// Save material index for later
		matIndices[i] = mesh->mMaterialIndex;

		// Load vertices
		vector<Vertex> vertices;
		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			Vertex vert;

			vert.position = vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);

			if (mesh->mTextureCoords[0] != NULL) // TODO: fix this
				vert.texcoord = vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
			else
				vert.texcoord = vec2(0.f);

			if (mesh->mNormals != NULL)
				vert.normal = vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);

			if (mesh->HasTangentsAndBitangents())
			{
				vert.tangent = vec3(mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z);
				vert.bitangent = vec3(mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z);
			}
				

			vertices.emplace_back(std::move(vert));
		} // End for each vertex

		// Load element (index) array
		vector<unsigned int> indices;
		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < 3; k++)
			{
				indices.push_back(face.mIndices[k]);
			}
		} // End for each index

		meshes[i].getVAO().attachBuffers(vertices, indices);
	} // End for each mesh

	// Load materials
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* mat = scene->mMaterials[i];
		int texIndex = 0;
		aiString textureFile;
		aiColor3D col;
		for (size_t j = 0; j < meshes.size(); j++)
		{
			Mesh& mesh = meshes[j];
			if (matIndices[j] != i) continue;

			if (mat->GetTexture(aiTextureType_DIFFUSE, texIndex, &textureFile) == AI_SUCCESS)  // Diffuse
				mesh.getMaterial().addTexture(filepath + textureFile.C_Str(), TextureType::Diffuse);
			if (mat->GetTexture(aiTextureType_SPECULAR, texIndex, &textureFile) == AI_SUCCESS) // Specular
				mesh.getMaterial().addTexture(filepath + textureFile.C_Str(), TextureType::Specular);
			if (mat->GetTexture(aiTextureType_SHININESS, texIndex, &textureFile) == AI_SUCCESS)  // Shininess (exponent)
				mesh.getMaterial().addTexture(filepath + textureFile.C_Str(), TextureType::Shininess);
			if (mat->GetTexture(aiTextureType_NORMALS, texIndex, &textureFile) == AI_SUCCESS)  // Normals
				mesh.getMaterial().addTexture(filepath + textureFile.C_Str(), TextureType::Normals);

			if (mat->Get(AI_MATKEY_TEXTURE_HEIGHT(0), textureFile) == AI_SUCCESS)
			{
				mesh.getMaterial().addTexture(filepath + textureFile.C_Str(), TextureType::Normals);
			}
				

			// Material colours
			if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, col) == AI_SUCCESS)  // Diffuse
				mesh.getMaterial().addColour(vec3(col.r, col.g, col.b), ColourType::Diffuse);
			if (mat->Get(AI_MATKEY_COLOR_AMBIENT, col) == AI_SUCCESS)  // Ambient
				mesh.getMaterial().addColour(vec3(col.r, col.g, col.b), ColourType::Ambient);
			if (mat->Get(AI_MATKEY_COLOR_SPECULAR, col) == AI_SUCCESS) // Specular
				mesh.getMaterial().addColour(vec3(col.r, col.g, col.b), ColourType::Specular);

			// Other constants
			float val;
			if (mat->Get(AI_MATKEY_SHININESS, val) == AI_SUCCESS) // Shininess (exponent)
				mesh.getMaterial().addAttrib(val, Constants::SHININESS_NAME);

			// Load the material into the shaders
			mesh.getProgram().use();
			mesh.getMaterial().addToProgram(mesh.getProgram());
		}
	}

	return true;
}

void Model::attachUniformBlock(string name, int index) const
{
	for (const auto& mesh : meshes)
	{
		mesh.getProgram().attachUniformBlock(name, index);
	}
}

void Model::render()
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		//meshes[i].getProgram().setUniform(Constants::VIEW_MATRIX_LOCATION, viewMatrix);
		//meshes[i].getProgram().setUniform(Constants::PROJECTION_MATRIX_LOCATION, projectionMatrix);
		meshes[i].getProgram().setUniform(Constants::MODEL_MATRIX_LOCATION, getModelMatrix());
		meshes[i].render();
	}
}

mat4 Model::getModelMatrix()
{
	if (!modelMatUpdated)
	{
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		modelMatUpdated = true;
	}
	return modelMatrix;
}

void Model::translateTo(vec3 translation)
{
	static mat4 identity = mat4();
	translationMatrix = glm::translate(identity, translation);
	modelMatUpdated = false;
}
void Model::translateBy(vec3 translation)
{
	translationMatrix = glm::translate(translationMatrix, translation);
	modelMatUpdated = false;
}
void Model::rotateTo(float angle, vec3 axis)
{
	static mat4 identity = mat4();
	rotationMatrix = rotate(identity, angle, axis);
	modelMatUpdated = false;
}
void Model::rotateBy(float angle, vec3 axis)
{
	rotationMatrix = rotate(rotationMatrix, angle, axis);
	modelMatUpdated = false;
}
void Model::scaleTo(vec3 factor)
{
	static glm::mat4 identity = glm::mat4();
	scaleMatrix = glm::scale(identity, factor);
	modelMatUpdated = false;
}
void Model::scaleBy(vec3 factor)
{
	scaleMatrix = glm::scale(scaleMatrix, factor);
	modelMatUpdated = false;
}