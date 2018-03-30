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

2. The MPI implementation has the ability to handle groups of processors whereas the assignment's implementation assumes
that every processor will be in the same group. Processors
