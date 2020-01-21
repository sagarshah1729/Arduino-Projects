# Flight Computer for High Powered Rocket
Payload (Aerodynamics Data Logger) code for STAR-XL (High Powered Rocket designed by Star Labs, Surat)

## Sensors used - 
1. BMP180 (Used for determining Temperature, Pressure and Altitude of the Rocket during it's trajectory) 
2. SD Card Module - Used for writing data inside a sdcard with the real-time datas 

- For Pin configurations, refer Internet 

## Libraries used - 
1. Neotee Library (Used for combining serial.print() and myFile.print() outputs with tee.print())
2. SFE_BMP180 for integration of BMP180 sensor
3. Wire prebuilt arduino library
4. SD prebuilt arduino library
5. SDI prebuilt arduino library

NOTE: For the output of time, millis() function was used

## Install with Git

If you clone this repo, also update the submodules:

    $ git submodule update --init

To pull submodule updates later on:

    $ git submodule foreach git pull

## Adding a new module to the Repo

    $ git submodule add https://github.com/repo_path/name libraries/name

For information on installing libraries, see: http://www.arduino.cc/en/Guide/Libraries

