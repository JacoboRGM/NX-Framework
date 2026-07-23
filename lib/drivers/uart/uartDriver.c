/**
 * @file uartDriver.c
 * @brief UART driver implementation at register level exploiting the 8-level FIFO.
 */
#include <stddef.h>
#include "xc.h"
#include "uartDriver.h"

#define TX_BUFFER_SIZE      512  // RAM space to store massive outgoing bursts
#define RX_FRAME_MAX_SIZE   100  // Physical limit for incoming frame reconstruction

// Internal storage structures with strict numerical index control
static uint8_t tx_ring[TX_BUFFER_SIZE];
static volatile uint16_t tx_head = 0;
static volatile uint16_t tx_tail = 0;

static uint8_t rx_frame[RX_FRAME_MAX_SIZE];
static uint16_t idx_rx = 0;

// Private pointer variables initialized in a safe state (NULL)
static uart_rx_callback_t    Rx_Frame_Ready_Callback = NULL;
static uart_event_callback_t Event_Callback          = NULL;

void UART_Init(void)
{
    IEC1bits.U2TXIE = 0;
    IEC1bits.U2RXIE = 0;
    U2MODEbits.UARTEN = 0;

    // Baud Rate Configuration (115200 Baud @ Fcy = 90MHz)
    U2MODEbits.BRGH = 1;
    U2BRG = 194; 
    U2BRGH = 0x00;
    U2MODEbits.MOD = 0;     
    U2MODEHbits.STSEL = 0;  

    // Hardware FIFO interrupt trigger modes in silicon
    U2STAHbits.URXISEL = 0; // Interrupt byte by byte on reception
    U2STAHbits.UTXISEL = 2; // Interrupt ONLY when the 8-level hardware FIFO becomes completely empty

    tx_head = 0; tx_tail = 0; idx_rx = 0;
    Rx_Frame_Ready_Callback = NULL;
    Event_Callback = NULL;
    
    //here configure the pin for PPS
    __builtin_write_RPCON(0x0000); // unlock PPS
    
    RPINR19bits.U2RXR = 0x0026;    //RB6->UART2:U2RX
    RPOR2bits.RP37R = 0x0003;    //RB5->UART2:U2TX
    
    __builtin_write_RPCON(0x0800); // lock PPS
    
    
    //here activate the interrupt for UART
    //    UEVTI: UART2 Event
    //    Priority: 1
        IPC47bits.U2EVTIP = 1;
    //    UTXI: UART2 TX
    //    Priority: 1
        IPC7bits.U2TXIP = 1;
    //    UEI: UART2 Error Interrupt
    //    Priority: 1
        IPC12bits.U2EIP = 1;
    //    URXI: UART2 RX
    //    Priority: 1
        IPC6bits.U2RXIP = 1;

  
    IFS1bits.U2RXIF = 0;
    IFS1bits.U2TXIF = 0;
    IEC1bits.U2RXIE = 1; // RX remains enabled permanently

    U2MODEbits.UARTEN = 1; 
    U2MODEbits.UTXEN = 1;  
    U2MODEbits.URXEN = 1;  
}

void UART_Register_Rx_Callback(uart_rx_callback_t callback_function)
{
    if (callback_function != NULL) Rx_Frame_Ready_Callback = callback_function;
}

void UART_Register_Event_Callback(uart_event_callback_t callback_function)
{
    if (callback_function != NULL) Event_Callback = callback_function;
}

bool UART_IsTxIdle(void)
{
    return ((tx_head == tx_tail) && (U2STAbits.TRMT == 1));
}

bool UART_Write_Buffer(const uint8_t *data, uint16_t length)
{
    uint16_t free_space;
    if (tx_tail >= tx_head) {
        free_space = TX_BUFFER_SIZE - (tx_tail - tx_head) - 1;
    } else {
        free_space = tx_head - tx_tail - 1;
    }

    if (length > free_space) return false; 

    // Fast indexed copy using modular arithmetic
    for (uint16_t i = 0; i < length; i++)
    {
        tx_ring[tx_tail] = data[i];
        tx_tail = (tx_tail + 1) % TX_BUFFER_SIZE;
    }

    // If the transmission ISR is idle/disabled, we wake up the hardware engine
    if (IEC1bits.U2TXIE == 0)
    {
        IFS1bits.U2TXIF = 0; 
        // Direct continuous burst injection into the physical FIFO
        while ((U2STAHbits.UTXBF == 0) && (tx_head != tx_tail))
        {
            U2TXREG = tx_ring[tx_head];
            tx_head = (tx_head + 1) % TX_BUFFER_SIZE;
        }
        IEC1bits.U2TXIE = 1; 
    }
    return true;
}

void __attribute__((interrupt, no_auto_psv)) _U2TXInterrupt(void)
{
    IFS1bits.U2TXIF = 0; // Clear hardware physical flag

    // UTXISEL = 2 executes this ISR exactly when the hardware FIFO becomes empty
    if (tx_head == tx_tail)
    {
        IEC1bits.U2TXIE = 0; // Disable TX ISR before notification to allow safe daisy-chaining
        
        if (Event_Callback != NULL)
        {
            Event_Callback(UART_EVENT_TX_COMPLETE);
        }
    }
    else
    {
        while ((U2STAHbits.UTXBF == 0) && (tx_head != tx_tail))
        {
            U2TXREG = tx_ring[tx_head];
            tx_head = (tx_head + 1) % TX_BUFFER_SIZE;
        }
    }
}

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void)
{
    IFS1bits.U2RXIF = 0; 
    static uint8_t count_ff = 0; 

    // LINE ERROR MANAGEMENT: If the CPU didn't read fast enough, OERR (Overflow) freezes the hardware.
    // Clearing it unlocks the reception module immediately.
    if (U2STAbits.OERR == 1)
    {
        U2STAbits.OERR = 0; 
        if (Event_Callback != NULL) Event_Callback(UART_EVENT_HW_ERROR);
    }

    while (U2STAHbits.URXBE == 0) 
    {
        uint8_t byte = U2RXREG; 

        if (idx_rx < RX_FRAME_MAX_SIZE)
        {
            rx_frame[idx_rx++] = byte;
        }
        else
        {
            idx_rx = 0; count_ff = 0;
            if (Event_Callback != NULL) Event_Callback(UART_EVENT_RX_OVERFLOW);
            continue; 
        }

        // State Machine: Searching for the delimiter pattern [0xFF, 0xFF, 0xFF]
        if (byte == 0xFF)
        {
            count_ff++;
            if (count_ff == 3)
            {
                uint16_t useful_length = idx_rx - 3;

                // DECOUPLED NOTIFICATION: Dispatch data to the registered external library
                if (Rx_Frame_Ready_Callback != NULL)
                {
                    Rx_Frame_Ready_Callback(rx_frame, useful_length);
                }

                idx_rx = 0;
                count_ff = 0;
            }
        }
        else
        {
            count_ff = 0; 
        }
    }
}
