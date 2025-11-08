#include "Core/Mesh.h"
#include "RendererAPI/Buffer.h"
#include "RendererAPI/VertexArray.h"
#include <glad/glad.h>
#include <memory>

Mesh::Mesh(std::vector<Core::Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
    : textures(textures)
{
    std::shared_ptr<Core::VertexBuffer> vertexBuffer = Core::VertexBuffer::Create((float*)vertices.data(), vertices.size() * sizeof(Core::Vertex));
    std::shared_ptr<Core::IndexBuffer> indexBuffer = Core::IndexBuffer::Create(indices.data(), indices.size());
    vertexArray = std::make_shared<Core::VertexArray>();
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);
}

void Mesh::Draw(std::shared_ptr<Core::Shader> shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int emissionNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
		else if (name == "texture_emission")
			number = std::to_string(emissionNr++);

        shader->setInt(("material." + name + number).c_str(), i);
        textures[i].ptr->Bind(i);
    }

    // draw mesh
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}