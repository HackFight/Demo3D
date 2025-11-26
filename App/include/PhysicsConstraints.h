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
		Constraint(std::vector<PointMass*> particles) : m_Particles(particles) {}
		~Constraint() {}

		virtual void Solve(double ts) {}

		std::vector<PointMass*> m_Particles;
	};

	class GroundConstraint : public Constraint
	{
    public:
		GroundConstraint(std::vector<PointMass*> particles, float groundHeight = 0.0f) : Constraint(particles), m_GroundHeight(groundHeight) {}
		~GroundConstraint() {}

        void Solve(double ts);

    private:
        float m_GroundHeight;
	};
}