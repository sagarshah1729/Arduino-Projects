#include <VirtualWire.h>


const int tx = 10;
int x = A0;
int y = A1;
int z = A2;
char msg[3];


void setup()
{

  pinMode(x,INPUT);
  pinMode(y,INPUT);
  pinMode(z,INPUT); 
  Serial.begin(9600); 
 
  vw_setup(2000);
  vw_set_tx_pin(tx);
    
}
    
void loop()
{
  
   int a = analogRead(x); 
   Serial.println(a);
   itoa(a,msg,10);
   vw_send((uint8_t *)msg, strlen(msg));
   vw_wait_tx();
      
   int b = analogRead(y); 
   Serial.println(b);
   itoa(b,msg,10);
   vw_send((uint8_t *)msg, strlen(msg));
   vw_wait_tx();
      
   int c = analogRead(z); 
   Serial.println(c);
   itoa(c,msg,10);
   vw_send((uint8_t *)msg, strlen(msg));
   vw_wait_tx();   delay(2000);
   
}
