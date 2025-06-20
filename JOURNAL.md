---
title: "PomodoroTimer"
author: "Eddy Zhao (@eddyzow)"
description: "A simple pomodoro timer with time adjustment function, swap between modes (normal, extended break, 52/17), and alarm."
created_at: "2025-06-16"
---

*Total hours spent: 11* | *Date started: 16 June 2025*

# Pomodoro Timer
_i need to study_

## entry #5 - 6/19

yesterday my project was not approved since the entire BOM was from amazon! thus, i went and removed some stuff and found a piezo buzzer i thought i had lost. and i was able to reduce the BOM by about $30!

i also updated the wiring diagram to show the direct wiring connections:

![image](https://github.com/user-attachments/assets/dccf9091-fe3c-4fe3-82ff-072d8d9f364b)

afterwards my project was approved -- can't wait to start!

_time spent during session: 1 hour_

## entry #4 - 6/17

finally, i've finished making the BOM and added pictures into the repo! i realized that the case is intended to come together in only two pieces (the back cover and the rest) -- i think this is great since my printer can print the entire case in one part (just with some supports). 

![image](https://github.com/user-attachments/assets/7ad71a21-b5eb-4201-8cd2-2f37d4211a31)

i'm looking forward to receiving my parts and getting to build this thing! it's been a great project and i hope to see you again in my next!

_time spent during session: 1 hour_

## entry #3 - 6/17

with some help from my best buddy (shh) i finished the firmware! it's based on the Adafruit_ST7796S_kbv package, a package that i'm not sure will seamlessly work with the TFT screen i'm using right now. if it doesn't work i'll just the TFT_eSPI library instead.

the code covers some of the tiny features of this timer that make it satisfying to use. for example, when you click the rotary encoder once, it flashes the minutes on the clock, then you can turn the encoder to increase or decrease the minutes of your timer! then when you click again, it will flash the seconds on the clock, so you can turn the encoder to change that! then when you click again, it sets the time! each click of a button has a sound feedback from the buzzer.

here are the modes (good luck reading code!)

![image](https://github.com/user-attachments/assets/5546b22c-aebc-4a77-a8f9-6d638b1ca8e6)

and this about wraps it up for the building phase of this short project! i'll add the STEP files into the repo. the last thing i need to do is create the BOM and then submit! it should be well under the budget limit of $150 for highway projects.

_time spent during session: 2 hours_

## entry #2 - 6/17

i spent a big chunk of the day working on this, and it's nearing completion (wow that was a fast project)! i did LOTS of research to figure out the electronics and how they were going to play out in this little housing. first, i've decided to use an Arduino Nano since it's smaller and fits onto a breadboard. i manually figured out each pin that was being used and determined that i would be able to fit everything onto a nano. also, i realized that the rotary encoders I have are surface-mount -- however, there was a way to mount them using the given washer and nut! so there is a rotary encoder on the side for adjusting time. 

there are four buttons. The left one will be a latching pushbutton switch that turns on when you press it and off when you press it again. this is the power switch. the other three will be normal buttons with START/STOP, EXTEND, and MODE functions respectively. 

voila the finished (unwired) product!

![image](https://github.com/user-attachments/assets/6f465b89-ef5a-4e12-8e44-3f62c5d3d08d)

![image](https://github.com/user-attachments/assets/a8de058f-4532-4dff-8756-441f35433a08)

![image](https://github.com/user-attachments/assets/09cdf5a2-80dc-4fb5-a667-1d0015178ad0)

_time spent during session: 6 hours_

## entry #1 - 6/16

i spent the last hour figuring out what i was going to use for the display, and i decided on this 4.0" TFT LCD (ST7796S) driven by an arduino nano. i then created a quick cad of the case, whuich does not contain any electronics components yet -- i will need to add the hole for the rotary encoder, buttons, power, etc. and all of it will fit inside this tight housing, likely powered by four AA batteries. if needed i will enlarge the housing. i will also add a hidden message inside / outside the case! 

tomorrow i will look at electronics and try to get some real stuff going on. i anticipate this will be a short project but i'll still learn a lot because i've never used SPI before and i've also never played with displays or buzzers before!

![image](https://github.com/user-attachments/assets/0397fb25-0675-433d-847f-a421fdb4fbc8)

_time spent during session: 1 hour_

## entry #0 - 6/16

welcome back! it's time for my FOURTH highway project! i can't believe i'm starting another one even though none of my first three have been built yet -- shame on long wait times for project approval and shipping. :(

this time, i'll be building something really necessary for me especially as i'm going into college. introducing... the Pomodoro Timer!

if you haven't heard of the pomodoro system, you study for 25 minutes, take a 5 min break, and repeat. it's supposed to give you the best study feel, and i thought a physical device to be the timer would be intersting.

i envision getting to use the rest of my rotary encoders and extra keyboard switches to build this, so the budget will hopefully be low! there will be other modes on the timer such as 52/17 (52 min work, 17 min break) for people like me who have a hard time locking in, and when locked in, a hard time locking out (ADHD people!)

that'll be it for this entry. i'm going to begin creating my design. no PCBs - i'm just going to use a good old breadboard for this one.
