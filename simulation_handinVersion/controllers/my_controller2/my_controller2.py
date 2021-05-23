"""my_controller controller."""

from math import sqrt, acos
from controller import Supervisor, Lidar, Motor, Node, Field, Robot
import numpy as np
import pandas as pd
import math 



# You may need to import some classes of the controller module. Ex:
#  from controller import Robot, Motor, DistanceSensor


def get_my_location():

    return robot.getSelf().getPosition()

def distance_to(goal_location):
    """Get the current distance to a goal"""
    my_location = get_my_location()
    dist = np.linalg.norm(np.array(goal_location) - np.array(my_location))
    return dist

#def place_marker(robot_translation): potential line marker 
    """Place a marker at the robot's location."""
#    root = robot.getRoot()
#    world_children = root.getField("children")
#    world_children.importMFNodeFromString(
#        -1,
        """
    Transform {{
        translation {} 0.1 {}
        children [
            Shape {{
            appearance PBRAppearance {{
                baseColor 0.8 0 0
            }}
            geometry Sphere {{
                radius 0.1
            }}
            }}
        ]
    }}
#    #""".format(
#            robot_translation[0], robot_translation[2]
#        ),
#    )



if __name__ == "__main__":
    
    #df = pd.read_csv('fieldfast.csv')
    #print(df)

    
    # create the Robot instance.
    robot = Supervisor()
    
    # get the time step of the current world.
    timestep = 64
    
    max_speed = 6.28 #angular velocity
    
    
    # Find motors and set them up to velocity control
    left_motor = Motor("wheel_left_joint")
    right_motor = Motor("wheel_right_joint")
    
    #Creating motor instances
    #left_motor = robot.getMotor('motor1')
    #right_motor = robot.getMotor('motor2')
    
    
    left_motor.setPosition(float('inf'))
    left_motor.setVelocity(0.0)
    
    right_motor.setPosition(float('inf'))
    right_motor.setVelocity(0.0)

    num_side = 4 #number of line segments
    length_line = 0.2 #change this depending on csv 
    
    wheel_radius = 0.025
    linear_velocity = wheel_radius * max_speed
    
    durationLine = length_line/linear_velocity
    
    start_time = robot.getTime()
    
    angle_of_rotation = 6.28/num_side #depends on csv value 
    distance_between_wheels = 0.090 #change depending on turtlebot or turf tank
    rate_of_rotation = (2 * linear_velocity) / distance_between_wheels
    durationTurn = angle_of_rotation/rate_of_rotation
   
   
    rot_start_time =start_time + durationLine
    rot_end_time = rot_start_time + durationTurn
   
   
   
   
    # Main loop:
    # - perform simulation steps until Webots is stopping the controller
    while robot.step(timestep) != -1:
    
        current_time = robot.getTime()
        
        left_speed = 0.5 * max_speed
        right_speed = 0.5 * max_speed
        
        if rot_start_time < current_time < rot_end_time:
            left_speed = -max_speed
            right_speed = max_speed
        
        elif current_time > rot_end_time:
            rot_start_time = current_time + durationLine
            rot_end_time = rot_start_time + durationTurn
        
        left_motor.setVelocity(left_speed)
        right_motor.setVelocity(right_speed)

        pass
    
    # Enter here exit cleanup code.
    