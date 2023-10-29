#include <NeoSWSerial.h>

#include <NMEAGPS.h>

#define rx0 7 // D0 orange rx serial pin
#define tx0 6 // D1 yellow tx serial pin
#define in1 2 // D2 brown
#define in2 3 // D3 yellow
#define in3 4 // D4 white
#define in4 5 // D5 grey
#define ENA 9 // D9 green right
#define ENB 10 // D10 dark left relative to powerbank
#define des_LAT -31.993795
#define des_LNG 115.795700

NeoSWSerial gpsPort(rx0, tx0);
NMEAGPS gps;

NeoGPS::Location_t des(des_LAT, des_LNG);


void setup() {
  // put your setup code here, to run once:
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  

  Serial.begin(115200);
  gpsPort.begin(9600);
  Serial.println("setup");
 
  
}
double timer = millis();

void loop() {
  while (gps.available(gpsPort) > 0) {
    gps_fix fix = gps.read();
    if (fix.valid.location) {
      double distanceToDestination = fix.location.DistanceKm(des);
      double courseToDestination = fix.location.BearingToDegrees(des);
      int courseChangeNeeded = (int)(360 + courseToDestination - fix.heading()) % 360;

      Serial.print( F("Lat: ") );
      Serial.print( fix.latitude(), 6 );
      Serial.print( F("  Lon: ") );
      Serial.println( fix.longitude(), 6 );


      // Within 20 meters of destination?  We're here!
      if (distanceToDestination <= 0.0075)
      {
        Serial.println( F("CONGRATULATIONS: You've arrived!") );
        stop();
        exit(1);
      }

      Serial.print( F("DISTANCE: ") );
      Serial.print(distanceToDestination);
      Serial.println( F(" meters to go.") );
      Serial.print( F("INSTRUCTION: ") );

      if ((courseChangeNeeded >= 345) || (courseChangeNeeded < 15)) {
      Serial.println( F("Keep on straight ahead!") );
      forwards();
      
      }
      
      else if ((courseChangeNeeded >= 315) && (courseChangeNeeded < 345)) {
        Serial.println( F("Veer slightly to the left.") );
        forwardsleft(135);
      }
      else if ((courseChangeNeeded >= 15) && (courseChangeNeeded < 45)) {
        Serial.println( F("Veer slightly to the right.") );
        forwardsright(135);
      }
      else if ((courseChangeNeeded >= 255) && (courseChangeNeeded < 315)) {
        Serial.println( F("Turn to the left.") );
        forwardsleft(175);
        
      }
      else if ((courseChangeNeeded >= 45) && (courseChangeNeeded < 105)) {
        Serial.println( F("Turn to the right.") );
        forwardsright(175);
      }
      else {
        Serial.println( F("Turn completely around.") );
        forwards();
        } 
     
    }
    else {
      Serial.println( F("Waiting for GPS fix...") );
    }
  }  
}

  
void rotate_on_spot() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}
void forwards() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ENA, 225);
  analogWrite(ENB, 225);
}
void backwards() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void stop() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void forwardsleft(int howfar) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ENA, howfar);
  analogWrite(ENB, 100);
}
void forwardsright(int howfar) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ENA, 100);
  analogWrite(ENB, howfar);
}