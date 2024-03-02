const int VIB_PIN = 11;
const int BUTTON_PIN = 3;

// button (debounce) state machine
const unsigned long DEBOUNCE_DELAY = 50;
unsigned long lastDebounceTime = 0; 
int lastButtonState = LOW;  

// the debounced button event
int buttonState;            
bool hasReading;

// actual state machine
const unsigned long CYCLE = 10 * 60 * 1000UL; // ten minutes
const unsigned long BUZ = 300UL; // 300 milliseconds

bool isOn = false;
unsigned long startedCycleTime;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(VIB_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // button is active low
}

void loop() {
  unsigned long now = millis();

  // button state machine

  // debounce: take reading as button event if input has been stable for at least DEBOUNCE_DELAY milliseconds
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = now;
  }

  hasReading = false;
  if ((now - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
      hasReading = true;
    }
  }
  lastButtonState = reading;

  // actual state machine
  if (hasReading && !buttonState) {
    // toggle on button down
    isOn = !isOn;
    startedCycleTime = now;
  }

  if (now > (startedCycleTime + CYCLE)) {
    startedCycleTime = now;
  }

  // actuators
  digitalWrite(LED_BUILTIN, isOn);
  digitalWrite(VIB_PIN, isOn && (now < startedCycleTime + BUZ));  
}