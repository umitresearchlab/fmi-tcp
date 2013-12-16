fmi-master
==========

### Manual page
```
FMI-MASTER(1)                                                    FMI-MASTER(1)

NAME
       fmi-master

DESCRIPTION
       A TCP server to simulate connected FMUs.

FLAGS
       -h     Show help and quit.

OPTIONS
       -port [PORT]
              The port to run the server on.
       -host [HOSTNAME]
              The host name to run the server on.
       -n [numFMUS]
              Number of FMUs to connect.
       -d [TIMESTEP]
              Timestep size. Default is 0.1.
       -t [ENDTIME]
              End simulation time in seconds. Default is 1.0.
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
              
EXAMPLES
       To simulate two FMUs connected from the first output of the  first  FMU
       to the first input of the second:
           fmi-master -n=2 -c=0,0,1,0:0,1,1,1

ABOUT
       The app was built by Stefan Hedman at UMIT Research Lab 2013.

                                     local                       FMI-MASTER(1)
```
