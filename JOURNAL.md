*Total hours spent: 7* | *Date started: 16 June 2025*

---
# Pomodoro Timer
_i need to study_

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
