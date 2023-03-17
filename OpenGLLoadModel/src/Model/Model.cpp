
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Model.h"
#include "Log/Log.h"

void Model::load_model(const std::string& _path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::string error_info(importer.GetErrorString());
        GQY_OPENGL_LOAD_MODEL_ERROR("error::assimp:: " + error_info);
        return;
    }
    // retrieve the directory path of the filepath
    m_directory = _path.substr(0, _path.find_last_of('/'));

    // process ASSIMP's root node recursively
    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* _node, const aiScene* _scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < _node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
        m_meshes.push_back(process_mesh(mesh, _scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < _node->mNumChildren; i++)
    {
        process_node(_node->mChildren[i], _scene);
    }
}

Mesh Model::process_mesh(aiMesh* _mesh, const aiScene* _scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // positions
        vertex.position = glm::vec3(_mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z);

        // normals
        if (_mesh->HasNormals())
        {
            vertex.normal = glm::vec3(_mesh->mNormals[i].x, _mesh->mNormals[i].y, _mesh->mNormals[i].z);
        }

        // texture coordinates
        if (_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vertex.tex_coords = glm::vec2(_mesh->mTextureCoords[0][i].x, _mesh->mTextureCoords[0][i].y);
            
            // tangent
            vertex.tangent = glm::vec3(_mesh->mTangents[i].x, _mesh->mTangents[i].y, _mesh->mTangents[i].z);
            
            // bitangent
            vertex.bitangent = glm::vec3(_mesh->mBitangents[i].x, _mesh->mBitangents[i].y, _mesh->mBitangents[i].z);
        }
        else
        {
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
    {
        aiFace face = _mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process materials
    aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    
    // 2. specular maps
    std::vector<Texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    
    // 3. normal maps
    std::vector<Texture> normal_maps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
    
    // 4. height maps
    std::vector<Texture> height_maps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), height_maps.begin(), height_maps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::load_material_textures(aiMaterial* _mat, aiTextureType _type, const std::string& _type_name)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < _mat->GetTextureCount(_type); i++)
    {
        aiString str;
        _mat->GetTexture(_type, i, &str);
        
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (Texture& loaded_texture : m_textures_loaded)
        {
            if (std::strcmp(loaded_texture.path.c_str(), str.C_Str()) == 0)
            {
                textures.push_back(loaded_texture);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }

        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = texture_from_file(str.C_Str(), this->m_directory);
            texture.type = _type_name;
            texture.path = str.C_Str();
            textures.push_back(texture);
            m_textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}

GLuint texture_from_file(const std::string& _path, const std::string& _directory, bool gamma)
{
    std::string filename = _directory + '/' + _path;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
        {
            format = GL_RED;
        }
        else if (nrComponents == 3)
        {
            format = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        GQY_OPENGL_LOAD_MODEL_ERROR("Texture failed to load at path: " + _path);
        stbi_image_free(data);
    }

    return textureID;
}