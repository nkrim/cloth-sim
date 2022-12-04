# Cloth Simulation

A physics-driven 3D cloth simulation built with C++ and SFML.

Uses Verlet integration with positional contstraints in order to efficiently simulate intra-cloth spring forces while preventing explosive growth due to numerical instability and extreme inputs from the user.

Supports mouse interactions with the cloth such as pulling, pinning, and deleting vertices. Also includes a variety of customizable fields in the UI to adjust the appearance of the cloth, the arrangement of pinned vertices, and parameters for the simulation such as node count, iterations, and external forces. Also supports loading images to use as the cloth texture as well as saving the current state of the simulation as an image (only works on OSX and was not working when the video was recorded).

# How to Build
- Install SFML and move all of its headers, libs, and frameworks to their appropriate system paths (only tested with MacOS)
- Run `make` in the project directory
- Run `./clothsim.out`

# Notes
- Removed image saving and loading because the included library was too platform-inconsistent 
- The OpenGL headers are only tested to work with MacOS. They may with work with Linux, but they are unlikely to work for Windows

# Video
Video presentation: https://www.youtube.com/watch?v=4SW5vj9Ksc8  
(recorded on a 2014 macbook so it had a hard time running the simulation and recording the screen simoultaneously)  

![Preview Image](/cloth-sim-readme-img.png)
