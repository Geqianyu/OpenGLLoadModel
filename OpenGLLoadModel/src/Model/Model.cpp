
#include "Model.h"
#include "Log/Log.h"

void Model::load_model(const std::string& _path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::string error_info(importer.GetErrorString());
        GQY_OPENGL_LOAD_MODEL_ERROR("error::assimp:: " + error_info);
        return;
    }

    m_directory = _path.substr(0, _path.find_last_of('/'));

    process_node(scene->mRootNode, scene);
    GQY_OPENGL_LOAD_MODEL_INFO("success to load model: " + _path);
}

void Model::process_node(aiNode* _node, const aiScene* _scene)
{
    for (unsigned int i = 0; i < _node->mNumMeshes; i++)
    {
        m_meshes.push_back(process_mesh(_scene->mMeshes[_node->mMeshes[i]], _scene));
    }

    for (unsigned int i = 0; i < _node->mNumChildren; i++)
    {
        process_node(_node->mChildren[i], _scene);
    }
}

std::shared_ptr<Mesh> Model::process_mesh(aiMesh* _mesh, const aiScene* _scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
    {
        Vertex vertex{};

        vertex.position = glm::vec3(_mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z);
        if (_mesh->HasNormals())
        {
            vertex.normal = glm::vec3(_mesh->mNormals[i].x, _mesh->mNormals[i].y, _mesh->mNormals[i].z);
        }
        if (_mesh->mTextureCoords[0])
        {
            vertex.tex_coords = glm::vec2(_mesh->mTextureCoords[0][i].x, _mesh->mTextureCoords[0][i].y);
            vertex.tangent = glm::vec3(_mesh->mTangents[i].x, _mesh->mTangents[i].y, _mesh->mTangents[i].z);
            vertex.bitangent = glm::vec3(_mesh->mBitangents[i].x, _mesh->mBitangents[i].y, _mesh->mBitangents[i].z);
        }
        else
        {
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
    {
        aiFace face = _mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];
    Material mesh_material;
    aiColor3D color_value;
    aiString texture_path;
    if (material->GetTextureCount(aiTextureType_AMBIENT) == 0)
    {
        material->Get(AI_MATKEY_COLOR_AMBIENT, color_value);
        mesh_material.set_Ka(glm::vec3(color_value.r, color_value.g, color_value.b));
    }
    else
    {
        material->GetTexture(aiTextureType_AMBIENT, 0, &texture_path);
        mesh_material.set_Ka(m_directory + "/" + texture_path.C_Str());
    }
    if (material->GetTextureCount(aiTextureType_DIFFUSE) == 0)
    {
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color_value);
        mesh_material.set_Kd(glm::vec3(color_value.r, color_value.g, color_value.b));
    }
    else
    {
        material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
        mesh_material.set_Kd(m_directory + "/" + texture_path.C_Str());
    }
    if (material->GetTextureCount(aiTextureType_SPECULAR) == 0)
    {
        material->Get(AI_MATKEY_COLOR_SPECULAR, color_value);
        mesh_material.set_Ks(glm::vec3(color_value.r, color_value.g, color_value.b));
    }
    else
    {
        material->GetTexture(aiTextureType_SPECULAR, 0, &texture_path);
        mesh_material.set_Ks(m_directory + "/" + texture_path.C_Str());
    }
    float shininess = 0.0f;
    material->Get(AI_MATKEY_SHININESS, shininess);
    mesh_material.set_Ns(shininess);

    return std::make_shared<Mesh>(vertices, indices, std::make_shared<Material>(mesh_material));
}
