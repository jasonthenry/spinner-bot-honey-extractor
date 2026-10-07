// Define pins for the motors
const int leftFrontForward = 10;
const int leftFrontBackward = 11;
const int leftBackForward = 9;
const int leftBackBackward = 8;
const int rightFrontForward = 6;
const int rightFrontBackward = 5;
const int rightBackForward = 3;
const int rightBackBackward = 2;

// Define pins for the ultrasonic sensors
const int leftTrigPin = 12;
const int leftEchoPin = 13;
const int frontTrigPin = A1;
const int frontEchoPin = A2;
const int rightTrigPin = 7;
const int rightEchoPin = 4;

const int distanceThreshold = 15; // in inches

bool robotActive = true;

void setup() {
  Serial.begin(9600);

  // Set motor pins as outputs
  pinMode(leftFrontForward, OUTPUT);
  pinMode(leftFrontBackward, OUTPUT);
  pinMode(leftBackForward, OUTPUT);
  pinMode(leftBackBackward, OUTPUT);
  pinMode(rightFrontForward, OUTPUT);
  pinMode(rightFrontBackward, OUTPUT);
  pinMode(rightBackForward, OUTPUT);
  pinMode(rightBackBackward, OUTPUT);

  // Set ultrasonic sensor pins
  pinMode(leftTrigPin, OUTPUT);
  pinMode(leftEchoPin, INPUT);
  pinMode(frontTrigPin, OUTPUT);
  pinMode(frontEchoPin, INPUT);
  pinMode(rightTrigPin, OUTPUT);
  pinMode(rightEchoPin, INPUT);
}

long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration/2) / 29.1;  // Convert to distance in cm
  distance = distance * 0.393701; // Convert cm to inches
  
  return distance;
}

void forward() {
  digitalWrite(leftFrontForward, HIGH);
  digitalWrite(leftFrontBackward, LOW);
  digitalWrite(leftBackForward, HIGH);
  digitalWrite(leftBackBackward, LOW);
  digitalWrite(rightFrontForward, HIGH);
  digitalWrite(rightFrontBackward, LOW);
  digitalWrite(rightBackForward, HIGH);
  digitalWrite(rightBackBackward, LOW);
}

void reverse() {
  digitalWrite(leftFrontForward, LOW);
  digitalWrite(leftFrontBackward, HIGH);
  digitalWrite(leftBackForward, LOW);
  digitalWrite(leftBackBackward, HIGH);
  digitalWrite(rightFrontForward, LOW);
  digitalWrite(rightFrontBackward, HIGH);
  digitalWrite(rightBackForward, LOW);
  digitalWrite(rightBackBackward, HIGH);
}

void turnLeft() {
  digitalWrite(leftFrontForward, LOW);
  digitalWrite(leftFrontBackward, HIGH);
  digitalWrite(leftBackForward, LOW);
  digitalWrite(leftBackBackward, HIGH);
  digitalWrite(rightFrontForward, HIGH);
  digitalWrite(rightFrontBackward, LOW);
  digitalWrite(rightBackForward, HIGH);
  digitalWrite(rightBackBackward, LOW);
}

void turnRight() {
  digitalWrite(leftFrontForward, HIGH);
  digitalWrite(leftFrontBackward, LOW);
  digitalWrite(leftBackForward, HIGH);
  digitalWrite(leftBackBackward, LOW);
  digitalWrite(rightFrontForward, LOW);
  digitalWrite(rightFrontBackward, HIGH);
  digitalWrite(rightBackForward, LOW);
  digitalWrite(rightBackBackward, HIGH);
}

void stopRobot() {
  digitalWrite(leftFrontForward, LOW);
  digitalWrite(leftFrontBackward, LOW);
  digitalWrite(leftBackForward, LOW);
  digitalWrite(leftBackBackward, LOW);
  digitalWrite(rightFrontForward, LOW);
  digitalWrite(rightFrontBackward, LOW);
  digitalWrite(rightBackForward, LOW);
  digitalWrite(rightBackBackward, LOW);
}

void loop() {
  long leftDistance = measureDistance(leftTrigPin, leftEchoPin);
  long frontDistance = measureDistance(frontTrigPin, frontEchoPin);
  long rightDistance = measureDistance(rightTrigPin, rightEchoPin);

  if (robotActive) {
    if (frontDistance > distanceThreshold) {
      forward();
    } else {
      stopRobot();
      if (leftDistance > rightDistance) {
        reverse();
        delay(500);
        turnLeft();
        delay(500);
      } else {
        reverse();
        delay(500);
        turnRight();
        delay(500);
      }
    }
  } else {
    stopRobot();
  }

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command == "r") {
      Serial.println("Hive,ID,Ready");
      robotActive = false;
    } else if (command == "k") {
      Serial.println("Honey_Extraction_Complete");
      robotActive = true;
    } else if (command == "d") {
      Serial.println("DockingReady");
    }
  }
  
  delay(200);
}
