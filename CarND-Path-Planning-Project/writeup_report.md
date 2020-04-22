# **Traffic Sign Recognition** 

## Writeup

---

**Hghway Driving Project**

The goals / steps of this project are the following:
* In this project my goal is to safely navigate around a virtual highway with other traffic that is driving +-10 MPH of the 50 MPH speed limit. You will be provided the car's localization and sensor fusion data, there is also a sparse map list of waypoints around the highway. The car should try to go as close as possible to the 50 MPH speed limit, which means passing slower traffic when possible, note that other cars will try to change lanes too. The car should avoid hitting other cars at all cost as well as driving inside of the marked road lanes at all times, unless going from one lane to another. The car should be able to make one complete loop around the 6946m highway. Since the car is trying to go 50 MPH, it should take a little over 5 minutes to complete 1 loop. Also the car should not experience total acceleration over 10 m/s^2 and jerk that is greater than 10 m/s^3.




## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/1971/view) individually and describe how I addressed each point in my implementation.  

---
### Compilation

#### 1. The code compiles correctly.

The code is compiling without any errors with make and cmake. I did not change anything in Cmakelists.txt file. I just added spline header file to the src folder.

### Valid Trajectories

#### 1. The car is able to drive at least 4.32 miles without incident..

I tested the planner with  the simulator many times and it did well and drove more than the required.


#### 2. The car drives according to the speed limit.

The maximum speed is 49.5 and there is a constrain on max and min speed ( Code lines 253 - 259 )

#### 3. Max Acceleration and Jerk are not Exceeded.
The speed increase and decrease with a constant rate which is 0.224 and that's don't exceed the max acceleration.

#### 4. Car does not have collisions.
According to what I have tested, the car does not collide. The car slows down or change lane when there is car ahead of it.

#### 5. The car stays in its lane, except for the time between changing lanes.
According to what I have tested, the car stays in its lane.

#### 6.The car is able to change lanes
The car change lanes when there  is a close car ahead of it < 30m and if its safe to change lanes. It's safe to change lanes when the left or right cars does not exist in specefic range. If there is no space for lane changing, The car starts to slow down to avoid collision. ( Code lines 140-172 ).



### Reflection

#### 1. There is a reflection on how to generate paths.

I started working from the code provided in Q&A video. Generating paths is done using previous points from the previous path for smoothing and continuity. I generate 50 point. All the previous points points that are not used by the simulator. The rest of the points are generated from getting points from a spline function 30m ahead. I fit spline using 5 points.two of them are previous points. three of them are initial points separated by 30m each. to get a smoothing lane changes and paths. Finally,  All the 50 points are sent to the simulator. ( Code lines  180 - 277)


