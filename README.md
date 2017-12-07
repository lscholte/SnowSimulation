# SnowSimulation
A real-time simulation of a snowfall and accumulation of snow onto a surface

# How to Build and Run
First clone the repository. Remember to add the recursive option as there is a submodule that must also be cloned.

`git clone --recursive https://github.com/lscholte/SnowSimulation.git`

If you forget to add the recursive option, then execute the following from within your local copy of the repository:

`git submodule update --init --recursive`

Next, create an empty build directory wherever you like. It is suggested to create this directory outside of your local copy of the repository. Once created, navigate to the directory and execute

`cmake <path to root of repository>`

`cmake --build .`

If the build was successful, then you will have an executable named `snowSimulation` in your build directory. Execute it with

`./snowSimulation`

# Demo
See <https://www.youtube.com/watch?v=W3_5ML0GOos> for a video demo of the snow accumulation
