/*
   Motor forward:
   GPIO_8 = low, GPIO_9 = hi
   Motor reverse:
   GPIO_8 = hi, GPIO_9 = low

   Alternating ticks sleep
                             */


// Declare vars
float stepCount = 0;
float stepTick = 0;
float stepTime = 0;
float totalSteps = 0;
float revolutions = 0;
float shortTick = 0;
float longTick = 0;
float rampingTime = 0;
int buttonState = 0;


// Establish connection to device GPIO
void setup() {

  // declare GPIO pins
  Serial.begin(9600); // 9.6 kbit/s
  while ( !Serial );
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(7, INPUT_PULLUP);

  // Initialize GPIO
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);

  // Constants
  rampingTime = 0.1; // Percent of oscillation
  revolutions = 1;   // 1 rev = 3.7665 in.
  longTick = 300;    // microseconds
  shortTick = 100;
  totalSteps = 6400 * revolutions;
}


// Motor run loop
void loop() {

  stepTick = longTick;

  // Push-button activation
  buttonState = digitalRead(7);
  if ( buttonState == LOW ) {

    stepCount = 0;
    Serial.println(buttonState);
    stepping();
  }
}


// Step action
void stepping() {

  while ( stepCount < totalSteps ) {

    // Ramping
    stepTime = stepCount/totalSteps;

    // Accelerate from 300 to 100 microsecond delay
    if ( stepTime < rampingTime ) { 
      stepTick = longTick - stepTime*(longTick - shortTick)/rampingTime; 
    }

    // Decelerate from 100 to 300 microsecond delay
    if ( stepTime > (1 - rampingTime) ) {
        stepTick = (stepTime - (1 - rampingTime))*(longTick - shortTick)/rampingTime + shortTick;
    }

    // Take Step
    digitalWrite(9, HIGH);
    delayMicroseconds(stepTick);
    digitalWrite(9, LOW);
    delayMicroseconds(stepTick);

    stepCount++;
  }
}
