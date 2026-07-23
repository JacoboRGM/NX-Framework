/**
 * @file uartDriver.h
 * @brief Asynchronous transport driver for UART on dsPIC33CH.
 * @note 100% Event-Driven architecture using native hardware interrupts.
 */

#ifndef H_UART_DRIVER_H
#define H_UART_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Enumeration of UART communication status events.
 */
typedef enum {
    UART_EVENT_TX_COMPLETE,  /**< Software ring buffer and hardware FIFO are completely empty */
    UART_EVENT_RX_OVERFLOW,   /**< Software application buffer overflowed (Frame > max size) */
    UART_EVENT_HW_ERROR       /**< Hardware error occurred (OERR Overflow or Line Noise) */
} uart_event_t;

// Type definitions for function pointers (Callbacks) using Typename
typedef void (*uart_rx_callback_t)(const uint8_t *data, uint16_t length);
typedef void (*uart_event_callback_t)(uart_event_t event);

/**
 * @brief Initializes the UART2 peripheral at register level (115200 Baud, 8N1).
 */
void UART_Init(void);

/**
 * @brief Registers the callback function for valid frame reception (terminated by 0xFF, 0xFF, 0xFF).
 */
void UART_Register_Rx_Callback(uart_rx_callback_t callback_function);

/**
 * @brief Registers the callback function to monitor peripheral status events.
 */
void UART_Register_Event_Callback(uart_event_callback_t callback_function);

/**
 * @brief Pushes a massive data block into the transmission ring buffer.
 * @return true if data fits into the RAM buffer, false if rejected due to insufficient space.
 */
bool UART_Write_Buffer(const uint8_t *data, uint16_t length);

/**
 * @brief Non-blocking query to check if the TX channel is completely idle.
 */
bool UART_IsTxIdle(void);

#endif /* H_UART_DRIVER_H */
