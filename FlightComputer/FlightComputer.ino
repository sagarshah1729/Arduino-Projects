#include <NeoTee.h>
#include <NeoTeeStream.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

SFE_BMP180 pressure;

char status;
double baseline; // baseline pressure
double T,P,a,x;
double groundAltitude; // Stores starting altitude, before launch
double lastAltitude;
double apogee; // Stores max altitude for total height

File myFile;
Print *outputs[] = { &Serial, &myFile };  // <--  list all the output destinations here
NeoTee tee( outputs, sizeof(outputs)/sizeof(outputs[0]) );

long currentMillis;
long startMillis;

void setup() {
  
  Serial.begin(9600);
  startMillis = millis();  //initial start time

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  Serial.println("SD card initialization done.");

  myFile = SD.open("test.txt", FILE_WRITE);
  
  pressure.begin();
  baseline = getPressure(); // Get the baseline pressure
  groundAltitude = pressure.altitude(P,baseline);
  tee.print("Ground Altitude: ");
  tee.print(groundAltitude);
  tee.println(" m");

  pinMode(2, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
  
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

void loop() {
  myFile = SD.open("test.txt", FILE_WRITE);
  x= digitalRead(12);
 
  P = getPressure();
  a = pressure.altitude(P,baseline);
  
  tee.print("Altitude: ");
  tee.print(a,2);
  tee.print(" m, ");  

  if(a - lastAltitude <= -1) // Checks for a 1 meter drop in altitude
    {
     delay(150); // Gravity is 0.102ms per meter, so 150ms is enough to compensate for drag.
     a = pressure.altitude(P,baseline); // Gets a new altitude reading
     if(a - lastAltitude <= -2) // Checks if new altitude reading is 1 meter less than last measured
       {
         delay(150); // Gravity is 0.102ms per meter, so 150ms is enough to compensate for drag.
         a = pressure.altitude(P,baseline); // Gets a new altitude reading
         if(a - lastAltitude <= -3) // Checks if new altitude reading is 2 meters less than last measured
          {
             apogee = lastAltitude;
             apogee = apogee - 3;
             if (x == HIGH) {
             // read the incoming byte:
             digitalWrite(2, HIGH);
             } else if (x == LOW)
             {
             digitalWrite(2, LOW);
             }
          }
          else lastAltitude = a; // False reading. Sets current altitude to the last measured altitude
       }
       else lastAltitude = a; // False reading. Sets current altitude to the last measured altitude     
    }
    else lastAltitude = a; // False reading. Sets current altitude to the last measured altitude

  currentMillis = millis();
  double durSS = (currentMillis/1000);    //Seconds 

  tee.print("Time: ");
  tee.print(durSS,2);
  tee.println(" s");
      
  delay(5000);  // Pause for 5 seconds.
  myFile.close();
}

double getPressure()
{
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
    
    status = pressure.getTemperature(T);
    if ( status != 0)
    {
      // Print out the measurement:
      tee.print("Temperature: ");
      tee.print(T,2);
      tee.print(" C, ");
      
      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);
        
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          tee.print("Pressure: ");
          tee.print(P*0.0295333727,2);
          tee.println(" Hg, ");
          return(P);
        }
        else tee.println("Error retrieving pressure measurement\n");
      }
      else tee.println("Error starting pressure measurement\n");
    }
    else tee.println("Error retrieving temperature measurement\n");
  }
  else tee.println("Error starting temperature measurement\n");
}

