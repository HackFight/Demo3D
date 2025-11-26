#pragma once

#include "GameObject.h"
#include "PhysicsConstraints.h"
#include "Core/Mesh.h"
#include <glm/glm.hpp>
#include <vector>

namespace App
{
	class SoftBodyModel : public Core::Model
	{
	public:
		SoftBodyModel() {}
		SoftBodyModel(Core::Mesh mesh, std::vector<Core::Vertex> vertices, std::vector<std::vector<size_t>> pointsAttach, std::vector<PointMass> pointMasses, std::vector<Constraint> constraints);
		~SoftBodyModel();

		std::vector<PointMass> m_PointMasses;
		std::vector<Constraint> m_Constraints;

		void Update(double ts);
		void UpdateGPUBuffer();

	private:
		glm::vec3 m_Gravity;
		uint32_t m_VertexBuffer;
		std::vector<Core::Vertex> m_Vertices;
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