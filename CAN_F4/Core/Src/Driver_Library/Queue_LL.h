/*
 * Queue_LL.h
 *
 *  Created on: Sep 16, 2025
 *      Author: Admin
 */

#ifndef SRC_DRIVER_LIBRARY_QUEUE_LL_H_
#define SRC_DRIVER_LIBRARY_QUEUE_LL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* In this case we ultilize the singly LL for normal FIFO operation*/

#define LENGTH 8

struct Node{
	uint8_t data[LENGTH];
	uint8_t len;
	struct Node *next;
};

struct Queue{
	struct Node *front, *rear;
};

struct Node* newNode(uint8_t *data, uint8_t len);

struct Queue* createQueue();

int isEmpty(struct Queue* q);

void enqueue(struct Queue* q, uint8_t *data, uint8_t len);

void dequeue(struct Queue* q);

#endif /* SRC_DRIVER_LIBRARY_QUEUE_LL_H_ */
