#pragma once
#include <glm/glm.hpp>
#include <vector>


namespace App
{
    struct PointMass
	{
		glm::vec3 position;
		glm::vec3 velocity;
		float inverseMass;
	};

	class Constraint
	{
	public:
		Constraint(std::vector<size_t> indices) : m_Indices(indices) {}
		virtual ~Constraint() = default;

		virtual void Solve(std::vector<PointMass>& particles, double ts) {}

		std::vector<size_t> m_Indices;
	};

	class GroundConstraint : public Constraint
	{
    public:
		GroundConstraint(std::vector<size_t> indices, float groundHeight = 0.0f)
			: Constraint(indices), m_GroundHeight(groundHeight) {}

        void Solve(std::vector<PointMass>& particles, double ts) override;

    private:
        float m_GroundHeight;
	};

	class DistanceConstraint : public Constraint
	{
	public:
		DistanceConstraint(std::vector<size_t> indices, float restDistance)
			: Constraint(indices), m_restDistance(restDistance) {}

		void Solve(std::vector<PointMass>& particles, double ts) override;

	private:
		float m_restDistance;
	};
}