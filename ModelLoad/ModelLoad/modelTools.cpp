#include "modelTools.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <functional>
#include <set>
#include "program.h"
void model::load_model(model::Model& p_model, std::string path, std::string vShader, std::string fShader)
{
	p_model._program = program::Program(vShader,fShader);
	size_t lastDivide = path.find_last_of('\\');
	std::string archive = path.substr(0, lastDivide + 1);
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		throw std::string("Import Model Failed::") + importer.GetErrorString();
	}
	auto loadTexture = [&p_model,&archive](aiMaterial* mat,aiTextureType type)->std::vector<Texture>
	{
		texture_type t_type;
		if (type == aiTextureType_DIFFUSE)
			t_type = texture_type::DIFFUSE;
		else
			t_type = texture_type::SPECULAR;
		std::vector<Texture> results;
		for (size_t t_num = 0; t_num < mat->GetTextureCount(type); t_num++)
		{
			aiString path;
			mat->GetTexture(type, t_num, &path);
			std::string str_path(path.C_Str());
			str_path = archive + str_path;
			std::map<std::string,Texture>::iterator it;
			if ((it = p_model.texturesHasLoaded.find(str_path)) != p_model.texturesHasLoaded.end())
			{
				results.push_back(it->second);
			}
			else {
				Texture tex(str_path, t_type);
				results.push_back(tex);
				p_model.texturesHasLoaded.insert({ str_path,tex });
			}
		}
		return results;
	};
	std::function<Mesh(aiMesh*)> processMesh;
	processMesh = [&scene,&loadTexture](aiMesh* mesh) ->Mesh{
		std::vector<Vetex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		//vertex
		for (size_t vIdx = 0; vIdx < mesh->mNumVertices; vIdx++)
		{
			glm::vec3 pos;
			aiVector3D cur_vertex = mesh->mVertices[vIdx];
			pos.x = cur_vertex.x;
			pos.y = cur_vertex.y;
			pos.z = cur_vertex.z;
			glm::vec3 norm;
			aiVector3D cur_norm = mesh->mNormals[vIdx];
			norm.x = cur_norm.x;
			norm.y = cur_norm.y;
			norm.z = cur_norm.z;
			//assimp allows every single vertex to have up to 8 texture coordinates
			//here we only retrieve the first(0) one
			glm::vec2 tCoord;
			if (mesh->mTextureCoords[0])
			{
				aiVector3D cur_tCoord = mesh->mTextureCoords[0][vIdx];
				tCoord.x = cur_tCoord.x;
				tCoord.y = cur_tCoord.y;
			}
			else
				tCoord = glm::vec2(0, 0);
			vertices.push_back({ pos,norm,tCoord });
		}
		//indices
		for (size_t fIdx = 0; fIdx < mesh->mNumFaces; fIdx++)
		{
			aiFace face = mesh->mFaces[fIdx];
			for (size_t iIdx = 0; iIdx < face.mNumIndices; iIdx++)
			{
				indices.push_back(face.mIndices[iIdx]);
			}
		}
		//material
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Texture> diffuseMaps = loadTexture(mat, aiTextureType_DIFFUSE);
			std::vector<Texture> specularMaps = loadTexture(mat, aiTextureType_SPECULAR);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		return Mesh(vertices, indices, textures);
	};
	std::function<void(aiNode*)> processNode;
	processNode = [&p_model, &scene,&processMesh,&processNode](aiNode* node)
	{
		for (size_t meshIdx = 0; meshIdx < node->mNumMeshes; meshIdx++)
		{
			p_model._meshs.push_back(processMesh(scene->mMeshes[node->mMeshes[meshIdx]]));
		}
		for (size_t childNodeIdx = 0; childNodeIdx < node->mNumChildren;childNodeIdx++)
		{
			processNode(node->mChildren[childNodeIdx]);
		}
	};
	processNode(scene->mRootNode);


}
