
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
    std::vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    std::vector<Texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    std::vector<Texture> normal_maps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
    std::vector<Texture> height_maps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), height_maps.begin(), height_maps.end());

    return std::make_shared<Mesh>(vertices, indices, textures);
}

std::vector<Texture> Model::load_material_textures(aiMaterial* _material, aiTextureType _type, const std::string& _type_name)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < _material->GetTextureCount(_type); i++)
    {
        aiString str;
        _material->GetTexture(_type, i, &str);
        bool skip = false;

        for (Texture& loaded_texture : m_textures_loaded)
        {
            if (std::strcmp(loaded_texture.path.c_str(), str.C_Str()) == 0)
            {
                textures.push_back(loaded_texture);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture(texture_from_file(str.C_Str(), this->m_directory), _type_name, str.C_Str());
            textures.push_back(texture);
            m_textures_loaded.push_back(texture);
        }
    }
    return textures;
}

GLuint texture_from_file(const std::string& _path, const std::string& _directory, bool _gamma)
{
    std::string filename = _directory + '/' + _path;

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    int width, height, channels;
    stbi_uc* image = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (image)
    {
        glBindTexture(GL_TEXTURE_2D, texture_id);
        switch (channels)
        {
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, image);
            break;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            break;
        default:
            break;
        }
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(image);
    }
    else
    {
        GQY_OPENGL_LOAD_MODEL_ERROR("Texture failed to load at path: " + _path);
        stbi_image_free(image);
    }

    return texture_id;
}