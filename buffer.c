/********************************************************
 * this file includes the data packing, buffer, and 
 * "communication"
 * 
 * inspo from: https://embedjournal.com/implementing-circular-buffer-embedded-c/
*********************************************************/
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>


/* function prototypes */
void buffer_init(CircularBuffer *buff);
int buffer_push(CircularBuffer *buff, BufferEntry entry);
int buffer_pop(CircularBuffer *buff, BufferEntry *entry);


/*********************************************************
 * initializes buffer:
 *  head and tail = 0
 *  mutex is initialized
*********************************************************/
void buffer_init(CircularBuffer *buff) {

  buff->head = 0;
  buff->tail = 0;
  
  if (pthread_mutex_init(&buff->mutex, NULL) != 0) {
    printf("Mutex initialization failed.\n");
    exit(1);
  }
}


/*********************************************************
 * pushes entry into buffer
 * 
 * returns:
 *   -1 for error when buffer is full
 *   0 for successful push
*********************************************************/
int buffer_push(CircularBuffer *buff, BufferEntry entry) {
  /* next is where head will point to after write */
  int next;

  /* lock mutex */
  pthread_mutex_lock(&buff->mutex); 

  /* if head + 1 exceeds buffer size, circular buffer starts back at 0 */
  next = buff->head + 1;  
  if (next >= BUFFER_SIZE) { next = 0; }

  /* check if buffer is full */
  if (next == buff->tail) { 
    pthread_mutex_unlock(&buff->mutex); /* unlock before returning */
    return -1; 
  }  

  buff->entries[buff->head] = entry;  /* loads data and then move */
  buff->head = next;                  /* head to next data offset */
  
  /* unlock mutex */
  pthread_mutex_unlock(&buff->mutex); 
  return 0;
}


/*********************************************************
 * pushes entry into buffer
 * 
 * returns:
 *   -1 for error when buffer is empty
 *   0 for successful pop
*********************************************************/
int buffer_pop(CircularBuffer *buff, BufferEntry *entry) {
  /* lock mutex */
  pthread_mutex_lock(&buff->mutex); 

  /* check if buffer is empty */
  if (buff->head == buff->tail) { 
    pthread_mutex_unlock(&buff->mutex); /* unlock before returning */
    return -1; 
  }  /* no data to read */
  
  int next = buff->tail + 1;  /* next is where tail points after read */
  if (next >= BUFFER_SIZE) { next = 0; }
  
  *entry = buff->entries[buff->tail]; /* read data */
  buff->tail = next;

  /* unlock mutex */
  pthread_mutex_unlock(&buff->mutex);
  return 0;
}



int main(int argc, char *argv[]) {
  /**************************************************************
   * tests the circular buffer
   * with BUFFER_SIZE (4)
   * can change size in buffer.h to be larger, etc
  ***************************************************************/
  CircularBuffer buff;
  buffer_init(&buff);
  
  /* create buffer entries */
  BufferEntry temp1;
  temp1.type = TEMP;
  temp1.value = 55;

  BufferEntry temp2;
  temp2.type = TEMP;
  temp2.value = 32;

  BufferEntry pressure1;
  pressure1.type = PRESSURE;
  pressure1.value = 31;

  /* with BUFFER_SIZE (4), will not push */
  BufferEntry pressure2;
  pressure2.type = PRESSURE;
  pressure2.value = 8;

  /* push value */
  if (buffer_push(&buff, temp1)) { printf("error pushing onto buffer, entry 1 \n"); }
  if (buffer_push(&buff, temp2)) { printf("error pushing onto buffer, entry 2\n"); }
  if (buffer_push(&buff, pressure1)) { printf("error pushing onto buffer, entry 3\n"); }
  
  /* should return error */
  if (buffer_push(&buff, pressure2)) { printf("error pushing onto buffer, enrty 4\n"); }
  
  for (int i = 0; i < 4; i++) {
    BufferEntry current_entry;
    /* fourth try should return error */
    if (buffer_pop(&buff, &current_entry)) { printf("error popping buffer, entry %d\n", i); }
    printf("entry [tail]=%d: type=%d, value=%d\n", buff.tail, current_entry.type, current_entry.value);
  }

  /* with BUFFER_SIZE (4), should push now*/
  if (buffer_push(&buff, pressure2)) { printf("error pushing onto buffer, enrty 4\n"); }

  BufferEntry current_entry;
  if (buffer_pop(&buff, &current_entry)) { printf("error popping buffer, entry 4\n"); }
  printf("entry [tail]=%d: type=%d, value=%d\n", buff.tail, current_entry.type, current_entry.value);
}
  
  
