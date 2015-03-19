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
float insertionLength = 0;
float revolutions = 0;
float shortTick = 0;
float longTick = 0;
float rampingTime = 0;
float insertionTime = 0;
int buttonState = 0;
int GPIOpin = 0;


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
  rampingTime = 0.1;                     // Percent of oscillation
  insertionLength = 1;                   // inches
  revolutions = insertionLength/3.7665;  // 1 rev = 3.7665 in.
  longTick = 300;                        // microseconds
  shortTick = 100;                       //
  totalSteps = 6400*revolutions;         //
  insertionTime = (1e6)*10;              // seconds
}


// Motor run loop
void loop() {

  stepTick = longTick;

  // Push-button activation
  buttonState = digitalRead(7);
  if ( buttonState == LOW ) {

    // Display button state
    Serial.println(buttonState);

    // Insertion
    GPIOpin = 9;
    steppingAction();
    delayMicroseconds(insertionTime);

    // Withdrawal
    GPIOpin = 8;
    steppingAction();
  }
}


// Step action
void steppingAction() {

  stepCount = 0;
  while ( stepCount < totalSteps ) {

    // Ramping
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
    digitalWrite(GPIOpin, HIGH);
    delayMicroseconds(stepTick);
    digitalWrite(GPIOpin, LOW);
    delayMicroseconds(stepTick);

    stepCount++;
  }
}
