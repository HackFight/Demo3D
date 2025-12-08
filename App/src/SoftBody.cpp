#include "SoftBody.h"
#include "PhysicsConstraints.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <vector>

namespace App
{
	SoftBodyModel::SoftBodyModel(std::vector<Core::Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::vector<size_t>> pointsAttach, std::vector<PointMass> pointMasses, std::vector<std::shared_ptr<Constraint>> constraints)
		: Core::Model(Core::Mesh(vertices, indices, std::vector<Core::Mesh::Texture>{}, false)),
		m_VertexBuffer(meshes.at(0).GetVertexBuffer()),
		m_Vertices(vertices),
		m_Indices(indices),
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
					constraint->Solve(m_PointMasses, 5000.0f, ts);
				}
			}

			for(int j = 0; j < m_PointMasses.size(); j++)
			{
				m_PointMasses.at(j).velocity = (m_PointMasses.at(j).position - oldPositions.at(j))/(float)ts;
			}
		}
	}

	void SoftBodyModel::UpdateGPUBuffer()
	{
		for(size_t i = 0; i < m_PointMasses.size(); i++)
		{
			for (size_t j = 0; j < m_PointsAttach.at(i).size(); j++)
			{
				size_t vertexIndex = m_PointsAttach.at(i).at(j);
				m_Vertices.at(vertexIndex).position = m_PointMasses.at(i).position;
			}
		}
		
		std::vector<std::vector<glm::vec3>> normsList;
		for(int i = 0; i < m_Vertices.size(); i++) {
			normsList.push_back(std::vector<glm::vec3>());
		}
		for(int i = 0; i < m_Indices.size(); i+=3) {
			glm::vec3 normal = glm::normalize(glm::cross(m_Vertices.at(m_Indices.at(i+1)).position - m_Vertices.at(m_Indices.at(i)).position, m_Vertices.at(m_Indices.at(i+2)).position - m_Vertices.at(m_Indices.at(i)).position));
			normsList.at(m_Indices.at(i)).push_back(normal);
			normsList.at(m_Indices.at(i+1)).push_back(normal);
			normsList.at(m_Indices.at(i+2)).push_back(normal);
		}
		for(int i = 0; i < normsList.size(); i++) {
			glm::vec3 total(0.0f);
			for(glm::vec3 vec : normsList.at(i)) {
				total += vec;
			}
			m_Vertices.at(i).normal = total;
		}

		Core::VertexBufferManager::SetSubData(m_VertexBuffer, (float*)m_Vertices.data(), m_Vertices.size() * sizeof(Core::Vertex), 0);
	}

	SoftBody::SoftBody(SoftBodyModel model, uint32_t shader, glm::vec3 position, BlinnPhongMaterial material)
		: GameObject(model, shader, position, material),
		m_Model(model) {}
	SoftBody::~SoftBody() {}
}