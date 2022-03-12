/* Pisco-LED-Code.ino
 * 
 * This sketch demonstrates how to use the Pisco-LED-Code
 * library to show decimal or hexadecimal values using just
 * a single LED. 
 * 
 * These values can be shown as positive or negative when needed.
 * 
 * The Pisco-LED-Code library is a nom blocking function that 
 * should be called frequently from the loop function. 
 * 
 * Andre Viegas
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/
#include "Pisco-LED-Code.h"
#include "RTClib.h"
#include "Wire.h"

#define DEBUG
/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/
PiscoCode      ledBuiltin;         // declare an object of class PiscoCode
RTC_DS1307     rtc;                // declare an object of class RTC

const unsigned long  timeBetweenCounter = 30000UL;
unsigned long        lastMillis = timeBetweenCounter;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup() {
  #ifdef DEBUG
    char somedata[] = "this is data from the eeprom"; // data to write
    Wire.begin();
    Serial.begin(57600);
    Serial.println("One LED Clock!");
    i2c_eeprom_write_page(0x50, 0, (byte *)somedata, sizeof(somedata)); // write to EEPROM
    delay(100); //add a small delay
    Serial.println("Memory written");
  #endif
  pinMode(LED_BUILTIN, OUTPUT);                    // initialize digital pin LED_BUILTIN as an output.                  
  ledBuiltin.setup(&turnLed1On, &turnLed1Off, 0);  // calling the PiscoCode class constructor.
  #ifdef DEBUG
//    ledBuiltin.showDec(12, 10, 1);
  #endif

    int addr=0; //first address
    byte b = i2c_eeprom_read_byte(0x50, 0); // access the first address from the memory

    while (b!=0)
    {
        Serial.print((char)b); //print content to serial port
        addr++; //increase address
        b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
    }
    Serial.println(" ");

   readViaWire();

  
  if (! rtc.begin()) {
    #ifdef DEBUG
       Serial.println("Couldn't find RTC");
       Serial.flush();
    #endif
    //while (1) delay(10);
  }
  #ifdef DEBUG
  else {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  #endif

  #ifdef DEBUG
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }  
  #endif
}

void turnLed1On(void) {
  digitalWrite(LED_BUILTIN, HIGH);
}

void turnLed1Off(void) {
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {   
   if ( (unsigned long)(millis() - lastMillis) >= timeBetweenCounter &&
         ! ledBuiltin.isSequencing() ) {
      DateTime now = rtc.now();
      
      unsigned long    bcd_now = now.twelveHour()*100+now.minute();
      
      ledBuiltin.showDec(bcd_now, 10, 1);
      lastMillis = millis();
   }  
   
   ledBuiltin.loop(millis());                  // We should call the LOOP function regularly.
}

void readViaWire(void){
  char * week[] = { "Domingo"      , "Segunda-feira" , "Terca-feira" ,
                    "Quarta-feira" , "Quinta-feira"  , "Sexta-feira" ,
                    "Sabado" } ;
 
  char * month[] = { "Janeiro"  , "Fevereiro" , "Marco"    , "Abril"      ,
                     "Maio"     , "Junho"     , "Julho"    , "Agosto"     ,
                     "Setembro" , "Outubro"   , "Novembro" , "Dezembro" } ;
  
  unsigned char buf[ 7 ] ;
  unsigned char i ;
  
  Wire.beginTransmission( 0x50 ) ;
  Wire.write( 0x00 ) ; // Endereço 00h.
  Wire.endTransmission();
 
  Wire.requestFrom( 0x68 , 7 ) ;
 
  for( i = 0 ; i < 7 ; i++ )
  {
    buf[ i ] = Wire.read() ;
  }
 
  Serial.print( week[ ( buf[ 3 ] - 1 ) & 0x07 ] ) ;
  Serial.print( ", " ) ;
  Serial.print( buf[ 4 ] , HEX ) ;
  Serial.print( " de " ) ;
  Serial.print( month[ ( buf[ 5 ] - 1 ) % 11 ] ) ;
  Serial.print( " de 20" ) ;
  Serial.print( buf[ 6 ] >>   4 , HEX ) ;
  Serial.print( buf[ 6 ] & 0x0F , HEX ) ;
  Serial.print( " " ) ;
  Serial.print( ( buf[ 2 ] >> 4 ) & 0x03 , HEX ) ;
  Serial.print( buf[ 2 ] & 0x0F , HEX ) ;
  Serial.print( ":" ) ;
  Serial.print( buf[ 1 ] >>   4 , HEX ) ;
  Serial.print( buf[ 1 ] & 0x0F , HEX ) ;
  Serial.print( ":" ) ;
  Serial.print( ( buf[ 0 ] & 0x07 ) >>   4 , HEX ) ;
  Serial.print( buf[ 0 ] & 0x0F , HEX ) ;
 
  Serial.println() ;
}



void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
        Wire.write(data[c]);
    Wire.endTransmission();
}

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
}

// maybe let's not read more than 30 or 32 bytes at a time!
void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
        if (Wire.available()) buffer[c] = Wire.read();
}
