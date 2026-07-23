/**
 * @file nx_api.h
 * @brief Centralized application programming interface (API) for Nextion C Driver.
 * @note Implements high-level abstraction setters for all component types, geometry,
 *       style, and device routing tasks using a unified NX_Screen_t container.
 * @copyright Copyright (c) 2026 Jacobo Gonzalez
 * @license MIT License
 */

#ifndef H_NX_API_H
#define H_NX_API_H

#include "nx_types.h"
#include "nx_screen.h"
#include "nx_page.h"
#include "nx_transport.h"


// --- CONFIGURACIÓN DE SELECCIÓN DE HARDWARE ---
// Descomenta una única línea para el control total del firmware:
// #define NEXTION_SERIES_BASIC
//#define NEXTION_SERIES_ENHANCED
#define NEXTION_SERIES_INTELLIGENT

/*============================================================================*
 * Core
 *============================================================================*/

/**
 * @brief Initializes the unified device instance and attaches hardware transmission hooks.
 */
NX_Status_t NX_Init(NX_Screen_t *screen);

/*============================================================================*
 * drag
 *============================================================================*/

NX_Status_t NX_SetDrag(NX_Screen_t *screen, NX_Object_t * object, bool state);

/*============================================================================*
 * aph
 *============================================================================*/

NX_Status_t NX_SetAph(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * effect
 *============================================================================*/

NX_Status_t NX_SetEffect(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * font
 *============================================================================*/

NX_Status_t NX_SetFont(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * bco
 *============================================================================*/

NX_Status_t NX_SetBco(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);


/*============================================================================*
 * bco1
 *============================================================================*/

NX_Status_t NX_SetBco1(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);

/*============================================================================*
 * bco2
 *============================================================================*/

NX_Status_t NX_SetBco2(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);

/*============================================================================*
 * pco
 *============================================================================*/

NX_Status_t NX_SetPco(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);

/*============================================================================*
 * pco0
 *============================================================================*/

NX_Status_t NX_SetPco0(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);

/*============================================================================*
 * pco1
 *============================================================================*/

NX_Status_t NX_SetPco1(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);

/*============================================================================*
 * pco2
 *============================================================================*/

NX_Status_t NX_SetPco2(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);

/*============================================================================*
 * xcen
 *============================================================================*/

NX_Status_t NX_SetXcen(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * ycen
 *============================================================================*/

NX_Status_t NX_SetYcen(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);


/*============================================================================*
 * pw
 *============================================================================*/

NX_Status_t NX_SetPw(NX_Screen_t *screen, NX_Object_t * object, bool status);

/*============================================================================*
 * Text
 *============================================================================*/

/**
 * @brief Sets a textual attribute of a validated component.
 * NX_ATTR_TXT
 */
NX_Status_t NX_SetText(NX_Screen_t *screen, const NX_Object_t *object, 
        const NX_String_t *buffer);

/*============================================================================*
 * Text
 *============================================================================*/

/*=============================================================================
 * Path
 =============================================================================*/
NX_Status_t NX_SetPath(NX_Screen_t *screen, const NX_Object_t *object, 
        NX_Attribute_t attr, const NX_String_t *buffer);

/*============================================================================*
 * isbr
 *============================================================================*/

NX_Status_t NX_SetIsbr(NX_Screen_t *screen, NX_Object_t * object, bool status);


/*============================================================================*
 * spax
 *============================================================================*/

NX_Status_t NX_SetSpax(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * spay
 *============================================================================*/

NX_Status_t NX_SetSpay(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * x
 *============================================================================*/

NX_Status_t NX_SetX(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);

/*============================================================================*
 * Y
 *============================================================================*/

NX_Status_t NX_SetY(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);

/*============================================================================*
 * dir
 *============================================================================*/

NX_Status_t NX_SetDir(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * dis
 *============================================================================*/

NX_Status_t NX_SetDis(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);

/*============================================================================*
 * tim
 *============================================================================*/

NX_Status_t NX_SetTim(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);

/*============================================================================*
 * en
 *============================================================================*/

NX_Status_t NX_SetEn(NX_Screen_t *screen, NX_Object_t * object, bool status);


/*============================================================================*
 * val
 *============================================================================*/

NX_Status_t NX_SetVal(NX_Screen_t *screen, NX_Object_t * object, int32_t value);

/*============================================================================*
 * lenth
 *============================================================================*/

NX_Status_t NX_SetLenth(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * format
 *============================================================================*/

NX_Status_t NX_SetFormat(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * ws0
 *============================================================================*/

NX_Status_t NX_SetWs0(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * ws1
 *============================================================================*/

NX_Status_t NX_SetWs1(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * ws2
 *============================================================================*/

NX_Status_t NX_SetWs2(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * pic
 *============================================================================*/

NX_Status_t NX_SetPic(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * pic1
 *============================================================================*/

NX_Status_t NX_SetPic1(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * pic2
 *============================================================================*/

NX_Status_t NX_SetPic2(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * wid
 *============================================================================*/

NX_Status_t NX_SetWid(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * hig
 *============================================================================*/

NX_Status_t NX_SetHig(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * maxval
 *============================================================================*/

NX_Status_t NX_SetMaxval(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);


/*============================================================================*
 * minval
 *============================================================================*/

NX_Status_t NX_SetMinval(NX_Screen_t *screen, NX_Object_t * object, uint16_t value);

/*============================================================================*
 * ch
 *============================================================================*/

NX_Status_t NX_SetCh(NX_Screen_t *screen, NX_Object_t * object, uint8_t value);

/*============================================================================*
 * up
 *============================================================================*/

NX_Status_t NX_SetUp(NX_Screen_t *screen, NX_Object_t * object, int32_t value);


/*============================================================================*
 * down
 *============================================================================*/

NX_Status_t NX_SetDown(NX_Screen_t *screen, NX_Object_t * object, int32_t value);

/*============================================================================*
 * left
 *============================================================================*/

NX_Status_t NX_SetLeft(NX_Screen_t *screen, NX_Object_t * object, int32_t value);

/*============================================================================*
 * gdc
 *============================================================================*/

NX_Status_t NX_SetGdc(NX_Screen_t *screen, NX_Object_t * object, int32_t value);


/*============================================================================*
 * gdh
 *============================================================================*/

NX_Status_t NX_SetGdh(NX_Screen_t *screen, NX_Object_t * object, int32_t value);

/*=============================================================================
 *Comands 
 ==============================================================================*/
/*
 * ref_stop (Detener refresco de Waveform)
 * ref_star o ref_start (Reanudar refresco de Waveform)
 * sendme (Pedir el ID de la página actual)
 * touch_j (Iniciar calibración de pantalla resistiva)
 * com_stop / com_star (Detener/Reanudar ejecución de la UART)
 * code_c (Limpiar buffer de comandos de la pantalla)
 * rest (Reiniciar/Rebootear dispositivo Nextion)
 * doevents (Forzar refresco visual inmediato)
 */

NX_Status_t NX_ExecuteCommand(NX_Screen_t *screen, NX_CommandEvents_t cmd);

/*============================================================================*
 * Set control commands
 *============================================================================*/
/*
 * page (change the active page)
*/

NX_Status_t NX_setPage(NX_Screen_t *screen, NX_Page_t *page);

/*
 ref (refresh component)
 */

NX_Status_t NX_setRef(NX_Screen_t *screen, NX_Object_t *object);

/*
 *Click click (Simulate touch press/release software component)
 */

NX_Status_t NX_setClick(NX_Screen_t *screen, NX_Object_t *object, bool status);

/*
 *get (get a value from nextion, responds 0x70 or 0x71)
 */
NX_Status_t NX_dataGet(NX_Screen_t *screen, NX_Object_t *object, NX_Attribute_t attr);

/*
  *vis (hide/show a component on execution)
 */
NX_Status_t NX_setVis(NX_Screen_t *screen, NX_Object_t *object, bool status);

/*
 * tsw (enable/disable touch events on widget)
 */
NX_Status_t NX_setTsw(NX_Screen_t *screen, NX_Object_t *object, bool status);



/*The next instruccions only could be used in Intelligent & Edge Series*/

/**
 * setlayer (Move a component in a layer specific)
 */
NX_Status_t NX_setLayer(NX_Screen_t *screen, NX_Object_t *object1, NX_Object_t *object2);

/*
 *delfile (delete external file)
*/

NX_Status_t NX_delfile(NX_Screen_t *screen, const char *path1, const char *path2);

/*
 *refile (rename external file)
 */
NX_Status_t NX_relfile(NX_Screen_t *screen, const char *path1, const char *path2);

/*
 *newdir (Crear una nueva carpeta/directorio)
*/
NX_Status_t NX_newdir(NX_Screen_t *screen, const char *path1);

/*
 * deldir (Eliminar una carpeta)
*/
NX_Status_t NX_deldir(NX_Screen_t *screen, const char *path1);

/*
 * redir (Renombrar un directorio)
*/

NX_Status_t NX_redir(NX_Screen_t *screen, const char *path1, const char *path2);

/*
  * findfile
*/
NX_Status_t NX_findfile(NX_Screen_t *screen, const char *path1);


/**
 * @brief Sends real-time streaming data points or clearance signals to a specific Waveform channel.
 * @details Handles high-frequency graphic channel feeding ('add') and channel emptying ('cle')
 *          with zero string-searching overhead.
 * 
 * @param[in] screen    Pointer to the active HMI screen layout instance.
 * @param[in] action    The waveform add value
 * @param[in] waveId    The unique hardware ID of the target Waveform component component.
 * @param[in] channelId The index of the targeted channel inside the graph (0 to 3, or 255 for all channels in 'cle').
 * @param[in] value     The integer raw numeric value to feed (Ignored if action is NX_WAVE_CLE).
 * 
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_waveformAdd(NX_Screen_t *screen, NX_Object_t *object, 
        uint8_t channelId, uint8_t value);

/**
 * @brief Sends real-time streaming data points or clearance signals to a specific Waveform channel.
 * @details Handles high-frequency graphic channel feeding ('add') and channel emptying ('cle')
 *          with zero string-searching overhead.
 * 
 * @param[in] screen    Pointer to the active HMI screen layout instance.
 * @param[in] action    The waveform addt value
 * @param[in] waveId    The unique hardware ID of the target Waveform component component.
 * @param[in] channelId The index of the targeted channel inside the graph (0 to 3, or 255 for all channels in 'cle').
 * @param[in] value     The integer quantity of bytes send
 * 
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_waveformAddt(NX_Screen_t *screen, NX_Object_t *object, 
        uint8_t channelId, uint8_t qty);

/**
 * @brief Sends real-time streaming data points or clearance signals to a specific Waveform channel.
 * @details Handles high-frequency graphic channel feeding ('add') and channel emptying ('cle')
 *          with zero string-searching overhead.
 * 
 * @param[in] screen    Pointer to the active HMI screen layout instance.
 * @param[in] action    The waveform addt value
 * @param[in] waveId    The unique hardware ID of the target Waveform component component.
 * @param[in] channelId The index of the targeted channel inside the graph (0 to 3, or 255 for all channels in 'cle').
 * 
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_waveformCle(NX_Screen_t *screen, NX_Object_t *object, uint8_t channelId);

/**
 * @brief Interfaces with the internal Nextion hardware CRC engine accumulator.
 * @details Automates the safety validation pipeline to verify command chain checksums natively.
 * 
 * @param[in] screen Pointer to the active HMI screen layout instance.
 * @param[in] action The targeted CRC calculation mode (NX_CrcCmd_t index).
 * @param[in] value  The payload value string, hex array, or numeric value to feed into the accumulator (Pass NULL if NX_CRC_RESET).
 * 
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_ControlCrcEngine(NX_Screen_t *screen, NX_CrcCmd_t action, const char *value);


/**
 * @brief Prepares Nextion to open a transparent binary buffer pipeline for streaming raw bytes.
 * @details Opens data transmission tunnels for high-speed chart plot arrays ('addt') or SD card file manipulation blocks ('twfile').
 * 
 * @param[in] screen Pointer to the active HMI screen layout instance.
 * @param[in] action The data stream mode command selector (NX_StreamCmd_t index).
 * @param[in] target Target channel or filename setup descriptor string (e.g., "1,0" or "sd0/log.bin").
 * @param[in] size   The exact number of raw unformatted binary bytes the dsPIC33 will send immediately following this instruction.
 * 
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_OpenDataStream(NX_Screen_t *screen, NX_StreamCmd_t action, const char *target, uint32_t size);


/**
 * @brief Copies bytes from the internal Nextion UART receive circular buffer into a display variable.
 * @details This command is exclusively operational during Protocol Reparse mode execution.
 * 
 * @param[in] screen     Pointer to the active HMI screen layout instance.
 * @param[in] targetVar  The internal display variable name to receive the payload data (e.g., "va0.txt").
 * @param[in] startIndex The zero-based byte index pointer position inside the display UART buffer.
 * @param[in] length     The exact number of raw bytes to copy.
 * @param[in] mode       The parsing mode configuration (0 for standard ASCII, 1 for raw/escaped strings).
 * 
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_UartBufferCopy(NX_Screen_t *screen, const char *targetVar, 
        uint16_t startIndex, uint16_t length, uint8_t mode);


/**
 * @brief Removes and drops a specific number of raw bytes from the Nextion UART receive circular buffer.
 * @details Vital to clear remaining junk bytes or process customized communication protocols.
 * 
 * @param[in] screen Pointer to the active HMI screen layout instance.
 * @param[in] length The exact number of bytes to flush out of the buffer matrix.
 * 
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_UartBufferDelete(NX_Screen_t *screen, uint16_t length);



/**
 * @brief Reads a specific block of bytes from an SD card file into a display internal buffer/variable.
 * 
 * @param[in] screen      Pointer to the active HMI screen layout instance.
 * @param[in] filePath    The filename path wrapped in quotes natively by the function (e.g., "sd0/data.bin").
 * @param[in] targetVar   The internal display text variable or buffer name to load with the payload bytes.
 * @param[in] startOffset The starting byte address offset pointer inside the physical file.
 * @param[in] length      The total number of raw bytes to read from the storage stream.
 * 
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_ReadFileFromSd(NX_Screen_t *screen, const char *filePath, 
        const char *targetVar, uint32_t startOffset, uint16_t length);


/*=============================================================================
 * GUI commands
=============================================================================== */


/**
 * @brief Clears the screen and fills it entirely with a specified color.
 * @param[in] screen Pointer to the active HMI screen layout instance.
 * @param[in] color  The RGB565 decimal value or hardware color constant.
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_ClearScreen(NX_Screen_t *screen, uint16_t color);


/**
 * @brief Displays a Resource Picture asset at the specified coordinates.
 * @param[in] screen Pointer to the active HMI screen layout instance.
 * @param[in] x      Upper left corner X coordinate.
 * @param[in] y      Upper left corner Y coordinate.
 * @param[in] picId  The asset identification resource index number.
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_DrawPicture(NX_Screen_t *screen, uint16_t x, uint16_t y, uint16_t picId);


/**
 * @brief Crops an area from a full-screen Resource Picture and renders it at the same coordinates.
 * @param[in] screen Pointer to the active HMI screen layout instance.
 * @param[in] x      Upper left corner X coordinate of the defined crop area.
 * @param[in] y      Upper left corner Y coordinate of the defined crop area.
 * @param[in] w      Width of the defined crop area.
 * @param[in] h      Height of the defined crop area.
 * @param[in] picId  The asset identification resource index number.
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_CropPicture(NX_Screen_t *screen, uint16_t x, uint16_t y, 
        uint16_t w, uint16_t h, uint16_t picId);


/**
 * @brief Crops an area from a source Resource Picture and renders it at a different destination coordinate.
 * @param[in] screen Pointer to the active HMI screen layout instance.
 * @param[in] dstX   Destination upper left corner X coordinate.
 * @param[in] dstY   Destination upper left corner Y coordinate.
 * @param[in] w      Width of the defined crop area.
 * @param[in] h      Height of the defined crop area.
 * @param[in] srcX   Source crop window origin X coordinate.
 * @param[in] srcY   Source crop window origin Y coordinate.
 * @param[in] picId  The asset identification resource index number.
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_DrawPictureAdvanced(NX_Screen_t *screen, uint16_t dstX, 
        uint16_t dstY, uint16_t w, uint16_t h, uint16_t srcX, uint16_t srcY, uint16_t picId);


/**
 * @brief Prints customized text strings on the device within a defined bounding region.
 * @param[in] screen   Pointer to the active HMI screen layout instance.
 * @param[in] x        Bounding area starting X position coordinate.
 * @param[in] y        Bounding area starting Y position coordinate.
 * @param[in] w        Width boundary constraint of the text box.
 * @param[in] h        Height boundary constraint of the text box.
 * @param[in] fontId   The Flash-loaded Font Resource index number.
 * @param[in] fColor   Foreground font text color (RGB565 or constant).
 * @param[in] bColor   Background color value OR background picId.
 * @param[in] hAlign   Horizontal Alignment (0=Left, 1=Centered, 2=Right).
 * @param[in] vAlign   Vertical Alignment (0=Top, 1=Center, 3=Bottom).
 * @param[in] backFill Background filling style (0=Crop, 1=Solid, 2=Image, 3=None).
 * @param[in] text     The string literal or Nextion text attribute pointer (e.g. "\"Label\"", "va0.txt").
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_PrintTextAdvanced(NX_Screen_t *screen, uint16_t x, uint16_t y, 
        uint16_t w, uint16_t h, uint8_t fontId, uint16_t fColor, uint16_t bColor, 
        uint8_t hAlign, uint8_t vAlign, uint8_t backFill, const char *text);


/**
 * @brief Draws a single point-to-point geometric vector line with a specified color.
 * @param[in] screen Pointer to the active HMI screen layout instance.
 * @param[in] x1     Starting X point coordinate.
 * @param[in] y1     Starting Y point coordinate.
 * @param[in] x2     Ending X point coordinate.
 * @param[in] y2     Ending Y point coordinate.
 * @param[in] color  The vector line color (RGB565 or constant).
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_DrawLine(NX_Screen_t *screen, uint16_t x1, uint16_t y1, 
        uint16_t x2, uint16_t y2, uint16_t color);

/**
 * @brief Draws a hollow rectangular frame border around a specified area.
 * @param[in] screen Pointer to the active HMI screen layout instance.
 * @param[in] x1     Upper left corner starting X point coordinate.
 * @param[in] y1     Upper left corner starting Y point coordinate.
 * @param[in] x2     Lower right corner ending X point coordinate.
 * @param[in] y2     Lower right corner ending Y point coordinate.
 * @param[in] color  The rectangle border line color (RGB565 or constant).
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_DrawHollowRectangle(NX_Screen_t *screen, uint16_t x1, uint16_t y1, 
        uint16_t x2, uint16_t y2, uint16_t color);


/**
 * @brief Draws a hollow circle frame layout using a center focus point and pixel radius.
 * @param[in] screen Pointer to the active HMI screen layout instance.
 * @param[in] cx     The X coordinate point of the circle center focal point.
 * @param[in] cy     The Y coordinate point of the circle center focal point.
 * @param[in] radius The outer radius boundary distance in pixels.
 * @param[in] color  The circle border line color (RGB565 or constant).
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_DrawHollowCircle(NX_Screen_t *screen, uint16_t cx, uint16_t cy, 
        uint16_t radius, uint16_t color);



/**
 * @brief Draws a solid filled-color circle matrix using a center focus point and 
 * pixel radius.
 * @param[in] screen Pointer to the active HMI screen layout instance.
 * @param[in] cx     The X coordinate point of the circle center focal point.
 * @param[in] cy     The Y coordinate point of the circle center focal point.
 * @param[in] radius The outer radius boundary distance in pixels.
 * @param[in] color  The internal matrix circle fill color (RGB565 or constant).
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_DrawFilledCircle(NX_Screen_t *screen, uint16_t cx, uint16_t cy, 
        uint16_t radius, uint16_t color);

/*=============================================================================
 *System Variables
 ==============================================================================*/

/*
 *dp chages of page, same functionality that page
 */

NX_Status_t NX_setDP(NX_Screen_t *screen, NX_Page_t * page);

/**
 * @brief Sets the display backlight brightness level temporarily.
 * @details This setting is volatile and will reset to default upon power cycle.
 * @param[in] screen     Pointer to the active HMI screen layout instance.
 * @param[in] percentage Brightness level from 0 (backlight off) to 100 (maximum brightness).
 * @return NX_Status_t   Status code of the transmission operation.
 */
NX_Status_t NX_SetBrightness(NX_Screen_t *screen, uint8_t percentage);

/**
 * @brief Sets the default power-on display backlight brightness level permanently.
 * @details Writes the value directly into the Nextion internal non-volatile Flash memory.
 * @param[in] screen     Pointer to the active HMI screen layout instance.
 * @param[in] percentage Brightness level from 0 to 100.
 * @return NX_Status_t   Status code of the transmission operation.
 */
NX_Status_t NX_SetBrightnessPersistent(NX_Screen_t *screen, uint8_t percentage);


/**
 * @brief Changes the display UART baud rate immediately for the current session.
 * @details This is a volatile change. Nextion reverts to default baud rate upon reboot.
 * @param[in] screen   Pointer to the active HMI screen layout instance.
 * @param[in] baudrate Standard baud rate value (e.g., 9600, 115200, etc.).
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_SetBaudRate(NX_Screen_t *screen, uint32_t baudrate);

/**
 * @brief Sets the default power-on UART baud rate permanently.
 * @details Saves the configuration directly into Nextion's non-volatile Flash memory.
 * @param[in] screen   Pointer to the active HMI screen layout instance.
 * @param[in] baudrate Standard baud rate value (e.g., 9600, 115200, etc.).
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_SetBaudRatePersistent(NX_Screen_t *screen, uint32_t baudrate);


/**
 * @brief Forces the display to immediately enter or exit sleep low-power mode.
 * @param[in] screen   Pointer to the active HMI screen layout instance.
 * @param[in] enable   Set to 1 to force sleep mode, or 0 to wake up.
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_SetSleepMode(NX_Screen_t *screen, bool enable);

/**
 * @brief Configures the automatic sleep timeout timer due to touch inactivity.
 * @param[in] screen       Pointer to the active HMI screen layout instance.
 * @param[in] timeoutSecs  Inactivity time in seconds before entering sleep mode (Use 0 to disable auto-sleep).
 * @return NX_Status_t     Status code of the transmission operation.
 */
NX_Status_t NX_SetAutoSleepTimeout(NX_Screen_t *screen, uint16_t timeoutSecs);

/**
 * @brief Configures if touching the screen wakes the display up from sleep mode automatically.
 * @param[in] screen   Pointer to the active HMI screen layout instance.
 * @param[in] enable   Set to 1 to enable wake-on-touch, or 0 to disable it.
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_SetWakeOnTouch(NX_Screen_t *screen, bool enable);

/**
 * @brief Configures Nextion's UART return command echo/response level back to the dsPIC33.
 * @details Adjusts what notification bytes Nextion transmits upon executing instructions.
 *          0 = No return data at all.
 *          1 = Only return success status.
 *          2 = Only return execution failed/error statuses.
 *          3 = Always return both success and failure notification frames.
 * @param[in] screen   Pointer to the active HMI screen layout instance.
 * @param[in] level    The echo verbosity level (0 to 3).
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_SetUartResponseLevel(NX_Screen_t *screen, uint8_t level);

/**
 * @brief Writes an integer value into one of Nextion's global system registers.
 * @param[in] screen   Pointer to the active HMI screen layout instance.
 * @param[in] regId    The specific register index number (0 for sys0, 1 for sys1, 2 for sys2).
 * @param[in] value    The 32-bit signed integer value to store.
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_WriteSystemRegister(NX_Screen_t *screen, uint8_t regId, int32_t value);


// --- Prototipos Avanzados Condicionales ---
#if defined(NEXTION_SERIES_ENHANCED) || defined(NEXTION_SERIES_INTELLIGENT)
/**
 * @brief Sets the Nextion internal hardware RTC clock time (Hours, Minutes, Seconds).
 * @param[in] screen  Pointer to the active HMI screen layout instance.
 * @param[in] hours   Hour value (24-hour format: 0 to 23).
 * @param[in] minutes Minute value (0 to 59).
 * @param[in] seconds Second value (0 to 59).
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_SetRtcTime(NX_Screen_t *screen, uint8_t hours, uint8_t minutes, uint8_t seconds);

/**
 * @brief Sets the Nextion internal hardware RTC calendar date (Year, Month, Day).
 * @param[in] screen Pointer to the active HMI screen layout instance.
 * @param[in] year   Four-digit calendar year value (e.g., 2026).
 * @param[in] month  Month value (1 to 12).
 * @param[in] day    Day value (1 to 31).
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_SetRtcDate(NX_Screen_t *screen, uint16_t year, uint8_t month, uint8_t day);
#endif

#if defined(NEXTION_SERIES_INTELLIGENT)
/**
 * @brief Adjusts the global hardware speaker volume of the Nextion device.
 * @param[in] screen    Pointer to the active HMI screen layout instance.
 * @param[in] volumePct Volume level percentage from 0 (mute) to 100 (maximum output).
 * @return NX_Status_t  Status code of the transmission operation.
 */
NX_Status_t NX_SetAudioVolume(NX_Screen_t *screen, uint8_t volumePct);
/**
 * @brief Adjusts the global hardware speaker volume of the Nextion device.
 * @param[in] screen    Pointer to the active HMI screen layout instance.
 * @param[in] volumePct Volume level percentage from 0 (mute) to 100 (maximum output).
 * @return NX_Status_t  Status code of the transmission operation.
 */
NX_Status_t NX_SetAudioSlotStatus(NX_Screen_t *screen, uint8_t slotId, uint8_t status);
/**
 * @brief Changes the target playlist video resource ID currently allocated for background playback.
 * @param[in] screen  Pointer to the active HMI screen layout instance.
 * @param[in] videoId Video resource index identifier inside the local flash memory.
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_SetVid(NX_Screen_t *screen, uint16_t videoId);


/**
 * @brief Interfaces with the Nextion display's on-board non-volatile EEPROM storage.
 * @details Manages object property tracking ('wepo' / 'repo') and block memory array streams ('wept' / 'rept').
 * 
 * @param[in] screen  Pointer to the active HMI screen layout instance.
 * @param[in] action  The targeted EEPROM operation (NX_EepromCmd_t index).
 * @param[in] target  Object property name for single tracking (e.g., "va0.val") OR starting EEPROM RAM address for block transfers.
 * @param[in] length  The quantity size in bytes to write/read (Only used for block-based WEPT/REPT commands).
 * 
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_ControlEeprom(NX_Screen_t *screen, NX_EepromCmd_t action, 
        const char *target, uint16_t length);

/**
 * @brief Configures and drives the physical hardware GPIO pins located on the Nextion display panel.
 * @details Executes the 'cfgpio' hardware instruction to alter pin modes, PWM duty cycles, or pull-up configurations.
 * 
 * @param[in] screen  Pointer to the active HMI screen layout instance.
 * @param[in] pinId   The targeted physical GPIO pin index number on the HMI panel.
 * @param[in] mode    The mode selection byte (0=Binding Input, 1=Output, 2=PWM Output, 3=Analog Input, etc.).
 * @param[in] binding Component target name or component ID string if configured as an input bind (Pass NULL if unused).
 * 
 * @return NX_Status_t Status code of the transmission operation.
 */
NX_Status_t NX_ConfigureGpio(NX_Screen_t *screen, uint8_t pinId, uint8_t mode, 
        const char *binding);
#endif




#endif /* H_NX_API_H */