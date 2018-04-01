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

Answers for the questions
/-------------------------------------------/
1. Because the barrier implementation used in the assignment is implemented using a distributed memory approach.
Barrier implementations that use a mutex lock, use it because the program is implemented using a shared memory approach.
This approach is normally used in multi-threaded programs. Since this program is implemented using a distributed memory approach,
it does not require a mutex lock because changes made in memory by one processor will not be applied to other processors.

2.
https://6xq.net/barrier-intro/
less messages being sent
The MPI implementation of the barrier function is more scalable because it uses a tree structure approach.
In this approach, every node except the root node has a parent. When a child node reaches the barrier, it sends an arrival message to its parent.
Once every node has reached the barrier, the root node then sends a departure signal to its children nodes and the children nodes send departure signals
to their children nodes (if applicable). This approach is more scalable because the root node is less likely
to become overwhelmed with arrival messages as the nodes reach the barrier.
