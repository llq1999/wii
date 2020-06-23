/*
 * Weitere Infos: www.makerblog.at
 * 
 * Library zum Download: https://github.com/timtro/wiinunchuck-h
 * Achtung: Nach dem Download des ZIPs und vor dem Import der Library in die Arduino IDE
 * das "-master" aus dem Dateinamen löschen.
 */
 
#include <Wire.h>
#include <Servo.h> 
#include <wiinunchuck.h>
 
Servo panServo; 
Servo tiltServo;
 
int pinPan = 10; // Links-Rechts
int pinTilt = 11; // Oben-Unten
 
float panPos = 90.0; // Aktuelle Position
float tiltPos = 90.0; // Aktuelle Position
int dxPan = 0; // Positionsänderung des Joysticks X
int dyTilt = 0; // Positionsänderung des Joysticks Y
 
float slowDown = 0.05; // Faktor zur Steuerung der Geschwindigkeit
 
 
void setup(){
 
 Serial.begin(57600);
 
 nunchuk_init();
 delay(100);
 nunchuk_calibrate_joy();
 delay(100);
 nunchuk_get_data();
 delay(100); 
 
 panServo.attach(pinPan);
 tiltServo.attach(pinTilt); 
 delay(10);
 panServo.write(panPos);
 tiltServo.write(tiltPos);
 
 panPos = 90.0;
 tiltPos = 60.0; 
}
 
void loop(){
 
 nunchuk_get_data(); // Daten vom Nunchuk empfangen
 delay(10);
 
 // Wenn C-Taste gedrückt, dann Position resetten
 if (nunchuk_cbutton() == 1) {
 dxPan = 0;
 dyTilt = 0;
 panPos = 90;
 tiltPos = 60;
 panServo.write(panPos);
 tiltServo.write(tiltPos);
 }
 
 // Wenn Z-Taste gedrückt dann Joystick auslesen
 if (nunchuk_zbutton() == 1) {
 // Wert an der X-Achse vom Nunchuk ist 0-255, ergibt
 // ein dxPan und dyTilt von -127 bis +128
 dxPan = -(128 - nunchuk_cjoy_x()); 
 dyTilt = (128 - nunchuk_cjoy_y());
 
 // Joystick-Bewegung rund um die Mitte ignorieren
 if (abs(dxPan) > 20) {
 // Wenn Joystick-Auschlag größer als 20, dann neue Position berechnen
 // Positionsänderung pro Durchlauf mit slowDown skalieren, sonst zu stark
 panPos = panPos + (float)dxPan*slowDown;
 }
 // Gleicher Ablauf wie dxPan
 if (abs(dyTilt) > 20) {
 tiltPos = tiltPos + (float)dyTilt*slowDown;
 }
 
 // Positionswerte für die Servos auf 0-180 begrenzen
 panPos = min(panPos, 180);
 panPos = max(panPos, 0);
 tiltPos = min(tiltPos, 100);
 tiltPos = max(tiltPos, 0);
 }
 
 // Ausgabe am seriellen Monitor, Achtung 57600 Baud!
 Serial.print("dX: ");
 Serial.print(dxPan);
 Serial.print(" dY: ");
 Serial.print(dyTilt);
 Serial.print(" X: ");
 Serial.print(panPos);
 Serial.print(" Y: ");
 Serial.println(tiltPos);
 
 // Neue Positionen an die Servos schreiben
 panServo.write((int)panPos);
 tiltServo.write((int)tiltPos);
 
 delay(10); 
}

