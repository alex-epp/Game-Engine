#include "Model.h"
#include "../Core/Logger.h"
#include "../Core/Constants.h"

using core::Constants;

// https://stackoverflow.com/questions/17074324/how-can-i-sort-two-vectors-in-the-same-way-with-criteria-that-uses-only-one-of
template <typename T, typename Compare>
std::vector<std::size_t> sort_permutation(
	const std::vector<T>& vec,
	Compare& compare)
{
	std::vector<std::size_t> p(vec.size());
	std::iota(p.begin(), p.end(), 0);
	std::sort(p.begin(), p.end(),
		[&](std::size_t i, std::size_t j) { return compare(vec[i], vec[j]); });
	return p;
}

template <typename T>
void apply_permutation_in_place(
	std::vector<T>& vec,
	const std::vector<std::size_t>& p)
{
	std::vector<bool> done(vec.size());
	for (std::size_t i = 0; i < vec.size(); ++i)
	{
		if (done[i])
		{
			continue;
		}
		done[i] = true;
		std::size_t prev_j = i;
		std::size_t j = p[i];
		while (i != j)
		{
			std::swap(vec[prev_j], vec[j]);
			done[j] = true;
			prev_j = j;
			j = p[j];
		}
	}
}

Model::Model()
{
	modelMatUpdated = false;
}


Model::~Model()
{
}

bool Model::loadFromFile(string filepath, string filename)
{
	auto startTime = time(0);
	Assimp::Importer importer;
	const aiScene* aiscene = importer.ReadFile(filepath + filename,
		aiProcess_Triangulate
		| aiProcess_JoinIdenticalVertices
		| aiProcess_OptimizeMeshes
		| aiProcess_GenSmoothNormals
		| aiProcess_GenUVCoords
		| aiProcess_ValidateDataStructure
		| aiProcess_CalcTangentSpace
		);

	if (!aiscene)
	{
		LOG_ERR("Could not load model ", filename);
		return false;
	}
	cout << "Assimp: " << time(0) - startTime << endl;
	startTime = time(0);

	vector<shared_ptr<Material>> matsByIndex;
	// Load materials
	for (decltype(aiscene->mNumMaterials) i = 0; i < aiscene->mNumMaterials; ++i)
	{
		auto material = make_shared<Material>();
		matsByIndex.push_back(material);

		const aiMaterial* aimat = aiscene->mMaterials[i];
		int texIndex = 0;
		aiString textureFile;
		aiColor3D col;
		if (aimat->GetTexture(aiTextureType_DIFFUSE, texIndex, &textureFile) == AI_SUCCESS)  // Diffuse
			material->addTexture(filepath + textureFile.C_Str(), Material::TextureType::Diffuse);
		if (aimat->GetTexture(aiTextureType_SPECULAR, texIndex, &textureFile) == AI_SUCCESS) // Specular
			material->addTexture(filepath + textureFile.C_Str(), Material::TextureType::Specular);
		if (aimat->GetTexture(aiTextureType_SHININESS, texIndex, &textureFile) == AI_SUCCESS)  // Shininess (exponent)
			material->addTexture(filepath + textureFile.C_Str(), Material::TextureType::Shininess);
		if (aimat->GetTexture(aiTextureType_NORMALS, texIndex, &textureFile) == AI_SUCCESS)  // Normals
			material->addTexture(filepath + textureFile.C_Str(), Material::TextureType::Normals);


		if (aimat->Get(AI_MATKEY_TEXTURE_HEIGHT(0), textureFile) == AI_SUCCESS)
		{
			material->addTexture(filepath + textureFile.C_Str(), Material::TextureType::Normals);
		}

		// Material colours
		if (aimat->Get(AI_MATKEY_COLOR_DIFFUSE, col) == AI_SUCCESS)  // Diffuse
			material->addColour(vec3(col.r, col.g, col.b), Material::ColourType::Diffuse);
		if (aimat->Get(AI_MATKEY_COLOR_AMBIENT, col) == AI_SUCCESS)  // Ambient
			material->addColour(vec3(col.r, col.g, col.b), Material::ColourType::Ambient);
		if (aimat->Get(AI_MATKEY_COLOR_SPECULAR, col) == AI_SUCCESS) // Specular
			material->addColour(vec3(col.r, col.g, col.b), Material::ColourType::Specular);

		// Other constants
		float val;
		if (aimat->Get(AI_MATKEY_SHININESS, val) == AI_SUCCESS) // Shininess (exponent)
			material->addAttrib(val, Constants::get().getString("shininess_name"));

		material->compileShaders(Constants::get().getString("shader_path"), Constants::get().getString("basic_shader"));
		material->getProgram().link();
		material->getProgram().use();
		material->addToProgram();
	}

	cout << "Materials: " << time(0) - startTime << endl;
	startTime = time(0);

	// Load meshes
	for (unsigned int i = 0; i < aiscene->mNumMeshes; i++)
	{
		aiMesh* aimesh = aiscene->mMeshes[i];
		meshes.emplace_back(make_shared<Mesh>());
		materials.emplace_back(matsByIndex[aimesh->mMaterialIndex]);

		// Load vertices
		vector<Vertex> vertices;
		for (unsigned int j = 0; j < aimesh->mNumVertices; j++)
		{
			Vertex vert;

			vert.position = vec3(aimesh->mVertices[j].x, aimesh->mVertices[j].y, aimesh->mVertices[j].z);

			if (aimesh->mTextureCoords[0] != NULL) // TODO: fix this
				vert.texcoord = vec2(aimesh->mTextureCoords[0][j].x, aimesh->mTextureCoords[0][j].y);
			else
				vert.texcoord = vec2(0.f);

			if (aimesh->mNormals != NULL)
				vert.normal = vec3(aimesh->mNormals[j].x, aimesh->mNormals[j].y, aimesh->mNormals[j].z);

			if (aimesh->HasTangentsAndBitangents())
			{
				vert.tangent = vec3(aimesh->mTangents[j].x, aimesh->mTangents[j].y, aimesh->mTangents[j].z);
				vert.bitangent = vec3(aimesh->mBitangents[j].x, aimesh->mBitangents[j].y, aimesh->mBitangents[j].z);
			}
				

			vertices.emplace_back(std::move(vert));
		} // End for each vertex

		// Load element (index) array
		vector<unsigned int> indices;
		for (unsigned int j = 0; j < aimesh->mNumFaces; j++)
		{
			const aiFace& face = aimesh->mFaces[j];
			for (int k = 0; k < 3; k++)
			{
				indices.push_back(face.mIndices[k]);
			}
		} // End for each index

		meshes[i]->getVAO().attachBuffers(vertices, indices);
	} // End for each mesh

	cout << "Meshes: " << time(0) - startTime << endl;
	startTime = time(0);
	
	auto p = sort_permutation(materials, [](auto lhs, auto rhs) { return lhs < rhs; });
	apply_permutation_in_place(materials, p);
	apply_permutation_in_place(meshes, p);

	cout << "Sorting: " << time(0) - startTime << endl;
	startTime = time(0);

	return true;
}

void Model::attachUniformBlock(string name, int index) const
{
	for (const auto& mat : materials)
	{
		mat->getProgram().use();
		mat->getProgram().attachUniformBlock(name, index);
	}
}

void Model::render()
{
	shared_ptr<Material> curMat = nullptr;
	static auto modelMatrixLoc = Constants::get().getNum<int>("model_matrix_location");
	for (size_t i = 0; i < meshes.size(); i++)
	{
		if (curMat != materials[i])
		{
			materials[i]->getProgram().use();
			materials[i]->getProgram().setUniform(modelMatrixLoc, getModelMatrix());
			materials[i]->activateTextures();
			materials[i]->addToProgram();
			curMat = materials[i];
		}
		meshes[i]->getVAO().draw();
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