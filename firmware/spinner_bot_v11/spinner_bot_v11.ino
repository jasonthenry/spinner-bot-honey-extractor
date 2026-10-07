#include <Servo.h>

const int trigPin = 2;
const int echoPin = 3;
const int spinMotorPin = 13;
const int servoPin = 12;

const int HONEY_LEVEL_HIGH_THRESHOLD = 5;
const int HONEY_LEVEL_LOW_THRESHOLD = 10;

const unsigned long SPIN_CYCLE_DURATION = 180000;  // 3 minutes
bool isSpinning = false;
unsigned long spinCycleStartTime = 0;
unsigned long lastPrintTime = 0;

Servo doorServo;

// Servo positions
const int DOOR_OPEN_POSITION = 90;  
const int DOOR_CLOSE_POSITION = 0;

void setup() {
    Serial.begin(9600);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(spinMotorPin, OUTPUT); // Spinner motor

    doorServo.attach(servoPin); // Servo for chamber door
    doorServo.write(DOOR_CLOSE_POSITION);  // Start with the door closed
}

long measureDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 25000);  // 25ms timeout
    long distance = (duration / 2) / 29.1;  // Convert to distance in cm

    return distance;
}

void openDoor() {
    doorServo.write(DOOR_OPEN_POSITION);
    delay(500);
}

void closeDoor() {
    doorServo.write(DOOR_CLOSE_POSITION);
    delay(500);
}

void startSpinCycle() {
    if (!isSpinning) {
        digitalWrite(spinMotorPin, HIGH);  // Start the spinner motor
        isSpinning = true;
        spinCycleStartTime = millis(); 
        Serial.println("Spinning Started");
    } else {
        Serial.println("Already Spinning");
    }
}

void stopSpinCycle() {
    if (isSpinning) {  // Only stop if it's currently spinning
        digitalWrite(spinMotorPin, LOW); // Stop the spinner motor
        isSpinning = false;
        Serial.println("Spinning Stopped");
    } else {
        Serial.println("Not Spinning");
    }
}

void loop() {
    // Check if spin cycle needs to be stopped after 3 minutes
    if (isSpinning && (millis() - spinCycleStartTime >= SPIN_CYCLE_DURATION)) {
        stopSpinCycle();
    }

    // Check for Serial commands
    if (Serial.available()) {
        char command = Serial.read();
        switch (command) {
            case 'c':
                Serial.println("DockingComplete");
                break;
            case 'd':
                Serial.println("DockingReady");
                break;
            case 'f':
                Serial.println("Full");
                break;
            case 'h':
                Serial.println("HoneyLevelHigh");
                break;
            case 'l':
                Serial.println("HoneyLevelLow");
                break;
            case 'n':
                Serial.println("SpinCycleOn");
                startSpinCycle();
                break;
            case 'x':
                Serial.println("SpinCycleOff");
                stopSpinCycle();
                break;
            case 'p':
                Serial.println("ValueOpen");
                break;
            case 'v':
                Serial.println("ValveClosed");
                break;
            case 'r':
                Serial.println("Hive,ID,Ready"); 
                break;
            case 'z':
                Serial.println("ChamberOpen"); 
                openDoor();
                break;
            case 'q':
                Serial.println("ChamberClose");
                closeDoor();
                break;
            default:
                Serial.println("Unknown command");
                break;
        }
    }

    if (isSpinning && (millis() - lastPrintTime >= 5000)) { // print every 5 seconds
        Serial.print("Spin Cycle Time: ");
        Serial.println(millis() - spinCycleStartTime);
        lastPrintTime = millis();
    }
  
    delay(200);

    // Check honey level
    long distance = measureDistance();
    Serial.print("Measured Distance: ");
    Serial.println(distance);

    if (distance <= HONEY_LEVEL_HIGH_THRESHOLD) {  
        Serial.println("HoneyLevelHigh");
    } else if (distance >= HONEY_LEVEL_LOW_THRESHOLD) {  
        Serial.println("HoneyLevelLow");
    }
}
