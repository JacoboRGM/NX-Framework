/**
 * @file nx_types.h
 * @brief Unified data types, protocol configurations, and architectural shared declarations.
 * @note This file isolates core definitions from logic blocks to prevent circular dependencies.
 * @copyright Copyright (c) 2026 Jacobo Gonzalez
 * @license MIT License
 */

#ifndef NX_TYPES_H
#define NX_TYPES_H

#include <stdint.h>
#include <stdbool.h>

#define NX_MAX_VECTOR_LEN  128  // Maximum safe byte length of a standalone serial message vector
#define NX_FIFO_DEPTH      20  // Depth of the two-dimensional message vector FIFO matrix

/*============================================================================*
 * Vectorial Message Models (Two-Dimensional Communication Payload)
 *============================================================================*/

/**
 * @brief Explicitly bounded byte vector structure representing an atomic protocol package.
 */
typedef struct {
    uint8_t data[NX_MAX_VECTOR_LEN]; // Data storage byte block
    uint8_t length;                  // Exact total population of valid bytes in the vector
} NX_Vector_t;

/*============================================================================*
 * Core Graphic Components & Text Wrapper
 *============================================================================*/

/**
 * @brief Identifiers for Nextion HMI graphic components.
 */
typedef enum
{
    NX_TYPE_PAGE = 0,
    NX_TYPE_TEXT,
    NX_TYPE_BUTTON,
    NX_TYPE_PROGRESSBAR,
    NX_TYPE_PICTURE,
    NX_TYPE_CROP,
    NX_TYPE_HOTSPOT,
    NX_TYPE_TOUCH_CAP,
    NX_TYPE_GAUGE,
    NX_TYPE_WAVEFORM,
    NX_TYPE_SLIDER,
    NX_TYPE_TIMER,
    NX_TYPE_NUMBER,
    NX_TYPE_VARIABLE,
    NX_TYPE_DUALSTATE,
    NX_TYPE_CHECKBOX,
    NX_TYPE_RADIO,
    NX_TYPE_XFLOAT,
    NX_TYPE_STEXT,
    NX_TYPE_QRCODE,
    NX_TYPE_SWITCH,
    NX_TYPE_GMOV,
    NX_TYPE_VIDEO,
    NX_TYPE_COMBO_BOX,
    NX_TYPE_SELECT_TEXT,
    NX_TYPE_SLTEXT,
    NX_TYPE_DATA_RECORD,
    NX_TYPE_FILE_BROWSER,
    NX_TYPE_EX_PICTURE,
    NX_TYPE_FILE_STREAM,
    NX_TYPE_AUDIO,
            
    NX_TYPE_MAX, // <--- control limit for safety
    NX_TYPE_ERROR        
} NX_Type_t;
    
typedef enum
{
    // --- System & Initialization ---
    NX_EVENT_STARTUP                    = 0x00, // Successful screen initialization
    // --- Interaction & Pages ---
    NX_EVENT_TOUCH                      = 0x65, // Component touch event (press/release)
    NX_EVENT_CURRENT_PAGE               = 0x66, // Current page ID report (triggered by 'sendme')
    NX_EVENT_TOUCH_COORDINATE_AWK       = 0x67, // Touch coordinates sent in active mode
    NX_EVENT_TOUCH_COORDINATE_SLEEP     = 0x68, // Touch coordinates sent in sleep mode

    // --- Requested Data Retrieval ---
    NX_EVENT_STRING_DATA_ENCL           = 0x70, // String data response (triggered by 'get')
    NX_EVENT_NUMERIC_DATA_ENCL          = 0x71, // 4-byte numeric data response (triggered by 'get')

} NX_EventCode_t;

typedef enum
{
    // --- General Execution Status ---
    NX_STATUS_INVALID_INSTRUCTION       = 0x00, // Instruction sent by user has failed
    NX_STATUS_SUCCESSFUL                = 0x01, // Instruction sent by user was successful

    // --- Component & Page Errors ---
    NX_STATUS_INVALID_COMPONENT_ID      = 0x02, // Invalid Component ID or name was used
    NX_STATUS_INVALID_PAGE_ID           = 0x03, // Invalid Page ID or name was used

    // --- Resource Errors ---
    NX_STATUS_INVALID_PICTURE_ID        = 0x04, // Invalid Picture ID was used
    NX_STATUS_INVALID_FONT_ID           = 0x05, // Invalid Font ID was used
    NX_STATUS_INVALID_FILE_OPERATION    = 0x06, // File system operation failed

    // --- Communication & Hardware Errors ---
    NX_STATUS_INVALID_CRC               = 0x09, // CRC validation check failed (always returned if CRC is wrong)
    NX_STATUS_INVALID_BAUDRATE          = 0x11, // Invalid Baud rate setting was used
    NX_STATUS_INVALID_WAVEFORM          = 0x12, // Invalid Waveform ID or Channel number was used

    // --- Variable & Attribute Errors ---
    NX_STATUS_INVALID_VARIABLE_NAME     = 0x1A, // Invalid Variable name or attribute was used
    NX_STATUS_INVALID_VARIABLE_OP       = 0x1B, // Invalid Variable operation (e.g., type mismatch)
    NX_STATUS_ASSIGNMENT_FAILED         = 0x1C, // Attribute assignment failed to apply

    // --- Memory & Peripherals Errors ---
    NX_STATUS_EEPROM_OPERATION_FAILED   = 0x1D, // EEPROM read/write operation failed
    NX_STATUS_INVALID_PARAM_QUANTITY    = 0x1E, // Number of instruction parameters is invalid
    NX_STATUS_IO_OPERATION_FAILED       = 0x2F, // GPIO or IO operation failed
    NX_STATUS_INVALID_ESCAPE_CHAR       = 0x20, // Unsupported escape character used in string

    // --- Buffer Constraints ---
    NX_STATUS_VARIABLE_NAME_TOO_LONG    = 0x23, // Variable name exceeds max limit (29 characters)
    NX_EVENT_SERIALBUFF_OVERFLOW        = 0x24, // Serial receive buffer overflow on Nextion side
 
    // --- Power Management & Updates ---
    NX_EVENT_AUTO_ENTERD_SLEEP_MODE     = 0x86, //Returned when Nextion leaves sleep automatically Using sleep=0 will not return an 0x87
    NX_EVENT_AUTO_WAKE_FROM_SLEEP       = 0x87, // Device automatically woke up from sleep mode
    NX_EVENT_NEXTION_READY              = 0x88, // Nextion is ready after power-on or reset
    NX_EVENT_START_USD_UPGRD            = 0x89, // MicroSD card firmware upgrade initialization

    // --- Transparent Data Transmission ---
    NX_EVENT_TRANSPARENT_DATA_FINISHED  = 0xFD, // Transparent data transmission completed
    NX_EVENT_TRANSPARENT_DATA_READY     = 0xFE  // Nextion is ready to receive transparent data
            

} NX_StatusCode_t;

//Error typedefs 
typedef enum
{
    NX_OK = 0,

    NX_ERROR_NULL_POINTER,
    NX_ERROR_NO_TRANSPORT,
    NX_ERROR_NO_WRITE_CALLBACK,
    NX_ERROR_NO_READ_CALLBACK,
    NX_ERROR_NO_PAGE_FOUND,
    NX_ERROR_NO_PAGES,
    NX_ERROR_OBJECT_NOT_FOUND,
    NX_ERROR_NO_ATTR_FOUND,
    NX_ERROR_NO_ATTR_OBJECT_FOUND,
    NX_ERROR_INVALID_PAGE,
    NX_ERROR_BUFFER_OVERFLOW,
    NX_ERROR_BUFFER_OUT_OF_SIZE,
    NX_ERROR_INVALID_PARAMETER,
    //transpor errors
    NX_ERROR_TRANSPORT,
    NX_ERROR_FIFO_FULL,
    NX_ERROR_FIFO_OVERFLOW,
    NX_ERROR_FIFO_UNDERFLOW,
    NX_ERROR_FIFO_VOID,        
    NX_ERROR_COMM,
            
   //send to command
    NX_ERROR_INVALID_COMMAND,
    NX_ERROR_INVALID_CONTROL,
    NX_ERROR_INVALID_FILE_ACTION,
    NX_ERROR_INVALID_PARAM,

    NX_ERROR_MAX    // <--- control limit for safety
} NX_Status_t;

/*typedefs dor build comands*/
typedef enum {
    
    //Control Events
    NX_CTRL_PAGE=0,
    NX_CTRL_REF,
    NX_CTRL_CLICK,
    NX_CTRL_GET,
    NX_CTRL_VIS,
    NX_CTRL_TSW,
    NX_CTRL_SETLAYER,
    
    //File systems
    NX_FS_DELFILE,
    NX_FS_REFILE,
    NX_FS_NEWDIR,
    NX_FS_DELDIR,
    NX_FS_REDIR,
    NX_FS_FINDFILE,
    NX_FS_RDFILE,
    
    //Stream string      
    NX_STR_COVX,
    NX_STR_SUBSTR,
    NX_STR_SPSTR,
            
    //Strings 
    NX_LEN_STRLEN,
    NX_LEN_BTLEN,
            
    //WAVEFORM        
    NX_WAVE_ADD,
    NX_WAVE_CLE,
            
    //wave stream
    NX_STRM_WAVE,

    //GPIO
    NX_GPIO_CFGPIO,
            
    //audio
    NX_CMD_PLAY,

    //ucopy
    NX_CMD_UCOPY,
    NX_CMD_UDELETE,        
            
    NX_CMD_MAX // <--- control limit for safety
} NX_Command_t;

typedef enum{
    //EEPROM         
    NX_EEP_WRITE_OBJ=0,
    NX_EEP_READ_OBJ,
    NX_EEP_WRITE_BLOCK,
    NX_EEP_READ_BLOCK,

    NX_EEP_MAX
}NX_EepromCmd_t;

typedef enum{
    //CRC
    NX_CRC_RESET=0,
    NX_CRC_APPEND_STR,
    NX_CRC_APPEND_HEX,
    NX_CRC_APPEND_UINT,

    NX_CRC_MAX
}NX_CrcCmd_t;

typedef enum {
    NX_VAR_SYSTEM_DP = 0,
    NX_VAR_SYSTEM_DIM,
    NX_VAR_SYSTEM_DIMS,
    NX_VAR_SYSTEM_BAUD,
    NX_VAR_SYSTEM_BAUDS,
    NX_VAR_SYSTEM_SLEEP,
    NX_VAR_SYSTEM_THSP,
    NX_VAR_SYSTEM_THUP,
    NX_VAR_SYSTEM_BKCMD,
    NX_VAR_SYSTEM_SYS,

    NX_VAR_SYSTEM_MAX
}NX_SystemVar_t;

typedef enum{
    NX_STRM_FILE_NEW=0,
    NX_STRM_FILE_WRITE,

    NX_STRM_MAX
}NX_StreamCmd_t;

typedef enum{
    //Comand Events
    NX_CMD_REF_STOP = 0,
    NX_CMD_REF_START,
    NX_CMD_SENDME,
    NX_CMD_TOUCH_J,
    NX_CMD_COM_STOP,
    NX_CMD_COM_START,
    NX_CMD_CODE_C,
    NX_CMD_RESET,
    NX_CMD_DOEVENTS,

    NX_CMD_EVENTS_MAX
}NX_CommandEvents_t;

typedef enum {
    NX_GUI_CLS = 0,
    NX_GUI_PIC,
    NX_GUI_PICC,
    NX_GUI_XPIC,
    NX_GUI_XSTR,
    NX_GUI_LINE,
    NX_GUI_DRAW,
    NX_GUI_CIR,
    NX_GUI_CIRS,

    NX_GUI_MAX          
}NX_CommandGUI_t;

typedef enum {
    NX_CMD_RTC0 = 0,
    NX_CMD_RTC1,
    NX_CMD_RTC2,
    NX_CMD_RTC3,
    NX_CMD_RTC4,
    NX_CMD_RTC5,

    NX_CMD_RTC_MAX
    
}NX_RTC_Commands_t;

typedef enum{
    
    NX_CMD_VOL = 0,
    NX_CMD_AUDIO,
    NX_CMD_VID,

    NX_CMD_AUDIO_MAX
}NX_Command_Audio_t;


/**
 * @brief Explicitly sized string tracker.
 * Defends against memory leaks and buffer overflows by carrying its own boundary metadata.
 */
typedef struct {
    char *data;   // Pointer to an immutable constant character array
    uint8_t length;     // Absolute string length excluding any implicit null termination
} NX_String_t;

/*============================================================================*
 * Forward Opaque Structure Declarations (Global Architecture Shared Scope)
 *============================================================================*/
struct NX_Object;
typedef struct NX_Object NX_Object_t;

struct NX_Page;
typedef struct NX_Page NX_Page_t;

struct NX_Screen;
typedef struct NX_Screen NX_Screen_t;

struct NX_Transport;
typedef struct NX_Transport NX_Transport_t;

/*============================================================================*
 * Strictly Typed Callback Pointers (Functional Application Hooks)
 *============================================================================*/
typedef void (*NX_ObjectCallback_t)(NX_Object_t *object);
typedef void (*NX_PageCallback_t)(NX_Page_t *page);

/*============================================================================*
 * Low-Level Hardware Abstraction Layer (HAL) Pointers
 *============================================================================*/
typedef uint8_t NX_PortComm_ID_t;
//callback for TX event to driver 
typedef NX_Status_t (*NX_TxCallback_fnc)(NX_Screen_t * screen, uint8_t *buffer, uint8_t length);
//callback for RX event from driver 
typedef NX_Status_t (*NX_RxCallback_fnc)(NX_Screen_t * screen, const uint8_t *buffer, uint8_t length);
typedef NX_Status_t (*NX_EventCallback_fnc)(NX_Vector_t *eventVector);
typedef NX_Status_t (*Timer_callback_fnc)(NX_Screen_t *screen);

/**
 * @brief Callback global de escape for events not  management for SDK.
 * @param[in] device pointer to master device what receive the message
 * @param[in] pointer to un-handled vector for UART
 */
typedef void (*NX_GlobalFallbackCallback_t)(void *device_ptr, const NX_Vector_t *unhandled_vector);

#endif /* NX_TYPES_H */
