// Pin out variables
const int dataPin = 2;
const int latchPin = 3;
const int clockPin = 4;
const int scoreUpPin = 10;
const int scoreDownPin = 9;
const int scoreResetPin = 8;
const int sensorOnPin = 13;
const int sensorInPin = 12;

// Button states variables
int buttonUpState = LOW;
int lastButtonUpState = LOW;
int buttonDownState = LOW;
int lastButtonDownState = LOW;
int buttonResetState = LOW;
int lastButtonResetState = LOW;
int sensorState = LOW;
int lastSensorState = LOW;
int buttonUpRead = LOW;
int buttonDownRead = LOW;
int buttonResetRead = LOW;
int sensorRead = HIGH;

// Button read variables

// Debounce variables
long lastUpDebounceTime = 0;  // the last time the output pin was toggled
long lastDownDebounceTime = 0;  // the last time the output pin was toggled
long lastResetDebounceTime = 0;  // the last time the output pin was toggled
long lastSensorDebounceTime = 0; // the last time the sensor was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
long sensorDebounceDelay = 100;

// Display variables
int score = 0;
int numberDisplay[10] = {126, 48, 109, 121, 51, 91, 95, 112, 127, 115};

void setup() {
  Serial.begin(9600);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(sensorOnPin, OUTPUT);
  pinMode(scoreUpPin, INPUT);
  pinMode(scoreDownPin, INPUT);
  pinMode(scoreResetPin, INPUT);
  pinMode(sensorInPin,INPUT);
  digitalWrite(sensorOnPin, HIGH);
  delay(100);
  updateDisplay(0);
}

void loop(){

  // Read in button values
  buttonUpRead = digitalRead(scoreUpPin);
  buttonDownRead = digitalRead(scoreDownPin);
  buttonResetRead = digitalRead(scoreResetPin);
  sensorRead = digitalRead(sensorInPin);

  // Check if debounce timer variables need to be updated
  debounceUpdate();

  checkUpButton();
  checkDownButton();
  checkResetButton();
  checkSensorIn();

  lastButtonUpState = buttonUpRead;
  lastButtonDownState = buttonDownRead;
  lastButtonResetState = buttonResetRead;
}

void updateCount(char* action, char* reset){
  if (reset == "y"){
    score = 0;
  }
  else{
    if (action == "+"){
      score++;
      if (score > 99){
        score = 0;
      }
    }else{
      score--;
      if (score < 0){
        score = 0;
      }
    }
  }
  updateDisplay(score);
//  delay(100);
}

void updateDisplay(int value){
  // convert decimal number to tens and units.
  int tens = 0;
  int units = 0;
  tens = value/10;
  units = value % 10;

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, numberDisplay[units]);
  shiftOut(dataPin, clockPin, LSBFIRST, numberDisplay[tens]);
  digitalWrite(latchPin, HIGH);
}

void debounceUpdate(){
  if (buttonUpRead != lastButtonUpState) {
    // set new debounce time.
    lastUpDebounceTime = millis();
  }

  if (buttonDownRead != lastButtonDownState) {
    // set new debounce time.
    lastDownDebounceTime = millis();
  }

  if (buttonResetRead != lastButtonResetState) {
    // set new debounce time.
    lastResetDebounceTime = millis();
  }
  if (sensorRead != lastSensorState) {
    // set newdebounce time.
    lastSensorDebounceTime = millis();
  }
}

void checkUpButton(){
  // If the time the program has been running - the time the button was pressed is greater than the debounceDelay
  // ie. if the time since last press is > than the debouceDelay
  if ((millis() - lastUpDebounceTime) > debounceDelay) {
    // if the button has changed from the state:
    if (buttonUpRead != buttonUpState) {
      // Change the buttonState Variable
      buttonUpState = buttonUpRead;
      // if the button state has gone from low to high then button has been pressed.
      if (buttonUpState == HIGH) {
        updateCount("+","n");
      }
    }
  }
}

void checkDownButton(){
  // If the time the program has been running - the time the button was pressed is greater than the debounceDelay
  // ie. if the time since last press is > than the debouceDelay
  if ((millis() - lastDownDebounceTime) > debounceDelay) {
  // if the button state has changed:
    if (buttonDownRead != buttonDownState) {
      // Change the buttonState Variable
      buttonDownState = buttonDownRead;
      // if button state has gone from low to high it has been pressed.
      if (buttonDownState == HIGH) {
        updateCount("-","n");
      }
    }
  }
}

void checkResetButton(){
  // If the time the program has been running - the time the button was pressed is greater than the debounceDelay
  // ie. if the time since last press is > than the debouceDelay
  if ((millis() - lastResetDebounceTime) > debounceDelay) {
  // if the button state has changed:
    if (buttonResetRead != buttonResetState) {
      // Change the buttonState Variable
      buttonResetState = buttonResetRead;
      // if button state has gone from low to high it has been pressed.
      if (buttonResetState == HIGH) {
        updateCount("+","y");
      }
    }
  }
}

void checkSensorIn(){
  //if (!sensorRead){
  //   updateCount("+","n");
  // }
  if ((millis() - lastResetDebounceTime) > sensorDebounceDelay) {
    if (sensorRead != sensorState) {
      sensorState = sensorRead;
      if (sensorState == LOW){
        updateCount("+","n");
      }
    }
    else {
      powerCycleSensor(sensorInPin);
    }
  }
 Serial.println(sensorRead);
}

void powerCycleSensor(int sensorPin){
  digitalWrite(sensorPin, LOW);
  delay(10);
  digitalWrite(sensorPin, HIGH);
}
