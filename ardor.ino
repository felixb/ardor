// inputs
const int BUTTON = 10;
const int GATE = 11;
const int GATE_OPEN = HIGH;

// outputs
const int LED_BUTTON = 13;
const int RELAY_CTRL = 8;
const int RELAY_OFF = HIGH;
const int RELAY_ON = LOW;

// consts
const int STATE_OFF = 0;
const int STATE_INIT = 1;
const int STATE_READY = 2;
const int STATE_OPEN = 3;

const unsigned long STATE_READY_DURATION = 60000;
const unsigned long LOOP_DELAY = 100;
const unsigned long RING_DELAY = 500;
const unsigned long BLINK_SLOW_FREQ = 2000;
const unsigned long BLINK_FAST_FREQ = 1000;

// variables
int state = STATE_OFF;
unsigned long initStart = 0;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON, INPUT);
  pinMode(GATE, INPUT);

  pinMode(LED_BUTTON, OUTPUT);
  pinMode(RELAY_CTRL, OUTPUT);

  digitalWrite(LED_BUTTON, LOW);
  digitalWrite(RELAY_CTRL, RELAY_OFF);
}

void loop() {
  delay(LOOP_DELAY);

  if (digitalRead(BUTTON) == LOW) {
    if (state != STATE_OFF) {
      stateChangeOff();
    }
  } else {
    if (state == STATE_OFF) {
      stateChangeInit();
    }
    if (state == STATE_INIT) {
      unsigned long t = millis();
      if (t - initStart > STATE_READY_DURATION) {
        stateChangeReady();
      } else {
        blinkButton(BLINK_SLOW_FREQ);
      }
    }
    if (state == STATE_READY or state == STATE_OPEN) {
      blinkButton(BLINK_FAST_FREQ);
    }
    if (state == STATE_READY and digitalRead(GATE) == GATE_OPEN) {
      stateChangeOpen();
    }
    if (state == STATE_OPEN and digitalRead(GATE) != GATE_OPEN) {
      stateChangeReady();
    }
  }
}

void stateChangeOff() {
  Serial.println("State OFF");
  state = STATE_OFF;
  digitalWrite(LED_BUTTON, LOW);
}

void stateChangeInit() {
  Serial.println("State INIT");
  state = STATE_INIT;
  initStart = millis();
}

void stateChangeReady() {
  Serial.println("State READY");
  state = STATE_READY;
  digitalWrite(LED_BUTTON, HIGH);
}

void stateChangeOpen() {
  Serial.println("State OPEN");
  state = STATE_OPEN;
  ringBell();
}

void blinkButton(unsigned long freq) {
  if (millis() % freq > freq >> 1) {
    digitalWrite(LED_BUTTON, HIGH);
  } else {
    digitalWrite(LED_BUTTON, LOW);
  }
}

void ringBell() {
  Serial.println("RING RING");
  digitalWrite(RELAY_CTRL, RELAY_ON);
  delay(RING_DELAY);
  digitalWrite(RELAY_CTRL, RELAY_OFF);
}
