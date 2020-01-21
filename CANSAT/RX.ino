#include <VirtualWire.h>

const int transmit_pin = 12;
const int receive_pin = 11;

void setup()
{
  
    delay(1000);
    Serial.begin(9600);
    Serial.println("setup");
    vw_set_tx_pin(transmit_pin);
    vw_set_rx_pin(receive_pin);
    vw_setup(4000);  
    vw_rx_start();
            
}

void loop()
{
      
    char buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    
    if (vw_get_message(buf, &buflen)) 
    {      
      int i;
      Serial.print("Telemetry data :");
      { 
       for (i = 0; i < buflen; i++)
       {
        Serial.print(buf[i]);
       }
       Serial.print(' ');
      }
      Serial.println();
     }
     
}
