# API Reference

All functions return `NX_Status_t`. `NX_OK` (0) means the command was validated and handed off to the transport layer successfully. See [`nx_types.h`](../nextion.X/lib/core/nx_types.h) for the full status/error enum.

Every setter below shares the same validation chain (see [ARCHITECTURE.md](ARCHITECTURE.md#layer-5--public-api-nx_api)): null-pointer check → object belongs to the currently active page → component type supports the attribute → command rendered and forwarded to transport.

---

## Initialization

| Function | Signature | Description |
|---|---|---|
| `NX_Init` | `(NX_Screen_t *screen)` | Validates transport callbacks and page table integrity, sets the initial active page. Call once after `NX_Transport_Init`. |

---

## Generic Attribute Setters

These map 1:1 to Nextion protocol attributes (`object.attr=value`). The library validates that the target object's component type actually supports the attribute before sending anything.

| Function | Value type | Attribute | Applies to (examples) |
|---|---|---|---|
| `NX_SetDrag` | `bool` | `drag` | Most visual components |
| `NX_SetAph` | `uint8_t` | `aph` | Most visual components |
| `NX_SetEffect` | `uint8_t` | `effect` | Most visual components |
| `NX_SetFont` | `uint8_t` | `font` | Text, button, number, xfloat, select text... |
| `NX_SetBco` / `NX_SetBco1` / `NX_SetBco2` | `uint16_t` | `bco`/`bco1`/`bco2` | Background color(s) |
| `NX_SetPco` / `NX_SetPco0..3` | `uint16_t` | `pco*` | Foreground/channel colors |
| `NX_SetXcen` / `NX_SetYcen` | `uint8_t` | `xcen`/`ycen` | Text alignment |
| `NX_SetPw` | `bool` | `pw` | Password mode |
| `NX_SetText` | `const NX_String_t *` | `txt` | Text, button, scrolling text, SLText |
| `NX_SetPath` | `const NX_String_t *` | `path` | Combo box, file browser, data record |
| `NX_SetIsbr` | `bool` | `isbr` | Line-wrap toggle |
| `NX_SetSpax` / `NX_SetSpay` | `uint8_t` | `spax`/`spay` | Character/line spacing |
| `NX_SetX` / `NX_SetY` | `uint16_t` | `x`/`y` | Position (all components) |
| `NX_SetDir` | `uint8_t` (clamped 0–4) | `dir` | Scrolling text direction |
| `NX_SetDis` | `uint16_t` (range depends on type — see note) | `dis` | Scroll speed / slider step / waveform grid / GMOV frame delay |
| `NX_SetTim` | `uint16_t` (min 50 for timers, 80 otherwise) | `tim` | Timer period, scrolling text |
| `NX_SetEn` | `bool` | `en` | Enable flag (timers, video, audio) |
| `NX_SetVal` | `int32_t` | `val` | Numeric value (number, gauge, slider, variable, checkbox, radio, data record) |
| `NX_SetLenth` | `uint8_t` (max 15) | `length` | Number max digit length |
| `NX_SetFormat` | `uint8_t` (max 3 for `NX_TYPE_NUMBER`) | `format` | Number display format |
| `NX_SetWs0` / `NX_SetWs1` / `NX_SetWs2` | `uint8_t` | `ws0`/`ws1`/`ws2` | Waveform/gauge channel styling |
| `NX_SetPic` / `NX_SetPic1` / `NX_SetPic2` | `uint8_t` | `pic`/`pic1`/`pic2` | Picture resource IDs |
| `NX_SetWid` / `NX_SetHig` | `uint8_t` | `wid`/`hig` | Width / height |
| `NX_SetMaxval` / `NX_SetMinval` | `uint16_t` | `maxval`/`minval` | Slider/gauge bounds |
| `NX_SetCh` | `uint8_t` | `ch` | Channel index |
| `NX_SetUp` / `NX_SetDowm` / `NX_SetLeft` | `int32_t` | `up`/`down`/`left` | Gauge needle bounds |
| `NX_SetGdc` / `NX_SetGdw` / `NX_SetGdh` | `int32_t` | `gdc`/`gdw`/`gdh` | Waveform grid color/width/height |

> **Note on `NX_SetDis` clamping:** the valid range is type-dependent — scrolling text clamps to 2–50, sliders to ≤100, waveforms to ≤1000, GMOV to 10–100. Double-check the clamp bounds in `nx_api.c` against your firmware version; this is one of the areas most likely to be adjusted as the driver is refined.

---

## Navigation & Object Control

| Function | Signature | Description |
|---|---|---|
| `NX_ExecuteCommand` | `(screen, NX_CommandEvents_t cmd)` | Sends a parameterless command (`ref_stop`, `ref_star`, `sendme`, `touch_j`, `com_stop`, `com_star`, `code_c`, `rest`, `doevents`). |
| `NX_setPage` | `(screen, NX_Page_t *page)` | Navigates to a page (`page <name>`). |
| `NX_setRef` | `(screen, NX_Object_t *object)` | Forces a refresh/redraw of a single object (`ref <name>`). |
| `NX_setClick` | `(screen, NX_Object_t *object, bool status)` | Simulates a press/release event on an object (`click <name>,<0/1>`). |
| `NX_dataGet` | `(screen, NX_Object_t *object, NX_Attribute_t attr)` | Requests the current value of `object.attr` from the display (`get <name>.<attr>`); the reply arrives asynchronously through the event dispatcher (`NX_EVENT_STRING_DATA_ENCL` / `NX_EVENT_NUMERIC_DATA_ENCL`). |
| `NX_setVis` | `(screen, NX_Object_t *object, bool status)` | Shows/hides an object (`vis <name>,<0/1>`). |
| `NX_setTsw` | `(screen, NX_Object_t *object, bool status)` | Enables/disables touch response on an object (`tsw <name>,<0/1>`). |
| `NX_setLayer` | `(screen, NX_Object_t *object1, NX_Object_t *object2)` | Reorders Z-layering between two objects (`setlayer`). |
| `NX_setDP` | `(screen, NX_Page_t *page)` | Alternate page-change mechanism via the `dp` system variable. |

---

## File System (SD Card / Internal Flash)

| Function | Signature | Description |
|---|---|---|
| `NX_delfile` | `(screen, path1, path2)` | Deletes a file (`delfile`). |
| `NX_relfile` | `(screen, path1, path2)` | Renames/moves a file (`refile`). |
| `NX_newdir` | `(screen, path1)` | Creates a directory (`newdir`). |
| `NX_deldir` | `(screen, path1)` | Deletes a directory (`deldir`). |
| `NX_redir` | `(screen, path1, path2)` | Renames a directory (`redir`). |
| `NX_findfile` | `(screen, path1)` | Searches for a file on `sys0` storage (`findfile`). |
| `NX_ReadFileFromSd` | `(screen, filePath, ...)` | Streams file content from SD into a target buffer/variable. |
| `NX_UartBufferCopy` | `(screen, targetVar, ...)` | Copies UART receive buffer content into a variable (`ucopy`). |
| `NX_UartBufferDelete` | `(screen, uint16_t length)` | Clears a portion of the UART receive buffer (`udelete`). |

---

## Waveform Channels

| Function | Signature | Description |
|---|---|---|
| `NX_waveformAdd` | `(screen, object, channelId, value)` | Appends a single point to a waveform channel (`add`). |
| `NX_waveformAddt` | `(screen, object, channelId, qty)` | Opens a bulk transparent-data stream to add `qty` points at once (`addt`). |
| `NX_waveformCle` | `(screen, object, channelId)` | Clears a waveform channel (`cle`). |

---

## GUI Drawing Primitives

| Function | Signature | Description |
|---|---|---|
| `NX_ClearScreen` | `(screen, uint16_t color)` | Fills the entire screen with a solid color (`cls`). |
| `NX_DrawPicture` | `(screen, x, y, picId)` | Draws a full picture resource at a position (`pic`). |
| `NX_CropPicture` | `(screen, x, y, ...)` | Draws a cropped region of a picture resource (`picc`). |
| `NX_DrawPictureAdvanced` | `(screen, dstX, ...)` | Draws a picture region into an arbitrary destination rectangle (`xpic`). |
| `NX_PrintTextAdvanced` | `(screen, x, y, ...)` | Draws styled text directly onto the canvas (`xstr`). |
| `NX_DrawLine` | `(screen, x1, y1, ...)` | Draws a line between two points (`line`). |
| `NX_DrawHollowRectangle` | `(screen, x1, y1, ...)` | Draws an unfilled rectangle (`draw`). |
| `NX_DrawHollowCircle` | `(screen, cx, cy, ...)` | Draws an unfilled circle (`cir`). |
| `NX_DrawFilledCircle` | `(screen, cx, cy, ...)` | Draws a filled circle (`cirs`). |

---

## System Configuration

| Function | Signature | Description | Persistence |
|---|---|---|---|
| `NX_SetBrightness` | `(screen, uint8_t percentage)` | Backlight level, 0–100. | Volatile |
| `NX_SetBrightnessPersistent` | `(screen, uint8_t percentage)` | Backlight level, 0–100. | Written to flash |
| `NX_SetBaudRate` | `(screen, uint32_t baudrate)` | UART baud rate. | Volatile |
| `NX_SetBaudRatePersistent` | `(screen, uint32_t baudrate)` | UART baud rate. | Written to flash |
| `NX_SetSleepMode` | `(screen, bool enable)` | Forces sleep/wake. | — |
| `NX_SetAutoSleepTimeout` | `(screen, uint16_t timeoutSecs)` | Inactivity timeout before auto-sleep (0 disables). | — |
| `NX_SetWakeOnTouch` | `(screen, bool enable)` | Whether touch wakes the panel from sleep. | — |
| `NX_SetUartResponseLevel` | `(screen, uint8_t level 0-3)` | Controls which instruction-result frames the panel echoes back. | — |
| `NX_WriteSystemRegister` | `(screen, uint8_t regId, int32_t value)` | Writes `sys0`/`sys1`/`sys2`. | — |
| `NX_ControlCrcEngine` | `(screen, NX_CrcCmd_t action, const char *value)` | Reset/append to the panel's onboard CRC engine (`crcrest`/`crcputs`/`crcputh`/`crcputu`). | — |
| `NX_OpenDataStream` | `(screen, NX_StreamCmd_t action, target, size)` | Opens/writes a transparent data file stream (`newfile`/`twfile`). | — |

---

## Panel-Tier Features (Enhanced / Intelligent series only)

Gated behind `#if defined(NEXTION_SERIES_ENHANCED) || defined(NEXTION_SERIES_INTELLIGENT)` (RTC) or `#if defined(NEXTION_SERIES_INTELLIGENT)` (audio/video/EEPROM/GPIO) in `nx_api.h`. Set the matching define for your hardware tier before building.

| Function | Signature | Description |
|---|---|---|
| `NX_SetRtcTime` | `(screen, hours, minutes, seconds)` | Sets the onboard hardware RTC clock. |
| `NX_SetRtcDate` | `(screen, year, month, day)` | Sets the onboard hardware RTC calendar. |
| `NX_SetAudioVolume` | `(screen, uint8_t volumePct)` | Global speaker volume, 0–100. |
| `NX_SetAudioSlotStatus` | `(screen, slotId, status)` | Controls an audio playback slot. |
| `NX_SetVid` | `(screen, uint16_t videoId)` | Selects the active background video resource. |
| `NX_ControlEeprom` | `(screen, NX_EepromCmd_t action, target, length)` | Object-property tracking (`wepo`/`repo`) or block memory transfer (`wept`/`rept`) to/from onboard EEPROM. |
| `NX_ConfigureGpio` | `(screen, pinId, mode, binding)` | Configures a physical GPIO pin on the panel (input/output/PWM/analog, optional component binding). |

---

## Transport & Event Layer (advanced / driver integration)

You normally don't call these directly from application logic — they're the glue between your UART ISR and the API above.

| Function | Signature | Description |
|---|---|---|
| `NX_Transport_Init` | `(screen, port, txCallback)` | Binds the hardware TX function pointer and resets both FIFOs. Call once at startup. |
| `NX_Transport_RxCallback` | `(screen, buffer, length)` | Call from your UART RX ISR to push a received frame into the RX FIFO. |
| `NX_Transport_TxCallback` | `(screen, buffer, length)` | Pushes a pre-assembled frame into the TX FIFO (used internally by the API layer). |
| `NX_Transport_Tasks` | `(screen)` | Call periodically (e.g. from a timer ISR) to drain one RX vector into the event dispatcher and flush one TX vector to the wire. |
| `NX_eventDispatch` | `(screen, NX_Vector_t *rxEvent)` | Parses one raw incoming vector: fires `onPress`/`onRelease` for touch events, forwards everything else to `screen->globalEvent`. |

---

## Status Codes

All functions return one of the `NX_Status_t` values defined in `nx_types.h`, grouped as:

- **Success:** `NX_OK`
- **Argument errors:** `NX_ERROR_NULL_POINTER`, `NX_ERROR_INVALID_PARAMETER`, `NX_ERROR_INVALID_PARAM`
- **Object/page errors:** `NX_ERROR_OBJECT_NOT_FOUND`, `NX_ERROR_NO_PAGE_FOUND`, `NX_ERROR_NO_PAGES`, `NX_ERROR_INVALID_PAGE`
- **Attribute errors:** `NX_ERROR_NO_ATTR_FOUND`, `NX_ERROR_NO_ATTR_OBJECT_FOUND` (attribute exists, but not supported by this component type)
- **Transport errors:** `NX_ERROR_NO_TRANSPORT`, `NX_ERROR_NO_WRITE_CALLBACK`, `NX_ERROR_NO_READ_CALLBACK`, `NX_ERROR_TRANSPORT`, `NX_ERROR_COMM`
- **FIFO errors:** `NX_ERROR_FIFO_FULL`, `NX_ERROR_FIFO_OVERFLOW`, `NX_ERROR_FIFO_UNDERFLOW`, `NX_ERROR_FIFO_VOID`
- **Buffer errors:** `NX_ERROR_BUFFER_OVERFLOW`, `NX_ERROR_BUFFER_OUT_OF_SIZE`
- **Command errors:** `NX_ERROR_INVALID_COMMAND`, `NX_ERROR_INVALID_CONTROL`, `NX_ERROR_INVALID_FILE_ACTION`, `NX_ERROR_INVALID_PARAM`
