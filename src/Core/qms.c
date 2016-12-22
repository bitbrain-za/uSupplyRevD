/*
 * qms.c
 *
 * Created: 2016/12/20 1:44:32 PM
 *  Author: Philip
 */ 

#include "../system.h"



/* Prototypes */
void *next_free(Queue *queue);


/* External Functions */
bool b_queue_init(Queue *queue, U8 type_size, U8 max_items)
{
  queue->data = malloc(type_size * max_items);
  if(NULL == queue->data)
    return false;

  queue->item_size = type_size;
  queue->depth = max_items;
  queue->items = 0;

  return true;
}


void b_qms_delete(Queue *queue)
{
  free(queue->data);
}

bool b_is_queue_full(Queue *queue)
{
  return (queue->items == queue->depth);
}


bool b_queue_send(Queue *queue, void *data)
{
  if(b_is_queue_full(queue))
    return false;

  memcpy(next_free(queue), data, queue->item_size);
  return true;
}

bool b_queue_peak(Queue *queue, void *return_data)
{
  if(0 == queue->items)
    return false;

  memcpy(return_data, queue->data, queue->item_size);

  return true;
}

bool b_queue_read(Queue *queue, void *return_data)
{
  U8 i = 0;
  void *data_ptr = queue->data;
  if(0 == queue->items)
    return false;

  memcpy(return_data, queue->data, queue->item_size);

  queue->items--;

  if(0 == queue->items)
    return true;

  for(i = 0 ; i < queue->items; i++)
  {
    memcpy(data_ptr, data_ptr + queue->item_size, queue->item_size);
    data_ptr += queue->item_size;
  }
  return true;
}


/* Local Functions */


void *next_free(Queue *queue)
{
  if(b_is_queue_full(queue))
    return NULL;
  else
    return (queue->data + (queue->items * queue->item_size));
}