// void calibrate(){
// 1) Create array of past 10 seconds of data
// 2) Determine the maximum voltage of that list
// 3) Set the threshold voltage to .75 * Max
// 4) Find the first instance of voltage data which crosses the threshold
// 5) Find the next instance of voltage data which goes below the threshold
// 6) Repeat previous 2 steps until all peaks are isolated
// 7) Calculate the average distance between those peaks
//
// void moniter(){
// Repeat steps 4-7
// Ensure that healthy breaths per second are occuring


// void calibrate(){
// 1) Read data from past 8 seconds
// 2) Determine the maximum and minimum voltage of that data
// 3) Set the threshold voltage to .75 * Max and .25 * (Min+1) (to avoid multiplying by 0)

// void loop(){
// Ensure that healthy breaths are occuring
// 1) Read data from past 10 seconds (dont record in list)
// 2) Determine the maximum voltage of that data
// If max < .75*UpperMax if max > 1.5*LowerMax then alarm, else repeat

/*
  Attach the center pin of a the MPX pressure sensor to pin
  A0, and the outside pins to +5V and ground.
*/

// Pin assignments

int sensorPin = A0;         // MPX Pressure sensor pin
int alarmPin = 8;			      // Piezo Pin
int buttonPin = 2;			    // Pushbutton Pin
bool buttonState = false;   // Variable for state of pushbutton
int sensorValue = 0;		    // Reading from pressure sensor
int thresholdMax = 0;		    // Lower threshold value for breath 
int upperThresholdMax = 0;	// Upper threshold value for breath
int minSensorValue = 1023;
int maxSensorValue = 0;

//rings the alarm once
void alarmRing() {
  tone(alarmPin, 500);
  delay(500);
  noTone(alarmPin);
  delay(500);
}



void readAndRecordMax(double duration) {
  minSensorValue = 1023;
  maxSensorValue = 0;
  long startTime = millis();
  while (millis() - startTime < (duration * 1000)) {
    // read the input on analog pin 0:
    sensorValue = analogRead(sensorPin);
    if (sensorValue < minSensorValue)
      minSensorValue = sensorValue;
    if (sensorValue > maxSensorValue)
      maxSensorValue = sensorValue;

    Serial.println(sensorValue);

    //    //print the data for debugging purposes
    //    Serial.print("Val = ");
    //    Serial.print(sensorValue);
    //    Serial.print("\tMin = ");
    //    Serial.print(minSensorValue);
    //    Serial.print("\tMax = ");
    //    Serial.println(maxSensorValue);
    delay(50);
  }
}

/**
  1) Read data from past 10 seconds (dont record in list)
  2) Determine the maximum voltage of that data
  3) Set the threshold voltage to .75 * Max
*/
void calibrate() {
  // 1) Read data from past 10 seconds (dont record in list)
  // 2) Determine the maximum voltage of that data
  readAndRecordMax(8);

  // 3) Set the threshold voltage to .75 * Max
  thresholdMax = 0.75 * maxSensorValue;
  upperThresholdMax = 1.5 * maxSensorValue;
}

//update the button state
void updateButtonState() {

  //  if (counter == 0) {
  //    //reset counter
  //    counter = 10;
  //    buttonState = false;
  //  } else {
  //    counter--;
  //  }
  if (digitalRead(buttonPin) == HIGH) {
    buttonState = true;
    counter = 10;
  }
}

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  Serial.println("Begin Calibration");
  calibrate();
  Serial.println("Finish Calibration");
}

/**
  1) Read data from past 10 seconds (dont record in list)
  2) Determine the maximum voltage of that data
  3) If max = .75* Max then alarm... else repeat
*/
void loop() {
  Serial.println("Begin Monitering Block");
  // 1) Read data from past 10 seconds (dont record in list)
  // 2) Determine the maximum voltage of that data
  readAndRecordMax(4);

  Serial.print("Val = ");
  Serial.print(sensorValue);
  Serial.print("\tMin = ");
  Serial.print(minSensorValue);
  Serial.print("\tMax = ");
  Serial.println(maxSensorValue);
  Serial.print("Threshold = ");
  Serial.println(thresholdMax);
  Serial.print("ButtonState = ");
  Serial.println(buttonState);

  // 3) If max = .75* Max then alarm... else repeat
  if (maxSensorValue <= thresholdMax || maxSensorValue >= upperThresholdMax) {
    while (buttonState == false) {
      alarmRing();
      updateButtonState();
    }
    buttonState = false;
  }
}
