fmi-master
==========

Command line interface for doing co-simulation using FMUs. Built on top of [FMILibrary](http://www.jmodelica.org/FMILibrary) from [jModelica](http://www.jmodelica.org).

### Build
The build was done successfully on Ubuntu Linux 12.10.

Begin with installing [FMILibrary](http://www.jmodelica.org/FMILibrary). Make sure the libraries and include files ends up in /usr/lib and /usr/include (or whatever suits you best).

Build and install the master using the following commands. You'll need [CMake](http://www.cmake.org/).
```
cd path/to/fmilib-master;
mkdir build;
cd build;
cmake ..;
make;
sudo make install;
```

Now you should be able to run ```fmi-master``` from the command line. Run it without arguments to view the help page.

### Manual page
```
fmi-master(1)                                                    fmi-master(1)

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
              Outout file format. Currently only "csv" is supported, and it is
              also the default.

       -m [METHOD]
              Stepping method. Currently only "jacobi" is supported, and it is
              also the default.

       -o [OUTFILE]
              Result  output  file. If set to "stdout", then output will be to
              STDOUT. Default value is "result.csv".

       -p [PARAMS]
              Parameter specification. Triplets separated by :, specifying FMU
              index,  value  reference index and parameter value. Example set‐
              ting   the   three   first   value   references   of   FMU    0:
              "0,0,12.3:0,1,true:0,2,9". Default is no parameters.

       -s [SEPARATOR]
              CSV separator character. Default is comma (,).

       -t [ENDTIME]
              End simulation time in seconds. Default is 1.0.

EXAMPLES
       To run an FMU simulation from time 0 to 5 with timestep 0.01:
           fmi-master -t 5 -d 0.01 ../myFMU.fmu

       To  simulate  two FMUs connected from the first output of the first FMU
       to the first input of the second:
           fmi-master -c 0,0,1,0 a.fmu b.fmu

       To simulate quietly (without output to STDOUT) and save the results  to
       file:
           fmi-master -q -o out.csv a.fmu

ABOUT
       The app was built by Stefan Hedman at UMIT Research Lab 2013.

                                     local                       fmi-master(1)
```
