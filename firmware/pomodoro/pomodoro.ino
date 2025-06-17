// pomodoro timer firmware

#include <Adafruit_GFX.h>
#include <Adafruit_ST7796S_kbv.h>
#include <SPI.h>
#include <Encoder.h>

// IMPORTANT!!!
// need to use logic level shifter! the TFT display is 3.3V and arduino nano is 5v. DO THIS OR ELSE THE ENTIRE PROJECT DIES!!


#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  -1  // Connect to Arduino's RST pin for hardware reset, or set to -1 for software reset.
#define ENCODER_A   2
#define ENCODER_B   3
#define ENCODER_SW  4
#define START_PAUSE_BUTTON  8
#define EXTEND_BUTTON       7
#define MODE_BUTTON         6
#define BUZZER_PIN  5
#define BLACK   0x0000
#define WHITE   0xFFFF
#define GREEN   0x07E0
#define YELLOW  0xFFE0
#define DARKGREY 0x7BEF

Adafruit_ST7796S_kbv tft;
Encoder myEncoder(ENCODER_A, ENCODER_B);

enum OperatingMode { MODE_POMODORO, MODE_52_17, MODE_ALARM };
OperatingMode currentMode = MODE_POMODORO;

enum TimerState {
  STATE_IDLE, STATE_RUNNING_WORK, STATE_RUNNING_BREAK, STATE_PAUSED,
  STATE_SETTING_MINUTES, STATE_SETTING_SECONDS, STATE_ALARM_TRIGGERED
};
TimerState currentState = STATE_IDLE;

unsigned long timeLeftInSeconds = 0;
unsigned long workTimeSetting = 25 * 60;
unsigned long breakTimeSetting = 5 * 60;
unsigned long lastSecondMillis = 0;

long encoderPosition  = 0;
long newPosition;

bool isBlinking = false;
unsigned long lastBlinkMillis = 0;

void setup() {
  pinMode(ENCODER_SW, INPUT_PULLUP);
  pinMode(START_PAUSE_BUTTON, INPUT_PULLUP);
  pinMode(EXTEND_BUTTON, INPUT_PULLUP);
  pinMode(MODE_BUTTON, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  tft.begin(0x7796);
  tft.setRotation(1); 

  buzz(1, 100);
  resetToIdle();
}

void loop() {
  handleInputs();

  if (currentState != STATE_SETTING_MINUTES && currentState != STATE_SETTING_SECONDS) {
    if (currentState == STATE_RUNNING_WORK || currentState == STATE_RUNNING_BREAK) {
      updateTimer();
    }
  } else {
    handleBlinking();
  }
}

void updateTimer() {
  if (millis() - lastSecondMillis >= 1000) {
    lastSecondMillis = millis();
    if (timeLeftInSeconds > 0) {
      timeLeftInSeconds--;
      updateDisplay();
    } else {
      handleTimerEnd();
    }
  }
}

void handleTimerEnd() {
  if (currentMode == MODE_POMODORO || currentMode == MODE_52_17) {
    if (currentState == STATE_RUNNING_WORK) {
      buzz(2, 150);
      currentState = STATE_RUNNING_BREAK;
      timeLeftInSeconds = breakTimeSetting;
    } else {
      buzz(1, 200);
      currentState = STATE_RUNNING_WORK;
      timeLeftInSeconds = workTimeSetting;
    }
  } else { 
    currentState = STATE_ALARM_TRIGGERED;
    while (true) {
      buzz(1, 500);
      delay(500);
      if (digitalRead(START_PAUSE_BUTTON) == LOW || digitalRead(EXTEND_BUTTON) == LOW ||
          digitalRead(MODE_BUTTON) == LOW || digitalRead(ENCODER_SW) == LOW) {
        break;
      }
    }
    resetToIdle();
  }
  updateDisplay();
}

void resetToIdle() {
  currentState = STATE_IDLE;
  switch (currentMode) {
    case MODE_POMODORO:
      workTimeSetting = 25 * 60; breakTimeSetting = 5 * 60; break;
    case MODE_52_17:
      workTimeSetting = 52 * 60; breakTimeSetting = 17 * 60; break;
    case MODE_ALARM:
      workTimeSetting = 10 * 60; breakTimeSetting = 0; break;
  }
  timeLeftInSeconds = workTimeSetting;
  updateDisplay();
}

void handleInputs() {
  if (digitalRead(START_PAUSE_BUTTON) == LOW) {
    delay(50);
    if (digitalRead(START_PAUSE_BUTTON) == LOW) {
      if (currentState == STATE_IDLE) {
        currentState = STATE_RUNNING_WORK;
      } else if (currentState == STATE_RUNNING_WORK || currentState == STATE_RUNNING_BREAK) {
        currentState = STATE_PAUSED;
      } else if (currentState == STATE_PAUSED) {
        bool wasWorkTimer = (currentMode == MODE_POMODORO || currentMode == MODE_52_17) && (timeLeftInSeconds > breakTimeSetting);
        if (currentMode == MODE_ALARM) wasWorkTimer = true;
        currentState = wasWorkTimer ? STATE_RUNNING_WORK : STATE_RUNNING_BREAK;
      }
      updateDisplay();
      while (digitalRead(START_PAUSE_BUTTON) == LOW);
    }
  }

  if (digitalRead(EXTEND_BUTTON) == LOW) {
    delay(50);
    if (digitalRead(EXTEND_BUTTON) == LOW) {
      if (currentState == STATE_RUNNING_WORK || currentState == STATE_RUNNING_BREAK || currentState == STATE_PAUSED) {
        timeLeftInSeconds += 5 * 60;
        updateDisplay();
      }
      while (digitalRead(EXTEND_BUTTON) == LOW);
    }
  }

  if (digitalRead(MODE_BUTTON) == LOW) {
    delay(50);
    if (digitalRead(MODE_BUTTON) == LOW) {
      if (currentState == STATE_IDLE || currentState == STATE_PAUSED) {
        currentMode = static_cast<OperatingMode>((currentMode + 1) % 3);
        resetToIdle();
      }
      while (digitalRead(MODE_BUTTON) == LOW);
    }
  }

  if (digitalRead(ENCODER_SW) == LOW) {
    delay(50);
    if (digitalRead(ENCODER_SW) == LOW) {
      if (currentState == STATE_IDLE || currentState == STATE_PAUSED) {
        setTime();
      }
      while (digitalRead(ENCODER_SW) == LOW);
    }
  }
}

void updateDisplay() {
  uint16_t bgColor = DARKGREY;
  if (currentState == STATE_RUNNING_WORK || currentState == STATE_ALARM_TRIGGERED) bgColor = BLACK;
  if (currentState == STATE_RUNNING_BREAK) bgColor = GREEN;
  tft.fillScreen(bgColor);

  tft.setCursor(10, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  String modeText = "";
  switch (currentMode) {
    case MODE_POMODORO: modeText = "Pomodoro"; break;
    case MODE_52_17:    modeText = "52/17"; break;
    case MODE_ALARM:    modeText = "Alarm"; break;
  }
  tft.print(modeText);

  int minutes = timeLeftInSeconds / 60;
  int seconds = timeLeftInSeconds % 60;
  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", minutes, seconds);
  
  tft.setTextSize(7);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor((tft.width() - w) / 2, (tft.height() - h) / 2);
  tft.print(timeStr);

  // Display PAUSED message
  if (currentState == STATE_PAUSED) {
    tft.setTextSize(3);
    tft.getTextBounds("PAUSED", 0, 0, &x1, &y1, &w, &h);
    tft.setCursor((tft.width() - w) / 2, tft.height() - h - 20);
    tft.print("PAUSED");
  }
}

void handleBlinking() {
  if (millis() - lastBlinkMillis > 400) {
    lastBlinkMillis = millis();
    isBlinking = !isBlinking;

    int minutes = timeLeftInSeconds / 60;
    int seconds = timeLeftInSeconds % 60;
    
    char minStr[3];
    sprintf(minStr, "%02d", minutes);
    char secStr[3];
    sprintf(secStr, "%02d", seconds);
    char colonStr[] = ":";

    tft.setTextSize(7);
    int16_t x1, y1;
    uint16_t w_min, w_col, w_sec, h;
    tft.getTextBounds(minStr, 0, 0, &x1, &y1, &w_min, &h);
    tft.getTextBounds(colonStr, 0, 0, &x1, &y1, &w_col, &h);
    tft.getTextBounds(secStr, 0, 0, &x1, &y1, &w_sec, &h);
    
    uint16_t totalWidth = w_min + w_col + w_sec;
    int16_t startX = (tft.width() - totalWidth) / 2;
    int16_t startY = (tft.height() - h) / 2;

    tft.setCursor(startX, startY);
    tft.setTextColor((currentState == STATE_SETTING_MINUTES && isBlinking) ? YELLOW : WHITE, DARKGREY);
    tft.print(minStr);

    tft.setTextColor(WHITE, DARKGREY);
    tft.print(colonStr);
    
    tft.setTextColor((currentState == STATE_SETTING_SECONDS && isBlinking) ? YELLOW : WHITE, DARKGREY);
    tft.print(secStr);
  }
}

void setTime() {
  TimerState previousState = currentState;
  
  currentState = STATE_SETTING_MINUTES;
  buzz(1, 50);
  updateDisplay();
  myEncoder.write((timeLeftInSeconds / 60) * 4);
  encoderPosition = myEncoder.read() / 4;

  while (true) {
    handleBlinking();
    newPosition = myEncoder.read() / 4;
    if (newPosition != encoderPosition) {
      if (newPosition < 0) newPosition = 0;
      if (newPosition > 59) newPosition = 59;
      myEncoder.write(newPosition * 4);
      encoderPosition = newPosition;
      int seconds = timeLeftInSeconds % 60;
      timeLeftInSeconds = (encoderPosition * 60) + seconds;
      handleBlinking();
    }
    if (digitalRead(ENCODER_SW) == LOW) { delay(50); while (digitalRead(ENCODER_SW) == LOW); break; }
  }

  currentState = STATE_SETTING_SECONDS;
  buzz(1, 50);
  isBlinking = false; updateDisplay();
  myEncoder.write((timeLeftInSeconds % 60) * 4);
  encoderPosition = myEncoder.read() / 4;
  
  while (true) {
    handleBlinking();
    newPosition = myEncoder.read() / 4;
    if (newPosition != encoderPosition) {
       if (newPosition < 0) newPosition = 0;
       if (newPosition > 59) newPosition = 59;
       myEncoder.write(newPosition * 4);
       encoderPosition = newPosition;
       int minutes = timeLeftInSeconds / 60;
       timeLeftInSeconds = (minutes * 60) + encoderPosition;
       handleBlinking();
    }
    if (digitalRead(ENCODER_SW) == LOW) { delay(50); while (digitalRead(ENCODER_SW) == LOW); break; }
  }

  if (timeLeftInSeconds == 0) timeLeftInSeconds = 1;
  workTimeSetting = timeLeftInSeconds;
  buzz(2, 75);
  currentState = previousState;
  updateDisplay();
}

void buzz(int count, int duration) {
  for (int i = 0; i < count; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
    if (count > 1) delay(duration / 2);
  }
}
