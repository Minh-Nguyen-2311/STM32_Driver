This is my repository for STM32_Peripheral_Drivers which can be used for either bare metal or RTOS applications.

There will be a number of parts that I will be covered in this repository:

I) Communication Interfaces:
1. USART/UART: Serial communication between 2 devices.
2. SPI: Synchronous, full-duplex protocol for connecting multiple slave devices to a single master device.
3. I2C: Synchronous, half-duplex protocol for multi-master/multi-slave.
4. CAN: Controller Area Network for peer-to-peer communication without a host.
5. SDIO/I2S: SD card interface and digital audio interface.
6. Ethernet: Communication over LAN for high-speed networking.

II) Timers:
1. General-Purpose Timers: For timing, counting, PWM generation, etc.
2. Advanced Control Timers: Used in motor control, advanced PWM applications.
3. Watchdog Timers: Used for system monitoring and reset in case of a fault.

III) Analog & Other Peripherals:
1. ADC: Converting analog signals into digital signals (ex: sensor reading).
2. DAC: Converting digital signals into analog signals (ex: audio processing).
3. DMA: Directly data transfer between peripherals and memory without CPU intervention.
4. CRC: Error detection for data transmission.
