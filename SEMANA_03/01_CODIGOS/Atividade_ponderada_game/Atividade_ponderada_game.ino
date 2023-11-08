// Declare constants and variables
const int MAX_LEVEL = 20;
const int BUTTON_PINS[] = {23, 4, 15, 13}; // Array for button pins
const int LED_PINS[] = {16, 17, 18, 19};   // Array for LED pins
const int RESET_PIN = 22;
int sequence[MAX_LEVEL];
int user_sequence[MAX_LEVEL];
int current_level = 1;
int delay_time = 1000; // Initial delay between LED "blinks"

void setup() {
  Serial.begin(9600);
  // Initialize button pins
  for (int i = 0; i < 4; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
  // Initialize LED pins
  for (int i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
  // Initialize reset button pin
  pinMode(RESET_PIN, INPUT_PULLUP);
  generate_sequence(); // Initial sequence generation
}

void loop() {
  if (digitalRead(RESET_PIN) == LOW) {
    reset_game();
  } else if (current_level == 1) {
    show_sequence();
    get_user_sequence();
  } else {
    show_sequence();
    get_user_sequence();
  }
}

void generate_sequence() {
  randomSeed(millis());
  for (int i = 0; i < MAX_LEVEL; i++) {
    sequence[i] = random(16, 20); // Generate a random sequence within LED pin numbers
  }
}

void show_sequence() {
  for (int i = 0; i < current_level; i++) {
    digitalWrite(sequence[i], HIGH);
    delay(delay_time);
    digitalWrite(sequence[i], LOW);
    delay(200);
  }
}

void get_user_sequence() {
  for (int i = 0; i < current_level; i++) {
    int flag = 0;
    while (flag == 0) {
      for (int j = 0; j < 4; j++) {
        if (digitalRead(BUTTON_PINS[j]) == LOW) {
          digitalWrite(LED_PINS[j], HIGH);
          delay(200);
          digitalWrite(LED_PINS[j], LOW);
          user_sequence[i] = LED_PINS[j];
          flag = 1;
          if (user_sequence[i] != sequence[i]) {
            indicate_error();
            return;
          }
          break; // Break the for-loop once a button is pressed
        }
      }
      if (digitalRead(RESET_PIN) == LOW) { // Check the reset button
        reset_game();
        return;
      }
    }
  }
  indicate_success();
}

void indicate_error() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PINS[i], HIGH);
    delay(250);
    digitalWrite(LED_PINS[i], LOW);
    delay(250);
  }
  reset_game(); // Reset the game if the sequence is incorrect
}

void indicate_success() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PINS[i], HIGH);
  }
  delay(500);
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }
  delay(500);
  if (current_level < MAX_LEVEL) {
    current_level++;
    delay_time -= 50;
  }
}

void reset_game() {
  // Code to reset the game
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }
  current_level = 1;
  delay_time = 1000;
  // Restart the sequence generation and game play
  generate_sequence();
  delay(1000); // Delay to allow for release of the reset button
}
