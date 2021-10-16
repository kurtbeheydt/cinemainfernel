//#define DEBUG 1

#include <AccelStepper.h>
#define motorInterfaceType 1

const byte pinDir = 2;
const byte pinStep = 3;
const byte pinBreak = 4;

const int speedMax = 1000;
int motorSpeed = 100;
int motorDirection = 1;

AccelStepper stepper = AccelStepper(motorInterfaceType, pinStep, pinDir);

const byte pinPotMeter = A5;

void readPotMeter() {
    int sensorValue = analogRead(pinPotMeter);

    motorSpeed = map(sensorValue, 0, 916, 0, speedMax) * motorDirection;

#ifdef DEBUG
    Serial.print("Sensor: ");
    Serial.println(sensorValue);
    Serial.print("motorspeed: ");
    Serial.println(motorSpeed);
#endif

    stepper.setSpeed(motorSpeed);
}

void runMotor() {
    if (motorSpeed != 0) {
        digitalWrite(pinBreak, LOW);
        stepper.runSpeed();
#ifdef DEBUG
        Serial.print("Running");
#endif
    } else {
        digitalWrite(pinBreak, HIGH);
#ifdef DEBUG
        Serial.print("Break");
#endif
    }
}

void setup() {
    stepper.setMaxSpeed(speedMax);

    pinMode(pinBreak, OUTPUT);
    digitalWrite(pinBreak, HIGH);

    pinMode(pinPotMeter, INPUT);

#ifdef DEBUG
    Serial.begin(115200);
    Serial.println("start");
#endif
}

void loop() {
    readPotMeter();
    runMotor();
#ifdef DEBUG
    delay(100);
#endif
}
