fmi-co-simulation
==========

Command line interface for doing co-simulation using FMUs. Built on top of [FMILibrary](http://www.jmodelica.org/FMILibrary) from [jModelica](http://www.jmodelica.org).

### Build
The build was done successfully on Ubuntu Linux 12.10.

Begin with installing [FMILibrary](http://www.jmodelica.org/FMILibrary). Make sure the libraries and include files ends up in /usr/lib and /usr/include.

Build and install the master using the following commands. You'll need [CMake](http://www.cmake.org/).
```
cd path/to/fmi-master;
mkdir build;
cd build;
cmake ..;
make;
sudo make install;
```

If the libraries and include files are not in /usr/lib and /usr/include then use the INCLUDE_PATHS variable to specify the include files location and LINK_DIRECTORIES variable to specify the libraries location.
```
cmake .. -DINCLUDE_PATHS=<include files location> -DLIBS_PATHS=<libraries location>
```

Now you should be able to run ```fmi-master``` & ```fmi-slave``` from the command line. Run them without arguments to view the help page.

ABOUT
       The app was built by Stefan Hedman at UMIT Research Lab 2013.

                                     local                       FMI-CO-SIMULATION(1)
```
