/**
 * @file nx_fifo.h
 * @brief Two-dimensional bounded vector FIFO queue interface for safe task buffering.
 * @copyright Copyright (c) 2026 Jacobo Gonzalez
 */

#ifndef NX_FIFO_H
#define NX_FIFO_H

#include "nx_types.h"

/**
 * @brief Matrix structure scheduling complete atomic message frames sequentially.
 */
typedef struct {
    NX_Vector_t buffer[NX_FIFO_DEPTH]; // Two-dimensional matrix array
    uint8_t head;                      // Write-pointer tracking index
    uint8_t tail;                      // Read-pointer tracking index
    uint8_t count;                     // Instantaneous population tracker
} NX_VectorFifo_t;

void NX_VectorFifo_Init(NX_VectorFifo_t *fifo);

NX_Status_t NX_VectorFifo_Push(NX_VectorFifo_t *fifo, const NX_Vector_t *vector);

NX_Status_t NX_VectorFifo_Pop(NX_VectorFifo_t *fifo, NX_Vector_t *out_vector);

uint8_t NX_VectorFifo_GetCount(const NX_VectorFifo_t *fifo);

#endif /* NX_FIFO_H */
