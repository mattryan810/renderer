# renderer
This is my first C++ project!

The purpose of this was to develop a simple 3D graphics renderer using only basic 2D grahics packages. I set myself the challenge of doing all the conceptual stuff and math myself, only using external resources to help me learn to code.

I like drawing and specifically focus a lot on perspective. Since I'm a math major and wanted to learn C++ anyway, this seemed like an interesting project. The math was fairly simple - most of which you can see in my notes - but implementing it in C without knowing the language at all was not. I accessed several resources to help me learn how to write in C++ and figure out how to implement the math in my code. I used the SFML and Eigen documentation and my savior - ChatGPT. I did not directly copy any code without writing it myself and ensuring I understood as best I could what I was writing. I used it mostly for suggestions (how to approach problems, best practices, and which commands to use) and revisions/explaining why stuff didn't work.

There are a few things I would do differently if I had to restart and a few bugs I would fix if this was more than just for fun:
I would:
  - Alter some classes to simplify, maybe combine the camera and renderer
  - Maybe add a vertex class to carry more info and do computations internally, thus simplifying cube and shape
  - Combine project and show functions
  - Find a better way to do the movement controls
  - Reconsider how I package the vectors/matrices and do computations on them
    - Specifically use more elementwise operations instead of weird for loops and stuff
  - Do the math/inplement a projection onto a flat area instead of just a polar projection
  Bugs:
  - Fix project function such that moving backwards and forwards doesn't seem to go up and down also
  - Fix project so when the object is moved off to the side of the screen, it actually disappears

This was compiled for MacOS and so from what I've read, the .exe may not work on Windows. To recompile, Eigen and SFML need to be linked, but otherwise it should work!
