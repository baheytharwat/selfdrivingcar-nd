## Writeup

---

**PID Control Project**

The goals / steps of this project are the following:
* Controllingn the car to navigate in the track using PID Control without leaving the drivable portion.



## Rubric Points
### Here I will consider the [rubric points]https://review.udacity.com/#!/rubrics/1972/view) individually and describe how I addressed each point in my implementation.  

---
### Compilation

#### 1. The code compiles correctly.

The code is compiling without any errors with make and cmake. I did not change anything in Cmakelists.txt file.

### Implementation


#### 1. The PID procedure follows what was taught in the lessons.

PID control algorithm and all calculations follows what I was taught in the lessons. I translated most of the python code to C++.


### Reflection

#### 1. Describe the effect each of the P, I, D components had in your implementation.

The P paramater is the parameter that is proportional to cte. Increasing P gives better results until it reaches to a value. When P reachs this value, The car starts to oscillate. Here comes D paramater part, Increasing D decrease these oscillations. Large I paramater makes the car unstable.

#### 2. Describe how the final hyperparameters were chosen.

I have tuned the P,I and D manually. At first, I put all the values to 0, I started to increase P untill the car oscillates,then I started to increase D to reduce these oscillations and tried to play with I paramater but it gabe an unstable behaviour. I made two tuned paramaters (0.075, 0, 2), (0.15, 0, 2.5) Feel free to use both of them. (Code Lines 40-41).


### Simulations

#### 1. The vehicle must successfully drive a lap around the track.

The two tuned paramaters work well and can drive more than a lab around the track.
