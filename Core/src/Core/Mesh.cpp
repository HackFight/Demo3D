#include "Core/Mesh.h"

// Core
#include "RendererAPI/BufferManager.h"
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/ShaderManager.h"
#include "RendererAPI/TextureManager.h"

namespace Core
{
    Mesh::Mesh(std::vector<Core::Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures, bool staticDraw)
        : textures(textures)
    {
        m_VertexBuffer = VertexBufferManager::CreateVertexBuffer((float*)vertices.data(), vertices.size() * sizeof(Core::Vertex), staticDraw);
        m_IndexBuffer = IndexBufferManager::CreateIndexBuffer(indices.data(), indices.size());
        vertexArray = VertexArrayManager::CreateVertexArray();
        VertexArrayManager::AddVertexBuffer(vertexArray, m_VertexBuffer);
        VertexArrayManager::SetIndexBuffer(vertexArray, m_IndexBuffer);
    }
    Mesh::Mesh(uint32_t vertexBuffer, uint32_t indexBuffer, std::vector<Texture> textures)
        : m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), textures(textures)
    {
        vertexArray = VertexArrayManager::CreateVertexArray();
        VertexArrayManager::AddVertexBuffer(vertexArray, vertexBuffer);
        VertexArrayManager::SetIndexBuffer(vertexArray, indexBuffer);
    }

    void Mesh::Draw(uint32_t shader) const
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int emissionNr = 1;
        unsigned int normalmapNr = 1;
        for (unsigned int i = 0; i < 4; i++)
        {
            TextureManager::Unbind(i);
        }
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            else if (name == "texture_emission")
                number = std::to_string(emissionNr++);
            else if (name == "texture_normal")
                number = std::to_string(normalmapNr++);

            ShaderManager::setInt(shader, ("material." + name + number).c_str(), i);
            TextureManager::Bind(textures[i].ptr, i);
        }

        // draw mesh
		RendererAPI::DrawIndexed(vertexArray);
    }
}