// Pin out variables
const int dataPin = 2;
const int latchPin = 3;
const int clockPin = 4;
const int scoreUpPin = 10;
const int scoreDownPin = 9;
const int scoreResetPin = 8;

// Button states variables
int buttonUpState = LOW;
int lastButtonUpState = LOW;
int buttonDownState = LOW;
int lastButtonDownState = LOW;
int buttonResetState = LOW;
int lastButtonResetState = LOW;
int buttonUpRead = LOW;
int buttonDownRead = LOW;
int buttonResetRead = LOW;
// Button read variables

// Debounce variables
long lastUpDebounceTime = 0;  // the last time the output pin was toggled
long lastDownDebounceTime = 0;  // the last time the output pin was toggled
long lastResetDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Display variables
int score = 0;
int numberDisplay[10] = {126, 48, 109, 121, 51, 91, 95, 112, 127, 115};

void setup() {
  Serial.begin(9600);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(scoreUpPin, INPUT);
  pinMode(scoreDownPin, INPUT);
  pinMode(scoreResetPin, INPUT);
  updateDisplay(0);
}

void loop(){

  // Read in button values
  buttonUpRead = digitalRead(scoreUpPin);
  buttonDownRead = digitalRead(scoreDownPin);
  buttonResetRead = digitalRead(scoreResetPin);
  
  // Check if debounce timer variables need to be updated
  debounceUpdate();
 
  checkUpButton();
  checkDownButton();
  checkResetButton();
  
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
      if (score > 9){
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
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, numberDisplay[value]);
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
/* 
  for (int currentValue = 0; currentValue <= 9; currentValue ++){
  
  // Disable the latch whil we clock in data
  digitalWrite(latchPin, LOW);
  
  // Send the value as a binary sequence to the module
  shiftOut(dataPin, clockPin, LSBFIRST, numberDisplay[currentValue]);
  
  // Enable the latch again to set the output states
  digitalWrite(latchPin, HIGH);
  
  delay(1000);
  }
 */ 

