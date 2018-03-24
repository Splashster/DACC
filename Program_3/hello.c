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

   //printf("Processor: %i Current msg2: %i\n", barrier->current_processor, msg[0]);

   while(barrier->flag != local_sense){
       MPI_Recv(&msg, 3, MPI_INT, MPI_ANY_SOURCE, 0 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // printf("Process: %i received from: %i message: %i\n", barrier->current_processor, msg[2], msg[0]);
       if(msg[0] == MY_INCREMENT_REPLY){
         arrived = msg[1];
         //printf("I've arrived: %i\n", arrived);
         if(arrived == barrier->processors){
          // printf("Time to reset\n");
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
           //printf("Inc me by: %i\n", msg[2]);
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
       printf("Processor: %i Flag is: %i sense is: %i\n",barrier->current_processor, barrier->flag, local_sense);
 }



int main(int argc, char** argv) {

    barrier_t barrier;
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    int i;
    FILE *file;
    char* msg;

    my_barrier_init(&barrier);

    if(barrier.current_processor == 0){
      file = fopen("tstfile.txt", "w");
    }else{
      file = fopen("tstfile.txt", "r");
    }

    for(i=0; i <= 10; i++){
      if(barrier.current_processor == 0){
        sleep(1);
        if(file!=NULL){
          //sprintf(msg, "Greetings the current phase is: %i\n", i);
        //  fputs(msg, file);
          fclose(file);
        }
      }
      my_barrier(&barrier);
      barrier.flag++;
      if(barrier.current_processor != 0){
        if(file!=NULL){
        //  fgets(msg, 200, file);
        //  printf("%s",msg);
          fclose(file);
        }
      }
    }


    // Print off a hello world message


    /*printf("Hello world from processor %s, rank %d"
           " out of %d processors\n",
           processor_name, world_rank, world_size);*/

    // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}
