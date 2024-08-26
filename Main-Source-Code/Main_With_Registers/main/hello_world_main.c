
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/igmp.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "soc/rtc_periph.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "esp_spi_flash.h"

#include "driver/gpio.h"
#include "esp_intr_alloc.h"


#include "ATM90E36.h"


#define BLINK_GPIO GPIO_NUM_17 




void app_main(void)

{

    esp_rom_gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


start_spi();

//ESP_LOGI(TAG, "AS5048 device initialized on SPI with MISO=%d, MOSI=%d, CLK=%d, CS=%d, HZ=%d", pinMISO, pinMOSI, pinCLK, pinCS, SPI_HZ);

while(1){

vTaskDelay(100); 

uint16_t res = CommEnergyIC(1,0xBD,0x0); 
 
printf("Received: 0x%04X\n", res);

 gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS); 
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
}
}






// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/spi_master.h"
// #include "driver/gpio.h"
// #include "sdkconfig.h"
// #include "esp_log.h"

// static const char TAG[] = "VSPI";


// #define GPIO_MOSI 5
// #define GPIO_MISO 6
// #define GPIO_SCLK 4
// #define GPIO_CS 7

// #define BLINK_GPIO GPIO_NUM_17  // PCB LED

// void app_main(void)
// {

//     esp_rom_gpio_pad_select_gpio(BLINK_GPIO);
//     gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    
    
//     spi_device_interface_config_t devcfg = {
//         .clock_speed_hz = 1000000, // Clock out at 1 MHz with 1 us cycle
//         .mode = 3,                 // SPI mode 0 - the clock signal starts with a low signal
//         .spics_io_num = GPIO_CS,        // CS pin
//         .queue_size = 1,  
//         .duty_cycle_pos = 128,         // Queue 7 transactions at a time
//     };


  

//     ESP_LOGI(TAG, "Initializing VSPI"); // SPI3 = VSPI
//     spi_bus_config_t buscfg = {
//         .miso_io_num = GPIO_MISO,
//         .mosi_io_num = GPIO_MOSI,
//         .sclk_io_num = GPIO_SCLK,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//         .max_transfer_sz = 32,
        
//     };

//     // Initialize the SPI
//     spi_bus_initialize(SPI3_HOST, &buscfg,SPI_DMA_CH_AUTO);

//     // Define SPI handle
//     spi_device_handle_t spi;
//     spi_bus_add_device(SPI3_HOST, &devcfg, &spi);
 
//     // SPI transaction structure
//     spi_transaction_t trans;
//     memset(&trans, 0, sizeof(trans));
//     trans.length = 32;              // length in bits
//     const char test_str[] = "AB";    // ASCII A, Hex 41, Binary 01000001
//     uint32_t tx_buf = 0x80DB0000;
//     uint16_t rx_buf = 0x0;
    
    
//     trans.rx_buffer = &rx_buf;
//     trans.tx_buffer = &tx_buf;     // pointer to data to be transmitted
//     ESP_LOGI(TAG, "Write via VSPI: %s", test_str);

//     while (1)
//     {
//         spi_device_transmit(spi, &trans);
//         vTaskDelay(100);              // Every 10 ms
//         printf("Received: 0x%04X\n", rx_buf);


//                 //Blink
//         gpio_set_level(BLINK_GPIO, 1);
//         vTaskDelay(100 / portTICK_PERIOD_MS); 
//         gpio_set_level(BLINK_GPIO, 0);
//         vTaskDelay(100 / portTICK_PERIOD_MS);
//     }
// }




// // Master as a receiver for SPI communication

// // #include <stdio.h>
// // #include <stdint.h>
// // #include <stddef.h>
// // #include <string.h>

// // #include "freertos/FreeRTOS.h"
// // #include "freertos/task.h"
// // #include "freertos/semphr.h"
// // #include "freertos/queue.h"

// // #include "lwip/sockets.h"
// // #include "lwip/dns.h"
// // #include "lwip/netdb.h"
// // #include "lwip/igmp.h"

// // #include "esp_wifi.h"
// // #include "esp_system.h"
// // #include "esp_event.h"
// // #include "nvs_flash.h"
// // #include "soc/rtc_periph.h"
// // #include "driver/spi_master.h"
// // #include "esp_log.h"
// // #include "esp_spi_flash.h"

// // #include "driver/gpio.h"
// // #include "esp_intr_alloc.h"



// // #define BLINK_GPIO GPIO_NUM_17  // PCB LED


// // // Pins in use
// // #define GPIO_MOSI 5
// // #define GPIO_MISO 6
// // #define GPIO_SCLK 4
// // #define GPIO_CS 7






// // // Main application
// // void app_main(void)

// // {


// //     esp_rom_gpio_pad_select_gpio(BLINK_GPIO);
// //     gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


// //     spi_device_handle_t handle;

// //     // Configuration for the SPI bus
// //     spi_bus_config_t buscfg = {
// //         .mosi_io_num = GPIO_MOSI,
// //         .miso_io_num = GPIO_MISO,
// //         .sclk_io_num = GPIO_SCLK,
// //         .quadwp_io_num = -1,
// //         .quadhd_io_num = -1};

// //     // Configuration for the SPI device on the other side of the bus
// //     spi_device_interface_config_t devcfg = {
// //         .command_bits = 1,
// //         .address_bits = 15,
// //         .dummy_bits = 0,
// //         .clock_speed_hz = 5000000,
// //         .duty_cycle_pos = 128, // 50% duty cycle
// //         .mode = 3,
// //         .spics_io_num = GPIO_CS,
// //         .cs_ena_posttrans = 3, // Keep the CS low 3 cycles after transaction
// //         .queue_size = 3
        
// //         };
    
    
// //     spi_bus_initialize(SPI3_HOST, &buscfg, SPI_DMA_CH_AUTO);
// //     spi_bus_add_device(SPI3_HOST, &devcfg, &handle); // adding the slave

// // //END of init
    
    
// //     // data struct of spi
// //    // uint16_t* rcvbuf;  // Declaration of uint8_t variable
    
// //     uint16_t recvbuf = 0;

// //     uint16_t sendbuf1 = 0xf0DB ;
// //     // uint16_t sendbuf2 = 0x8001;
// //     // uint16_t sendbuf3 = 0x8002;
// //     // uint16_t sendbuf4 = 0x8095;
    
// //     //uint16_t sendbuf1 = 0x6901;
// //     uint16_t sendbuf2 = 0x8001;
// //     uint16_t sendbuf3 = 0x4001;
// //     uint16_t sendbuf4 = 0xA901;
    

// //     //uint32_t sendbuf = 0x8096;
// //     //memset(recvbuf, 0, 33);
// //     //char sendbuf[128] = {0};//send buff
// //     // Add 0xCC to a specific index in sendbuf
    
    
    
   
    
    
// //     spi_transaction_t t;
// //     memset(&t, 0, sizeof(t));

// //     printf("Master input:\n");
// //     while (1)
// //     {   //uint8_t thenu = 0xAA;
// //         t.length = 16;
// //         t.rxlength = 16;
// //         //int32_t thenu = *(int32_t *)t.rx_buffer;
// //         //t.flags = SPI_TRANS_USE_TXDATA;
// //         //t.tx_data[0] = 0xAA;
// //         t.cmd = 0x1;
// //         t.addr = 0xDBDB;
// //         t.tx_buffer = &sendbuf1;
// //         t.rx_buffer = &recvbuf;
// //         spi_device_transmit(handle, &t);
        
        
// //         printf("Received: 0x%04X\n", recvbuf);
// //         vTaskDelay(1000 / portTICK_PERIOD_MS);
        

// //         // t.tx_buffer = &sendbuf2;
// //         // spi_device_transmit(handle, &t);
// //         // printf("Received: 0x%04X\n", recvbuf);
// //         // vTaskDelay(1000 / portTICK_PERIOD_MS);

// //         // t.tx_buffer = &sendbuf3;
// //         // spi_device_transmit(handle, &t);
// //         // printf("Received: 0x%04X\n", recvbuf);
// //         // vTaskDelay(1000 / portTICK_PERIOD_MS);

// //         // t.tx_buffer = &sendbuf4;
// //         // spi_device_transmit(handle, &t);
// //         // printf("Received: 0x%04X\n", recvbuf);
// //         // vTaskDelay(1000 / portTICK_PERIOD_MS);
    

// //         //Blink
// //         gpio_set_level(BLINK_GPIO, 1);
// //         vTaskDelay(1000 / portTICK_PERIOD_MS); 
// //         gpio_set_level(BLINK_GPIO, 0);
// //         vTaskDelay(1000 / portTICK_PERIOD_MS);
// //     }
// // }
// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>
// // #include "freertos/FreeRTOS.h"
// // #include "freertos/task.h"
// // #include "driver/spi_master.h"
// // #include "driver/gpio.h"
// // #include "sdkconfig.h"
// // #include "esp_log.h"

// // static const char TAG[] = "VSPI";

// // void app_main(void)
// // {
// //     spi_device_interface_config_t devcfg = {
// //         .clock_speed_hz = 1000000, // Clock out at 1 MHz with 1 us cycle
// //         .mode = 0,                 // SPI mode 0 - the clock signal starts with a low signal
// //         .spics_io_num = 21,        // CS pin
// //         .queue_size = 7,           // Queue 7 transactions at a time
// //     };

  

// //     ESP_LOGI(TAG, "Initializing VSPI"); // SPI3 = VSPI
// //     spi_bus_config_t buscfg = {
// //         .miso_io_num = 18,
// //         .mosi_io_num = 19,
// //         .sclk_io_num = 5,
// //         .quadwp_io_num = -1,
// //         .quadhd_io_num = -1,
// //         .max_transfer_sz = 32,
// //     };

// //     // Initialize the SPI
// //     spi_bus_initialize(VSPI_HOST, &buscfg, 1);

// //     // Define SPI handle
// //     spi_device_handle_t spi;
// //     spi_bus_add_device(VSPI_HOST, &devcfg, &spi);
 
// //     // SPI transaction structure
// //     spi_transaction_t trans;
// //     memset(&trans, 0, sizeof(trans));
// //     trans.length = 32;              // length in bits
// //     const char test_str[] = "AB";    // ASCII A, Hex 41, Binary 01000001
// //     trans.tx_buffer = test_str;     // pointer to data to be transmitted
// //     ESP_LOGI(TAG, "Write via VSPI: %s", test_str);

// //     while (1)
// //     {
// //         spi_device_transmit(spi, &trans);
// //         vTaskDelay(100);              // Every 10 ms
// //     }
// // }






















// // // Master as a receiver for SPI communication

// // #include <stdio.h>
// // #include <stdint.h>
// // #include <stddef.h>
// // #include <string.h>

// // #include "freertos/FreeRTOS.h"
// // #include "freertos/task.h"
// // #include "freertos/semphr.h"
// // #include "freertos/queue.h"

// // #include "lwip/sockets.h"
// // #include "lwip/dns.h"
// // #include "lwip/netdb.h"
// // #include "lwip/igmp.h"

// // #include "esp_wifi.h"
// // #include "esp_system.h"
// // #include "esp_event.h"
// // #include "nvs_flash.h"
// // #include "soc/rtc_periph.h"
// // #include "driver/spi_master.h"
// // #include "esp_log.h"
// // #include "esp_spi_flash.h"

// // #include "driver/gpio.h"
// // #include "esp_intr_alloc.h"

// // // Pins in use
// // #define GPIO_MOSI 19
// // #define GPIO_MISO 18
// // #define GPIO_SCLK 5
// // #define GPIO_CS 21






// // // Main application
// // void app_main(void)
// // {
// //     spi_device_handle_t handle;

// //     // Configuration for the SPI bus
// //     spi_bus_config_t buscfg = {
// //         .mosi_io_num = GPIO_MOSI,
// //         .miso_io_num = GPIO_MISO,
// //         .sclk_io_num = GPIO_SCLK,
// //         .quadwp_io_num = -1,
// //         .quadhd_io_num = -1};

// //     // Configuration for the SPI device on the other side of the bus
// //     spi_device_interface_config_t devcfg = {
// //         .command_bits = 0,
// //         .address_bits = 0,
// //         .dummy_bits = 0,
// //         .clock_speed_hz = 5000000,
// //         .duty_cycle_pos = 128, // 50% duty cycle
// //         .mode = 0,
// //         .spics_io_num = GPIO_CS,
// //         .cs_ena_posttrans = 3, // Keep the CS low 3 cycles after transaction
// //         .queue_size = 3};
    
    
// //     spi_bus_initialize(VSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
// //     spi_bus_add_device(VSPI_HOST, &devcfg, &handle); // adding the slave

// // //END of init
    
    
// //     // data struct of spi
// //    // uint16_t* rcvbuf;  // Declaration of uint8_t variable
// //     char recvbuf[32] = "";
// //     memset(recvbuf, 0, 33);
// //     char sendbuf[128] = {0};//send buff
// //     // Add 0xCC to a specific index in sendbuf
// //     sendbuf[0] = 0xCC;
    
    
   
    
    
// //     spi_transaction_t t;
// //     memset(&t, 0, sizeof(t));

// //     printf("Master input:\n");
// //     while (1)
// //     {   //uint8_t thenu = 0xAA;
// //         t.length = 32;
// //         t.rxlength = 32;
// //         //int32_t thenu = *(int32_t *)t.rx_buffer;
// //         //t.flags = SPI_TRANS_USE_TXDATA;
// //         //t.tx_data[0] = 0xAA;
// //         t.rx_buffer = recvbuf;
// //         spi_device_transmit(handle, &t);
        
        
// //         printf("Received: %s\n", recvbuf);
// //         vTaskDelay(1000 / portTICK_PERIOD_MS);
// //     }
// // }



// // // #include <stdio.h>
// // // #include <stdlib.h>
// // // #include <string.h>
// // // #include "freertos/FreeRTOS.h"
// // // #include "freertos/task.h"
// // // #include "driver/spi_master.h"
// // // #include "driver/gpio.h"
// // // #include "sdkconfig.h"
// // // #include "esp_log.h"

// // // static const char TAG[] = "VSPI";

// // // void app_main(void)
// // // {
// // //     spi_device_interface_config_t devcfg = {
// // //         .clock_speed_hz = 1000000, // Clock out at 1 MHz with 1 us cycle
// // //         .mode = 0,                 // SPI mode 0 - the clock signal starts with a low signal
// // //         .spics_io_num = 21,        // CS pin
// // //         .queue_size = 7,           // Queue 7 transactions at a time
// // //     };

  

// // //     ESP_LOGI(TAG, "Initializing VSPI"); // SPI3 = VSPI
// // //     spi_bus_config_t buscfg = {
// // //         .miso_io_num = 18,
// // //         .mosi_io_num = 19,
// // //         .sclk_io_num = 5,
// // //         .quadwp_io_num = -1,
// // //         .quadhd_io_num = -1,
// // //         .max_transfer_sz = 32,
// // //     };

// // //     // Initialize the SPI
// // //     spi_bus_initialize(VSPI_HOST, &buscfg, 1);

// // //     // Define SPI handle
// // //     spi_device_handle_t spi;
// // //     spi_bus_add_device(VSPI_HOST, &devcfg, &spi);
 
// // //     // SPI transaction structure
// // //     spi_transaction_t trans;
// // //     memset(&trans, 0, sizeof(trans));
// // //     trans.length = 32;              // length in bits
// // //     const char test_str[] = "AB";    // ASCII A, Hex 41, Binary 01000001
// // //     trans.tx_buffer = test_str;     // pointer to data to be transmitted
// // //     ESP_LOGI(TAG, "Write via VSPI: %s", test_str);

// // //     while (1)
// // //     {
// // //         spi_device_transmit(spi, &trans);
// // //         vTaskDelay(100);              // Every 10 ms
// // //     }
// // // }