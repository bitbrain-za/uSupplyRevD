/*
 * qms.h
 *
 * Created: 2016/12/20 1:44:50 PM
 *  Author: Philip
 */ 


#ifndef QMS_H_
#define QMS_H_

typedef struct
{
  U8 items;
  U8 item_size;
  U8 depth;
  void *data;
}Queue;

extern bool b_queue_init(Queue *queue, U8 type_size, U8 max_items);
extern void b_qms_delete(Queue *queue);

extern bool b_is_queue_full(Queue *queue);
extern bool b_queue_send(Queue *queue, void *data);

extern bool b_queue_read(Queue *queue, void *return_data);
extern bool b_queue_peak(Queue *queue, void *return_data);


#endif /* QMS_H_ */