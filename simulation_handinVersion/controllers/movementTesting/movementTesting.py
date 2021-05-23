"""movementTesting controller."""

# You may need to import some classes of the controller module. Ex:
#  from controller import Robot, Motor, DistanceSensor
from controller import Robot, Supervisor, Motor

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

WHEEL_BASE_RADIUS = 0.2 
WHEEL_RADIUS = 0.0985  
TARGET_FORWARD_VELOCITY = 1.0  # m/s, 
MAX_WHEEL_VELOCITY = 2.0  #  10.15
LIDAR_FOV = 6.28  # radians
OBJECT_HIT_DIST = 0.8  # when is it considered a hit, measured in m
OBJECT_FOLLOW_DIST = 1.2  # how close does it follow in m

# Main loop:
# - perform simulation steps until Webots is stopping the controller
while robot.step(timestep) != -1:
    C = 50
   # print('Circumference:', C)
    C2 = C+0.2
    
    #ratio = C/C2
   # print('ratio', ratio)
    
    linear_velocity = WHEEL_RADIUS * MAX_WHEEL_VELOCITY
    
    durationCircle = C/linear_velocity
   # print('Duration of the circle', durationCircle)
    start_time = robot.getTime()
    current_time = robot.getTime()
    
    angle_of_rotation = 6.28 
    distance_between_wheels = 0.090 #change depending on turtlebot or turf tank
    rate_of_rotation = (2 * linear_velocity) / distance_between_wheels
    durationTurning = angle_of_rotation/rate_of_rotation
    

    left_speed = 0.9*TARGET_FORWARD_VELOCITY #ratio*TARGET_FORWARD_VELOCITY
    right_speed = TARGET_FORWARD_VELOCITY
   # print ('left speed for circle', left_speed)
   # print('right speed for circle', right_speed)
    left_motor.setVelocity(left_speed)
    right_motor.setVelocity(right_speed)


    pass

# Enter here exit cleanup code.
