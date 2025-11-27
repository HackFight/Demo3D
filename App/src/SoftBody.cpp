#include "SoftBody.h"
#include "PhysicsConstraints.h"
#include <vector>

namespace App
{
	SoftBodyModel::SoftBodyModel(Core::Mesh mesh, std::vector<Core::Vertex> vertices, std::vector<std::vector<size_t>> pointsAttach, std::vector<PointMass> pointMasses, std::vector<std::shared_ptr<Constraint>> constraints)
		: Core::Model(mesh),
		m_VertexBuffer(mesh.GetVertexBuffer()),
		m_Vertices(vertices),
		m_PointsAttach(pointsAttach),
		m_PointMasses(pointMasses),
		m_Constraints(constraints),
		m_Gravity({0.0f, -9.805f, 0.0f})
	{}
	SoftBodyModel::~SoftBodyModel() {}

	void SoftBodyModel::Update(double ts, int substeps, int iterations)
	{
		ts /= substeps;
		for(int i = 0; i < substeps; i++)
		{
			std::vector<glm::vec3> oldPositions;
			for(PointMass& particle : m_PointMasses)
			{
				particle.velocity += m_Gravity * (float)ts;
				oldPositions.push_back(particle.position);
				particle.position += particle.velocity * (float)ts;
			}

			for(int j = 0; j < iterations; j++)
			{
				for(const std::shared_ptr<Constraint>& constraint : m_Constraints)
				{
					constraint->Solve(m_PointMasses, 1000.0f, ts);
				}
			}

			for(int j = 0; j < m_PointMasses.size(); j++)
			{
				m_PointMasses[j].velocity = (m_PointMasses[j].position - oldPositions[j])/(float)ts;
			}
		}
	}

	void SoftBodyModel::UpdateGPUBuffer()
	{
		for (size_t i = 0; i < m_PointMasses.size(); i++)
		{
			for (size_t j = 0; j < m_PointsAttach[i].size(); j++)
			{
				size_t vertexIndex = m_PointsAttach[i][j];
				m_Vertices[vertexIndex].position = m_PointMasses[i].position;
			}
		}
		Core::VertexBufferManager::SetSubData(m_VertexBuffer, (float*)m_Vertices.data(), m_Vertices.size() * sizeof(Core::Vertex), 0);
	}

	SoftBody::SoftBody(SoftBodyModel model, uint32_t shader, glm::vec3 position, BlinnPhongMaterial material)
		: GameObject(model, shader, position, material),
		m_Model(model)
	{
	}
	SoftBody::~SoftBody() {}
}