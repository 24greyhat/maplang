
# Design

- simple general purpose functional language


FEATURES:

- [X] list = [1,2,3] // arrays
- [X] objects = {key:value} // objects
- [X] functions
- [X] variables
- [] for loops
- [] while loops
- [] standard library


# TODO


- Standard Library Functions

 - [] IO (print, printFatal, readFile, writeFile, appendFile, readStruct, writeStruct) using stdio and stdlib

 - [] NET (tcpListen, udpListen, tcpConnect, udpConnect, netSend, netRecv, httpGet, httpPost, httpDelete, ....) using zmq to create all network functionality
    
    - tcpListen = listen on specified tcp port
    - udpListen = listen on specified udp port
    - tcpConnect = connects to a given host and port via tcp
    - udpConnect = connects to a given host and port via udp
    - netRcv = receives data from a given connection
    - netSend = sends data to a connection
    - httpGet = sends a get request
    - httpPost = sends a post request
    - httpDelete = sends a delete request


 - [] MapGuiKit (newScreen, registerScreen, ...) using opengl to build a gui toolkit that allows for gui apps to be written in maplang and with a modern elegant array of clean ui components.
    
    - newScreen = create a new screen

    - renderScreen = pass a directory path that has an index.html file


 - [] OS (exec, uname, execve, etc...) using given kernel syscalls

    - os commands support

 - [] COMPILER (compile) essentially package the maplang interpreter and compile the ast of the source code into maplang binaries and compile the c file into an executable using gcc.

    - compile = takes the path of the .mpl file and will output a .exe or .out file with all the 

    - Note: compiled maplang apps tend to be a little larger than a pure c app because maplang apps are compiled along with the interpreter.


- [66%] Implement Parser

    - [X] implement io readFile function

    - [X] implement Lexer

    - [X] implement ast

    - [X] implement parser

    - [] implement "import" support

    - [] implement "while" loops support

    - [] implement "for" loops support




- [.] Implement Interpreter 

    - [X] Implement memory.h

    - [X] Implement bytecode.h

    - [.] Implement exception.h

    - [.] Implement interpreter.h



- [] Implement Stdlib

    - [] Implement bridge.h




















 - [] Implement MapGuiKit

    - Note: after successfully implementing import, while and for loops and getting code execution, start the development of the MapGuiKit


    - [] create a small opengl app to learn about the Library

    - [] Implement the VStack Component

    - [] Implement the HStack Component

    - [] Implement the MapGuiKit navigation structs and methods

    - [] Implement the Button Component

    - [] Implement the Input Component


 - Note: since MapGuiKit is an independent c gui kit publish it on github with it's own directory.

 - MapGuiKit is an elegant modern pure c Gui Development Kit built on top of opengl



