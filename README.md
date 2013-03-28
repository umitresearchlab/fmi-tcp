fmi-master
==========

Command line interface for doing co-simulation using FMUs. Built on top of [FMILibrary](http://www.jmodelica.org/FMILibrary) from [jModelica](http://www.jmodelica.org).

### Build
The build was done successfully on Ubuntu Linux 12.10.

Begin with installing [FMILibrary](http://www.jmodelica.org/FMILibrary). Make sure the libraries and include files ends up in /usr/lib and /usr/include (or whatever suits you best).

Build and install the master using the following commands. You'll need [CMake](http://www.cmake.org/).
```
cd path/to/fmi-master;
mkdir build;
cd build;
cmake ..;
make;
sudo make install;
```

Now you should be able to run ```fmi-master``` from the command line. Run it without arguments to view the help page.

### Manual page
```
FMI-MASTER(1)                                                    FMI-MASTER(1)

NAME
       fmi-master - simulate FMUs

SYNOPSIS
       fmi-master [ -hlqrvcdfmopst ] [ files ...  ]

DESCRIPTION
       fmi-master simulates FMUs using the FMILibrary from JModelica.org

FLAGS
       -h     Show help and quit.

       -l     Show FMILibrary logs.

       -q     Quiet  mode,  do  not print simulation info to STDOUT. This flag
              should be used together with -r.

       -r     Real time mode. Will usleep() in between communication steps  if
              the execution was faster than real time.

       -v     Show version and quit.

       -x     Print XML and quit.

OPTIONS
       -c [CONNECTIONS]
              Connection  specification. No connections by default. Quadruples
              of positive integers, representing which FMU and value reference
              to connect from and what to connect to. Syntax is

                  CONN1:CONN2:CONN3...

              where     CONNX     is     four     comma-separated    integers;
              FMUFROM,VRFROM,FMUTO,VRTO.   An  example  connection  string  is
              0,0,1,0:0,1,1,1 which means: connect FMU0 (value reference 0) to
              FMU1 (vr 0) and FMU0 (vr 1) to FMU1 (vr 1).  Default is no  con‐
              nections.

       -d [TIMESTEP]
              Timestep size. Default is 0.1.

       -f [OUTFORMAT]
              Output file format. Currently only "csv" is supported, and it is
              also the default.

       -g [STEPPING ORDER SPEC]
              FMU stepping order. Used in serial algorithms. Should be a comma
              separated  list  of FMU indices. If the list starts with 0, then
              the first FMU will be stepped, and  its  utput  values  will  be
              transferred through their connections (if any).

       -m [METHOD]
              Stepping  method. Available methods are "jacobi" (Jacobi, paral‐
              lel) and "gs" (Gauss-Seidel, serial). Default is "jacobi".
```
