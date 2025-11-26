#pragma once
#include <glm/glm.hpp>
#include <vector>


namespace App
{
    struct PointMass
	{
		glm::vec3 position;
		glm::vec3 velocity;
	};

	class Constraint
	{
	public:
		Constraint(std::vector<size_t> indices) : m_Indices(indices) {}
		~Constraint() {}

		virtual void Solve(std::vector<PointMass>& particles, double ts) {}

		std::vector<size_t> m_Indices;
	};

	class GroundConstraint : public Constraint
	{
    public:
		GroundConstraint(std::vector<size_t> indices, float groundHeight = 0.0f)
			: Constraint(indices), m_GroundHeight(groundHeight) {}
		~GroundConstraint() {}

        void Solve(std::vector<PointMass>& particles, double ts);

    private:
        float m_GroundHeight;
	};
}