# STM32F407VGT6 C Sample Codes

A structured collection of **Embedded C examples for the STM32F407VGT6** microcontroller.

This repository is primarily a learning and experimentation workspace: the examples progress from basic GPIO control to register-level programming, button state handling, timers and PWM, ADC sampling, UART interrupt/DMA techniques, LCD drivers, and more advanced buffering patterns.

> **Learning progression:** GPIO → timing → buttons → interrupts → timers/PWM → LCD → ADC → UART → DMA/circular buffers → practical peripheral projects

## What is covered

* STM32CubeMX-generated projects
* STM32 HAL
* Direct register-level programming
* GPIO and LED control
* Button polling and debouncing
* Non-blocking button state machines
* EXTI interrupts
* Hardware timers
* Software PWM and timer-based PWM
* Timer-based microsecond/millisecond delays
* Character LCD drivers
* Nokia 1661 LCD/TFT driver
* ADC polling
* ADC interrupts
* ADC overlapping windows
* ADC + DMA
* UART polling
* UART interrupts
* UART + DMA
* Single-buffer UART line reception
* Multi-buffer UART reception
* DMA-backed circular UART buffering

## Project progression

```text
001–004   GPIO / timing
005–009   Buttons / input / interrupts
010–014   Timers / PWM / delays
015–016   Character LCD / driver architecture
017–020   ADC / interrupts / overlap / DMA
021–026   UART / interrupts / DMA / buffering
901–902   LCD / practical peripheral projects
Finals    Integrated beginner project
```

## Sample Project Overview

| Project                                 | Main topic                                          |
| --------------------------------------- | --------------------------------------------------- |
| `001_SimpleLED`                         | Basic HAL GPIO output and delay                     |
| `002_PeriodicLEDBlink`                  | Periodic scheduling with a fixed software time base |
| `003_RegistryLEDBlink`                  | Register-level GPIO programming                     |
| `004_ParallelLEDBlink`                  | Independent non-blocking LED timing                 |
| `005_ButtonPolling`                     | Blocking button polling and debounce                |
| `006_RegistryButtonPolling`             | Register-level GPIO input/output                    |
| `007_NonBlockingButtonPolling`          | Non-blocking button state detection                 |
| `008_AdvancedNonBlockingButtonPolling`  | Callback-based reusable key handling                |
| `009_ExternalInterruptButton`           | GPIO EXTI interrupt handling                        |
| `010_TIMLedBlink`                       | Timer interrupt based LED control                   |
| `011_SimplePWM`                         | Software PWM                                        |
| `012_SimpleTIMPWM`                      | Timer-assisted PWM                                  |
| `013_TIMPWM`                            | Timer-driven multi-LED PWM logic                    |
| `014_TIMDelay`                          | Timer-based microsecond/millisecond delays          |
| `015_ALCD01`                            | Custom HD44780-style character LCD driver           |
| `016_ALCDlib`                           | Reusable LCD library + CMake structure              |
| `017_ADCPolling`                        | ADC polling and voltage conversion                  |
| `018_ADCInterrupt`                      | Interrupt-driven ADC sampling                       |
| `019_ADCInterruptOverlap`               | Overlapping ADC sample windows                      |
| `020_ADCDMA`                            | ADC + DMA acquisition and averaging                 |
| `021_UARTEcho`                          | Blocking UART echo                                  |
| `022_UARTEchoInterrupt`                 | Interrupt-driven UART reception                     |
| `023_UARTEchoDMA`                       | UART reception using DMA                            |
| `024_UARTReadLineInterruptSingleBuffer` | Single-buffer line reception                        |
| `025_UARTReadLineInterruptMultiBuffer`  | Multiple receive buffers                            |
| `026_UARTReadLineDMACircularBuffer`     | DMA + circular UART RX buffer                       |
| `901_Nokia1661LCD`                      | Nokia 1661 graphical display                        |
| `901_Nokia1661LCD_IAR`                  | Nokia 1661 project for IAR                          |
| `902_RotaryBuzzer`                      | Timer/compare based practical peripheral project    |
| `Finals/BeginnerProject`                | More complete integrated exercise                   |

## Recommended learning order

### 1. GPIO

Start with:

```text
001_SimpleLED
002_PeriodicLEDBlink
003_RegistryLEDBlink
004_ParallelLEDBlink
```

This gives you the foundation for GPIO, delays, counters, `HAL_GetTick()`, and direct register manipulation.

### 2. Buttons

Continue with:

```text
005_ButtonPolling
006_RegistryButtonPolling
007_NonBlockingButtonPolling
008_AdvancedNonBlockingButtonPolling
009_ExternalInterruptButton
```

This progression is particularly useful because it moves from:

```text
polling
   ↓
debouncing
   ↓
non-blocking state handling
   ↓
callbacks
   ↓
interrupts
```

### 3. Timers and PWM

Then:

```text
010_TIMLedBlink
011_SimplePWM
012_SimpleTIMPWM
013_TIMPWM
014_TIMDelay
```

This section shows how timing gradually moves away from simple blocking delays into dedicated hardware peripherals.

### 4. Displays and drivers

Study:

```text
015_ALCD01
016_ALCDlib
901_Nokia1661LCD
901_Nokia1661LCD_IAR
```

These are useful for understanding the difference between application code and reusable peripheral-driver code.

### 5. ADC

Then:

```text
017_ADCPolling
018_ADCInterrupt
019_ADCInterruptOverlap
020_ADCDMA
```

The progression is:

```text
Polling
   ↓
Interrupt
   ↓
Overlapping windows
   ↓
DMA
```

### 6. UART

The UART sequence is one of the most useful parts of the repository:

```text
021_UARTEcho
022_UARTEchoInterrupt
023_UARTEchoDMA
024_UARTReadLineInterruptSingleBuffer
025_UARTReadLineInterruptMultiBuffer
026_UARTReadLineDMACircularBuffer
```

The architecture evolves into:

```text
Blocking UART
      ↓
Interrupt UART
      ↓
DMA UART
      ↓
Single buffer
      ↓
Multiple buffers
      ↓
Circular buffer + DMA
```

## Repository structure

```text
STM32F407VGT6_C_SampleCodes/
├── Finals/
│   └── BeginnerProject/
│
├── projects/
│   ├── 001_SimpleLED/
│   ├── 002_PeriodicLEDBlink/
│   ├── 003_RegistryLEDBlink/
│   ├── 004_ParallelLEDBlink/
│   ├── 005_ButtonPolling/
│   ├── 006_RegistryButtonPolling/
│   ├── 007_NonBlockingButtonPolling/
│   ├── 008_AdvancedNonBlockingButtonPolling/
│   ├── 009_ExternalInterruptButton/
│   ├── 010_TIMLedBlink/
│   ├── 011_SimplePWM/
│   ├── 012_SimpleTIMPWM/
│   ├── 013_TIMPWM/
│   ├── 014_TIMDelay/
│   ├── 015_ALCD01/
│   ├── 016_ALCDlib/
│   ├── 017_ADCPolling/
│   ├── 018_ADCInterrupt/
│   ├── 019_ADCInterruptOverlap/
│   ├── 020_ADCDMA/
│   ├── 021_UARTEcho/
│   ├── 022_UARTEchoInterrupt/
│   ├── 023_UARTEchoDMA/
│   ├── 024_UARTReadLineInterruptSingleBuffer/
│   ├── 025_UARTReadLineInterruptMultiBuffer/
│   ├── 026_UARTReadLineDMACircularBuffer/
│   ├── 901_Nokia1661LCD/
│   ├── 901_Nokia1661LCD_IAR/
│   └── 902_RotaryBuzzer/
│
├── .gitattributes
├── .gitignore
└── README.md
```

Most projects follow a CubeMX-style structure:

```text
<Project>/
├── Core/
│   ├── Inc/
│   └── Src/
├── Drivers/
├── EWARM/                 # IAR projects
├── Libs/                  # Reusable libraries
├── cmake/                 # CMake projects
├── .mxproject
├── <Project>.ioc
├── CMakeLists.txt
├── CMakePresets.json
├── STM32F407XX_FLASH.ld
└── startup_stm32f407xx.s
```

Not every project contains every directory.

## Development stack

| Tool / technology            | Usage                               |
| ---------------------------- | ----------------------------------- |
| STM32CubeMX                  | Peripheral configuration            |
| STM32 HAL                    | High-level peripheral access        |
| Embedded C / C11             | Application code                    |
| IAR Embedded Workbench       | Projects containing `EWARM`         |
| CMake                        | Newer CMake-based projects          |
| CMSIS / STM32 device headers | MCU definitions and register access |

Some projects have explicit CMake configuration with C11 and CubeMX-generated CMake integration, while others retain the more traditional IAR/EWARM structure.

## Target MCU

The repository is built around the **STM32F407VG / STM32F407VGT6** family.

A common clock setup appears throughout the examples:

```text
HSI
 ↓
PLL
 ↓
168 MHz system clock
```

with typical settings:

```text
PLLM = 8
PLLN = 168
PLLP = 2
PLLQ = 4

AHB  = SYSCLK / 1
APB1 = HCLK   / 4
APB2 = HCLK   / 2
```

Always check the individual `.ioc` file and generated peripheral configuration before reproducing a project on another board.

## Building

Clone the repository:

```bash
git clone https://github.com/jezali/STM32F407VGT6_C_SampleCodes.git
cd STM32F407VGT6_C_SampleCodes
```

Then choose an individual project.

### IAR projects

Open the corresponding IAR project/workspace from the `EWARM` directory.

### STM32CubeMX projects

Open the project's `.ioc` file in STM32CubeMX to inspect or regenerate peripheral configuration.

### CMake projects

Some projects include:

```text
CMakeLists.txt
CMakePresets.json
cmake/
```

and can be configured using the normal CMake + ARM embedded toolchain workflow.

Do not assume every sample uses exactly the same build flow.

## Why both HAL and direct registers?

This repository deliberately mixes abstraction levels.

You will see high-level functions such as:

```c
HAL_GPIO_TogglePin(...)
HAL_Delay(...)
HAL_UART_Receive_IT(...)
HAL_ADC_Start_DMA(...)
```

and direct register manipulation such as:

```c
RCC->AHB1ENR
GPIOx->MODER
GPIOx->ODR
GPIOx->BSRR
TIMx->CCR1
```

That is intentional.

The idea is to first understand **what a peripheral does**, then understand **what the MCU registers are actually doing underneath the HAL**.

## Important implementation notes

### Blocking vs non-blocking timing

The early projects use `HAL_Delay()` because it is simple and easy to understand.

Later projects demonstrate:

* `HAL_GetTick()`
* software time bases
* timer interrupts
* state machines
* DMA
* event-driven handling

The progression is intentional.

### Interrupt handling

The interrupt examples demonstrate the mechanics of:

```text
Peripheral event
      ↓
ISR / HAL callback
      ↓
Capture data or set state
      ↓
Main loop processes it
```

For production firmware, interrupt service routines should generally remain short and deterministic.

### DMA

DMA becomes especially useful when peripheral data arrives continuously.

In this repository it is applied to:

* ADC acquisition
* UART reception

This lets the CPU spend more time on actual application processing.

### Circular UART buffering

The `026_UARTReadLineDMACircularBuffer` project implements a custom ring buffer with:

* read position
* write position
* available-byte calculation
* free-space calculation
* wrap-around handling
* overflow tracking
* direct contiguous-data access
* delimiter searching
* DMA/interrupt receive support

The data flow is essentially:

```text
UART
 ↓
DMA
 ↓
Circular RX buffer
 ↓
Search for '\n'
 ↓
Extract complete line
 ↓
Process / transmit
```

This pattern is useful for command interfaces, modems, GNSS receivers, sensors, and streaming protocols.

## Suggested experiments

After understanding each sample, modify it before moving on.

### GPIO

* change the LED pin
* invert active-high/active-low logic
* toggle multiple GPIOs simultaneously
* replace HAL writes with register writes

### Buttons

* add long-press detection
* add key-repeat handling
* change debounce timing
* add more callback events

### Timers

* change timer periods
* calculate prescaler values manually
* compare `HAL_GetTick()` timing with hardware timers
* generate several independent periodic events

### ADC

* change sample count
* add min/max tracking
* add moving average filtering
* add threshold detection
* compare polling vs interrupt vs DMA CPU usage

### UART

* add command parsing
* support `\r\n`
* define a binary packet format
* add CRC/checksum
* increase circular buffer size
* implement a non-blocking transmit queue

## Repository philosophy

This is a **learning repository**, not a finished embedded framework.

The progression is more important than any one implementation:

```text
"Make the LED work"
        ↓
"Add timing"
        ↓
"Understand the registers"
        ↓
"Read buttons"
        ↓
"Remove blocking behavior"
        ↓
"Use interrupts"
        ↓
"Use hardware timers"
        ↓
"Use DMA"
        ↓
"Build reusable drivers and buffers"
```

That makes the repository useful as a practical STM32F4 Embedded C study path.

## Author

**Jezali**

GitHub:

https://github.com/jezali

Repository:

https://github.com/jezali/STM32F407VGT6_C_SampleCodes

## Final takeaway

The strongest learning sequence in this repository is:

> **HAL → registers → polling → non-blocking logic → interrupts → timers → DMA → reusable drivers and buffering**

In particular, projects `024`, `025`, and `026` form a very useful progression from simple UART line reception to multi-buffer and DMA-backed circular-buffer designs.
:::
