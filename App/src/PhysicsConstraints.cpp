#include "PhysicsConstraints.h"
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
}