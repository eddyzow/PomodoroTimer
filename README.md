# PomodoroTimer
*i need to study and don't know how!*

I've struggled with autism in early childhood and ADHD my entire life. It goes without saying that studying has never been the easiest thing for me. I've always been keen on finding interesting ways to improve study habits as a result! One of these strategies is the Pomodoro Timer, a special study method that involves 25 minutes of work and then 5 minutes of break. There's also the 52/17 method, which gives a 17-minute break after 52 minutes of studying. After not enjoying having to use a Pomodoro Timer on some random online websites, I decided that a beautiful physical device to allow me to use the Pomodoro Timer effectively would be a very cool thing to build for Highway! I decided to use a TFT LCD screen because I've never done any hardware work with displays nor have I done any work with SPI communication devices. This is the first of those for me!

Behold PomodoroTimer! A simple pomodoro timer with time adjustment function, swap between modes (normal, extended break, 52/17), and alarm. Powered by an Arduino Nano and contains minimalistic controls: just three momentary pushbutton switches and an EC11 rotary encoder on the side. Timer is shown through a 4.0" TFT LCD screen. Custom firmware written in Arduino. The entire design was created in Onshape CAD! Created for Hack Club's Highway program.

# Design Photos

![image](https://github.com/user-attachments/assets/7c70e237-0dee-4561-a22a-16a817cc7446)
![image](https://github.com/user-attachments/assets/7f2ca509-f032-401e-9ef8-379abd75b036)
![image](https://github.com/user-attachments/assets/ef842b13-29de-476f-a823-ebbba32c1691)
![image](https://github.com/user-attachments/assets/695a9b8f-ceb3-4d98-bd90-63149d2656ee)

# Case Design

![image](https://github.com/user-attachments/assets/febabce9-ad7b-4055-9bb8-aab78de73a8a)

# Firmware
Firmware was written in Arduino with a small amount of help from Gemini -- library being used is the Adafruit_ST7796S_kbv package and pin choices and wire routing is still my own!

# Bill of Materials
For a full BOM with links, visit https://docs.google.com/spreadsheets/d/1WLxYVnncp4hckqmgex7FmrPYrEnqb0mDVtI4Y9LR5lQ/edit?usp=sharing

| Item Name                                | Purpose                                                  | Item Source | Quantity | Unit Cost (includes taxes) | Cost    |
|:-----------------------------------------|:----------------------------------------------------------|:------------|---------:|:----------------------------|:--------|
| 2pc 12mm Momentary Pushbutton Switch     | Buttons for mode, start/stop, and extend functions        | Amazon      |        2 | $7.43                      | $14.86  |
| 2pc 12mm Latching Pushbutton Switch      | Power button                                              | Amazon      |        1 | $7.43                      | $7.43   |
| Clone Arduino Nano Board (3 Pack)        | Logic / brains for the timer. Not sold individually!!     | Amazon      |        1 | $16.88                     | $16.88  |
| 4.0" 480x320 TFT LCD Screen              | Color screen to make the function of the timer beautiful  | Amazon      |        1 | $18.37                     | $18.37  |
| AA Battery Holder (4 AA)                 | Holds the batteries for the project                       | Amazon      |        1 | $6.35                      | $6.35   |
| Soldering Iron                           | I didn't get a soldering iron grant yet... and this project requires it | Amazon |        1 | $12.74                     | $12.74  |
| Jumper Wires                             | To connect the various electronics on the project         | Amazon      |        1 | $7.42                      | $7.42   |
| 3.3V-5V Logic Level Shifter              | Logic level shifter is necessary to communicate to the 3.3V LCD | Amazon |        1 | $7.95                      | $7.95   |
| Passive Buzzer                           | To make sounds on input and alarm/timer                   | Amazon      |        1 | $6.36                      | $6.36   |
| **TOTAL**                                |                                                           |             |       10 |                            | **$98.36** |

