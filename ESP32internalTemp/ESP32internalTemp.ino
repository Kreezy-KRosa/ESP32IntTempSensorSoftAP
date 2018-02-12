// ESP32 Internal Temp Sensor SoftAP Server Example
// By: Kreezy(KRosa) 2.12.2018

// This code is public domain

// Upload, connect to SSID "ESPSelfTemp" on your wifi enabled device, open a browser and go to 10.10.10.10, enjoy the view ;}

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif

#include <WiFi.h>

IPAddress staticIP(10,10,10,10);
IPAddress gateway(10,10,10,1);      // setting the broadcast IP to 10.10.10.10
IPAddress subnet(255,255,255,0);  

WiFiServer server(80);              // starting a server on port 80

int LED = 2;
uint8_t temprature_sens_read();     // variables for LED and playing with the internal temp data
int intTemp;
int cTemp;                          // current internal temp
int lTemp;                          // last internal temp reading
int tHigh;                          // highest recoreded temp reading booted
int tLow;                           // lowest recorded temp reading since booted

void setup()
{
  Serial.begin(9600);             // Serial for debugging


    intTemp = (int)((temprature_sens_read() - 32 ) / 1.8);  // Read the internal temp sensor
    tHigh = intTemp;                                        // set the high and low to the default boot temp
    tLow = intTemp;

  pinMode(LED,OUTPUT);                                    // led as output 

  digitalWrite(LED,LOW);                                  // led turned off to start fresh

  WiFi.softAP("ESP32SelfTemp");                           // this is the SSID of the SoftAP this board broadcasts
  WiFi.softAPConfig(staticIP, gateway, subnet);           // setting our broadcast ip info
  server.begin();                                         // begin the server
  digitalWrite(LED,HIGH);                                 // led is on when server is up and running
    
}
void loop()
{
    intTemp = (int)((temprature_sens_read() - 32 ) / 1.8);      // read the internal temp data

    lTemp = cTemp;                                              // sets the last reading to ltemp
    cTemp = intTemp;                                            // sets ctemp to the current sensor data
    
    if (cTemp > lTemp) {                                        // check to see if the current data is greater than the last reading
    if (cTemp > tHigh)                                          // check to see if the current is greater than the current high temp
    tHigh = cTemp; }                                           // if so change the new high to the current data value

    if (cTemp < lTemp) {                                      // check to see if the current data is less than the last reading
    if (cTemp < tLow)                                          // check to see if the current is less than the current low temp
    tLow = cTemp; }                                             // if so change the new low to the current data value


          //Uncomment to print to the serial monitor for debugging
    //Serial.println("ESP32 Internal Temp: ");
    //Serial.println(intTemp);
    //Serial.println("Current HIGH: ");
    //Serial.println(tHigh);
    //Serial.println("Current LOW: ");
    //Serial.println(tLow);
    //delay(2000);
    //for(int x = 0; x < 50; x++) Serial.println();
    //delay(2000);  
   
   
  WiFiClient client = server.available();               // if your connected device is available this will happen
    if (!client) { return; }
    while(!client.available()){  delay(1); }
    digitalWrite(LED,LOW);
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<h1 style=""color:#ff0000"">");
    client.println("ESP32 Internal Temp:");
    client.println("</h1>");
    client.println("</head>");
    client.println("<body>");
    client.println("  ");
    client.println(intTemp);                                  // this is the html page being sent to your device 
    client.println("</body>");
    client.println("<head>");
    client.println("<h1 style=""color:#ff0000"">");           // if you wish to change anything on the page displayed
    client.println("Current High:");                          // replace the text here where the headers and values are
    client.println("</h1>");
    client.println("</head>");
    client.println("<body>");
    client.println("  ");
    client.println(tHigh);
    client.println("</body>"); 
    client.println("<head>");
    client.println("<h1 style=""color:#ff0000"">");
    client.println("Current Low");
    client.println("</h1>");
    client.println("</head>");
    client.println("<body>");
    client.println("  ");
    client.println(tLow);
    client.println("</body>");
    client.println("</html>");
    client.println("<meta http-equiv=\"refresh\" content=\"5\">");  // refresh the page every 5 seconds
    delay(2);
    client.stop();                                                  // end of data 
    digitalWrite(LED,HIGH);                                         
}


