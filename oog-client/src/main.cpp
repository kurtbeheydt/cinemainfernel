//#define DEBUG 1

#include <AccelStepper.h>
#define motorInterfaceType 1

const byte pinSwitchLeft = A3;
const byte pinSwitchRight = A2;

const byte pinDir = 2;
const byte pinStep = 3;
const byte pinBreak = 4;

const int speedMax = 1000;
int motorSpeed = 100;
int motorDirection = 1;
bool isMotorActive = false;

AccelStepper stepper = AccelStepper(motorInterfaceType, pinStep, pinDir);

const byte pinPotMeter = A4;

void readSwitches() {
    uint8_t switchLeftValue = !digitalRead(pinSwitchLeft);
    uint8_t switchRightValue = !digitalRead(pinSwitchRight);

    if (switchLeftValue && switchRightValue) {
#ifdef DEBUG
    Serial.println("both switches blocked");
#endif
        isMotorActive = false;
    }

    if (switchLeftValue && !switchRightValue) {
#ifdef DEBUG
    Serial.println("left switche blocked");
#endif
        isMotorActive = true;
        motorDirection = 1;
    }

    if  (!switchLeftValue && switchRightValue) {
#ifdef DEBUG
    Serial.println("right switch blocked");
#endif
        isMotorActive = true;
        motorDirection = -1;
    }
}

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
    if (isMotorActive && motorSpeed != 0) {
        digitalWrite(pinBreak, LOW);
        stepper.runSpeed();
#ifdef DEBUG
        Serial.println("Running");
#endif
    } else {
        digitalWrite(pinBreak, HIGH);
#ifdef DEBUG
        Serial.println("Break");
#endif
    }
}

void setup() {
    stepper.setMaxSpeed(speedMax);

    pinMode(pinBreak, OUTPUT);
    digitalWrite(pinBreak, HIGH);

    pinMode(pinPotMeter, INPUT);
    pinMode(pinSwitchLeft, INPUT_PULLUP);
    pinMode(pinSwitchRight, INPUT_PULLUP);

#ifdef DEBUG
    Serial.begin(115200);
    Serial.println("start");
#endif
}

void loop() {
    readSwitches();
    readPotMeter();
    runMotor();

#ifdef DEBUG
    delay(100);
#endif
}
