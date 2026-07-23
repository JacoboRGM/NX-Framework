/**
 * @file nx_transport.c
 * @brief Nextion Serial Transmission HAL wrapper and command framing logic.
 * @author Jacobo Gonzalez
 * @date July 2, 2026
 */

#include <stddef.h>
#include <string.h>
#include "nx_transport.h"
#include "nx_fifo.h"
#include "nx_screen.h"
#include "../../mcc_generated_files/pin_manager.h"
#include "nx_event.h"

/* Preallocated private transmission frame buffer to prevent stack pollution inside MCU tasks */
static uint8_t tx_single_burst_buffer[NX_MAX_VECTOR_LEN];
/* Protocol-mandated 3-byte termination footprint used globally by Nextion displays */
static const uint8_t nextion_end_bytes[] = {0xFF, 0xFF, 0xFF};

NX_Status_t NX_Transport_Init(NX_Screen_t *screen, NX_PortComm_ID_t port,
        NX_TxCallback_fnc txCallback) {

    if (screen == NULL || txCallback == NULL || screen->transport == NULL)
        return NX_ERROR_NULL_POINTER;

    screen->transport->portId = port;
    // Link the physical hardware MCU UART transmit routine pointer
    screen->transport->tx_hardware_func = txCallback; //pointer to write UART
    screen->transport->rx_hardware_func = &NX_Transport_RxCallback; //ptr to read commands from UART
    screen->transport->task_transport = &NX_Transport_Tasks;    //ptr to task event
    
    NX_VectorFifo_Init(screen->transport->rxFifo);
    NX_VectorFifo_Init(screen->transport->txFifo);

    return NX_OK;
}


//this method invoque to ptr to function to UART TX driver
NX_Status_t NX_Transport_SendRaw(NX_Screen_t *screen)
{

    if (screen == NULL) 
        return NX_ERROR_NULL_POINTER;
    
    //get the NX_Vector_t
    NX_Vector_t outVector;

    NX_Status_t status = NX_VectorFifo_Pop(screen->transport->txFifo, &outVector);
    
    //check if void fifo or error
    if (status != NX_OK)
        return status; 
    
    
    // Safety check: Ensure the text length plus the 3 termination bytes fit into our static buffer
    if ((outVector.length > 0) && (outVector.length + 3) <= sizeof(tx_single_burst_buffer)) {
      
        // Step A: Copy the raw textual instruction into the start of the transmission stream
        memcpy(tx_single_burst_buffer, outVector.data, outVector.length);
        
        // Step B: Append the 3 protocol mandatory 0xFF delimiters at the exact ending offset
        tx_single_burst_buffer[outVector.length]     = nextion_end_bytes[0];
        tx_single_burst_buffer[outVector.length + 1] = nextion_end_bytes[1];
        tx_single_burst_buffer[outVector.length + 2] = nextion_end_bytes[2];
        
        // Step C: Stream the full unifed packet address to the physical UART port in ONE single atomic burst
        outVector.length += 3;
        
        return screen->transport->tx_hardware_func(screen, (uint8_t *)tx_single_burst_buffer, outVector.length);
    }
    return NX_ERROR_BUFFER_OUT_OF_SIZE;
}

//this method feed the event dispatcher
NX_Status_t NX_Transport_OnVectorReceived(NX_Screen_t *screen)
{
    
    if(screen == NULL || screen->transport == NULL)
        return NX_ERROR_NULL_POINTER;
    
    //check the abbalability on the RX buffer fifo
    if(NX_VectorFifo_GetCount(screen->transport->rxFifo) == 0)
        return NX_ERROR_FIFO_VOID;
    
    //function to mantain the FIFO RX and dispatch events
    NX_Vector_t readVector;
    NX_Status_t status_read = NX_VectorFifo_Pop(screen->transport->rxFifo, &readVector);
    
    
    if(status_read != NX_OK)
        return status_read;
    
    return NX_eventDispatch(screen, &readVector);
    
}

//this method fills the rx fifo buffer from uart side
NX_Status_t NX_Transport_RxCallback(NX_Screen_t *screen, const uint8_t *buffer, uint8_t length){
    
    if( screen == NULL)
        return NX_ERROR_NULL_POINTER;
    //recive the data each interrupt for UART driver
    NX_Status_t writeFIFO;
    // Defensive check: Guard against external vector array length overflow
    if (length > NX_MAX_VECTOR_LEN) 
        return NX_ERROR_BUFFER_OVERFLOW; 

    // Create a local instance of the structural vector object
    NX_Vector_t new_packet_vector;
    new_packet_vector.length = length;

    // Direct memory block copy from the external UART buffer to our internal object container
    memcpy(new_packet_vector.data, buffer, length);

    // Push the structured vector directly into the 2D matrix FIFO space
    writeFIFO = NX_VectorFifo_Push(screen->transport->rxFifo,&new_packet_vector);
    
    return writeFIFO;
}

//this method get the tx buffer from api side
NX_Status_t NX_Transport_TxCallback(NX_Screen_t *screen, uint8_t *buffer, uint8_t length){
   
    // Check only the actual pointers for NULL
    if ((screen == NULL) || (buffer == NULL)) {
        return NX_ERROR_NULL_POINTER;
    }
    
    // Check if data exceeds maximum allowed vector capacity
    
    if (length > NX_MAX_VECTOR_LEN) {
        return NX_ERROR_INVALID_PARAMETER; // Replace with your actual error macro
    }

    // Initialize the vector structure
    NX_Vector_t outVector;
    outVector.length = length;

    // Copy array data safely into the struct array buffer
    memcpy(outVector.data, buffer, outVector.length);
    
    NX_Status_t status =NX_VectorFifo_Push(
                        screen->transport->txFifo,
                        &outVector);
    return status;

}

NX_Status_t NX_Transport_Tasks(NX_Screen_t *screen){
    
    NX_Status_t status0;
    NX_Status_t status1;
    
    status0 = NX_Transport_OnVectorReceived(screen);
    
    status1 = NX_Transport_SendRaw(screen);
    
    if(status0 != NX_OK)
        return status0;
    
    if(status1 != NX_OK)
        return status1;
                
    return NX_OK;
}