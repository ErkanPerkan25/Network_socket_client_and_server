# Project4-ComputerSystems2
Purpose: <br>
• build a network socket based server in a Linux system <br>
• build a network socket based client in a Linux system

Author: Eric Hansson

- What platform you developed your solution on: <br>
Operating System: Arch Linux <br>
Text Editor: Neovim

- any special details on how to compile your project: <br>
Used a Makefile which creates two executables, one called `client`, and the other called `server`. This is done by running this command:
```
make
```
- any special details on how to run your project: <br>
To run the client program run the executable with passing a IP address and port number:
```
./client <ip-address> <port-num>
```
For the server program you run the executable passing the port number:
```
./server <port-num>
```
- any known bugs your project has, and possible fixes to those bugs: <br>
There are no known bugs with the programs.

- an overview of how you solved the project: <br>
For each step I took in this project I would hand-trace every detail to get the hang of what is going on in the program. And then I would slowly start to 
implement piece by piece. I would test each little piece to see if it would work, and then move on if it did.
Did the client send something to the server? Did the server read it? And with that I could easily fix issues that would come up.
