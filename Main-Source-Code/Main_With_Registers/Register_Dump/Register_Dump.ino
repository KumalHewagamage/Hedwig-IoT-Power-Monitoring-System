
/* The ESP32 has four SPi buses, however as of right now only two of
 * them are available to use, HSPI and VSPI. Simply using the SPI API 
 * as illustrated in Arduino examples will use VSPI, leaving HSPI unused.
 * 
 * However if we simply intialise two instance of the SPI class for both
 * of these buses both can be used. However when just using these the Arduino
 * way only will actually be outputting at a time.
 * 
 * Logic analyser capture is in the same folder as this example as
 * "multiple_bus_output.png"
 * 
 * created 30/04/2018 by Alistair Symonds
 */
#include <SPI.h>

// Define ALTERNATE_PINS to use non-standard GPIO pins for SPI bus

  #define LED 17
  #define VSPI_MISO   6
  #define VSPI_MOSI   5
  #define VSPI_SCLK   4
  #define VSPI_SS     7



#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#define VSPI FSPI
#endif

static const int spiClk = 1000000; // 1 MHz

//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;
SPIClass * hspi = NULL;


uint16_t myByte;

void setup() {

  pinMode(LED,OUTPUT);
  Serial.begin(115200);
  //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  vspi = new SPIClass(VSPI);
  hspi = new SPIClass(HSPI);
  
  //clock miso mosi ss



  //alternatively route through GPIO pins of your choice
  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS); //SCLK, MISO, MOSI, SS


#ifndef ALTERNATE_PINS
  //initialise hspi with default pins
  //SCLK = 14, MISO = 12, MOSI = 13, SS = 15
  hspi->begin();
#else
  //alternatively route through GPIO pins
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS); //SCLK, MISO, MOSI, SS
#endif

  //set up slave select pins as outputs as the Arduino API
  //doesn't handle automatically pulling SS low
  pinMode(vspi->pinSS(), OUTPUT); //VSPI SS
  pinMode(hspi->pinSS(), OUTPUT); //HSPI SS
  myByte = 0x8000;
}

// the loop function runs over and over again until power down or reset
void loop() {
  digitalWrite(LED,HIGH);
  //use the SPI buses
  //Some Bullshit
  char myChar = 'D'; // Example char value
  //myByte = myByte+0x1; // Convert char to byte
  myByte = 0x80BC;
  spiCommand(vspi, myByte);
  
  //spiCommand(vspi,0x0A);//Sends new line character
  delay(100);
}

void spiCommand(SPIClass *spi, uint16_t data) {
  //use it as you would the regular arduino SPI API
  spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE3));
  digitalWrite(spi->pinSS(), LOW); //pull SS slow to prep other end for transfer
  uint8_t dataHigh = (data >> 8); // Extract high byte
  uint8_t dataLow = data ;         // Extract low byte
  
 
  uint8_t reciHigh = spi->transfer(dataHigh); // Transfer high byte
  uint8_t reciLow = spi->transfer(dataLow); 
  reciHigh = spi->transfer(dataHigh); // Transfer high byte
  reciLow = spi->transfer(dataLow);  // Transfer low byte
  
  uint16_t reci = (reciHigh << 8) | reciLow;
  if (reci!=0){
    Serial.println("Thenuri");
    Serial.println(data,HEX);
    Serial.println(reci,HEX);
    delay(1000);
  }
  //Serial.println(myByte,HEX);
  digitalWrite(spi->pinSS(), HIGH); //pull ss high to signify end of data transfer
  spi->endTransaction();
}