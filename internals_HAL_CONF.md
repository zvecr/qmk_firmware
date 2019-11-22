# group `HAL_CONF` {#group___h_a_l___c_o_n_f}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`define `[`HAL_USE_PAL`](#group___h_a_l___c_o_n_f_1ga2311bcc7b41bfba183c9ec1a64a11e93)            | Enables the PAL subsystem.
`define `[`HAL_USE_ADC`](#group___h_a_l___c_o_n_f_1ga413a1b6c0ca2c9e524df50ceac1275fd)            | Enables the ADC subsystem.
`define `[`HAL_USE_CAN`](#group___h_a_l___c_o_n_f_1ga2e351185eace5acfd90b65c9fe796355)            | Enables the CAN subsystem.
`define `[`HAL_USE_DAC`](#group___h_a_l___c_o_n_f_1gafa9dbe1932df28930e9cfff6000b43d6)            | Enables the DAC subsystem.
`define `[`HAL_USE_EXT`](#group___h_a_l___c_o_n_f_1ga2dd40c0fdd833b861f9241f6d282fe34)            | Enables the EXT subsystem.
`define `[`HAL_USE_GPT`](#group___h_a_l___c_o_n_f_1gab4702b9e1b6fa2869e26c274fccd53f0)            | Enables the GPT subsystem.
`define `[`HAL_USE_I2C`](#group___h_a_l___c_o_n_f_1gae5eb94efa72e401dc17a356b27f4e7e5)            | Enables the I2C subsystem.
`define `[`HAL_USE_I2S`](#group___h_a_l___c_o_n_f_1ga5a92967aad4c273b71d8db15731d50d4)            | Enables the I2S subsystem.
`define `[`HAL_USE_ICU`](#group___h_a_l___c_o_n_f_1gac58fde738d018b1233a5441ae4c6245b)            | Enables the ICU subsystem.
`define `[`HAL_USE_MAC`](#group___h_a_l___c_o_n_f_1ga0da24d012c72a79db22e5621b391d3da)            | Enables the MAC subsystem.
`define `[`HAL_USE_MMC_SPI`](#group___h_a_l___c_o_n_f_1gafc4461ac2cc33590839ed91f2e940079)            | Enables the MMC_SPI subsystem.
`define `[`HAL_USE_PWM`](#group___h_a_l___c_o_n_f_1gafb7a560610f96b2858d51c36d65f8c2f)            | Enables the PWM subsystem.
`define `[`HAL_USE_QSPI`](#group___h_a_l___c_o_n_f_1gae1476f8b999cce0f6781a1da03071fb5)            | Enables the QSPI subsystem.
`define `[`HAL_USE_RTC`](#group___h_a_l___c_o_n_f_1ga17d20632810c9bb1601a19973977f55c)            | Enables the RTC subsystem.
`define `[`HAL_USE_SDC`](#group___h_a_l___c_o_n_f_1ga06f315a18b911f426da736c42ab20325)            | Enables the SDC subsystem.
`define `[`HAL_USE_SERIAL`](#group___h_a_l___c_o_n_f_1ga720d92b9c3a00d952f6f0c8665b54ebb)            | Enables the SERIAL subsystem.
`define `[`HAL_USE_SERIAL_USB`](#group___h_a_l___c_o_n_f_1ga79c970fb20565143ad8b65720c0b2d0e)            | Enables the SERIAL over USB subsystem.
`define `[`HAL_USE_SPI`](#group___h_a_l___c_o_n_f_1ga33a3c5bee9ed1f665aace8b88fed9b2b)            | Enables the SPI subsystem.
`define `[`HAL_USE_UART`](#group___h_a_l___c_o_n_f_1ga73220cad2edf2aee77b7ce1b53cacff0)            | Enables the UART subsystem.
`define `[`HAL_USE_USB`](#group___h_a_l___c_o_n_f_1gafc8f8fc6010f46f5baae4a369e14974e)            | Enables the USB subsystem.
`define `[`HAL_USE_WDG`](#group___h_a_l___c_o_n_f_1gac3be0803223f25a5edeed87f56b526c3)            | Enables the WDG subsystem.
`define `[`ADC_USE_WAIT`](#group___h_a_l___c_o_n_f_1ga39e892a4090185fbdda9bb105bc03b4f)            | Enables synchronous APIs.
`define `[`ADC_USE_MUTUAL_EXCLUSION`](#group___h_a_l___c_o_n_f_1gac0893cb47e338c2dabad34b974a0a88d)            | Enables the `adcAcquireBus()` and `adcReleaseBus()` APIs.
`define `[`CAN_USE_SLEEP_MODE`](#group___h_a_l___c_o_n_f_1ga5294d9d12e4186c781df3b2f1d8bd80b)            | Sleep mode related APIs inclusion switch.
`define `[`I2C_USE_MUTUAL_EXCLUSION`](#group___h_a_l___c_o_n_f_1ga7125642404a6fd3d4985d1cb6e1b7b0a)            | Enables the mutual exclusion APIs on the I2C bus.
`define `[`MAC_USE_ZERO_COPY`](#group___h_a_l___c_o_n_f_1gad763d9426413cf2fe3922ebb8578bc59)            | Enables an event sources for incoming packets.
`define `[`MAC_USE_EVENTS`](#group___h_a_l___c_o_n_f_1ga887da1c1383a9e7b45c4464877a7e05e)            | Enables an event sources for incoming packets.
`define `[`MMC_NICE_WAITING`](#group___h_a_l___c_o_n_f_1ga3087dfffa81dd8a0a80ee92746e65fe2)            | Delays insertions.
`define `[`SDC_INIT_RETRY`](#group___h_a_l___c_o_n_f_1ga8d39f0c9799062f0698d97c26e6fa42d)            | Number of initialization attempts before rejecting the card.
`define `[`SDC_MMC_SUPPORT`](#group___h_a_l___c_o_n_f_1ga4f938eff7370feb8de8411e255d21b01)            | Include support for MMC cards.
`define `[`SDC_NICE_WAITING`](#group___h_a_l___c_o_n_f_1ga3391c832c171a8606b0fc864766f08ba)            | Delays insertions.
`define `[`SERIAL_DEFAULT_BITRATE`](#group___h_a_l___c_o_n_f_1gacb4c08ac23f83ac9d58c50ff840de516)            | Default bit rate.
`define `[`SERIAL_BUFFERS_SIZE`](#group___h_a_l___c_o_n_f_1ga81a9fb00c7a1ce0fe70f263d8fd820e8)            | Serial buffers size.
`define `[`SERIAL_USB_BUFFERS_SIZE`](#group___h_a_l___c_o_n_f_1ga24727a5407c6a41af53b59588592da80)            | Serial over USB buffers size.
`define `[`SERIAL_USB_BUFFERS_NUMBER`](#group___h_a_l___c_o_n_f_1ga0c332b2493fd2106beb13425549120fe)            | Serial over USB number of buffers.
`define `[`SPI_USE_WAIT`](#group___h_a_l___c_o_n_f_1ga0c9ab1488423fa10a22f506c6e74b563)            | Enables synchronous APIs.
`define `[`SPI_USE_MUTUAL_EXCLUSION`](#group___h_a_l___c_o_n_f_1ga36d1818f9631f955f7cc94629b1d5498)            | Enables the `spiAcquireBus()` and `spiReleaseBus()` APIs.
`define `[`UART_USE_WAIT`](#group___h_a_l___c_o_n_f_1ga3ccbb19b26b53ba0a8e40cb6b8650114)            | Enables synchronous APIs.
`define `[`UART_USE_MUTUAL_EXCLUSION`](#group___h_a_l___c_o_n_f_1gae689e57cf792af401c324de566038687)            | Enables the `uartAcquireBus()` and `uartReleaseBus()` APIs.
`define `[`USB_USE_WAIT`](#group___h_a_l___c_o_n_f_1ga150144a73f541c7aca03c61f5ae16a6e)            | Enables synchronous APIs.

## Members

#### `define `[`HAL_USE_PAL`](#group___h_a_l___c_o_n_f_1ga2311bcc7b41bfba183c9ec1a64a11e93) {#group___h_a_l___c_o_n_f_1ga2311bcc7b41bfba183c9ec1a64a11e93}

Enables the PAL subsystem.

#### `define `[`HAL_USE_ADC`](#group___h_a_l___c_o_n_f_1ga413a1b6c0ca2c9e524df50ceac1275fd) {#group___h_a_l___c_o_n_f_1ga413a1b6c0ca2c9e524df50ceac1275fd}

Enables the ADC subsystem.

#### `define `[`HAL_USE_CAN`](#group___h_a_l___c_o_n_f_1ga2e351185eace5acfd90b65c9fe796355) {#group___h_a_l___c_o_n_f_1ga2e351185eace5acfd90b65c9fe796355}

Enables the CAN subsystem.

#### `define `[`HAL_USE_DAC`](#group___h_a_l___c_o_n_f_1gafa9dbe1932df28930e9cfff6000b43d6) {#group___h_a_l___c_o_n_f_1gafa9dbe1932df28930e9cfff6000b43d6}

Enables the DAC subsystem.

#### `define `[`HAL_USE_EXT`](#group___h_a_l___c_o_n_f_1ga2dd40c0fdd833b861f9241f6d282fe34) {#group___h_a_l___c_o_n_f_1ga2dd40c0fdd833b861f9241f6d282fe34}

Enables the EXT subsystem.

#### `define `[`HAL_USE_GPT`](#group___h_a_l___c_o_n_f_1gab4702b9e1b6fa2869e26c274fccd53f0) {#group___h_a_l___c_o_n_f_1gab4702b9e1b6fa2869e26c274fccd53f0}

Enables the GPT subsystem.

#### `define `[`HAL_USE_I2C`](#group___h_a_l___c_o_n_f_1gae5eb94efa72e401dc17a356b27f4e7e5) {#group___h_a_l___c_o_n_f_1gae5eb94efa72e401dc17a356b27f4e7e5}

Enables the I2C subsystem.

#### `define `[`HAL_USE_I2S`](#group___h_a_l___c_o_n_f_1ga5a92967aad4c273b71d8db15731d50d4) {#group___h_a_l___c_o_n_f_1ga5a92967aad4c273b71d8db15731d50d4}

Enables the I2S subsystem.

#### `define `[`HAL_USE_ICU`](#group___h_a_l___c_o_n_f_1gac58fde738d018b1233a5441ae4c6245b) {#group___h_a_l___c_o_n_f_1gac58fde738d018b1233a5441ae4c6245b}

Enables the ICU subsystem.

#### `define `[`HAL_USE_MAC`](#group___h_a_l___c_o_n_f_1ga0da24d012c72a79db22e5621b391d3da) {#group___h_a_l___c_o_n_f_1ga0da24d012c72a79db22e5621b391d3da}

Enables the MAC subsystem.

#### `define `[`HAL_USE_MMC_SPI`](#group___h_a_l___c_o_n_f_1gafc4461ac2cc33590839ed91f2e940079) {#group___h_a_l___c_o_n_f_1gafc4461ac2cc33590839ed91f2e940079}

Enables the MMC_SPI subsystem.

#### `define `[`HAL_USE_PWM`](#group___h_a_l___c_o_n_f_1gafb7a560610f96b2858d51c36d65f8c2f) {#group___h_a_l___c_o_n_f_1gafb7a560610f96b2858d51c36d65f8c2f}

Enables the PWM subsystem.

#### `define `[`HAL_USE_QSPI`](#group___h_a_l___c_o_n_f_1gae1476f8b999cce0f6781a1da03071fb5) {#group___h_a_l___c_o_n_f_1gae1476f8b999cce0f6781a1da03071fb5}

Enables the QSPI subsystem.

#### `define `[`HAL_USE_RTC`](#group___h_a_l___c_o_n_f_1ga17d20632810c9bb1601a19973977f55c) {#group___h_a_l___c_o_n_f_1ga17d20632810c9bb1601a19973977f55c}

Enables the RTC subsystem.

#### `define `[`HAL_USE_SDC`](#group___h_a_l___c_o_n_f_1ga06f315a18b911f426da736c42ab20325) {#group___h_a_l___c_o_n_f_1ga06f315a18b911f426da736c42ab20325}

Enables the SDC subsystem.

#### `define `[`HAL_USE_SERIAL`](#group___h_a_l___c_o_n_f_1ga720d92b9c3a00d952f6f0c8665b54ebb) {#group___h_a_l___c_o_n_f_1ga720d92b9c3a00d952f6f0c8665b54ebb}

Enables the SERIAL subsystem.

#### `define `[`HAL_USE_SERIAL_USB`](#group___h_a_l___c_o_n_f_1ga79c970fb20565143ad8b65720c0b2d0e) {#group___h_a_l___c_o_n_f_1ga79c970fb20565143ad8b65720c0b2d0e}

Enables the SERIAL over USB subsystem.

#### `define `[`HAL_USE_SPI`](#group___h_a_l___c_o_n_f_1ga33a3c5bee9ed1f665aace8b88fed9b2b) {#group___h_a_l___c_o_n_f_1ga33a3c5bee9ed1f665aace8b88fed9b2b}

Enables the SPI subsystem.

#### `define `[`HAL_USE_UART`](#group___h_a_l___c_o_n_f_1ga73220cad2edf2aee77b7ce1b53cacff0) {#group___h_a_l___c_o_n_f_1ga73220cad2edf2aee77b7ce1b53cacff0}

Enables the UART subsystem.

#### `define `[`HAL_USE_USB`](#group___h_a_l___c_o_n_f_1gafc8f8fc6010f46f5baae4a369e14974e) {#group___h_a_l___c_o_n_f_1gafc8f8fc6010f46f5baae4a369e14974e}

Enables the USB subsystem.

#### `define `[`HAL_USE_WDG`](#group___h_a_l___c_o_n_f_1gac3be0803223f25a5edeed87f56b526c3) {#group___h_a_l___c_o_n_f_1gac3be0803223f25a5edeed87f56b526c3}

Enables the WDG subsystem.

#### `define `[`ADC_USE_WAIT`](#group___h_a_l___c_o_n_f_1ga39e892a4090185fbdda9bb105bc03b4f) {#group___h_a_l___c_o_n_f_1ga39e892a4090185fbdda9bb105bc03b4f}

Enables synchronous APIs.

Disabling this option saves both code and data space.

#### `define `[`ADC_USE_MUTUAL_EXCLUSION`](#group___h_a_l___c_o_n_f_1gac0893cb47e338c2dabad34b974a0a88d) {#group___h_a_l___c_o_n_f_1gac0893cb47e338c2dabad34b974a0a88d}

Enables the `adcAcquireBus()` and `adcReleaseBus()` APIs.

Disabling this option saves both code and data space.

#### `define `[`CAN_USE_SLEEP_MODE`](#group___h_a_l___c_o_n_f_1ga5294d9d12e4186c781df3b2f1d8bd80b) {#group___h_a_l___c_o_n_f_1ga5294d9d12e4186c781df3b2f1d8bd80b}

Sleep mode related APIs inclusion switch.

#### `define `[`I2C_USE_MUTUAL_EXCLUSION`](#group___h_a_l___c_o_n_f_1ga7125642404a6fd3d4985d1cb6e1b7b0a) {#group___h_a_l___c_o_n_f_1ga7125642404a6fd3d4985d1cb6e1b7b0a}

Enables the mutual exclusion APIs on the I2C bus.

#### `define `[`MAC_USE_ZERO_COPY`](#group___h_a_l___c_o_n_f_1gad763d9426413cf2fe3922ebb8578bc59) {#group___h_a_l___c_o_n_f_1gad763d9426413cf2fe3922ebb8578bc59}

Enables an event sources for incoming packets.

#### `define `[`MAC_USE_EVENTS`](#group___h_a_l___c_o_n_f_1ga887da1c1383a9e7b45c4464877a7e05e) {#group___h_a_l___c_o_n_f_1ga887da1c1383a9e7b45c4464877a7e05e}

Enables an event sources for incoming packets.

#### `define `[`MMC_NICE_WAITING`](#group___h_a_l___c_o_n_f_1ga3087dfffa81dd8a0a80ee92746e65fe2) {#group___h_a_l___c_o_n_f_1ga3087dfffa81dd8a0a80ee92746e65fe2}

Delays insertions.

If enabled this options inserts delays into the MMC waiting routines releasing some extra CPU time for the threads with lower priority, this may slow down the driver a bit however. This option is recommended also if the SPI driver does not use a DMA channel and heavily loads the CPU.

#### `define `[`SDC_INIT_RETRY`](#group___h_a_l___c_o_n_f_1ga8d39f0c9799062f0698d97c26e6fa42d) {#group___h_a_l___c_o_n_f_1ga8d39f0c9799062f0698d97c26e6fa42d}

Number of initialization attempts before rejecting the card.

Attempts are performed at 10mS intervals.

#### `define `[`SDC_MMC_SUPPORT`](#group___h_a_l___c_o_n_f_1ga4f938eff7370feb8de8411e255d21b01) {#group___h_a_l___c_o_n_f_1ga4f938eff7370feb8de8411e255d21b01}

Include support for MMC cards.

MMC support is not yet implemented so this option must be kept at `FALSE`.

#### `define `[`SDC_NICE_WAITING`](#group___h_a_l___c_o_n_f_1ga3391c832c171a8606b0fc864766f08ba) {#group___h_a_l___c_o_n_f_1ga3391c832c171a8606b0fc864766f08ba}

Delays insertions.

If enabled this options inserts delays into the MMC waiting routines releasing some extra CPU time for the threads with lower priority, this may slow down the driver a bit however.

#### `define `[`SERIAL_DEFAULT_BITRATE`](#group___h_a_l___c_o_n_f_1gacb4c08ac23f83ac9d58c50ff840de516) {#group___h_a_l___c_o_n_f_1gacb4c08ac23f83ac9d58c50ff840de516}

Default bit rate.

Configuration parameter, this is the baud rate selected for the default configuration.

#### `define `[`SERIAL_BUFFERS_SIZE`](#group___h_a_l___c_o_n_f_1ga81a9fb00c7a1ce0fe70f263d8fd820e8) {#group___h_a_l___c_o_n_f_1ga81a9fb00c7a1ce0fe70f263d8fd820e8}

Serial buffers size.

Configuration parameter, you can change the depth of the queue buffers depending on the requirements of your application. The default is 16 bytes for both the transmission and receive buffers.

#### `define `[`SERIAL_USB_BUFFERS_SIZE`](#group___h_a_l___c_o_n_f_1ga24727a5407c6a41af53b59588592da80) {#group___h_a_l___c_o_n_f_1ga24727a5407c6a41af53b59588592da80}

Serial over USB buffers size.

Configuration parameter, the buffer size must be a multiple of the USB data endpoint maximum packet size. The default is 256 bytes for both the transmission and receive buffers.

#### `define `[`SERIAL_USB_BUFFERS_NUMBER`](#group___h_a_l___c_o_n_f_1ga0c332b2493fd2106beb13425549120fe) {#group___h_a_l___c_o_n_f_1ga0c332b2493fd2106beb13425549120fe}

Serial over USB number of buffers.

The default is 2 buffers.

#### `define `[`SPI_USE_WAIT`](#group___h_a_l___c_o_n_f_1ga0c9ab1488423fa10a22f506c6e74b563) {#group___h_a_l___c_o_n_f_1ga0c9ab1488423fa10a22f506c6e74b563}

Enables synchronous APIs.

Disabling this option saves both code and data space.

#### `define `[`SPI_USE_MUTUAL_EXCLUSION`](#group___h_a_l___c_o_n_f_1ga36d1818f9631f955f7cc94629b1d5498) {#group___h_a_l___c_o_n_f_1ga36d1818f9631f955f7cc94629b1d5498}

Enables the `spiAcquireBus()` and `spiReleaseBus()` APIs.

Disabling this option saves both code and data space.

#### `define `[`UART_USE_WAIT`](#group___h_a_l___c_o_n_f_1ga3ccbb19b26b53ba0a8e40cb6b8650114) {#group___h_a_l___c_o_n_f_1ga3ccbb19b26b53ba0a8e40cb6b8650114}

Enables synchronous APIs.

Disabling this option saves both code and data space.

#### `define `[`UART_USE_MUTUAL_EXCLUSION`](#group___h_a_l___c_o_n_f_1gae689e57cf792af401c324de566038687) {#group___h_a_l___c_o_n_f_1gae689e57cf792af401c324de566038687}

Enables the `uartAcquireBus()` and `uartReleaseBus()` APIs.

Disabling this option saves both code and data space.

#### `define `[`USB_USE_WAIT`](#group___h_a_l___c_o_n_f_1ga150144a73f541c7aca03c61f5ae16a6e) {#group___h_a_l___c_o_n_f_1ga150144a73f541c7aca03c61f5ae16a6e}

Enables synchronous APIs.

Disabling this option saves both code and data space.

