#include "SoftBody.h"

namespace App
{
	SoftBody::SoftBody(SoftBodyModel model, uint32_t shader, glm::vec3 position, BlinnPhongMaterial material)
		: GameObject(model, shader, position, material),
		m_Model(model)
	{}
	SoftBody::~SoftBody() {}


	SoftBodyModel::SoftBodyModel(Core::Mesh mesh, std::vector<Core::Vertex> vertices, std::vector<std::vector<size_t>> pointsAttach, std::vector<glm::vec3> physicsPoints)
		: Core::Model(mesh),
		m_VertexBuffer(mesh.GetVertexBuffer()),
		m_Vertices(vertices),
		m_PointsAttach(pointsAttach),
		m_PhysicsPoints(physicsPoints)
	{}
	SoftBodyModel::~SoftBodyModel() {}

	void SoftBodyModel::UpdateGPUBuffer()
	{
		for (size_t i = 0; i < m_PhysicsPoints.size(); i++)
		{
			for (size_t j = 0; j < m_PointsAttach[i].size(); j++)
			{
				size_t vertexIndex = m_PointsAttach[i][j];
				m_Vertices[vertexIndex].position = m_PhysicsPoints[i];
			}
		}
		Core::VertexBufferManager::SetSubData(m_VertexBuffer, (float*)m_Vertices.data(), m_Vertices.size() * sizeof(Core::Vertex), 0);
	}
}