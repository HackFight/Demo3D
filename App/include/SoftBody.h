#pragma once

#include "GameObject.h"
#include "PhysicsConstraints.h"
#include "Core/Mesh.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace App
{
	class SoftBodyModel : public Core::Model
	{
	public:
		SoftBodyModel() {}
		SoftBodyModel(std::vector<Core::Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::vector<size_t>> pointsAttach, std::vector<PointMass> pointMasses, std::vector<std::shared_ptr<Constraint>> constraints);
		~SoftBodyModel();

		std::vector<PointMass> m_PointMasses;
		std::vector<std::shared_ptr<Constraint>> m_Constraints;

		void Update(double ts, int substeps = 1, int iterations = 1);
		void UpdateGPUBuffer();

	private:
		glm::vec3 m_Gravity;
		uint32_t m_VertexBuffer;
		std::vector<Core::Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<std::vector<size_t>> m_PointsAttach; //Points attach are here to link multiple vertices to a single physics point
	};

	class SoftBody : public GameObject
	{
	public:
		SoftBody(SoftBodyModel model = SoftBodyModel(), uint32_t shader = 0, glm::vec3 position = glm::vec3(0.0f), BlinnPhongMaterial material = WhitePlastic);
		~SoftBody();

		SoftBodyModel m_Model;
	};
}