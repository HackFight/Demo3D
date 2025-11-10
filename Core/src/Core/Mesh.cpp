#include "Core/Mesh.h"

// Core
#include "RendererAPI/RendererAPI.h"

namespace Core
{
    Mesh::Mesh(std::vector<Core::Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
        : textures(textures)
    {
        VertexBuffer vertexBuffer((float*)vertices.data(), vertices.size() * sizeof(Core::Vertex));
        IndexBuffer indexBuffer(indices.data(), indices.size());
        vertexArray.AddVertexBuffer(vertexBuffer);
        vertexArray.SetIndexBuffer(indexBuffer);
    }

    void Mesh::Draw(Core::Shader shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int emissionNr = 1;
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

            shader.setInt(("material." + name + number).c_str(), i);
            textures[i].ptr.Bind(i);
        }

        // draw mesh
		RendererAPI::DrawIndexed(vertexArray);
    }
}