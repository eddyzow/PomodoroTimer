#include <Adafruit_GFX.h>
#include <Adafruit_ST7796S_kbv.h>
#include <SPI.h>
#include <Encoder.h>

#include "Fonts/FreeSansBold24pt7b.h"

// remember logic shifter!!

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  -1 // Connect display RST to Nano RST pin for hardware reset.
#define ENCODER_A   2 // Interrupt Pin
#define ENCODER_B   3 // Interrupt Pin
#define ENCODER_SW  4
#define START_PAUSE_BUTTON  8
#define EXTEND_BUTTON       7
#define MODE_BUTTON         A1
#define BUZZER_PIN  5
#define WORK_BG     0x000F  // A very dark, deep blue
#define BREAK_BG    0x0410  // A calming, dark teal
#define IDLE_BG     0x4208  // A medium-dark grey
#define WHITE       0xFFFF
#define YELLOW      0xFFE0

Adafruit_ST7796S_kbv tft(TFT_CS, TFT_DC, TFT_RST);
Encoder myEncoder(ENCODER_A, ENCODER_B);
enum OperatingMode { MODE_POMODORO, MODE_52_17, MODE_ALARM };
OperatingMode currentMode = MODE_POMODORO;
enum TimerState {
  STATE_IDLE, STATE_RUNNING_WORK, STATE_RUNNING_BREAK, STATE_PAUSED,
  STATE_SETTING_MINUTES, STATE_SETTING_SECONDS, STATE_ALARM_TRIGGERED
};
TimerState currentState = STATE_IDLE;
TimerState pausedState;

unsigned long timeLeftInSeconds = 0;
unsigned long workTimeSetting = 25 * 60;
unsigned long breakTimeSetting = 5 * 60;
unsigned long lastSecondMillis = 0;

long encoderPosition  = 0;
bool isBlinking = false;
unsigned long lastBlinkMillis = 0;

char lastDisplayedTime[6] = "00:00";
char lastSetTimeString[6] = "00:00";
uint16_t currentBgColor = IDLE_BG;

void buzz(int count, int duration, int frequency = 1175);

void setup() {
  delay(500); 

  pinMode(ENCODER_SW, INPUT_PULLUP);
  pinMode(START_PAUSE_BUTTON, INPUT_PULLUP);
  pinMode(EXTEND_BUTTON, INPUT_PULLUP);
  pinMode(MODE_BUTTON, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  tft.begin(8000000); 
  tft.setRotation(1);

  buzz(1, 100, 1175); 
  resetToIdle();
}

void loop() {
  handleInputs();

  if (currentState == STATE_RUNNING_WORK || currentState == STATE_RUNNING_BREAK) {
    updateTimer();
  }
  
  if (currentState == STATE_SETTING_MINUTES || currentState == STATE_SETTING_SECONDS) {
    handleTimeSetting();
    handleBlinking();
  }
}

void updateTimer() {
  if (millis() - lastSecondMillis >= 1000) {
    lastSecondMillis = millis();
    if (timeLeftInSeconds > 0) {
      timeLeftInSeconds--;
      drawTime();
    } else {
      handleTimerEnd();
    }
  }
}

void handleTimerEnd() {
  if (currentMode == MODE_POMODORO || currentMode == MODE_52_17) {
    if (currentState == STATE_RUNNING_WORK) {
      buzz(2, 150, 1500); 
      currentState = STATE_RUNNING_BREAK;
      timeLeftInSeconds = breakTimeSetting;
    } else {
      buzz(1, 200, 1175); 
      currentState = STATE_RUNNING_WORK;
      timeLeftInSeconds = workTimeSetting;
    }
  } else {
    currentState = STATE_ALARM_TRIGGERED;
    while (true) {
      buzz(1, 500, 2000); 
      delay(1000);
      if (digitalRead(START_PAUSE_BUTTON) == LOW || digitalRead(EXTEND_BUTTON) == LOW ||
          digitalRead(MODE_BUTTON) == LOW || digitalRead(ENCODER_SW) == LOW) {
        break;
      }
    }
    resetToIdle();
  }
  drawScreenLayout();
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
  drawScreenLayout();
}

void handleInputs() {
  if (digitalRead(START_PAUSE_BUTTON) == LOW) {
    delay(50);
    if (digitalRead(START_PAUSE_BUTTON) == LOW) {
      buzz(1, 50, 1175); 
      if (currentState == STATE_IDLE) currentState = STATE_RUNNING_WORK;
      else if (currentState == STATE_RUNNING_WORK || currentState == STATE_RUNNING_BREAK) {
        pausedState = currentState; currentState = STATE_PAUSED;
      } else if (currentState == STATE_PAUSED) currentState = pausedState;
      drawScreenLayout();
      while (digitalRead(START_PAUSE_BUTTON) == LOW);
    }
  }

  if (digitalRead(EXTEND_BUTTON) == LOW) {
    delay(50);
    if (digitalRead(EXTEND_BUTTON) == LOW) {
      if (currentState != STATE_IDLE && currentState != STATE_SETTING_MINUTES && currentState != STATE_SETTING_SECONDS) {
        buzz(1, 50, 1175);
        timeLeftInSeconds += 5 * 60;
        drawTime();
      }
      while (digitalRead(EXTEND_BUTTON) == LOW);
    }
  }

  if (digitalRead(MODE_BUTTON) == LOW) {
    delay(50);
    if (digitalRead(MODE_BUTTON) == LOW) {
      if (currentState == STATE_IDLE || currentState == STATE_PAUSED) {
        buzz(1, 50, 1175); 
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
        buzz(1, 50, 1175);
        currentState = STATE_SETTING_MINUTES;
        drawSettingScreenLayout();
        myEncoder.write((timeLeftInSeconds / 60) * 4);
        encoderPosition = myEncoder.read() / 4;
      } else if (currentState == STATE_SETTING_MINUTES) {
        buzz(1, 50, 1175);
        currentState = STATE_SETTING_SECONDS;
        drawSettingTime();
        myEncoder.write((timeLeftInSeconds % 60) * 4);
        encoderPosition = myEncoder.read() / 4;
      } else if (currentState == STATE_SETTING_SECONDS) {
        if (timeLeftInSeconds == 0) timeLeftInSeconds = 1;
        workTimeSetting = timeLeftInSeconds;
        buzz(2, 75, 1568);
        currentState = STATE_IDLE;
        drawScreenLayout();
      }
      while (digitalRead(ENCODER_SW) == LOW);
    }
  }
}

void handleTimeSetting() {
    long newEncoderPos = myEncoder.read() / 4;
    long oldEncoderPos = encoderPosition;
    
    if (newEncoderPos != oldEncoderPos) {
        if (currentState == STATE_SETTING_MINUTES) {
            long minutes = timeLeftInSeconds / 60;
            if (newEncoderPos < oldEncoderPos) minutes++;
            else minutes--;
            if (minutes < 0) minutes = 59;
            if (minutes > 59) minutes = 0;
            timeLeftInSeconds = (minutes * 60) + (timeLeftInSeconds % 60);
        } else if (currentState == STATE_SETTING_SECONDS) {
            long seconds = timeLeftInSeconds % 60;
            if (newEncoderPos < oldEncoderPos) seconds++;
            else seconds--;
            if (seconds < 0) seconds = 59;
            if (seconds > 59) seconds = 0;
            timeLeftInSeconds = (timeLeftInSeconds / 60) * 60 + seconds;
        }
        
        isBlinking = true;
        lastBlinkMillis = millis();
        drawSettingTime();
        encoderPosition = newEncoderPos;
    }
}
void drawScreenLayout() {
  currentBgColor = IDLE_BG;
  if (currentState == STATE_RUNNING_WORK || currentState == STATE_ALARM_TRIGGERED) currentBgColor = WORK_BG;
  if (currentState == STATE_RUNNING_BREAK) currentBgColor = BREAK_BG;
  tft.fillScreen(currentBgColor);

  tft.setFont();
  tft.setCursor(10, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  switch (currentMode) {
    case MODE_POMODORO: tft.print("Pomodoro"); break;
    case MODE_52_17:    tft.print("52/17");    break;
    case MODE_ALARM:    tft.print("Alarm");    break;
  }

  if (currentState == STATE_PAUSED) {
    tft.setCursor(10, 35);
    tft.print("PAUSED");
  }

  drawTime();
}

void drawTime() {
  int minutes = timeLeftInSeconds / 60;
  int seconds = timeLeftInSeconds % 60;
  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", minutes, seconds);
  
  tft.setFont(&FreeSansBold24pt7b);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds("00:00", 0, 0, &x1, &y1, &w, &h);
  int16_t x = (tft.width() - w) / 2;
  int16_t y = (tft.height() + h) / 2;

  tft.setTextColor(currentBgColor);
  tft.setCursor(x, y);
  tft.print(lastDisplayedTime);

  tft.setTextColor(WHITE);
  tft.setCursor(x, y);
  tft.print(timeStr);
  
  strcpy(lastDisplayedTime, timeStr);
}

void drawSettingTime(){
    int minutes = timeLeftInSeconds / 60;
    int seconds = timeLeftInSeconds % 60;
    
    char minStr[3]; sprintf(minStr, "%02d", minutes);
    char secStr[3]; sprintf(secStr, "%02d", seconds);
    char fullStr[6]; sprintf(fullStr, "%s:%s", minStr, secStr);
    
    uint16_t minColor = (currentState == STATE_SETTING_MINUTES && isBlinking) ? YELLOW : WHITE;
    uint16_t secColor = (currentState == STATE_SETTING_SECONDS && isBlinking) ? YELLOW : WHITE;

    tft.setFont(&FreeSansBold24pt7b);
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds("00:00", 0, 0, &x1, &y1, &w, &h);
    int16_t x = (tft.width() - w) / 2;
    int16_t y = (tft.height() + h) / 2;

    tft.setTextColor(IDLE_BG);
    tft.setCursor(x, y);
    tft.print(lastSetTimeString);

    tft.setCursor(x, y);
    
    tft.setTextColor(minColor);
    tft.print(minStr);

    tft.setTextColor(WHITE);
    tft.print(":");

    tft.setTextColor(secColor);
    tft.print(secStr);

    strcpy(lastSetTimeString, fullStr);
}

void drawSettingScreenLayout() {
    currentBgColor = IDLE_BG;
    tft.fillScreen(currentBgColor);
    tft.setFont();
    tft.setTextSize(2);
    tft.setCursor(10,10);
    tft.setTextColor(WHITE);
    tft.print("Set Time:");
    sprintf(lastSetTimeString, "%02d:%02d", (int)(timeLeftInSeconds / 60), (int)(timeLeftInSeconds % 60));
    drawTime();
}

void handleBlinking() {
  if (millis() - lastBlinkMillis > 400) {
    lastBlinkMillis = millis();
    isBlinking = !isBlinking;
    drawSettingTime();
  }
}

void buzz(int count, int duration, int frequency = 1175) {
  for (int i = 0; i < count; i++) {
    tone(BUZZER_PIN, frequency, duration);
    if(count > 1) delay(duration + 50);
  }
}

#include "Fonts/FreeSansBold24pt7b.h"

