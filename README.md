# Nextion HMI Framework

A layered, object-oriented C driver for **Nextion HMI displays**. The core (`lib/core`) is **MCU-agnostic** — it depends only on two function pointers (`tx`/`rx` callbacks, see [ARCHITECTURE.md](docs/ARCHITECTURE.md#layer-3--transport)) to talk to any UART peripheral, so it compiles and runs on any C99-capable target. It has been developed and tested on a **Microchip dsPIC33CH512MP508** with **MPLAB X** / **XC16**, which is why this repo also ships a ready-made UART driver and MCC-generated startup code for that specific part — but that's a reference integration, not a hard dependency of the driver itself.

The library models every Nextion component (buttons, text, gauges, waveforms, sliders, file browsers, etc.) as a typed runtime object with a **capability bitmask**, so the API refuses at runtime to send an attribute to a component that doesn't support it — instead of silently generating a malformed serial command.

> **Status:** actively developed / hardening in progress. See [Known Limitations](#known-limitations) before using in production.

---

## Features

- **Component/attribute validation model** — each Nextion widget type (button, text, gauge, waveform...) declares which of the ~60 protocol attributes it supports via a 64-bit bitmask. Setting an unsupported attribute returns an error instead of sending garbage over UART.
- **Zero dynamic allocation** — every object, page, and FIFO slot is statically allocated, sized for embedded/flash-constrained targets.
- **Two-directional vector FIFO** — incoming and outgoing serial frames are queued through a bounded circular buffer (`NX_VectorFifo_t`), decoupling the UART ISR from the application main loop.
- **Declarative page/object model** — pages own arrays of objects; objects carry optional `onPress` / `onRelease` callbacks that the event dispatcher fires automatically on touch events.
- **Full instruction set coverage** — page navigation, GUI drawing primitives (`line`, `circle`, `picture`, `crop`...), file system operations (SD/flash), waveform channels, EEPROM object tracking, GPIO configuration, RTC, audio/video control, and system variables (baud rate, brightness, sleep, etc.).
- **Hardware-agnostic transport layer** — the core only depends on two function pointers (`tx`/`rx` callbacks) to talk to any UART driver; the included driver targets dsPIC33 UART peripherals via Microchip Code Configurator (MCC).
- **Code-generation friendly** — the page/object/callback boilerplate (see `lib/nextion/nextion_layout.*`) is designed to be produced by an external Nextion project parser, so the object graph stays in sync with your `.HMI` layout instead of being hand-written.

---

## Project Structure

```
nextion.X/
├── lib/
│   ├── core/                  # Hardware-agnostic driver core (see ARCHITECTURE.md)
│   │   ├── nx_types.h         # Shared enums, typedefs, callback signatures
│   │   ├── nx_attributes.*    # Attribute registry (protocol string <-> enum)
│   │   ├── nx_component.*     # Component class blueprints (capability bitmasks)
│   │   ├── nx_object.*        # Concrete object instance API
│   │   ├── nx_page.*          # Page container / object lookup
│   │   ├── nx_screen.*        # Master device: active page + routing
│   │   ├── nx_fifo.*          # Bounded circular vector FIFO
│   │   ├── nx_event.*         # Touch/event dispatch
│   │   ├── nx_transport.*     # UART framing, TX/RX FIFO bridge
│   │   └── nx_api.*           # Public high-level command API (this is what you call)
│   ├── drivers/uart/          # dsPIC33 UART driver (MCC-based)
│   └── nextion/                # Auto-generated page/object layout for your project
├── mcc_generated_files/       # Microchip Code Configurator output (clocks, pins, UART, timers)
├── main.c                     # Application entry point / example wiring
└── nextion.X.mc3 / nbproject/ # MPLAB X project files
```

Full architectural explanation: **[docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)**
Complete function reference: **[docs/API.md](docs/API.md)**

---

## Requirements

### Core driver (`lib/core`) — platform-independent

| Requirement       | Notes                                                          |
|-------------------|------------------------------------------------------------------|
| Compiler          | Any C99-capable compiler                                        |
| RAM/Flash         | No dynamic allocation; sizing is driven by `#define` constants (`NX_FIFO_DEPTH`, `NX_MAX_VECTOR_LEN`, etc.) — adjust to fit your target |
| UART              | Any peripheral capable of exposing a byte-buffer TX function and delivering received bytes to a callback — wired in through two function pointers, no MCU-specific code in `lib/core` |
| Display           | Any Nextion **Basic**, **Enhanced**, or **Intelligent** series panel |

The `NEXTION_SERIES_BASIC` / `NEXTION_SERIES_ENHANCED` / `NEXTION_SERIES_INTELLIGENT` compile-time defines gate features that only exist on certain panel tiers (RTC, audio/video, GPIO, EEPROM object tracking). Set the one matching your hardware at the top of `nx_api.c`.

### Reference integration included in this repo — dsPIC33

The `lib/drivers/uart`, `mcc_generated_files/`, and `main.c` in this repo are a **working example**, not a requirement of the core. They target:

| Component        | Version / Notes                                             |
|-------------------|---------------------------------------------------------------|
| MCU               | Microchip dsPIC33CH512MP508 (development/test target)         |
| IDE               | [MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)   |
| Compiler          | XC16                                                          |
| Code generation   | MPLAB Code Configurator (MCC) — clocks, UART, TMR1, pins       |

To port to a different MCU or toolchain, keep `lib/core` and `lib/nextion` as-is, and replace `lib/drivers/uart` + the two callbacks passed to `NX_Transport_Init` with your own peripheral's TX/RX glue.

---

## Quick Start

```c
#include "lib/core/nx_api.h"
#include "lib/nextion/nextion_layout.h"   // your auto-generated page/object graph

int main(void)
{
    SYSTEM_Initialize();
    UART_Init();
    TMR1_SetInterruptHandler(&tmr0_tick);
    UART_Register_Rx_Callback(&rx_interrupt_callback);

    // Wire the transport layer to your UART driver's TX function
    NX_Transport_Init(&miDispositivoHMI, 0, &NX_send);

    // Validate transport + page table integrity
    NX_Init(&miDispositivoHMI);

    // Navigate and update a component
    NX_setPage(&miDispositivoHMI, &page_dashboard);

    const NX_String_t text = { .data = "READY", .length = 5 };
    NX_SetText(&miDispositivoHMI, &btn_start, &text);

    while (1) {
        // NX_Transport_Tasks() must run periodically (e.g. from a timer ISR)
        // to flush the TX FIFO and drain incoming touch/data events.
    }
}
```

Incoming UART bytes are pushed into the driver from your ISR:

```c
void rx_interrupt_callback(const uint8_t *buffer, uint16_t length) {
    NX_Transport_RxCallback(&miDispositivoHMI, buffer, (uint8_t)length);
}
```

Touch events are dispatched automatically to the `onPress` / `onRelease` callback registered on each `NX_Object_t` — no manual switch/case needed in application code.

---

## Error Handling

Every public API function returns an `NX_Status_t`. `NX_OK` (0) means success; any other value identifies exactly what failed (null pointer, unsupported attribute, object not found in the current page, FIFO full, transport not configured, etc.) — see the full list in `nx_types.h`.

```c
NX_Status_t status = NX_SetVal(&miDispositivoHMI, &number0, 42);
if (status != NX_OK) {
    // handle / log status
}
```

---

## Known Limitations

- `NX_ExecuteCommand`, and the direct attribute setters, currently render commands into a single shared static buffer (`api_scratch_buffer`) — **not reentrant**. Do not call the API concurrently from an ISR and the main loop.
- The event dispatch loop in `nx_screen.c` and `nx_event.c` currently duplicates the page/object lookup logic; a shared internal helper is planned.
- `docs/API.md` documents the intended/corrected behavior of the driver after the fixes described in [`CHANGELOG.md`](CHANGELOG.md) — always check the version you're building against.

Contributions to close these gaps are welcome — see [CONTRIBUTING.md](CONTRIBUTING.md).

---

## License

MIT License — see individual file headers. Copyright (c) 2026 Jacobo Gonzalez.
