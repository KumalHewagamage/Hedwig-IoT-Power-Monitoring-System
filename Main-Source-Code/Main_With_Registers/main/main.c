// #include <stdio.h>
// #include "driver/spi_master.h"
// #include "ATM90E36.h"



// #define GPIO_Base 0x60004000 // Base adress for esp32s3


// #define GPIO_OUT_W1TS_REG (GPIO_Base + 0x0008)
// #define GPIO_OUT_W1TC_REG (GPIO_Base + 0x000C)
// #define GPIO_ENABLE_REG   (GPIO_Base + 0x0020)


// //SPI PINS
// #define GPIO_MOSI 5
// #define GPIO_MISO 6
// #define GPIO_SCLK 4
// #define GPIO_CS 7

// //LED PIN
// #define LED 17






// void app_main(void)
// {

// volatile uint32_t* gpio_out_w1ts_reg = (volatile uint32_t*) GPIO_OUT_W1TS_REG;
// volatile uint32_t* gpio_out_w1tc_reg = (volatile uint32_t*) GPIO_OUT_W1TC_REG;
// volatile uint32_t* gpio_enable_reg = (volatile uint32_t*) GPIO_ENABLE_REG;



// *gpio_enable_reg = (1 << LED) | (1 << GPIO_CS) | (1 << GPIO_SCLK) | (1 <<GPIO_MOSI); // Setting the outputs


// *gpio_out_w1ts_reg = (1 << LED); // Remove this

// //Initialize SPI bus
// start();
// begin();
// //Connecting the SPI bus to the pins

// // //Bus Information
// //   spi_bus_config_t buscfg = {
// //         .miso_io_num = GPIO_MISO,
// //         .mosi_io_num = GPIO_MOSI,
// //         .sclk_io_num = GPIO_SCLK,
// //         .quadwp_io_num = -1,
// //         .quadhd_io_num = -1,
// //         .max_transfer_sz = 32,
        
// //     };
// // //Initialize the SPI bus
// // spi_bus_initialize(SPI3_HOST, &buscfg,SPI_DMA_CH_AUTO);


// // //Device Information

// // spi_device_interface_config_t devcfg = {
// //         .clock_speed_hz = 1000000, // Clock out at 1 MHz with 1 us cycle
// //         .mode = 3,                 // SPI mode 3 
// //         .spics_io_num = GPIO_CS,        // CS pin
// //         .queue_size = 1,  
// //         .duty_cycle_pos = 128,        
// //     };
// //     //Adding the device
// //     spi_device_handle_t spi;
// //     spi_bus_add_device(SPI3_HOST, &devcfg, &spi);


// // //Connecting to the WiFi




// }