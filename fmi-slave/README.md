fmi-slave
==========

### Manual page
```
FMI-SLAVE(1)                                                    FMI-SLAVE(1)

NAME
       fmi-slave

DESCRIPTION
       fmi-slave simulates FMU using the FMILibrary from JModelica.org

FLAGS
       -h     Show help and quit.

       -l     Show FMILibrary logs.
       
       -dl    Enables the FMU debug logging flag.

       -v     Show version and quit.

OPTIONS
       -m [METHOD]
              Stepping  method. Available methods are "jacobi" (Jacobi, paral‐
              lel) and "gs" (Gauss-Seidel, serial). Default is "jacobi".
              
       -port [PORT]
              The server port to connect.
              
       -host [HOSTNAME]
              The server host name to connect.

EXAMPLES
       To run an FMU simulation from time 0 to 5 with timestep 0.01:
           fmi-slave -port=10200 ../myFMU.fmu

ABOUT
       The app was built by Stefan Hedman at UMIT Research Lab 2013.

                                     local                       FMI-SLAVE(1)
```
