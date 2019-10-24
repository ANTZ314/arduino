/* 
 * The ESP32 has four SPi buses, however as of right now only two of
 * them are available to use, HSPI and VSPI. Simply using the SPI API 
 * as illustrated in Arduino examples will use VSPI, leaving HSPI unused.
 * 
 * However if we simply intialise two instance of the SPI class for both
 * of these buses both can be used. However when just using these the Arduino
 * way only will actually be outputting at a time.
 * 
 * SDI = MOSI - VSPID   - IO_23 (data_out)
 * SDO = MISO - VSPIQ   - IO_19 (data_in)
 * SCL = SCLK - VSPICLK - IO_18
 * CS  = CS   - VSPICS0 - IO_05
 */
#include <SPI.h>

#define VSPI_SS 5       			// VSPI Chip select pin

static const int spiClk = 1000000; 	// 1 MHz

//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;

void setup() {
  //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  vspi = new SPIClass(VSPI);
  
  /* Initialise vspi with default pins */
  //SCLK = 18, MISO = 19, MOSI = 23, SS = 5
  vspi->begin();
  /* Alternatively route through GPIO pins of your choice */
  //vspi->begin(0, 2, 4, 33); //SCLK, MISO, MOSI, SS

  /* set up slave select pins as outputs as the Arduino API */
  /* doesn't handle automatically pulling SS low */
  pinMode(VSPI_SS, OUTPUT);  //VSPI SS

}

// the loop function runs over and over again until power down or reset
void loop() {
  //use the SPI buses
  vspiCommand();
  delay(100);
}

void vspiCommand() {
  byte data = 0b01010101; // junk data to illustrate usage

  //use it as you would the regular arduino SPI API
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
  vspi->transfer(data);  
  digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
  vspi->endTransaction();
}
