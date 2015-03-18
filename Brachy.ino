float stepCount = 0;
float stepSpeed = 0;
float speedFraction = 0;
float totalSteps = 0;
float revolutions = 0;


// Establish connection to device GPIO
void setup() {

  Serial.begin(9600); // 9.6 kbit/s
  while (!Serial);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(7,INPUT_PULLUP);

  // Forward motion (HIGH and LOW are 5V and 0V)
  digitalWrite(8,HIGH);
  digitalWrite(9,LOW);

  // Motor init parameters
  stepSpeed = 300;
  revolutions = 1; // 1 rev = 3.7665 in.
  totalSteps = 6400 * revolutions;
}


// Motor run loop
void loop() {

  // Push-button activation
  int buttonState = digitalRead(7);
  if (buttonState == 0) {
    stepCount = 0;
    Serial.println(buttonState);
    stepping();
  }
}


// Step action
void stepping() {

  while (stepCount < totalSteps) {

    if (totalSteps < 1600) {
      stepSpeed = 300;
      digitalWrite(9,HIGH);
      delayMicroseconds(stepSpeed);
      digitalWrite(9,LOW);
      delayMicroseconds(stepSpeed);
      stepCount++;
    }

    // Ramping
    else {
      speedFraction = stepCount/totalSteps;

      // Accelerate from 300 to 100 microsecond delay
      if (speedFraction < .1 ) { 
        stepSpeed = (300 - (speedFraction*2000)); 
        digitalWrite(9,HIGH);
        delayMicroseconds(stepSpeed);
        digitalWrite(9,LOW);
        delayMicroseconds(stepSpeed);
        stepCount++;
      }
      // Decelerate from 100 to 300 microsecond delay
      if (speedFraction > .9 ) {
          stepSpeed = (((speedFraction - 0.9) * (300 - 100)) / (1 - 0.9)) + 100;
          digitalWrite(9,HIGH);
          delayMicroseconds(stepSpeed);
          digitalWrite(9,LOW);
          delayMicroseconds(stepSpeed);
          stepCount++;
      }

      // Normal action: AC power with 300 microsecond period
      else {
        digitalWrite(9,HIGH);
        delayMicroseconds(stepSpeed);
        digitalWrite(9,LOW);
        delayMicroseconds(stepSpeed);
        stepCount++;
      }  
    }
  }
}
