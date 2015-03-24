/*
   Motor forward:
   GPIO_8 = hi, GPIO_9 = hi/low steps

   Motor reverse:
   GPIO_8 = low, GPIO_9 = hi/low steps

   Alternating ticks sleep
                                       */


// Declare vars
float stepCount = 0;
float stepTick = 0;
float stepTime = 0;
float totalSteps = 0;
float insertionLength = 0;
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

  // Initialize GPIO forward
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);

  // Constants
  rampingTime = 0.1;                     // Percent of oscillation
  insertionLength = 10;                  // inches
  revolutions = insertionLength/3.7665;  // 1 rev = 3.7665 in.
  longTick = 300;                        // microseconds
  shortTick = 100;                       //
  totalSteps = 6400*revolutions;         //
}


// Motor run loop
void loop() {

  // Push-button insertion
  buttonState = digitalRead(7);
  if ( buttonState == LOW ) {

    digitalWrite(8, HIGH);
    steppingAction();
  }

  // Push-button withdrawl
  buttonState = digitalRead(7);
  if ( buttonState == LOW ) {

    digitalWrite(8, LOW);
    steppingAction();
  }
}


// Step action
void steppingAction() {

  stepCount = 0;
  while ( stepCount < totalSteps ) {

    // Default step
    stepTick = shortTick;
    stepTime = stepCount/totalSteps;

    // Initially accelerate from 300 to 100 microsecond delay
    if ( stepTime < rampingTime ) { 
      stepTick = longTick - stepTime*(longTick - shortTick)/rampingTime; 
    }

    // Terminally decelerate from 100 to 300 microsecond delay
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
