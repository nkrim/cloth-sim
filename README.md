# Cloth Simulation

A physics-driven 3D cloth simulation built with C++ and SFML.

Uses Verlet integration with positional contstraints in order to efficiently simulate intra-cloth spring forces while preventing explosive growth due to numerical instability and extreme inputs from the user.

Supports mouse interactions with the cloth such as pulling, pinning, and deleting vertices. Also includes a variety of customizable fields in the UI to adjust the appearance of the cloth, the arrangement of pinned vertices, and parameters for the simulation such as node count, iterations, and external forces.

Video presentation: https://www.youtube.com/watch?v=4SW5vj9Ksc8  
(recorded on a 2014 macbook so it had a hard time running the simulation and recording the screen simoultaneously)

![Preview Image](/cloth-sim-readme-img.png)
