#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <pthread.h>

/* adjust as needed */
#define BUFFER_SIZE (4)  


/* sensor types */
typedef enum {
  TEMP,
  PRESSURE,
  CAMERA
} SensorType;


/* buffer entry */
typedef struct {
  SensorType type;  /* sensor type */
  uint32_t value;   /* sensor value or frame count */
} BufferEntry;


/* circular buffer struct */
typedef struct {
  BufferEntry entries[BUFFER_SIZE];
  int head;
  int tail;
  pthread_mutex_t mutex;
} CircularBuffer;


/* buffer functions */
void buffer_init(CircularBuffer *buff);
int buffer_push(CircularBuffer *buff, BufferEntry entry);
int buffer_pop(CircularBuffer *buff, BufferEntry *entry);


#endif /* BUFFER_H */