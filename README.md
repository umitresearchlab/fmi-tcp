fmi-co-simulation       {#mainpage}
=================

Implementation of FMI over TCP. Includes two applications: a master and a slave. The applications are built on top of [FMILibrary](http://www.jmodelica.org/FMILibrary) from [jModelica](http://www.jmodelica.org) and [Lacewing](http://lacewing-project.org/).

### Typical usage

1. Host starts the Master server.
2. Client starts a Slave, which connects to the master.
3. Simulation is run.

### Build
The build was done successfully on Ubuntu Linux 12.10 & Windows 7.

Begin with installing [FMILibrary](http://www.jmodelica.org/FMILibrary) & [Lacewing](http://lacewing-project.org/). Make sure the libraries and include files ends up in /usr/lib and /usr/include.
Note that code is forced to link against the static lib of Lacewing.

Build and install using the following commands. You'll need [CMake](http://www.cmake.org/).

    cd path/to/fmi-co-simulation;
    mkdir build;
    cd build;
    cmake ..;
    make;
    sudo make install;

If the libraries and include files are not in /usr/lib and /usr/include then use the FMIL_INCLUDE_DIR & LACEWING_INCLUDE_DIR variables to specify the include files location and FMIL_LIBS_DIR & LACEWING_LIBS_DIR variables to specify the libraries location.

    cmake .. -DFMIL_INCLUDE_DIR=<FMIL include files location> -DLACEWING_INCLUDE_DIR=<Lacewing include files location> -DFMIL_LIBS_DIR=<FMIL libraries location> -DLACEWING_LIBS_DIR=<Lacewing libraries location>

Now you should be able to run ```fmi-master``` & ```fmi-slave``` from the command line. Run them without arguments to view the help page.

