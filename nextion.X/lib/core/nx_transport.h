/**
 * @file nx_transport.h
 * @brief Nextion Serial Transmission HAL wrapper and asynchronous command framing.
 * @note Manages the physical hardware communication abstraction and buffers input vectors.
 * @copyright Copyright (c) 2026 Jacobo Gonzalez
 * @license MIT License
 */

#ifndef NX_TRANSPORT_H
#define NX_TRANSPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nx_types.h"
#include "nx_fifo.h"
#include "nx_screen.h"
    


/**
 * @brief Nextion Transport Layer Structure.
 * Holds the dedicated bidirectional hardware abstraction contexts and bounds incoming vectors.
 */
    
struct NX_Transport {
  
    NX_PortComm_ID_t    portId;           // Token identifier for chips managing multiple physical ports
    NX_VectorFifo_t     *rxFifo;           // Internal FIFO rx
    NX_VectorFifo_t     *txFifo;           // Internal FIFO tx
    NX_RxCallback_fnc   rx_hardware_func; // Pointer to functions to get data from UART driver
    NX_TxCallback_fnc   tx_hardware_func; //pointer 
    Timer_callback_fnc  task_transport;   // Task TX event
};

/*============================================================================*
 * Transport Layer API
 *============================================================================*/

/**
 * @brief Initializes the transport layer context, resets the vector FIFO, and binds hardware hooks.
 * @param[out] transport Pointer to the uninitialized transport structure.
 * @param[in] tx_func Pointer to the MCU physical UART transmission function.
 * @param[in] port Physical hardware serial port token identifier.
 * @param[in] function to read event dispatch user implemented
 */
NX_Status_t NX_Transport_Init(NX_Screen_t *screen, NX_PortComm_ID_t port, NX_TxCallback_fnc txCallback);

/**
 * @brief Encapsulates a rendered alphanumeric instruction stream and appends protocol termination.
 * @param[in,out] transport Pointer to the operational transport layer context.
 * @param[in] cmd Null-terminated C-string containing the formatted Nextion command.
 */
NX_Status_t NX_Transport_SendRaw(NX_Screen_t *screen);


/**
 * @brief VECTOR INJECTION BRIDGE.
 * The user's external UART driver calls this function to inject a completely 
 * pre-assembled data vector with its explicit length straight into the 2D FIFO.
 * @param[in,out] transport Pointer to the operational transport layer context.
 * @param[in] raw_vector_buffer Pointer to the pre-assembled array of bytes from the UART.
 * @param[in] length Explicit number of bytes contained within the vector.
 * @return true if the vector was successfully pushed to the FIFO, false otherwise.
 */
NX_Status_t NX_Transport_OnVectorReceived(NX_Screen_t *screen);


/*Pointer to functions for MCU driver tasks rx y tx
 *This functiouns allows get and push data from transport layer and
 * used for MCU driver handled, each interrupt fot RX and TX
 */

NX_Status_t NX_Transport_RxCallback(NX_Screen_t *screen, const uint8_t *buffer, uint8_t length);

NX_Status_t NX_Transport_TxCallback(NX_Screen_t *screen, uint8_t *buffer, uint8_t length);

NX_Status_t NX_Transport_Tasks(NX_Screen_t *screen);

#ifdef __cplusplus
}
#endif

#endif /* NX_TRANSPORT_H */
