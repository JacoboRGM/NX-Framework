s/**
 * @file nx_api.c
 * @brief Direct property execution logic and asymmetric vector routing tables.
 * @author Jacobo Gonzalez
 * @date July 5, 2026
 */

#include <stdio.h>
#include <string.h>
#include "nx_api.h"
#include "nx_component.h"   
#include "nx_attributes.h"
#include "nx_object.h"
#include "nx_transport.h"

#define NEXTION_SERIES_INTELLIGENT


/* Preallocated scratch command rendering buffer (64 bytes) to ensure stack safety inside dsPIC RAM */
//no touch this buffer out of this file
static char api_scratch_buffer[NX_MAX_VECTOR_LEN];

static const NX_String_t NX_CommandEventsTable [NX_CMD_EVENTS_MAX] = {
    //Comand Events
    [NX_CMD_REF_STOP]  = { .data = "ref_stop", .length = sizeof("ref_stop") - 1 },
    [NX_CMD_REF_START] = { .data = "ref_star", .length = sizeof("ref_star") - 1 },
    [NX_CMD_SENDME]    = { .data = "sendme",   .length = sizeof("sendme")   - 1 },
    [NX_CMD_TOUCH_J]   = { .data = "touch_j",  .length = sizeof("touch_j")  - 1 },
    [NX_CMD_COM_STOP]  = { .data = "com_stop", .length = sizeof("com_stop") - 1 },
    [NX_CMD_COM_START] = { .data = "com_star", .length = sizeof("com_star") - 1 },
    [NX_CMD_CODE_C]    = { .data = "code_c",   .length = sizeof("code_c")   - 1 },
    [NX_CMD_RESET]     = { .data = "rest",     .length = sizeof("rest")     - 1 },
    [NX_CMD_DOEVENTS]  = { .data = "doevents", .length = sizeof("doevents") - 1 },
};

//command tables nam
static const NX_String_t NX_CommandTable[NX_CMD_MAX] = {
    
    //Control Events
    [NX_CTRL_PAGE]     = { .data = "page",     .length = sizeof("page")     - 1 },
    [NX_CTRL_REF]      = { .data = "ref",      .length = sizeof("ref")      - 1 },
    [NX_CTRL_CLICK]    = { .data = "click",    .length = sizeof("click")    - 1 },
    [NX_CTRL_GET]      = { .data = "get",      .length = sizeof("get")      - 1 },
    [NX_CTRL_VIS]      = { .data = "vis",      .length = sizeof("vis")      - 1 },
    [NX_CTRL_TSW]      = { .data = "tsw",      .length = sizeof("tsw")      - 1 },
    [NX_CTRL_SETLAYER] = { .data = "setlayer", .length = sizeof("setlayer") - 1 },
    
    //File systems
    [NX_FS_DELFILE] = { .data = "delfile", .length = sizeof("delfile")      - 1 },
    [NX_FS_REFILE]  = { .data = "refile",  .length = sizeof("refile")       - 1 },
    [NX_FS_NEWDIR]  = { .data = "newdir",  .length = sizeof("newdir")       - 1 },
    [NX_FS_DELDIR]  = { .data = "deldir",  .length = sizeof("deldir")       - 1 },
    [NX_FS_REDIR]   = { .data = "redir",   .length = sizeof("redir")        - 1 },
    [NX_FS_FINDFILE]= { .data = "findfile",.length = sizeof("findfile")     - 1 },
    [NX_FS_RDFILE]  = { .data = "rdfile",  .length = sizeof("rdfile")       - 1 },

    //Stream string
    [NX_STR_COVX]   = { .data = "covx",   .length = sizeof("covx")          - 1 },
    [NX_STR_SUBSTR] = { .data = "substr", .length = sizeof("substr")        - 1 },
    [NX_STR_SPSTR]  = { .data = "spstr",  .length = sizeof("spstr")         - 1 },
    
    //Strings
    [NX_LEN_STRLEN] = { .data = "strlen", .length = sizeof("strlen")        - 1 },
    [NX_LEN_BTLEN]  = { .data = "btlen",  .length = sizeof("btlen")         - 1 },
    
    //WAVEFORM
    [NX_WAVE_ADD]   = { .data = "add", .length = sizeof("add")              - 1 },
    [NX_WAVE_CLE]   = { .data = "cle", .length = sizeof("cle")              - 1 },
    
    //wave stream
    [NX_STRM_WAVE]  = { .data = "addt",   .length = sizeof("addt")          - 1 },

    
    //GPIO
    [NX_GPIO_CFGPIO] = { .data = "cfgpio", .length = sizeof("cfgpio")       - 1 },
    
    //ucopy
    [NX_CMD_UCOPY]   = { .data = "ucopy", .length = sizeof("ucopy")         - 1 },
    [NX_CMD_UDELETE] = { .data = "udelete", .length = sizeof("udelete")     - 1 },
    //audio
    [NX_CMD_PLAY]    = { .data = "play",   .length = sizeof("play")         - 1 }
};

//Systema variables table names
static const NX_String_t NX_SystemVarTable[NX_VAR_SYSTEM_MAX] = {
    
    [NX_VAR_SYSTEM_DP] = {.data = "dp", .length = sizeof ("dp") - 1},
    [NX_VAR_SYSTEM_DIM] = {.data = "dim", .length = sizeof ("dim") - 1},
    [NX_VAR_SYSTEM_DIMS] = {.data = "dims", .length = sizeof ("dims") - 1},
    [NX_VAR_SYSTEM_BAUD] = {.data = "baud", .length = sizeof("baud") - 1},
    [NX_VAR_SYSTEM_BAUDS] = {.data = "bauds", .length = sizeof("bauds") - 1},
    [NX_VAR_SYSTEM_SLEEP] = {.data = "sleep", .length = sizeof("sleep") - 1},
    [NX_VAR_SYSTEM_THSP] = {.data = "thsp", .length = sizeof("thsp") - 1},
    [NX_VAR_SYSTEM_THUP] = {.data = "thup", .length = sizeof("thup") - 1},
    [NX_VAR_SYSTEM_BKCMD] = {.data = "bkcmd", .length = sizeof("bkcmd") - 1},
    [NX_VAR_SYSTEM_SYS] = {.data = "sys", .length = sizeof("sys") - 1}
    
};

static const NX_String_t NX_EepromCmdTable [NX_EEP_MAX] = {
    
    //EEPROM
   [NX_EEP_WRITE_OBJ]   = { .data = "wepo", .length = sizeof("wepo")       - 1 },
   [NX_EEP_READ_OBJ]    = { .data = "repo", .length = sizeof("repo")       - 1 },
   [NX_EEP_WRITE_BLOCK] = { .data = "wept", .length = sizeof("wept")       - 1 },
   [NX_EEP_READ_BLOCK]  = { .data = "rept", .length = sizeof("rept")       - 1 },
    
};

static const NX_String_t NX_CrcCmdTable [NX_CRC_MAX] ={
    //CRC
   [NX_CRC_RESET]       = { .data = "crcrest", .length = sizeof("crcrest") - 1 },
   [NX_CRC_APPEND_STR]  = { .data = "crcputs", .length = sizeof("crcputs") - 1 },
   [NX_CRC_APPEND_HEX]  = { .data = "crcputh", .length = sizeof("crcputh") - 1 },
   [NX_CRC_APPEND_UINT] = { .data = "crcputu", .length = sizeof("crcputu") - 1 },
};

static const NX_String_t NX_StreamCmdTable [NX_STRM_MAX] ={
   [NX_STRM_FILE_NEW]   = { .data = "newfile",.length = sizeof("newfile")  - 1 },
   [NX_STRM_FILE_WRITE] = { .data = "twfile", .length = sizeof("twfile")   - 1 },
};

static const NX_String_t NX_CommandGUI_Table [NX_GUI_MAX] = {
    
  [NX_GUI_CLS]  = {.data = "cls",   .length = sizeof("cls")   -   1},
  [NX_GUI_PIC]  = {.data = "pic",   .length = sizeof("pic")   -   1},
  [NX_GUI_PICC] = {.data = "picc",  .length = sizeof("picc")  -   1},
  [NX_GUI_XPIC] = {.data = "xpic",  .length = sizeof("xpic")  -   1},
  [NX_GUI_XSTR] = {.data = "xstr",  .length = sizeof("xstr")  -   1},
  [NX_GUI_LINE] = {.data = "line",  .length = sizeof("line")  -   1},
  [NX_GUI_DRAW] = {.data = "draw",  .length = sizeof("draw")  -   1},
  [NX_GUI_CIR]  = {.data = "cir",   .length = sizeof("cir")   -   1},
  [NX_GUI_CIRS] = {.data = "cirs",  .length = sizeof("cirs")  -   1}
    
};

static const NX_String_t NX_CommandRTC [NX_CMD_RTC_MAX] = {
  [NX_CMD_RTC0]  = {.data = "rtc0", sizeof("rtc0")  -   1},
  [NX_CMD_RTC1]  = {.data = "rtc1", sizeof("rtc1")  -   1},
  [NX_CMD_RTC2]  = {.data = "rtc2", sizeof("rtc2")  -   1},
  [NX_CMD_RTC3]  = {.data = "rtc3", sizeof("rtc3")  -   1},
  [NX_CMD_RTC4]  = {.data = "rtc4", sizeof("rtc4")  -   1},
  [NX_CMD_RTC5]  = {.data = "rtc5", sizeof("rtc5")  -   1},
  
};


static const NX_String_t NX_CommandAudioTable [NX_CMD_AUDIO_MAX] = {
  [NX_CMD_VOL]    = {.data = "vol", .length = sizeof("vol")  -   1},
  [NX_CMD_AUDIO]  = {.data = "audio", .length = sizeof("audio")  -   1},
  [NX_CMD_VID]    = {.data = "vid", .length = sizeof("vid")  -   1},
};


/*============================================================================*
 * Core & Execution API
 *============================================================================*/
static NX_Status_t NX_ValidateTransport(NX_Transport_t *transport)
{
    if(transport == NULL)
        return NX_ERROR_NO_TRANSPORT;


    if(transport->tx_hardware_func == NULL)
        return NX_ERROR_NO_WRITE_CALLBACK;


    if(transport->rx_hardware_func == NULL)
        return NX_ERROR_NO_READ_CALLBACK;


    return NX_OK;
}

static NX_Status_t NX_ValidatePages(NX_Screen_t *screen)
{
    if(screen->pages == NULL)
        return NX_ERROR_NO_PAGES;


    for(uint16_t i = 0; i < screen->pageCount; i++)
    {
        if(screen->pages[i]->pageName.data == NULL)
        {
            return NX_ERROR_INVALID_PAGE;
        }
    }

    return NX_OK;
}

//t//This api connect the layer with transport_layer
static NX_Status_t NX_APITransporter(NX_Screen_t *screen, const char * buffer, 
        int length){
    
    if (length <= 0)
        return NX_ERROR_BUFFER_OVERFLOW;
    
    return NX_Transport_TxCallback(screen, (uint8_t *)buffer, length);
}

static NX_Status_t NX_APIAssignNumber(NX_Screen_t *screen, NX_Object_t *object,
        NX_Attribute_t attribute, int32_t value)
{
    //safe guard for null pointers
    if((screen == NULL) || (object == NULL))
        return NX_ERROR_NULL_POINTER;
    
    /* attribute validate */
    if (attribute >= NX_ATTR_MAX)
        return NX_ERROR_NO_ATTR_FOUND;
    
    const NX_AttributeInfo_t *attrInfo;

    if (!NX_PageContainsObject(screen->currentPage, object))
        return NX_ERROR_OBJECT_NOT_FOUND;

   
    if (!NX_ComponentHasAttribute(object->component, attribute))
        return NX_ERROR_NO_ATTR_OBJECT_FOUND;

    attrInfo = NX_GetAttributeInfo(attribute);

    if (attrInfo == NULL)
        return NX_ERROR_NO_ATTR_OBJECT_FOUND;

    /* Construir comando */

    int length = snprintf(
            api_scratch_buffer, 
            sizeof(api_scratch_buffer), 
            "%s.%s=%ld",
            object->objectName.data,
            attrInfo->name.data,
            (long int)value);
    
    //callback to the transporter method
    return NX_APITransporter(screen, (const char *)api_scratch_buffer, length);

}

static NX_Status_t NX_APIAssignText(NX_Screen_t *screen, const NX_Object_t *object,
        NX_Attribute_t attribute, const NX_String_t *string){
    //safe guard for null pointers
    if((screen == NULL) || (object == NULL) || (string == NULL))
        return NX_ERROR_NULL_POINTER;
    
    /* validate the attribute */
    if (attribute >= NX_ATTR_MAX)
        return NX_ERROR_NO_ATTR_FOUND;
    
    const NX_AttributeInfo_t *attrInfo;

    /* The object belong to the current page? */
    if (!NX_PageContainsObject(screen->currentPage, object))
        return NX_ERROR_OBJECT_NOT_FOUND;

    /* The component is supports the attribute? */
    if (!NX_ComponentHasAttribute(object->component, attribute))
        return NX_ERROR_NO_ATTR_OBJECT_FOUND;

    attrInfo = NX_GetAttributeInfo(attribute);

    if (attrInfo == NULL)
        return NX_ERROR_NO_ATTR_OBJECT_FOUND;
    
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer), 
            "%s.%s=\"%s\"",
            object->objectName.data, 
            attrInfo->name.data,
            string->data);
    
    return NX_APITransporter(screen, (const char *)api_scratch_buffer, length);
}


/*==============================================================================
* Init function
==============================================================================*/
//This function only check what the time-compiler declarations be correct
NX_Status_t NX_Init(NX_Screen_t *screen){
    
    NX_Status_t status;
    
    if(screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    status = NX_ValidateTransport(screen->transport);
    
     if(status != NX_OK)
        return status;

    status = NX_ValidatePages(screen);

    if(status != NX_OK)
        return status;

    screen->currentPage = screen->pages[0];

    return NX_OK;
  
}

/*==============================================================================
 * User functions for attribute
 =============================================================================*/
NX_Status_t NX_SetDrag(NX_Screen_t *screen, NX_Object_t * object, bool state){
    
    //call for the API assign number
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_DRAG,
            (int32_t) state);
}

NX_Status_t NX_SetAph(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
    
    //Maximum values
    if(value > 1027)
        value=127;

    //call for the API assign number
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_APH,
            (int32_t) value);
}

NX_Status_t NX_SetEffect(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
    
    if(value > 8)
        value=8;
    //call for the API assign number
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_EFFECT,
            (int32_t) value);
}

NX_Status_t NX_SetFont(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
   
    //call for the API assign number
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_FONT,
            (int32_t) value);
}

NX_Status_t NX_SetBco(NX_Screen_t *screen, NX_Object_t * object, uint16_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_BCO,
            (int32_t) value);
}

NX_Status_t NX_SetBco1(NX_Screen_t *screen, NX_Object_t * object, uint16_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_BCO1,
            (int32_t) value);
}

NX_Status_t NX_SetBco2(NX_Screen_t *screen, NX_Object_t * object, uint16_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_BCO2,
            (int32_t) value);
}

NX_Status_t NX_SetPco(NX_Screen_t *screen, NX_Object_t * object, uint16_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_PCO,
            (int32_t) value);
}

NX_Status_t NX_SetPco0(NX_Screen_t *screen, NX_Object_t * object, uint16_t value) {
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_PCO0,
            (int32_t) value);
}

NX_Status_t NX_SetPco1(NX_Screen_t *screen, NX_Object_t * object, uint16_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_PCO1,
            (int32_t) value);    
}

NX_Status_t NX_SetPco2(NX_Screen_t *screen, NX_Object_t * object, uint16_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_PCO2,
            (int32_t) value);
}

NX_Status_t NX_SetXcen(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
    
    if(value > 2)
        value=2;
    
     return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_XCEN,
            (int32_t) value);
}

NX_Status_t NX_SetYcen(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
    
      if(value > 2)
        value=2;
      
     return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_YCEN,
            (int32_t) value);
}

NX_Status_t NX_SetPw(NX_Screen_t *screen, NX_Object_t * object, bool status){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_PW,
            (int32_t)status);
    
}

NX_Status_t NX_SetText(NX_Screen_t *screen, const NX_Object_t *object,
        const NX_String_t *buffer) {
    
    return NX_APIAssignText(screen, object, NX_ATTR_TXT, buffer);
}

NX_Status_t NX_SetPath(NX_Screen_t *screen, const NX_Object_t *object, 
        NX_Attribute_t attr, const NX_String_t *buffer){
    
    return NX_APIAssignText(screen, object, NX_ATTR_PATH, buffer);
}

NX_Status_t NX_SetIsbr(NX_Screen_t *screen, NX_Object_t * object, bool status){
     return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_ISBR,
            (int32_t)status);
}

NX_Status_t NX_SetSpax(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_SPAX,
            (int32_t)value);
}

NX_Status_t NX_SetSpay(NX_Screen_t *screen, NX_Object_t * object, uint8_t value) {
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_SPAY,
            (int32_t) value);
}

NX_Status_t NX_SetX(NX_Screen_t *screen, NX_Object_t * object, uint16_t value){
     return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_X,
            (int32_t) value);
}

NX_Status_t NX_SetY(NX_Screen_t *screen, NX_Object_t * object, uint16_t value){
     return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_Y,
            (int32_t) value);
}

NX_Status_t NX_SetDir(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
    
    if(value > 4)
        value = 4;
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_DIR,
            (int32_t) value);
}

NX_Status_t NX_SetDis(NX_Screen_t *screen, NX_Object_t * object, uint16_t value) {
    
    if(object == NULL)
        return NX_ERROR_NULL_POINTER;
    
    if (object->component->type == NX_TYPE_STEXT) {
        if (value < 2)
            value = 2;

        if (value > 50)
            value = 50;
    }
    
    if(object->component->type == NX_TYPE_SLIDER){
        if(value > 100)
            value =100;
    }
    if(object->component->type == NX_TYPE_WAVEFORM){
        if(value < 10)
            value = 10;
        
        if(value > 1000)
            value=1000;
    }
    
    if(object->component->type == NX_TYPE_GMOV){
        if(value < 10)
            value =10;
        
        if(value > 100)
            value = 100;
    }
        
        
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_DIS,
            (int32_t) value);
}

NX_Status_t NX_SetTim(NX_Screen_t *screen, NX_Object_t * object, uint16_t value){
    
    if(object == NULL)
        return NX_ERROR_NULL_POINTER;
    
    if(object->component->type == NX_TYPE_TIMER){
        if(value < 50)
            value = 50;
    }
    else{
        if(value < 80)
            value =80;
    }
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_TIM,
            (int32_t) value);
}

NX_Status_t NX_SetEn(NX_Screen_t *screen, NX_Object_t * object, bool status){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_EN,
            (int32_t)status);
}

NX_Status_t NX_SetVal(NX_Screen_t *screen, NX_Object_t * object, int32_t value) {
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_VAL,
            value);
}

NX_Status_t NX_SetLenth(NX_Screen_t *screen, NX_Object_t * object, uint8_t value) {

    if (value > 15)
        value = 15;
    
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_LENGTH,
            (int32_t) value);
}

NX_Status_t NX_SetFormat(NX_Screen_t *screen, NX_Object_t * object, uint8_t value) {

    if(object == NULL)
        return NX_ERROR_NULL_POINTER;
    
    if (object->component->type == NX_TYPE_NUMBER)
        if (value > 3)
            value = 3;
    
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_FORMAT,
            (int32_t) value);
}

NX_Status_t NX_SetWs0(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
    
    if(object->component->type == NX_TYPE_GAUGE)
        if(value > 128)
            value=128;
  
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_WS0,
            (int32_t) value);
}

NX_Status_t NX_SetWs1(NX_Screen_t *screen, NX_Object_t * object, uint8_t value) {
    
    if (object->component->type == NX_TYPE_GAUGE)
        if (value > 128)
            value = 128;
    
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_WS1,
            (int32_t) value);
}

NX_Status_t NX_SetWs2(NX_Screen_t *screen, NX_Object_t * object, uint8_t value) {
    
    if (object->component->type == NX_TYPE_GAUGE)
        if (value > 128)
            value = 128;
    
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_WS2,
            (int32_t) value);
}

NX_Status_t NX_SetPic(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_PIC,
            (int32_t) value);    
}

NX_Status_t NX_SetPic1(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_PIC1,
            (int32_t) value);   
}

NX_Status_t NX_SetPic2(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_PIC2,
            (int32_t) value);    
}

NX_Status_t NX_SetWid(NX_Screen_t *screen, NX_Object_t * object, uint8_t value) {
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_WID,
            (int32_t) value);
}

NX_Status_t NX_SetHig(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_HIG,
            (int32_t) value);    
}

NX_Status_t NX_SetMaxval(NX_Screen_t *screen, NX_Object_t * object, uint16_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_MAXVAL,
            (int32_t) value);    
}

NX_Status_t NX_SetMinval(NX_Screen_t *screen, NX_Object_t * object, uint16_t value){
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_MINVAL,
            (int32_t) value);   
}

NX_Status_t NX_SetCh(NX_Screen_t *screen, NX_Object_t * object, uint8_t value){
    if(object->component->type == NX_TYPE_SLIDER)
        if(value > 100)
            value=100;
    
    if(object->component->type == NX_TYPE_SELECT_TEXT)
        if(value > 32)
            value=32;
   
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_CH,
            (int32_t) value);       
}

NX_Status_t NX_SetUp(NX_Screen_t *screen, NX_Object_t * object, int32_t value){
    
    if(object->component->type == NX_TYPE_GAUGE) {
        if (value > 32767)
            value = 32767;
        
        if(value < -32767)
            value = -32767;
    }
    
     return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_UP,
            (int32_t) value);    
}

NX_Status_t NX_SetDowm(NX_Screen_t *screen, NX_Object_t * object, int32_t value) {

    if (object->component->type == NX_TYPE_GAUGE) {
        if (value > 32767)
            value = 32767;

        if (value < -32767)
            value = -32767;
    }

    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_DOWN,
            (int32_t) value);
}

NX_Status_t NX_SetGdc(NX_Screen_t *screen, NX_Object_t * object, int32_t value) {
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_GDC,
            (int32_t) value);    
}

NX_Status_t NX_SetGdw(NX_Screen_t *screen, NX_Object_t * object, int32_t value) {
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_GDW,
            (int32_t) value);
}

NX_Status_t NX_SetGdh(NX_Screen_t *screen, NX_Object_t * object, int32_t value) {
    return NX_APIAssignNumber(
            screen,
            object,
            NX_ATTR_GDH,
            (int32_t) value);
}


/*==============================================================================
*Command fucntions
==============================================================================*/

NX_Status_t NX_ExecuteCommand(NX_Screen_t *screen, NX_CommandEvents_t cmd){
    if (screen == NULL ) 
        return NX_ERROR_NULL_POINTER;
    
    if(!((cmd >= NX_CMD_REF_STOP) && (cmd <= NX_CMD_DOEVENTS)))
        return NX_ERROR_INVALID_COMMAND;
    
    const NX_String_t *registry = &NX_CommandEventsTable[cmd]; 
        
    return NX_APITransporter(screen, registry->data, registry->length);
}

NX_Status_t NX_setPage(NX_Screen_t *screen, NX_Page_t *page) {
    if (screen == NULL || page == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[NX_CTRL_PAGE]; 
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s, %s",
            registry->data,
            page->pageName.data);
    
    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_setRef(NX_Screen_t *screen, NX_Object_t *object) {
    if (screen == NULL || object == NULL)
        return NX_ERROR_NULL_POINTER;

    const NX_String_t *registry = &NX_CommandTable[NX_CTRL_REF];
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s %s",
            registry->data,
            object->objectName.data);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}


NX_Status_t NX_setClick(NX_Screen_t *screen, NX_Object_t *object, bool status){
    
    if (screen == NULL || object == NULL)
        return NX_ERROR_NULL_POINTER;

    const NX_String_t *registry = &NX_CommandTable[NX_CTRL_CLICK];
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s %s,%d",
            registry->data,
            object->objectName.data,
            (int)status);

    return NX_APITransporter(screen, api_scratch_buffer, length);  
    
}

NX_Status_t NX_dataGet(NX_Screen_t *screen, NX_Object_t *object, NX_Attribute_t attr){
    
    if (screen == NULL || object == NULL)
        return NX_ERROR_NULL_POINTER;
    
    //validate the attribute in the object
    /* Is component supports the attribute? */
    if (!NX_ComponentHasAttribute(object->component, attr))
        return NX_ERROR_NO_ATTR_OBJECT_FOUND;
    
    const NX_AttributeInfo_t *attrInfo;
    attrInfo = NX_GetAttributeInfo(attr);

    if (attrInfo == NULL)
        return NX_ERROR_NO_ATTR_OBJECT_FOUND;
    
    const NX_String_t *registry = &NX_CommandTable[NX_CTRL_GET];
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s %s.%s",
            registry->data,
            object->objectName.data,
            attrInfo->name.data);
    
    return NX_APITransporter(screen, api_scratch_buffer, length);  
}

NX_Status_t NX_setVis(NX_Screen_t *screen, NX_Object_t *object, bool status){
    if (screen == NULL || object == NULL)
        return NX_ERROR_NULL_POINTER;

    const NX_String_t *registry = &NX_CommandTable[NX_CTRL_VIS];
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s %s,%d",
            registry->data,
            object->objectName.data,
            (int)status);

    return NX_APITransporter(screen, api_scratch_buffer, length);    
}

NX_Status_t NX_setTsw(NX_Screen_t *screen, NX_Object_t *object, bool status){
    if (screen == NULL || object == NULL)
        return NX_ERROR_NULL_POINTER;

    const NX_String_t *registry = &NX_CommandTable[NX_CTRL_TSW];
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s %s,%d",
            registry->data,
            object->objectName.data,
            (int)status);

    return NX_APITransporter(screen, api_scratch_buffer, length);    
}

NX_Status_t NX_setLayer(NX_Screen_t *screen, NX_Object_t *object1, NX_Object_t *object2){
    
    if (screen == NULL || object1 == NULL || object2 == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[NX_CTRL_TSW];
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s %s,%s",
            registry->data,
            object1->objectName.data,
            object2->objectName.data);
   
    return NX_APITransporter(screen, api_scratch_buffer, length);     
    
}
/*==============================================================================
 *Handled of file on SD or intern storage
 ===============================================================================*/

static NX_Status_t NX_API_FileSystems(NX_Screen_t *screen,
        const char *path1, const char *path2, NX_Command_t cmd) {
    
    if (screen == NULL || path1 == NULL || path2 == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[cmd];

    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s \"%s\",\"%s\"",
            registry->data,
            path1,
            path2);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_delfile(NX_Screen_t *screen, const char *path1, const char *path2){
    
    return NX_API_FileSystems(screen, path1, path2, NX_FS_DELFILE);
}

NX_Status_t NX_relfile(NX_Screen_t *screen, const char *path1, const char *path2){
    
    return NX_API_FileSystems(screen, path1, path2, NX_FS_REFILE);
}

NX_Status_t NX_newdir(NX_Screen_t *screen, const char *path1){
    
    if (screen == NULL || path1 == NULL)
        return NX_ERROR_NULL_POINTER;    
    
    const NX_String_t *registry = &NX_CommandTable[NX_FS_NEWDIR];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s \"%s\"", 
            registry->data, 
            path1);
    
    return NX_APITransporter(screen, api_scratch_buffer, length); 
}

NX_Status_t NX_deldir(NX_Screen_t *screen, const char *path1){
    if (screen == NULL || path1 == NULL)
        return NX_ERROR_NULL_POINTER;    
    
    const NX_String_t *registry = &NX_CommandTable[NX_FS_DELDIR];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s \"%s\"", 
            registry->data, 
            path1);
    
    return NX_APITransporter(screen, api_scratch_buffer, length);    
}

NX_Status_t NX_redir(NX_Screen_t *screen, const char *path1, const char *path2){
    
    return NX_API_FileSystems(screen, path1, path2, NX_FS_REDIR);
}

NX_Status_t NX_findfile(NX_Screen_t *screen, const char *path1) {
    if (screen == NULL || path1 == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[NX_FS_FINDFILE];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s \"%s\",sys0", 
            registry->data, 
            path1);
  
    return NX_APITransporter(screen, api_scratch_buffer, length);    
}

NX_Status_t NX_waveformAdd(NX_Screen_t *screen, NX_Object_t *object, 
        uint8_t channelId, uint8_t value){
    
    if(screen == NULL || object == NULL)
         return NX_ERROR_NULL_POINTER; 
    
    const NX_String_t *registry = &NX_CommandTable[NX_WAVE_ADD];
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s %u,%u,%u",
            registry->data,
            (unsigned int)object->componentId,
            (unsigned int)channelId,
            (unsigned int)value);

    return NX_APITransporter(screen, api_scratch_buffer, length);  
}

NX_Status_t NX_waveformAddt(NX_Screen_t *screen, NX_Object_t *object, 
        uint8_t channelId, uint8_t qty){
    
    if(screen == NULL || object == NULL)
         return NX_ERROR_NULL_POINTER; 
    
    const NX_String_t *registry = &NX_CommandTable[NX_STRM_WAVE];
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s %u,%u,%u",
            registry->data,
            (unsigned int)object->componentId,
            (unsigned int)channelId,
            (unsigned int)qty);

    return NX_APITransporter(screen, api_scratch_buffer, length);    
}

NX_Status_t NX_waveformCle(NX_Screen_t *screen, NX_Object_t *object, uint8_t channelId){
    
    if(screen == NULL || object == NULL)
         return NX_ERROR_NULL_POINTER; 
    
    const NX_String_t *registry = &NX_CommandTable[NX_WAVE_CLE];
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s %u,%u",
            registry->data,
            (unsigned int)object->componentId,
            (unsigned int)channelId);

    return NX_APITransporter(screen, api_scratch_buffer, length);   
}

NX_Status_t NX_ControlCrcEngine(NX_Screen_t *screen, NX_CrcCmd_t action, const char *value) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;

    if (action >= NX_CRC_MAX) {
        return NX_ERROR_INVALID_PARAM;
    }
    
    const NX_String_t *registry = &NX_CrcCmdTable[action];
    int length;
    
    if (action == NX_CRC_RESET) {
        // Format: crcrest
        length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
                "%s", registry->data);
    } else {
        // Format: crcputs <value>, crcputh <value>, or crcputu <value>
        if (value == NULL) return NX_ERROR_INVALID_PARAM;
        length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
                "%s %s", registry->data, value);
    }
    
    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_OpenDataStream(NX_Screen_t *screen, NX_StreamCmd_t action, const char *target, uint32_t size) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    if (action >= NX_STRM_MAX || target == NULL) {
        return NX_ERROR_INVALID_PARAM;
    }
    
    const NX_String_t *registry = &NX_StreamCmdTable[action];
    
    // Format: "<command> <target>,<size>" 
    // Examples: "newfile sd0/a.bin,1024", or "twfile sd0/a.bin,512"
    int length = snprintf(api_scratch_buffer,sizeof(api_scratch_buffer),
            "%s %s,%lu", 
            registry->data,
            target, size);
    
    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_UartBufferCopy(NX_Screen_t *screen, const char *targetVar, 
        uint16_t startIndex, uint16_t length, uint8_t mode) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    if (targetVar == NULL) return NX_ERROR_INVALID_PARAM;
    
    const NX_String_t *registry = &NX_CommandTable[NX_CMD_UCOPY];
    
    int length_1 = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s %s,%u,%u,%u",
            registry->data,
            targetVar,
            startIndex,
            length,
            mode);

    return NX_APITransporter(screen, api_scratch_buffer, length_1);
}

NX_Status_t NX_UartBufferDelete(NX_Screen_t *screen, uint16_t length) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[NX_CMD_UDELETE];
    
    int length_1 = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
             "%s %u", 
             registry->data,
             (unsigned int)length);
    
    return NX_APITransporter(screen, api_scratch_buffer, length_1);
}

NX_Status_t NX_ReadFileFromSd(NX_Screen_t *screen, const char *filePath, 
        const char *targetVar, uint32_t startOffset, uint16_t length) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    if (filePath == NULL || targetVar == NULL) 
        return NX_ERROR_INVALID_PARAM;
    
    const NX_String_t *registry = &NX_CommandTable[NX_FS_RDFILE];

    int size = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s \"%s\",%s,%lu,%u",
            registry->data,
            filePath,
            targetVar,
            startOffset,
            length);

    return NX_APITransporter(screen, api_scratch_buffer, size);
}

/*==============================================================================
 * GUI Commands
===============================================================================*/


NX_Status_t NX_ClearScreen(NX_Screen_t *screen, uint16_t color) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[NX_GUI_CLS];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s %u", 
            registry->data,
            color);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_DrawPicture(NX_Screen_t *screen, uint16_t x, uint16_t y, uint16_t picId) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[NX_GUI_PIC];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s %u,%u,%u",
            registry->data,
            x,
            y, 
            picId);
 
    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_CropPicture(NX_Screen_t *screen, uint16_t x, uint16_t y, 
        uint16_t w, uint16_t h, uint16_t picId) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[NX_GUI_PICC];
    
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s %u,%u,%u,%u,%u", 
            registry->data,
            x, y, w, h, picId);
    
    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_DrawPictureAdvanced(NX_Screen_t *screen, uint16_t dstX, uint16_t dstY, 
        uint16_t w, uint16_t h, uint16_t srcX, uint16_t srcY, uint16_t picId) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[NX_GUI_XPIC];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s %u,%u,%u,%u,%u,%u,%u", 
            registry->data,
            dstX, dstY, w, h, srcX, srcY, picId);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_PrintTextAdvanced(NX_Screen_t *screen, uint16_t x, uint16_t y, 
        uint16_t w, uint16_t h, uint8_t fontId, uint16_t fColor, 
        uint16_t bColor, uint8_t hAlign, uint8_t vAlign, uint8_t backFill, const char *text) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    if (text == NULL) return NX_ERROR_INVALID_PARAM;
    // Buffer size scaled up to allow custom strings embedded inside the function safely

    const NX_String_t *registry = &NX_CommandTable[NX_GUI_XSTR];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%s",
            registry->data,
            x, y, w, h, fontId, fColor, bColor, hAlign, vAlign, backFill, text);
    
    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_DrawLine(NX_Screen_t *screen, uint16_t x1, uint16_t y1, uint16_t x2, 
        uint16_t y2, uint16_t color) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[NX_GUI_LINE];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s %u,%u,%u,%u,%u",
            registry->data,
            x1, y1, x2, y2, color);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_DrawHollowRectangle(NX_Screen_t *screen, uint16_t x1, uint16_t y1, 
        uint16_t x2, uint16_t y2, uint16_t color) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[NX_GUI_LINE];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s %u,%u,%u,%u,%u", 
            registry->data,
            x1, y1, x2, y2, color);
 
    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_DrawHollowCircle(NX_Screen_t *screen, uint16_t cx, uint16_t cy, 
        uint16_t radius, uint16_t color) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[NX_GUI_CIR];
    
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s %u,%u,%u,%u",
            registry->data,
            cx, cy, radius, color);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_DrawFilledCircle(NX_Screen_t *screen, uint16_t cx, uint16_t cy, 
        uint16_t radius, uint16_t color) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_CommandTable[NX_GUI_CIRS];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s %u,%u,%u,%u",
            registry->data,
            cx, cy, radius, color);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}

/*==============================================================================
 * System variables 
===============================================================================*/
//static functions
static NX_Status_t NX_API_SystemVariable(NX_Screen_t * screen, uint32_t value, 
        NX_SystemVar_t var) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_SystemVarTable[var];
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s=%u",
            registry->data,
            (unsigned int)value);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_setDP(NX_Screen_t *screen, NX_Page_t * page){
    
    if (screen == NULL || page == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_SystemVarTable[NX_VAR_SYSTEM_DP];
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%s=%d",
            registry->data,
            page->pageId);

    return NX_APITransporter(screen, api_scratch_buffer, length);     
}

NX_Status_t NX_SetBrightness(NX_Screen_t *screen, uint8_t percentage) {

    if (percentage > 100) percentage = 100;

    return NX_API_SystemVariable(screen, (uint32_t) percentage, NX_VAR_SYSTEM_DIM);
}

NX_Status_t NX_SetBrightnessPersistent(NX_Screen_t *screen, uint8_t percentage) {
    
    if (percentage > 100) percentage = 100;

    return NX_API_SystemVariable(screen, (uint32_t) percentage, NX_VAR_SYSTEM_DIMS);
}

NX_Status_t NX_SetBaudRate(NX_Screen_t *screen, uint32_t baudrate) {
    
    return NX_API_SystemVariable(screen, (uint32_t) baudrate, NX_VAR_SYSTEM_BAUD);
}

NX_Status_t NX_SetBaudRatePersistent(NX_Screen_t *screen, uint32_t baudrate) {
    
    return NX_API_SystemVariable(screen, (uint32_t) baudrate, NX_VAR_SYSTEM_BAUDS);
}

NX_Status_t NX_SetSleepMode(NX_Screen_t *screen, bool enable) {

    return NX_API_SystemVariable(screen, (uint32_t) enable, NX_VAR_SYSTEM_SLEEP);
}

NX_Status_t NX_SetAutoSleepTimeout(NX_Screen_t *screen, uint16_t timeoutSecs) {
    
    return NX_API_SystemVariable(screen, (uint32_t) timeoutSecs, NX_VAR_SYSTEM_THSP);
}

NX_Status_t NX_SetWakeOnTouch(NX_Screen_t *screen, bool enable) {
    
   
    return NX_API_SystemVariable(screen, (uint32_t)enable, NX_VAR_SYSTEM_THSP);
}

NX_Status_t NX_SetUartResponseLevel(NX_Screen_t *screen, uint8_t level) {

    //guard for level
    if (level >= 3)
        level = 3;

    return NX_API_SystemVariable(screen, (uint32_t) level, NX_VAR_SYSTEM_BKCMD);
}

NX_Status_t NX_WriteSystemRegister(NX_Screen_t *screen, uint8_t regId, int32_t value) {

    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_SystemVarTable[NX_VAR_SYSTEM_SYS];
    
    int length = snprintf(api_scratch_buffer, sizeof (api_scratch_buffer),
            "%sys%u=%d",
            registry->data,
            (unsigned int)regId,
            (int)value);


    return NX_APITransporter(screen, api_scratch_buffer, length);
}



#if defined(NEXTION_SERIES_ENHANCED) || defined(NEXTION_SERIES_INTELLIGENT)

NX_Status_t NX_SetRtcTime(NX_Screen_t *screen, uint8_t hours, uint8_t minutes, uint8_t seconds) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    if (hours > 23 || minutes > 59 || seconds > 59) 
        return NX_ERROR_INVALID_PARAM;
    
    const NX_String_t *registry_0 = &NX_SystemVarTable[NX_CMD_RTC3];
    const NX_String_t *registry_1 = &NX_SystemVarTable[NX_CMD_RTC4];
    const NX_String_t *registry_2 = &NX_SystemVarTable[NX_CMD_RTC5];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s=%u\n%s=%u\n%s=%u",
            registry_0->data,
            hours,
            registry_1->data,
            minutes,
            registry_2->data,
            seconds);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_SetRtcDate(NX_Screen_t *screen, uint16_t year, uint8_t month, uint8_t day) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    if (month > 12 || day > 31) 
        return NX_ERROR_INVALID_PARAM;
    
    const NX_String_t *registry_0 = &NX_SystemVarTable[NX_CMD_RTC0];
    const NX_String_t *registry_1 = &NX_SystemVarTable[NX_CMD_RTC1];
    const NX_String_t *registry_2 = &NX_SystemVarTable[NX_CMD_RTC2];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s=%u\n%s=%u\n%s=%u",
            registry_0->data,
            year,
            registry_1->data,
            month,
            registry_2->data,
            day);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}

#endif // Fin de serie Enhanced


#if defined(NEXTION_SERIES_INTELLIGENT)

NX_Status_t NX_SetAudioVolume(NX_Screen_t *screen, uint8_t volumePct) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    if (volumePct > 100) volumePct = 100;
    
    const NX_String_t *registry = &NX_SystemVarTable[NX_CMD_VOL];

    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s=%u",
            registry->data,
            volumePct);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_SetAudioSlotStatus(NX_Screen_t *screen, uint8_t slotId, uint8_t status) {
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    if (slotId > 1) return NX_ERROR_INVALID_PARAM;

    const NX_String_t *registry = &NX_SystemVarTable[NX_CMD_AUDIO];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s%u=%u", 
            registry->data,
            slotId, status);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}

NX_Status_t NX_SetVid(NX_Screen_t *screen, uint16_t videoId) {
    
    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;
    
    const NX_String_t *registry = &NX_SystemVarTable[NX_CMD_AUDIO];
    
    int length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
            "%s=%u", 
            registry->data,
            videoId);

    return NX_APITransporter(screen, api_scratch_buffer, length);
}


NX_Status_t NX_ControlEeprom(NX_Screen_t *screen, NX_EepromCmd_t action, const char *target, uint16_t length) {

    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;


    if (action >= NX_EEP_MAX || target == NULL) {
        return NX_ERROR_INVALID_PARAM;
    }
    
    const NX_String_t *registry = &NX_EepromCmdTable[action];
    int length_1;
    
    if (action == NX_EEP_WRITE_OBJ || action == NX_EEP_READ_OBJ) {
        // Format: "wepo <variable>,<eeprom_address_offset>" or "repo <variable>,<eeprom_address_offset>"
        // For simple object storage, 'length' serves as the starting address offset.
        length_1 = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
                "%s %s,%u", 
                registry->data, 
                target, 
                length);
    } else {
        // Format: "wept <start_address>,<length>" or "rept <start_address>,<length>"
        // Here, 'target' represents the address string (e.g. "100").
        length_1 = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
                "%s %s,%u", 
                registry->data, 
                target,
                length);
    }
    
    return NX_APITransporter(screen, api_scratch_buffer, length_1);
}

NX_Status_t NX_ConfigureGpio(NX_Screen_t *screen, uint8_t pinId, uint8_t mode, const char *binding) {

    if (screen == NULL)
        return NX_ERROR_NULL_POINTER;

    int length;
    const NX_String_t *registry = &NX_CommandTable[NX_GPIO_CFGPIO];
    if (binding != NULL) {
        // Format with active software input link: cfgpio <pinId>,<mode>,<binding>
        length= snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
                "%s %u,%u,%s", registry->data, pinId, mode, binding);
    } else {
        // Standard hardware mode configurations: cfgpio <pinId>,<mode>
        length = snprintf(api_scratch_buffer, sizeof(api_scratch_buffer),
                "%s %u,%u", registry->data, pinId, mode);
    }
    
    return NX_APITransporter(screen, api_scratch_buffer, length);
}


#endif // Fin de serie Intelligent


