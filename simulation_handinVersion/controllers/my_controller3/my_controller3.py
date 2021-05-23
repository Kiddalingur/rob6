"""my_controller controller."""

from math import sqrt, acos
from controller import Supervisor, Lidar, Motor, Node, Field, Robot, InertialUnit
import numpy as np
import pandas as pd
import math 
import time 
import itertools


# create the Robot instance.
robot = Supervisor()
timestep = int(robot.getBasicTimeStep())
# get the time step of the current world.


max_speed = 6.28 #angular velocity


# Find motors and set them up to velocity control
left_motor = Motor("wheel_left_joint")
right_motor = Motor("wheel_right_joint")

left_motor.setPosition(float('inf'))
left_motor.setVelocity(0.0)

right_motor.setPosition(float('inf'))
right_motor.setVelocity(0.0)

#seting up positionSensors
left_sensor = robot.getPositionSensor('wheel_left_joint_sensor')
left_sensor.enable(timestep)

right_sensor = robot.getPositionSensor('wheel_right_joint_sensor')
right_sensor.enable(timestep)

#Setting up IMU
IMU = InertialUnit('inertial_unit')

#make sure it is running
if IMU is None:
    print("Could not find an IMU.")
    exit()
#timestep is the sampling period in milliseconds
IMU.enable(timestep)


def get_my_location():
    #robot = Supervisor()
    return robot.getSelf().getPosition()

#def getRollPitchYaw():

def set_velocities(linear_vel, angular_vel):
    """Set linear and angular velocities of the robot.

    Arguments:
    linear_velocity  -- Forward velocity in m/s.
    angular_velocity -- Rotational velocity rad/s. Positive direction is
                        counter-clockwise.
    """
    diff_vel = angular_vel * WHEEL_BASE_RADIUS / WHEEL_RADIUS
    right_vel = (linear_vel + diff_vel) / WHEEL_RADIUS
    left_vel = (linear_vel - diff_vel) / WHEEL_RADIUS
    fastest_wheel = abs(max(right_vel, left_vel))
    if fastest_wheel > MAX_WHEEL_VELOCITY:
        left_vel = left_vel / fastest_wheel * MAX_WHEEL_VELOCITY
        right_vel = right_vel / fastest_wheel * MAX_WHEEL_VELOCITY
    left_motor.setVelocity(left_vel)
    right_motor.setVelocity(right_vel)
   

       

def painting(robot_translation):
    root = robot.getRoot()
    world_children = root.getField("children")
    world_children.importMFNodeFromString(
        -1,
        """
    Transform {{
        translation {} 0.0 {}
        children [
            Shape {{
            appearance PBRAppearance {{
                baseColor 1 1 1
                metalness 0
            }}
            geometry Sphere {{
                radius 0.1
            }}
            }}
        ]
    }}
    """.format(
            robot_translation[0], robot_translation[2]
        ),
    )


def type(pathType):
    
    print("Type:", pathType)

    if pathType == 0:
       x1 = []
       x2 = []
       y1 = []
       y2 = [] 
       x1 = df.iloc[i, 3]   
       x2 = df.iloc[i, 5] 
       y1 = df.iloc[i, 4] 
       y2 = df.iloc[i, 6] 
       tt = df.iloc[i, 10]
       line(x1, y1, x2, y2, tt)
       
       #test = getRollPitchYaw()
       #print('Printing test RPY')
       #print(test)
       
       return
    
    if pathType == 1:
        #circle()  
        return 
       
    #add more line types   
def angle_func(x1, x2, y1, y2):
    vec_x = x2 - x1
    vec_y = y2 - y1
    print('vec_x:', vec_x)
    print('vec_y:', vec_y)
    
    vec_slope = vec_y/vec_x
    print('vec_slope:', vec_slope)
    
    imu_data = IMU.getRollPitchYaw()
    imu_yaw = imu_data[2]
    
    #angle = vec_slope - imu_yaw
    angle = imu_yaw - vec_slope
    #print('Angle:', angle)
    
    return angle 

def vec_length(x1, x2, y1, y2):
    vec_x = x2 - x1
    vec_y = y2 - y1 
    print('length func vec_x:', vec_x)   
    print('length func vec_y:', vec_y)
    
    length_vec = sqrt(pow(vec_x,2)+pow(vec_y,2))
    
    return length_vec
    
    
    
#point 1, point2 and travel type
def line(x1, y1, x2, y2, tt):
    
    print("Point 1")
    print(x1, y1)
    print("Point 2")
    print(x2, y2)
    #tt = 0, no paint. tt = 1, paint time
    print('Travel Type:', tt)
   
    #no paint
    if tt == 0:
        
        line_length = abs(x2-x1)
        print('length', line_length)
        driveLine(line_length, tt, x1, x2, y1, y2)
        
        return
    
    #paint
    if tt == 1:
        
        line_length = abs(x2-x1)
        print('length', line_length)
        driveLine(line_length, tt, x1, x2, y1, y2)
        
        return 

#def circle(x1,y1,x2,y2,rx,ry)
#    return

def driveLine(line_length, painting, x1, x2, y1, y2):
    #timestep = int(robot.getBasicTimeStep())
    #robot.step(100)
    #time.sleep(1)
    position = get_my_location()
    
    rpy_values = []
    rpy_values = IMU.getRollPitchYaw()
    
    angle = angle_func(x1, x2, y1, y2)
    length_vec = vec_length(x1, x2, y1, y2)
    
    print('Angle', angle)
    print('length vec func:', length_vec)
    print('Roll, pitch, yaw: ', rpy_values)
    

    while True:
        rpy_values = IMU.getRollPitchYaw()
        while robot.step(timestep) != -1:
        
        set_velocities(1, 6.28)
        
            
            
    
    

def driveCircle(): #make later
 
    return 

def driveEllipse():
    
    return
    
def driveBezier():

    return


df = pd.read_csv('fastfield.csv')
i = 0
print(len(df))
while i < len(df) and robot.step(timestep) != -1:   
    print('Segment number:', i)  
    pathType = []
    pathType = df.iloc[i, 2]
    type(pathType)
    i += 1

    
   
    
#if __name__ == "__main__":
#    pass
    


 