#include "PhysicsConstraints.h"
#include "glm/geometric.hpp"
#include <vector>

namespace App
{
    void GroundConstraint::Solve(std::vector<PointMass>& particles, double ts)
    {
        for(size_t index : m_Indices)
        {
            if(particles.at(index).position.y < m_GroundHeight)
            {
                particles.at(index).position.y = m_GroundHeight;
            }
        }
    }

    void DistanceConstraint::Solve(std::vector<PointMass>& particles, double ts)
    {
        glm::vec3 dif = particles.at(m_Indices.at(1)).position - particles.at(m_Indices.at(0)).position;
        float l = glm::length(dif);
        dif = (l-m_restDistance)*(dif/l);

        float w1 = particles.at(m_Indices.at(0)).inverseMass;
        float w2 = particles.at(m_Indices.at(1)).inverseMass;

        particles.at(m_Indices.at(0)).position += dif * (w1/(w1+w2));
        particles.at(m_Indices.at(1)).position += dif * -(w2/(w1+w2));
    }
}