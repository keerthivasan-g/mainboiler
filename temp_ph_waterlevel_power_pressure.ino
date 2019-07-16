#include "ACS712.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <SFE_BMP180.h>
ACS712 sensor(ACS712_30A, A14);
SFE_BMP180 pressure;
#define ALTITUDE 1655
SoftwareSerial sw(0, 1);
void setup() 
{
  
  Serial.begin(115200);
  Serial.println("Interfacing arduino with nodemcu");
  pinMode(9, OUTPUT);
  pinMode(10, INPUT); 
  sw.begin(115200);
  sensor.calibrate();
  Serial.println("REBOOT");
  if (pressure.begin())
  Serial.println("done hey !!!");
  else
  {
    Serial.println("die hey !!!");
    while(1);
    }
}

void loop() 
{
    Serial.println("Sending data to nodemcu");
    digitalWrite(9, LOW);
    delayMicroseconds(2);
    digitalWrite(9, HIGH);
    delayMicroseconds(10);
    digitalWrite(9, LOW);
    long dura;
    dura = pulseIn(10, HIGH);
    int dist;
    dist = dura*0.034/2;
    Serial.print("Distance: ");
    Serial.println(dist);
    sw.print("Distance: ");
    sw.println(dist);
    //delay(100);
    //float temp;
    //temp = analogRead(A10);
    //temp = temp * 0.488828125;
    //Serial.print("The Temperature: ");
    //Serial.print(temp);
    //Serial.println(" *c");
    //delay(100);
    float ph;
    ph = analogRead(A8);
    ph = ph/179.2982456140351;
    Serial.print("The pH: ");
    Serial.println(ph);
    sw.print("The pH: ");
    sw.println(ph);
    //delay(100);
    //float v = 230;
    //float i = sensor.getCurrentAC(A9);
    //float p=v*i;
    //Serial.print("i= ");
    //Serial.println(i);
    //Serial.println(String("power = ")+ p + "watts");
    //delay(100);
    char status;
    double T,P,p0,a;
    //Serial.print("provided altitude: ");
    //Serial.print(ALTITUDE,0);
    //Serial.print(" meters, ");
    //Serial.print(ALTITUDE*3.28084,0);
    //Serial.println(" feet");
    status = pressure.startTemperature();
    if (status != 0)
    {
      delay(status);
      status = pressure.getTemperature(T);
      if (status != 0)
    {
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");
      sw.print("temperature: ");
      sw.print(T,2);
      sw.print(" deg C, ");
      sw.print((9.0/5.0)*T+32.0,2);
      sw.println(" deg F");
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");
          sw.print("absolute pressure: ");
          sw.print(P,2);
          sw.print(" mb, ");
          sw.print(P*0.0295333727,2);
          sw.println(" inHg");
          p0 = pressure.sealevel(P,ALTITUDE);
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");
          sw.print("relative (sea-level) pressure: ");
          sw.print(p0,2);
          sw.print(" mb, ");
          sw.print(p0*0.0295333727,2);
          sw.println(" inHg");
          //a = pressure.altitude(P,p0);
          //Serial.print("computed altitude: ");
          //Serial.print(a,0);
          //Serial.print(" meters, ");
          //Serial.print(a*3.28084,0);
          //Serial.println(" feet");
        }
         else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

    Serial.println();
    sw.println();
    //delay(5000);
    }

