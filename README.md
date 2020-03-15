# Virtual Memory
This app is implemented in C and it is a small representation of the System’s Memory Management behavior.
   The simulator consists of three processes (PM1,PM2,Memory Manager). Each of the two processes (PM1,PM2), opens the file that corresponds to it, reads the hexadecimal  memory references and forms requests forwarded (IPC) to the third process (manager
memory, Memory Manager).Memory Manager implements the virtual memory mechanism based on one Hashed page table.
