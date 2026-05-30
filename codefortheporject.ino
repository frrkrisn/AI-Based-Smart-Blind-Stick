#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// GPS
SoftwareSerial gpsSerial(4, 3);
TinyGPSPlus gps;

// IR Sensors
const int ir1 = 2;
const int ir2 = 5;

// Outputs
const int buzzer = 7;
const int motor = 8;

// Front Ultrasonic Sensor
const int trig1 = 9;
const int echo1 = 10;

// Bottom Ultrasonic Sensor
const int trig2 = 11;
const int echo2 = 12;

// Water Sensor
const int waterPin = A0;

void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(9600);

  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(motor, OUTPUT);

  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);

  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);

  digitalWrite(buzzer, LOW);
  digitalWrite(motor, LOW);

  Serial.println("SMART BLIND STICK SYSTEM STARTED");
}

void loop()
{
  bool alert = false;

  // ==========================
  // FRONT ULTRASONIC SENSOR
  // ==========================

  digitalWrite(trig1, LOW);
  delayMicroseconds(2);

  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig1, LOW);

  long duration1 = pulseIn(echo1, HIGH, 10000);

  int distance1 = duration1 * 0.034 / 2;

  Serial.print("Front Distance: ");
  Serial.print(distance1);
  Serial.println(" cm");

  if(distance1 > 0 && distance1 <= 100)
  {
    Serial.println("OBSTACLE DETECTED");
    alert = true;
  }

  // ==========================
  // BOTTOM ULTRASONIC SENSOR
  // ==========================

  digitalWrite(trig2, LOW);
  delayMicroseconds(2);

  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig2, LOW);

  long duration2 = pulseIn(echo2, HIGH, 10000);

  int distance2 = duration2 * 0.034 / 2;

  Serial.print("Bottom Distance: ");
  Serial.print(distance2);
  Serial.println(" cm");

  if(distance2 > 25 && distance2 < 150)
  {
    Serial.println("PIT / STAIR DETECTED");
  }

  // ==========================
  // IR SENSOR 1
  // ==========================

  if(digitalRead(ir1) == LOW)
  {
    Serial.println("IR SENSOR 1 DETECTED");
  }

  // ==========================
  // IR SENSOR 2
  // ==========================

  if(digitalRead(ir2) == LOW)
  {
    Serial.println("IR SENSOR 2 DETECTED");
  }

  // ==========================
  // WATER SENSOR
  // ==========================

  int waterValue = analogRead(waterPin);

  Serial.print("Water Value: ");
  Serial.println(waterValue);

  if(waterValue > 300)
  {
    Serial.println("WATER DETECTED");
    alert = true;
  }

  // ==========================
  // ALERT SYSTEM
  // ==========================

  if(alert)
  {
    digitalWrite(buzzer, HIGH);
    digitalWrite(motor, HIGH);

    delay(400);

    digitalWrite(buzzer, LOW);
    digitalWrite(motor, LOW);
  }
  else
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(motor, LOW);
  }

  // ==========================
  // GPS MODULE
  // ==========================

  while(gpsSerial.available())
  {
    gps.encode(gpsSerial.read());
  }

  if(gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);

    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);

    Serial.println("-------------------------");
  }
  else
  {
    Serial.println("Waiting for GPS Signal...");
  }

  delay(200);
}