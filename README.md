# ArcadeOS

ArcadeOS is a 32-bit bare-metal application that contains different playable arcade games  

*Technically, it is not an OS as it is lacking some specific features.*

## About the Project

### Application Specifications

~~~~```plaintext```
Designed for the x86 CPU Architecture  
Supports a maximum of 4 GB of RAM  
Flat-Memory Model  
Resolution is 640 x 480: 32 Bits per pixel (RGBA)
~~~~

### Limitations

~~~~```plaintext```
No User Mode Transitions (Ring Zero), Filesystem, Networking, Multiprocessing, nor Audio Support
~~~~

## Creating the Executable

**NOTE:** The Development Process has only been tested on Linux. Windows support is unknown

### Required Tooling

~~~~```plaintext```
gcc - Used for Compiling the C Code 
nasm - Used for Assembling any Assembly Files
dd - Used for Creating the Final Image
make - Used for Executing the Compilation Scripts
ld - Used for Linking 
objcopy - Used for Modifying the Compiled Files
~~~~

### Optional Tooling

~~~~```plaintext```
QEMU - Linux Virtual Machine that it known to be able to run everything
~~~~

### Process

There is a bash script that will compile and run everything for you within QEMU, assuming the tooling has already been downloaded.

Run:

~~~```bash```
./tests/testIMG.sh
~~~

Otherwise Follow the Directions Below:

~~~~```plaintext```
1. Download all Required Tooling
2. Clone the Repository
3. Run the command: 'make all'
4. The Final .img File will be located at build/arcadeOS.img
~~~~
