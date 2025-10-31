#pragma once

namespace Core {

    class Layer
    {
    public:
        virtual ~Layer() = default;
        
        virtual void OnUpdate(double ts) {}
        virtual void OnRender() {}
    };
}