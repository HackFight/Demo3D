# Demo 3D

The goal of this project is to build a professional (looking, at least) app featuring:
- An App/Core project structure
- AppLayers
- Modules for 3D rendering starting with OpenGL
- 3D SoftBody and RigidBody physics with XPBD

<img width="2878" height="1798" alt="image" src="https://github.com/user-attachments/assets/edacaf2a-0cd3-4b77-ab59-5c405a634b21" />

## Installing & Compiling
1. Clone this repo recursively `git clone --recurse-submodules https://github.com/HackFight/Demo3D.git`
2. Go in the folder `cd Demo3D`
3. Generate CMake files `cmake .`
4. Build with CMake `cmake --build .`. The first build will take some time.
5. Run one of the demos `./App/App`, `./PhysicsTest/PhysicsTest` or `./Platformer/Platformer`
