/**
 * @file nx_fifo.c
 * @brief First-In, First-Out matrix storage logic utilizing safe memory copies.
 * @author Jacobo Gonzalez
 */
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "nx_fifo.h"

void NX_VectorFifo_Init(NX_VectorFifo_t *fifo) {
    if (fifo != NULL) {
        fifo->head = 0;
        fifo->tail = 0;
        fifo->count = 0;
    }
}

NX_Status_t NX_VectorFifo_Push(NX_VectorFifo_t *fifo, const NX_Vector_t *vector) {
    
    if (fifo == NULL || vector == NULL)
        return NX_ERROR_NULL_POINTER; // Overflow protection block

    if (fifo->count >= NX_FIFO_DEPTH)
        return NX_ERROR_FIFO_OVERFLOW;

    // Perform vector duplication into the matrix array slot
    memcpy(&fifo->buffer[fifo->head], vector, sizeof (NX_Vector_t));
    fifo->head = (fifo->head + 1) % NX_FIFO_DEPTH;
    fifo->count++;
    return NX_OK;
}

NX_Status_t NX_VectorFifo_Pop(NX_VectorFifo_t *fifo, NX_Vector_t *out_vector) {

    if (fifo == NULL || out_vector == NULL)
        return NX_ERROR_NULL_POINTER; 

    if (fifo->count == 0)// Underflow guard
        return NX_ERROR_FIFO_UNDERFLOW;
    
    memcpy(out_vector, &fifo->buffer[fifo->tail], sizeof(NX_Vector_t));
    fifo->tail = (fifo->tail + 1) % NX_FIFO_DEPTH;
    fifo->count--;
    return NX_OK;
}

uint8_t NX_VectorFifo_GetCount(const NX_VectorFifo_t *fifo) {
    if (fifo == NULL) {
        return 0; 
    }
    return fifo->count;
}