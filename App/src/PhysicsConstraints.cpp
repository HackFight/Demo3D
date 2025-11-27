#include "PhysicsConstraints.h"
#include "glm/geometric.hpp"
#include <vector>

namespace App
{
    void GroundConstraint::Solve(std::vector<PointMass>& particles, float a, float ts)
    {
        for(size_t index : m_Indices)
        {
            PointMass& particle = particles.at(index);
            float C = particle.position.y - m_GroundHeight;

            if(C < 0)
            {
                float lambda = -C/(particle.inverseMass+(a/ts*ts));
                particle.position.y += lambda * particle.inverseMass;
            }
        }
    }

    void DistanceConstraint::Solve(std::vector<PointMass>& particles, float a, float ts)
    {
        glm::vec3 dif = particles.at(m_Indices.at(1)).position - particles.at(m_Indices.at(0)).position;
        float l = glm::length(dif);
        dif = (l-m_restDistance)*(dif/l);

        float w1 = particles.at(m_Indices.at(0)).inverseMass;
        float w2 = particles.at(m_Indices.at(1)).inverseMass;

        particles.at(m_Indices.at(0)).position += dif * (w1/(w1+w2+(a/ts*ts)));
        particles.at(m_Indices.at(1)).position += dif * -(w2/(w1+w2+(a/ts*ts)));
    }
}