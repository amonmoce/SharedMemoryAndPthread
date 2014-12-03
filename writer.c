#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define MEM_SIZE 100
int main(){
  int shared_memory_id;
  key_t key;
  char *shared_memory;
  char *sum;
  
  key = 9876;
  shared_memory_id = shmget(key, MEM_SIZE, IPC_CREAT | 0666);
  shared_memory = shmat(shared_memory_id, NULL, 0);



  memcpy(shared_memory, sum, strlen(sum))

  return 0;
}
