🌌 Solar System Simulation (OpenGL)

A real-time 3D Solar System simulation written in C++ using OpenGL, visualizing planetary orbits and axial rotations through time-scaled animation.
The simulation models hierarchical celestial motion (Sun → Planets → Moons) and provides interactive camera controls for real-time exploration.

🚀 Key Highlights

Real-time rendering with OpenGL
Time-scaled planetary revolution and self-rotation
Hierarchical transformation system
Interactive camera controls
Configurable simulation speed

🎮 Controls
Key	Action
+ / -	Increase / decrease simulation speed
W / S	Zoom camera in / out
A / D	Rotate camera left / right
↑ / ↓	Tilt camera up / down
R	Reset camera and simulation
Esc	Exit application

Controls are implemented using GLUT keyboard callbacks and can be easily remapped or extended.

🧠 Simulation Model

Each celestial body is defined by orbital period, rotation period, relative size, and orbital distance.
Motion is time-driven and scaled to preserve correct relative behavior while maintaining visual stability.

🛠️ Tech Stack

Language: C++
Graphics API: OpenGL (GL / GLU / GLUT)
Platform: Windows
Build: GCC / MinGW or MSVC


▶️ Build & Run

Requirements:
- Windows
- OpenGL
- FreeGLUT / GLUT

Compile (MinGW)
g++ solar.cpp -o solar -lopengl32 -lglu32 -lfreeglut


📈 Possible Extensions

Modern OpenGL (VBO/VAO + shaders)
Texture-mapped planets
Physically accurate orbital dynamics
Cross-platform support

👤 Author
MD Tahmid Hamim
