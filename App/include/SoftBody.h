#pragma once

#include "GameObject.h"
#include "Core/Mesh.h"

namespace App
{
	class SoftBodyModel : public Core::Model
	{
	public:
		SoftBodyModel() {}
		SoftBodyModel(Core::Mesh mesh, std::vector<Core::Vertex> vertices, std::vector<std::vector<size_t>> pointsAttach, std::vector<glm::vec3> physicsPoints);
		~SoftBodyModel();

		std::vector<glm::vec3> m_PhysicsPoints;

		void UpdateGPUBuffer();

	private:
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