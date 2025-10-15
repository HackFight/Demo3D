#pragma once

namespace Core {

    class Layer
    {
    public:
        virtual ~Layer() = default;
        
        virtual void OnUpdate(float ts) {}
        virtual void OnRender() {}
    };
}