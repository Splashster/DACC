Command to build all files
/------------------------------------/
make

Commands to run program
/-----------------------------------/
make run (run automatically)
mpirun -n <number of processors> --hostfile <name of hostfile> barrier_mpi (run manually)

Command to delete files created by make
/------------------------------------/
make clean

Instructions
/--------------------------------------/
1. Build all of the files
2. Run the program

Answers To Assignment Questions
/-------------------------------------------/
1. Barrier implementations that use a mutex lock, use it because the program is implemented using a shared memory approach.
This approach is normally used in multi-threaded programs and requires a mutex lock in order to provide thread-safety and ensure that threads do not write over data being used by other threads. The barrier implemented in this program uses a distributed memory approach, which means that each process has its own allocated memory.
The barrier implementation used in this program does not require a mutex lock because changes made in memory by one processor will not be applied to other processors.

2. The MPI implementation of the barrier function is more scalable because it uses a tree structure approach.
In this approach, every node except the root node has a parent. When a child node reaches the barrier, it sends an arrival message to its parent.
Once every node has reached the barrier, the root node then sends a departure signal to its children nodes, and the children nodes send departure signals
to their children nodes (if applicable). This approach lessens the amount of communication happening between all of the nodes and the master node.
It should also speed up the time each node waits inside the barrier.


Notes
/---------------------------------------------/
The hostfile.txt contains the IP or hostname for all of the nodes being used
