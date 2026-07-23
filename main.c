/**
 * @file main.c
 * @brief Nextion SDK main application entry point for dsPIC33CH512MP508.
 * @note This file features strict start-up shields to prevent compiler-driven 
 *       reset loops from flooding the TX serial line.
 * @author Jacobo Gonzalez
 * @date July 12, 2026
 */
#define FCY 90000000
#include <libpic30.h>
#include <stdio.h>
#include "xc.h"
#include "mcc_generated_files/system.h"          // MCC master clock and pin initializer
#include "lib/core/nx_api.h"
#include "mcc_generated_files/pin_manager.h"
#include "lib/drivers/uart/uartDriver.h"
#include "lib/nextion/nextion_layout.h"
#include "mcc_generated_files/tmr1.h"

/*=============================================================================
// 0.- Internal functions
=============================================================================*/

//pointers to functions from uart drivers
NX_Status_t NX_send(NX_Screen_t *screen, uint8_t *buffer, uint8_t length);
void rx_interrupt_callback(const uint8_t *buffer, uint16_t length);
void event_callback(uart_event_t event);

void tmr0_tick(void){

    static volatile uint8_t timer_tick1 = 0;

    if (++timer_tick1 >=3) {
        NX_Status_t status;

        status = miDispositivoHMI.transport->task_transport(
                &miDispositivoHMI);
        
        timer_tick1=0;
    }
    
}

NX_Status_t ScreenGlobalEvent(NX_Vector_t *unhandled_vector)
{
    uint8_t event_code = unhandled_vector->data[0];

    switch (event_code)
    {
        case NX_EVENT_CURRENT_PAGE:
            printf("[HMI NOTICE] Current Page ID: %d\r\n", unhandled_vector->data[1]);
            break;

        case NX_EVENT_NUMERIC_DATA_ENCL:
        {
            int32_t val = (int32_t)((uint32_t)unhandled_vector->data[1]        |
                                   ((uint32_t)unhandled_vector->data[2] << 8)  |
                                   ((uint32_t)unhandled_vector->data[3] << 16) |
                                   ((uint32_t)unhandled_vector->data[4] << 24));
            printf("[HMI DATA] Value: %ld\n", val);
            break;
        }
        default:
            break;
    }
    return NX_OK;
}
//receive errors
NX_Status_t statusTasks;

int main(void)
{
    
    SYSTEM_Initialize();
    UART_Init();
    TMR1_SetInterruptHandler(&tmr0_tick);
    UART_Register_Rx_Callback(&rx_interrupt_callback);
    UART_Register_Event_Callback(&event_callback);
    
    statusTasks = NX_Transport_Init(&miDispositivoHMI, 0, &NX_send);
    
    NX_Init(&miDispositivoHMI);
    
    const NX_String_t text = {
        .data = (char *)"READY",
        .length = 6
    };
    
    NX_setPage(&miDispositivoHMI, &page_dashboard);
    NX_SetText(&miDispositivoHMI, &btn_start, &text);
   
    uint32_t counter=0;
   
    printf("Nextion Library V1 JRGM\r\n");
    while (1)
    {
       
        if (SW1_GetValue() == 0) {
            __delay_ms(20);
            if (SW1_GetValue() == 0) {
                while (!SW1_GetValue());
                NX_ExecuteCommand(&miDispositivoHMI, NX_CMD_SENDME);
            }
        }


        __asm__ volatile ("nop");

        if (++counter >= 0xF00000) {

            static volatile uint32_t number = 0;
            int length;
            char buffer[100] = {};

            length = snprintf(buffer, sizeof (buffer), "Hola mundo %lu", ++number);
            const NX_String_t btn_start_text = {
                .data = (char *) buffer,
                .length = (uint8_t) length
            };

            statusTasks = NX_SetText(&miDispositivoHMI, &btn_start, &btn_start_text);
            statusTasks = NX_SetVal(&miDispositivoHMI, &number0, (int32_t) number);

            counter = 0;
        }
    }
    return 1;
}

void rx_interrupt_callback(const uint8_t *buffer, uint16_t length){
    NX_Status_t rx_status;
    rx_status = miDispositivoHMI.transport->rx_hardware_func(&miDispositivoHMI, buffer, length);
    RGB_BLUE_Toggle();
}

NX_Status_t NX_send(NX_Screen_t *screen, uint8_t * buffer, uint8_t length){
    
    //RGB_GREEN_Toggle();
    if(UART_Write_Buffer((const uint8_t*)buffer, length))
        return NX_OK;
    
    return NX_ERROR_COMM;
}

void event_callback(uart_event_t event){

    switch(event){
        
        case UART_EVENT_TX_COMPLETE:
            
            //call to the transport function
            RGB_RED_Toggle();
            break;
            
        case UART_EVENT_RX_OVERFLOW:
            
            break;
            
        case UART_EVENT_HW_ERROR:
            
            break;
            
        default:
            
            break;
    }
}