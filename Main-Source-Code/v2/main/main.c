// Copyright (c) 2024 Mihiran Wickramarathne. All rights reserved.


#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "driver/gpio.h"


#include "ATM90E36.h"
#include "wifi.h"
#include "mqtt.h"

#define BLINK_GPIO GPIO_NUM_17 

#define INDICATOR_LED GPIO_NUM_18 

#define CONNECTED 1
#define CONNECTING 0

//Registers
#define GPIO_Base 0x60004000

#define GPIO_OUT_W1TS_REG (GPIO_Base + 0x0008)
#define GPIO_OUT_W1TC_REG (GPIO_Base + 0x000C)
#define GPIO_ENABLE_REG   (GPIO_Base + 0x0020)

volatile uint32_t* gpio_out_w1ts_reg = (volatile uint32_t*) GPIO_OUT_W1TS_REG;
volatile uint32_t* gpio_out_w1tc_reg = (volatile uint32_t*) GPIO_OUT_W1TC_REG;
volatile uint32_t* gpio_enable_reg = (volatile uint32_t*) GPIO_ENABLE_REG;

#define GPIO_MOSI 5
#define GPIO_MISO 6
#define GPIO_SCLK 4
#define GPIO_CS 7


bool device_state;

//LED Blink
void blink()
{
  while(1){
        if (device_state)
        {
          // blink pulse for CONNECTED state
            *gpio_out_w1ts_reg = (1 << INDICATOR_LED); // Remove this
            //gpio_set_level(INDICATOR_LED, 1);
            vTaskDelay(100 / portTICK_PERIOD_MS); // 100 ms ON
            *gpio_out_w1tc_reg = (1 << INDICATOR_LED);
            //gpio_set_level(INDICATOR_LED, 0);
            vTaskDelay(2000 / portTICK_PERIOD_MS); // 1000 ms OFF
            
        }
        else 
        {   
            
            // Fast blink for CONNECTING state
            gpio_set_level(INDICATOR_LED, 1);
            vTaskDelay(100 / portTICK_PERIOD_MS); // 100 ms ON
            gpio_set_level(INDICATOR_LED, 0);
            vTaskDelay(100 / portTICK_PERIOD_MS); // 100 ms OFF
        }
  }
}



void heart_beat(){
  while(1){
    mqtt_publish("Energy_monitor/monitor_1/heart_beat", 20);//Does 20 mean anything? NO!!!!
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 seconds
  }
}



void app_main(void)

{
//Register Definitions


*gpio_enable_reg = (1 << INDICATOR_LED) | (1 << GPIO_CS) | (1 << GPIO_SCLK) | (1 <<GPIO_MOSI); // Setting the outputs

device_state = CONNECTING;

esp_rom_gpio_pad_select_gpio(BLINK_GPIO);
gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

gpio_set_level(BLINK_GPIO, 1);//Turing on The LED

esp_rom_gpio_pad_select_gpio(INDICATOR_LED);
gpio_set_direction(INDICATOR_LED, GPIO_MODE_OUTPUT);



//creating the blink task
xTaskCreate(&blink, "led_blink_task", 2048, NULL, 5, NULL);


// Wifi 
esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
ESP_ERROR_CHECK(ret);
wifi_init_sta();// Wifi Init
vTaskDelay(1000 / portTICK_PERIOD_MS);
start_spi();
begin_ATM90E36();
mqtt_app_start();

//ESP_LOGI(TAG, "AS5048 device initialized on SPI with MISO=%d, MOSI=%d, CLK=%d, CS=%d, HZ=%d", pinMISO, pinMOSI, pinCLK, pinCS, SPI_HZ);

xTaskCreate(&heart_beat, "heartbeat_task", 2048, NULL, 5, NULL);

while(1){

vTaskDelay(100); 

uint16_t res = CommEnergyIC(1,0xBD,0x0); 


double current = GetLineCurrentC();
double temp = GetTemperature();



//Publishing The Data
   // Get Line Voltage A, print it, and publish it to MQTT
    double LineVoltageA = GetLineVoltageA();
    printf("Line Voltage A: %f\n", LineVoltageA);
    mqtt_publish("Energy_monitor/monitor_1/Phase_A/Line_Voltage_A", LineVoltageA);

    // Get Line Voltage B, print it, and publish it to MQTT
    double LineVoltageB = GetLineVoltageB();
    printf("Line Voltage B: %f\n", LineVoltageB);
    mqtt_publish("Energy_monitor/monitor_1/Phase_B/Line_Voltage_B", LineVoltageB);

    // Get Line Voltage C, print it, and publish it to MQTT
    double LineVoltageC = GetLineVoltageC();
    printf("Line Voltage C: %f\n", LineVoltageC);
    mqtt_publish("Energy_monitor/monitor_1/Phase_C/Line_Voltage_C", LineVoltageC);

    // Get Line Current A, print it, and publish it to MQTT
    double LineCurrentA = GetLineCurrentA();
    printf("Line Current A: %f\n", LineCurrentA);
    mqtt_publish("Energy_monitor/monitor_1/Phase_A/Line_Current_A", LineCurrentA);

    // Get Line Current B, print it, and publish it to MQTT
    double LineCurrentB = GetLineCurrentB();
    printf("Line Current B: %f\n", LineCurrentB);
    mqtt_publish("Energy_monitor/monitor_1/Phase_B/Line_Current_B", LineCurrentB);

    // Get Line Current C, print it, and publish it to MQTT
    double LineCurrentC = GetLineCurrentC();
    printf("Line Current C: %f\n", LineCurrentC);
    mqtt_publish("Energy_monitor/monitor_1/Phase_C/Line_Current_C", LineCurrentC);

    // Get Line Current N, print it, and publish it to MQTT
    double LineCurrentN = GetLineCurrentN();
    printf("Line Current N: %f\n", LineCurrentN);
    mqtt_publish("Energy_monitor/monitor_1/Neutral/Line_Current_N", LineCurrentN);

    // Get Active Power A, print it, and publish it to MQTT
    double ActivePowerA = GetActivePowerA();
    printf("Active Power A: %f\n", ActivePowerA);
    mqtt_publish("Energy_monitor/monitor_1/Phase_A/Active_Power_A", ActivePowerA);

    // Get Active Power B, print it, and publish it to MQTT
    double ActivePowerB = GetActivePowerB();
    printf("Active Power B: %f\n", ActivePowerB);
    mqtt_publish("Energy_monitor/monitor_1/Phase_B/Active_Power_B", ActivePowerB);

    // Get Active Power C, print it, and publish it to MQTT
    double ActivePowerC = GetActivePowerC();
    printf("Active Power C: %f\n", ActivePowerC);
    mqtt_publish("Energy_monitor/monitor_1/Phase_C/Active_Power_C", ActivePowerC);

    // Get Total Active Power, print it, and publish it to MQTT
    double TotalActivePower = GetTotalActivePower();
    printf("Total Active Power: %f\n", TotalActivePower);
    mqtt_publish("Energy_monitor/monitor_1/Total_Active_Power", TotalActivePower);

    // Get Reactive Power A, print it, and publish it to MQTT
    double ReactivePowerA = GetReactivePowerA();
    printf("Reactive Power A: %f\n", ReactivePowerA);
    mqtt_publish("Energy_monitor/monitor_1/Phase_A/Reactive_Power_A", ReactivePowerA);

    // Get Reactive Power B, print it, and publish it to MQTT
    double ReactivePowerB = GetReactivePowerB();
    printf("Reactive Power B: %f\n", ReactivePowerB);
    mqtt_publish("Energy_monitor/monitor_1/Phase_B/Reactive_Power_B", ReactivePowerB);

    // Get Reactive Power C, print it, and publish it to MQTT
    double ReactivePowerC = GetReactivePowerC();
    printf("Reactive Power C: %f\n", ReactivePowerC);
    mqtt_publish("Energy_monitor/monitor_1/Phase_C/Reactive_Power_C", ReactivePowerC);

    // Get Total Reactive Power, print it, and publish it to MQTT
    double TotalReactivePower = GetTotalReactivePower();
    printf("Total Reactive Power: %f\n", TotalReactivePower);
    mqtt_publish("Energy_monitor/monitor_1/Total_Reactive_Power", TotalReactivePower);

    // Get Apparent Power A, print it, and publish it to MQTT
    double ApparentPowerA = GetApparentPowerA();
    printf("Apparent Power A: %f\n", ApparentPowerA);
    mqtt_publish("Energy_monitor/monitor_1/Phase_A/Apparent_Power_A", ApparentPowerA);

    // Get Apparent Power B, print it, and publish it to MQTT
    double ApparentPowerB = GetApparentPowerB();
    printf("Apparent Power B: %f\n", ApparentPowerB);
    mqtt_publish("Energy_monitor/monitor_1/Phase_B/Apparent_Power_B", ApparentPowerB);

    // Get Apparent Power C, print it, and publish it to MQTT
    double ApparentPowerC = GetApparentPowerC();
    printf("Apparent Power C: %f\n", ApparentPowerC);
    mqtt_publish("Energy_monitor/monitor_1/Phase_C/Apparent_Power_C", ApparentPowerC);

    // Get Total Apparent Power, print it, and publish it to MQTT
    double TotalApparentPower = GetTotalApparentPower();
    printf("Total Apparent Power: %f\n", TotalApparentPower);
    mqtt_publish("Energy_monitor/monitor_1/Total_Apparent_Power", TotalApparentPower);

    // Get Frequency, print it, and publish it to MQTT
    double Frequency = GetFrequency();
    printf("Frequency: %f\n", Frequency);
    mqtt_publish("Energy_monitor/monitor_1/Frequency", Frequency);

    // Get Power Factor A, print it, and publish it to MQTT
    double PowerFactorA = GetPowerFactorA();
    printf("Power Factor A: %f\n", PowerFactorA);
    mqtt_publish("Energy_monitor/monitor_1/Phase_A/Power_Factor_A", PowerFactorA);

    // Get Power Factor B, print it, and publish it to MQTT
    double PowerFactorB = GetPowerFactorB();
    printf("Power Factor B: %f\n", PowerFactorB);
    mqtt_publish("Energy_monitor/monitor_1/Phase_B/Power_Factor_B", PowerFactorB);

    // Get Power Factor C, print it, and publish it to MQTT
    double PowerFactorC = GetPowerFactorC();
    printf("Power Factor C: %f\n", PowerFactorC);
    mqtt_publish("Energy_monitor/monitor_1/Phase_C/Power_Factor_C", PowerFactorC);

    // Get Total Power Factor, print it, and publish it to MQTT
    double TotalPowerFactor = GetTotalPowerFactor();
    printf("Total Power Factor: %f\n", TotalPowerFactor);
    mqtt_publish("Energy_monitor/monitor_1/Total_Power_Factor", TotalPowerFactor);

    // Get Phase A, print it, and publish it to MQTT
    double PhaseA = GetPhaseA();
    printf("Phase A: %f\n", PhaseA);
    mqtt_publish("Energy_monitor/monitor_1/Phase_A/Phase_A", PhaseA);

    // Get Phase B, print it, and publish it to MQTT
    double PhaseB = GetPhaseB();
    printf("Phase B: %f\n", PhaseB);
    mqtt_publish("Energy_monitor/monitor_1/Phase_B/Phase_B", PhaseB);

    // Get Phase C, print it, and publish it to MQTT
    double PhaseC = GetPhaseC();
    printf("Phase C: %f\n", PhaseC);
    mqtt_publish("Energy_monitor/monitor_1/Phase_C/Phase_C", PhaseC);

    // Get Temperature, print it, and publish it to MQTT
    double Temperature = GetTemperature();
    printf("Temperature: %f\n", Temperature);
    mqtt_publish("Energy_monitor/monitor_1/Temperature", Temperature);




 gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS); 
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);

//We are going to check the register values



// unsigned short UgainA_test = CommEnergyIC(READ,UgainA, 0xFFFF);
// unsigned short UgainC_test = CommEnergyIC(READ, UoffsetC, 0xFFFF);
// unsigned short HarmStart_test = CommEnergyIC(READ, HarmStart, 0xFFFF);
// unsigned short voltageC_test = CommEnergyIC(READ, IoffsetA, 0xFFFF);



// printf("Gain: %X\n", UgainA_test);
// printf("COFFSET: %X\n", UgainC_test);
// printf("HexadecimalC: %X\n", voltageC_test);

}
}





