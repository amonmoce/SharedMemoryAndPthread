#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define MEM_SIZE 100
#define DATA_SIZE 1024


//MEM variables
int shared_memory_id;
key_t key = 9876;
char *shared_memory;
//End MEM variables

//Mutex lock
pthread_mutex_t lock;
//End mutex lock
int which_thread1, which_thread2, which_thread3, which_thread4, count = 0;;
pthread_t thread1, thread2, thread3, thread4, thread5;
void *handlefiledata(void *filename);
void *handlememory(void *shared_memory);

int main(){
  char memory;

  int ret1, ret2, ret3, ret4, ret5;
  char *filename1 = "Testdata1/testdata1.txt",
       *filename2 = "Testdata1/testdata2.txt",
       *filename3 = "Testdata1/testdata3.txt",
       *filename4 = "Testdata1/testdata4.txt";

   //Start memory mgmnt
   shared_memory_id = shmget(key, MEM_SIZE, IPC_CREAT | 0666);
   shared_memory = shmat(shared_memory_id, NULL, 0);


   memcpy(shared_memory, "xxxxxxxxxxx", strlen(shared_memory));

   //End memory mgmnt

   //Init mutex
   pthread_mutex_init(&lock, NULL);
   //End mutex init

  ret1 = pthread_create(&thread1, NULL, handlefiledata, (void *) filename1);
  ret2 = pthread_create(&thread2, NULL, handlefiledata, (void *) filename2);
  ret3 = pthread_create(&thread3, NULL, handlefiledata, (void *) filename3);
  ret4 = pthread_create(&thread4, NULL, handlefiledata, (void *) filename4);
  ret5 = pthread_create(&thread5, NULL, handlememory, (void *) shared_memory);




  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
  pthread_join(thread4, NULL);
  pthread_join(thread5, NULL);
  pthread_mutex_destroy(&lock);
  pthread_exit(NULL);
  return 0;
}

void *handlefiledata(void *filename){
  FILE *fp;
  char read[DATA_SIZE], sum[DATA_SIZE], inmemory[DATA_SIZE], num[DATA_SIZE], *position;
  int number = 0, dif;
  filename = (char *) filename;
  fp = fopen(filename, "r+");

  while(fscanf(fp, "%s", read) == 1){
    if(strcmp(read, "wait")!=0){
      //printf("%s\n", read);
      number += atoi(read);

    }
    else {
      pthread_mutex_lock(&lock);

      memset(inmemory,'\0',strlen(inmemory));
      memset(num,'\0',strlen(num));
      //memset(position,'\0',strlen(position));

      if(strcmp(filename, "Testdata1/testdata1.txt") == 0){
        which_thread1 = 1;
      }
      if(strcmp(filename, "Testdata1/testdata2.txt") == 0){
        which_thread2 = 1;
      }
      if(strcmp(filename, "Testdata1/testdata3.txt") == 0){
        which_thread3 = 1;
      }
      if(strcmp(filename, "Testdata1/testdata4.txt") == 0){
        which_thread4 = 1;
      }

      strcpy(inmemory, shared_memory);
      position = strstr(inmemory, "x");


      if(position != NULL)
        dif = strlen(inmemory) - strlen(position);
      else
        dif = strlen(inmemory);

      strncpy (num, inmemory, (size_t) dif);

      number += atoi(num);

      sprintf(sum, "%d", number);
      memcpy(shared_memory, sum, strlen(sum));

      //printf("%d", which_thread);
      pthread_mutex_unlock(&lock);
      number = 0;

    }

  }


  fclose(fp);
  return NULL;
}

void *handlememory(void *shared_memory){
  char inmemory2[DATA_SIZE], num2[DATA_SIZE], *position2;
  int dif2;
  shared_memory = (char *) shared_memory;
  FILE *fp;
  fp = fopen("Result.txt", "wb+");
  while(1){
    if(which_thread1 == 1 && which_thread2 == 1
      && which_thread3 == 1 && which_thread4 == 1){
        pthread_mutex_lock(&lock);

        memset(inmemory2,'\0',strlen(inmemory2));
        memset(num2,'\0',strlen(num2));
        //memset(position2,'\0',strlen(position2));

        strcpy(inmemory2, shared_memory);
        position2 = strstr(inmemory2, "x");


        if(position2 != NULL)
          dif2 = strlen(inmemory2) - strlen(position2);
          else
            dif2 = strlen(inmemory2);

            //printf("%d\n", dif);

            strncpy (num2, inmemory2, (size_t) dif2);

            fprintf(fp, "%d %s\n",count++, num2);
            printf("%d %s\n",count++, num2);
            which_thread1 = 0;
            which_thread2 = 0;
            which_thread3 = 0;
            which_thread4 = 0;
            memcpy(shared_memory, "xxxxxxxxxxx", strlen(shared_memory));
            pthread_mutex_unlock(&lock);

      }

  }

  return NULL;
}
