#include <NeoTee.h>
#include <NeoTeeStream.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

SFE_BMP180 pressure;

double baseline; // baseline pressure

File myFile;
Print *outputs[] = { &Serial, &myFile };  // <--  list all the output destinations here
NeoTee tee( outputs, sizeof(outputs)/sizeof(outputs[0]) );

long currentMillis;
long startMillis;

void setup()
{
  Serial.begin(9600);
  startMillis = millis();  //initial start time
  pressure.begin();
  baseline = getPressure();
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  Serial.println("SD card initialization done.");

  myFile = SD.open("test.txt", FILE_WRITE);
   // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("Testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("Error opening test.txt");
  }
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("Error opening test.txt");
  }
}

void loop()
{
  char status;
  double T,P,a;
  myFile = SD.open("test.txt", FILE_WRITE);
 
  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      tee.print("Temperature: ");
      tee.print(T,2);
      tee.print(" deg C, ");
      
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          tee.print("Pressure: ");
          tee.print(P*0.0295333727,2);
          tee.print(" Hg, ");

          // Show the relative altitude difference between
          // the new reading and the baseline reading:
          a = pressure.altitude(P,baseline);
          tee.print("Altitude: ");
          tee.print(a,2);
          tee.print(" m, ");
        }
        else tee.println("Error retrieving pressure measurement\n");
      }
      else tee.println("Error starting pressure measurement\n");
    }
    else tee.println("Error retrieving temperature measurement\n");
  }
  else tee.println("Error starting temperature measurement\n");
  
  currentMillis = millis();
  double durSS = (currentMillis/1000);    //Seconds 

  tee.print("Time: ");
  tee.print(durSS,2);
  tee.println(" s");
      
  delay(5000);  // Pause for 5 seconds.
  myFile.close();
}
