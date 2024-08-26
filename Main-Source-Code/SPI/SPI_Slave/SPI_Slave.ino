#include<SPI.h>

volatile boolean received;
volatile byte Slavereceived,Slavesend;
void setup()


{
  Serial.begin(115200);
  pinMode(MISO,OUTPUT);                   //Sets MISO as OUTPUT (Have to Send data to Master IN 
  SPCR |= _BV(SPE) | _BV(CPOL) | _BV(CPHA);                   //Turn on SPI in Slave Mode
  received = false;
  SPI.attachInterrupt();                  //Interuupt ON is set for SPI commnucation
}
ISR (SPI_STC_vect)                        //Inerrrput routine function 
{
  Slavereceived = SPDR;         // Value received from master if store in variable slavereceived
  received = true;   

  //Some bullshit
  // char myChar = (char)Slavereceived;
  // Serial.print(myChar);
  //end of bullshit

  Serial.println(Slavereceived);
  Slavesend=0xCC;                             
  SPDR = Slavesend;                 //Sets received as True 
}
void loop()
{
  
}