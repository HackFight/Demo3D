#include "PhysicsConstraints.h"

namespace App
{
    void GroundConstraint::Solve(double ts)
    {
        for(PointMass* particle : m_Particles)
        {
            if(particle->position.y < m_GroundHeight)
            {
                particle->position.y = m_GroundHeight;
            }
        }
    }
}