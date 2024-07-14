
#include <Servo.h>  // Include the Servo library to control the servo motor.

Servo servo;  // Create a servo object.
int trigPin1 = 2;  // Pin for triggering the first ultrasonic sensor.
int echoPin1 = 3;  // Pin for receiving the echo from the first ultrasonic sensor.
int trigPin2 = 4;  // Pin for triggering the second ultrasonic sensor.
int echoPin2 = 5;  // Pin for receiving the echo from the second ultrasonic sensor.
int servoPin = 10;  // Pin connected to the servo motor.

long duration1, distance1;
long duration2, distance2;
bool doorOpen = false;  // State of the door, true if open.
int currentServoPos = 120;  // Current position of the servo.

void setup() {
    Serial.begin(9600);  // Start serial communication at 9600 baud.
    servo.attach(servoPin);  // Attach the servo motor to its control pin.
    pinMode(trigPin1, OUTPUT);  // Set the first trigPin as an output.
    pinMode(echoPin1, INPUT);  // Set the first echoPin as an input.
    pinMode(trigPin2, OUTPUT);  // Set the second trigPin as an output.
    pinMode(echoPin2, INPUT);  // Set the second echoPin as an input.
    servo.write(currentServoPos);  // Initially close the door (servo at 120 degrees).
    Serial.println("Door is closed.");
}

void measureDistance(long &duration, int trigPin, int echoPin, long &distance) {
    digitalWrite(trigPin, LOW);  // Clear the trigPin.
    delayMicroseconds(2);  // Short delay before sending a pulse.
    digitalWrite(trigPin, HIGH);  // Send a high pulse to trigPin to start the measurement.
    delayMicroseconds(10);  // Duration of the high pulse.
    digitalWrite(trigPin, LOW);  // End the pulse by setting trigPin low.
    
    duration = pulseIn(echoPin, HIGH);  // Measure the length of the incoming pulse.
    distance = duration * 0.034 / 2;  // Calculate the distance in centimeters.
}

void moveServo(int targetPos, int stepDelay) {
    if (currentServoPos < targetPos) {
        for (int pos = currentServoPos; pos <= targetPos; pos++) {
            servo.write(pos);
            delay(stepDelay);
        }
    } else {
        for (int pos = currentServoPos; pos >= targetPos; pos--) {
            servo.write(pos);
            delay(stepDelay);
        }
    }
    currentServoPos = targetPos;
}

void loop() {
    measureDistance(duration1, trigPin1, echoPin1, distance1);  // Measure the distance using the first ultrasonic sensor.
    measureDistance(duration2, trigPin2, echoPin2, distance2);  // Measure the distance using the second ultrasonic sensor.
    
    // Check distances from both sensors
    if ((distance1 < 10 || distance2 < 10) && !doorOpen) {  // If an object is detected within 10 cm by any sensor and the door is not already open.
        moveServo(0, 15);  // Open the door slowly with a delay of 15 ms between each step.
        doorOpen = true;  // Update the door's state to open.
        Serial.println("Door is opening.");
        delay(5000);  // Allow time for the door to stay open.
    } else if ((distance1 >= 10 && distance2 >= 10) && doorOpen) {  // If no object is detected within 10 cm by both sensors and the door is open.
        moveServo(120, 15);  // Close the door slowly with a delay of 15 ms between each step.
        doorOpen = false;  // Update the door's state to closed.
        Serial.println("Door is closing.");
        delay(3000);  // Allow time for the door to stay closed.
    }
    delay(1000);  // Delay between measurements to avoid rapid triggering.
}
