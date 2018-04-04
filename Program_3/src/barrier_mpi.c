/************************************************************************
The barrier_mpi program writes and reads from a file using the MPI library.
Processor 0 is the only processor that writes to the file.
While processor 0 writes to the file, the other processors wait at the barrier
until all of the processors have reached the barrier.
Once all of the processors have reached the barrier, the processors then read
the last contents that were written to the file.
This is done 10 times.
*************************************************************************/
#include <mpi.h>
#include <stdio.h>

typedef struct barrier_t{
  int count;
  int flag;
  int processors;
  int current_processor;
}barrier_t;

#define MY_INCREMENT 1
#define MY_INCREMENT_REPLY 2
#define MY_RESET 3
#define FILE_MSG_SIZE 2000

void my_barrier_init(barrier_t *barrier){
  //Set count to 0
  barrier->count = 0;

  //Set flag to 0
  barrier->flag = 0;

  //Get the number of processors
  MPI_Comm_size(MPI_COMM_WORLD, &barrier->processors);

  //Get the current processor rank
  MPI_Comm_rank(MPI_COMM_WORLD, &barrier->current_processor);
}

//Blocks all processors from continuing until the last processor has reached the barrier
void my_barrier(barrier_t *barrier){
   int local_sense;
   int my_items[3];
   int msg[3];
   int arrived = 0;
   int i;
   local_sense = 1 - barrier->flag;

   if(barrier->current_processor != 0){
     my_items[0] = MY_INCREMENT;
     my_items[1] = barrier->count;
     my_items[2] = barrier->current_processor;
     MPI_Send(&my_items, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
   }else{
      barrier->count++;
   }

   while(barrier->flag != local_sense){
       MPI_Recv(&msg, 3, MPI_INT, MPI_ANY_SOURCE, 0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);

       if(msg[0] == MY_INCREMENT_REPLY){
         arrived = msg[1];

         if(arrived == barrier->processors){
           my_items[0] = MY_RESET;
           my_items[1] = local_sense;
           my_items[2] =  barrier->current_processor;
           for(i = 0; i < barrier->processors; i++){
             if(i != barrier->current_processor){
               MPI_Send(&my_items, 3, MPI_INT, i, 0, MPI_COMM_WORLD);
             }
           }
           barrier->count = 0;
           barrier->flag = local_sense;
         }
         }else if(msg[0] == MY_INCREMENT){
           barrier->count += 1;
           my_items[0] = MY_INCREMENT_REPLY;
           my_items[1] = barrier->count;
           my_items[2] = barrier->current_processor;
           MPI_Send(&my_items, 3, MPI_INT, msg[2], 0, MPI_COMM_WORLD);
         }else if(msg[0] == MY_RESET){
           barrier->count = 0;
           barrier->flag = msg[1];
         }

       }
 }



int main(int argc, char** argv) {

    MPI_Init(NULL, NULL);
    int i;
    barrier_t barrier;
    FILE *file;
    char msg[FILE_MSG_SIZE];

    my_barrier_init(&barrier);

    for(i=0; i < 10; i++){
      if(barrier.current_processor == 0){
        file = fopen("tstfile.txt", "a");
        if(file!=NULL){
          sprintf(msg, "Greetings from Processor: %i the current run is: %i\n", barrier.current_processor, i);
          fputs(msg, file);
          fclose(file);
        }
      }else{
        sleep(1);
      }
      my_barrier(&barrier);
      file = fopen("tstfile.txt", "r");
      if(file!=NULL){
        while(fgets(msg, 2000, file) != NULL){}
        printf("Processor %i reads: %s",barrier.current_processor,msg);
        fclose(file);
      }
      my_barrier(&barrier);
      if(barrier.current_processor == 0){
        printf("\n");
      }

    }

    MPI_Finalize();
    return 0;
}
