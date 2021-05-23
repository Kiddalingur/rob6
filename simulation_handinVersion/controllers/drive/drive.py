"""bug1 controller."""

import math
from math import acos, sqrt, pi, cos, sin, radians
from controller import Supervisor, Lidar, Motor, Node, Field, Pen, Robot, InertialUnit
import numpy as np
import pandas as pd
from array import *  

def GOAL_POINT(x,y):
   
    Point = [0, 0, 0]
    Point[0] = x
    Point[1] = 0.88
    Point[2] = y
    
    
    return Point

WHEEL_BASE_RADIUS = 0.2 
WHEEL_RADIUS = 0.0985  
TARGET_FORWARD_VELOCITY = 1.0  # m/s, 
MAX_WHEEL_VELOCITY = 2.15  #  10.15
LIDAR_FOV = 6.28  # radians
OBJECT_HIT_DIST = 0.8  # when is it considered a hit, measured in m
OBJECT_FOLLOW_DIST = 1.2  # how close does it follow in m

#Distance between the objects spawned when "painting"
paint_fq_circle = 50
paint_fq_line = 50
paint_fq_ellipse = 50
paint_fq_bezier =50
paint_fq_travel =350
paint_fq_pattern =300

draw_everything = 0         

robot = Supervisor()

#robot = Robot()

timestep = int(robot.getBasicTimeStep())

# LIDAR
top_lidar = Lidar('top_lidar')

pen_paint = Pen('pen_paint')

Pen.setInkColor(pen_paint, 0xFFFFFF, 0.9)
Pen.write(pen_paint, False)

IMU = InertialUnit('inertial_unit')

if IMU is None:
    print("Could not find an IMU.")
    exit()
#timestep is the sampling period in milliseconds
IMU.enable(timestep)
# The user guide recommends to use robot.getDevice("top_lidar"), but pylint has
# a hard time finding the type of the return value. This also style works fine
# for me.
if top_lidar is None:
    print("Could not find a lidar.")
    exit()
top_lidar.enable(100)

if pen_paint is None:
    print('no pen!')
    exit()

#paint = Pen('pen_paint')


# MOTORS
# Find motors and set them up to velocity control
left_motor = Motor("wheel_left_joint")
# Same as with the lidar above: robot.getDevice("wheel_left_joint")
right_motor = Motor("wheel_right_joint")

if left_motor is None or right_motor is None:
    print("Could not find motors.")
    exit()
left_motor.setPosition(float("inf"))
right_motor.setPosition(float("inf"))
left_motor.setVelocity(0.0)
right_motor.setVelocity(0.0)


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


def pure_pursuit(x_track_error, angle_error):
    """Follow a line.

    Arguments:
    x_track_error -- Cross-track error. The distance error perpenticular to the
                     reference line. If the robot is located to the left of the
                     line,it gives a positive error; negative to the right.
    angle_error   -- Angle error is the difference between the angle of the line
                     and the heading of the robot. If the reference line points
                     towards North and the robot to East, the error is positive;
                     negative to West.
    """
    #original values -0.05 and 0.3
    ang_vel = -0.05 * x_track_error + 0.9 * angle_error
    #print('ang_vel', ang_vel)
    set_velocities(TARGET_FORWARD_VELOCITY, ang_vel)
    

def get_my_location():
    """Gets the robots translation in the world."""
    return robot.getSelf().getPosition()


def angle_to_object(ranges):
    """Find the angle that points to the closest lidar point.

    We will assume that the lidar is mounted symmetrically, so that the center
    lidar point is at the front of the robot. We will return this as 0 rads.
    """
    index_min = min(range(len(ranges)), key=ranges.__getitem__)
    angle = LIDAR_FOV*0.5 - LIDAR_FOV*float(index_min) / float(len(ranges) - 1)
    return angle


def heading_difference(start_point, end_point):
    """Get the angle difference between the robot heading and a line

    The line is defined by a start and an end point.
    """
    rot_robot = np.array(robot.getSelf().getOrientation()).reshape(3, 3)
    robot_heading = np.dot( rot_robot, np.array([1,0,0]) )
    line = (np.array(end_point) - np.array(start_point))
    angle = acos( np.dot(robot_heading, line)
                  / sqrt(np.dot(line, line))
                  / sqrt(np.dot(robot_heading, robot_heading)) )
    cross = np.cross(line, robot_heading)
    if np.dot(cross, np.array([0,1,0])) > 0.0:
        angle = -angle
    return angle


def distance_to(goal_location):
    """Get the current distance to a goal"""
    my_location = get_my_location()
    dist = np.linalg.norm(np.array(goal_location) - np.array(my_location))
    return dist


def distance_to_line(start_point, end_point):
    """Compute robot's perpendicular distance from the robot to a line.

    The line is defined by a start- and an end-point.
    """
    A = np.array(start_point)
    B = np.array(end_point)
    P = np.array(get_my_location())
    numerator = np.cross((A-P),(B-A))
    distance = np.linalg.norm(numerator) / np.linalg.norm(B-A)
    if np.dot(numerator, np.array([0,1,0])) < 0.0:
        distance = -distance
    return distance


def place_marker(robot_translation):
    """Place a marker at the robot's location."""
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
                radius 0.2
            }}
            }}
        ]
    }}
    """.format(
            robot_translation[0], robot_translation[2]
        ),
    )

def place_marker_orange(robot_translation):
    """Place a marker at the robot's location."""
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
                baseColor 1 0.5 0
                metalness 0
            }}
            geometry Sphere {{
                radius 0.2
            }}
            }}
        ]
    }}
    """.format(
            robot_translation[0], robot_translation[2]
        ),
    )

def place_marker_yellow(robot_translation):
    """Place a marker at the robot's location."""
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
                baseColor 1 1 0
                metalness 0
            }}
            geometry Sphere {{
                radius 0.2
            }}
            }}
        ]
    }}
    """.format(
            robot_translation[0], robot_translation[2]
        ),
    )

def place_marker_blue(robot_translation):
    """Place a marker at the robot's location."""
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
                baseColor 0 0 1
                metalness 0
            }}
            geometry Sphere {{
                radius 0.2
            }}
            }}
        ]
    }}
    """.format(
            robot_translation[0], robot_translation[2]
        ),
    )



def place_marker_red(robot_translation):
    """Place a marker at the robot's location."""
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
                baseColor 1 0 0
                metalness 0
            }}
            geometry Sphere {{
                radius 0.2
            }}
            }}
        ]
    }}
    """.format(
            robot_translation[0], robot_translation[2]
        ),
    )

def place_marker_purple(robot_translation):
    """Place a marker at the robot's location."""
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
                baseColor 0.5 0 0.5
                metalness 0
            }}
            geometry Sphere {{
                radius 0.2
            }}
            }}
        ]
    }}
    """.format(
            robot_translation[0], robot_translation[2]
        ),
    )

def rotation_dir(alfa, beta, zeta, px, py, pz, rob_y):
    is_it_cw = False
    result_y = math.sin(math.radians(alfa)) * math.cos(math.radians(beta)) * px + ( math.sin(math.radians(alfa)) * math.sin(math.radians(beta)) * math.sin(math.radians(zeta)) + math.cos(math.radians(alfa)) * math.cos(math.radians(zeta)) ) * py +( math.sin(math.radians(alfa)) * math.sin(math.radians(beta)) * math.cos(math.radians(zeta)) - math.cos(math.radians(alfa)) * math.sin(math.radians(zeta)) ) + pz + rob_y 
    
    if 0 > result_y:
        is_it_cw = True
    return is_it_cw

def rotationCalculation(Cx, Cy):
    possition = get_my_location()
    imu_data = IMU.getRollPitchYaw()
    
    length = 10;  
    y = possition[2] + 10
    print(imu_data)
    angle = imu_data[2]
    
    endy = y + length * math.sin(math.radians(angle))
    endx = length * math.cos(math.radians(angle))
    
    imu_vec_x = possition[0] -endx
    imu_vec_y = possition[2] -endy
    imu2_vec_x = cos(angle)*2
    imu2_vec_y = sin(angle)*2
    
    vec_x = float(Cx) - possition[0]
    vec_y = float(Cy) - possition[2]
    p1 = (vec_x, vec_y)
    p2 = (imu_vec_x, imu_vec_y)
    ang1 = np.arctan2(*p1[::-1])
    ang2 = np.arctan2(*p2[::-1])
    output = np.rad2deg((ang1 - ang2) % (2 * np.pi))
    print('angle', np.rad2deg(angle))
    imu_data = IMU.getRollPitchYaw()
    value = 1
    if 135 < np.rad2deg(angle) < 180 or -135 > np.rad2deg(angle) > -180:
        if output > 180:
            value = -1
    
    if 0 < np.rad2deg(angle) < 45 or 0 > np.rad2deg(angle) > -45:
        if output < 180:   
            value = -1

    if 45 < np.rad2deg(angle) < 90 or 90 < np.rad2deg(angle) < 135:
        if output < 180:
            value = -1

    if -135 < np.rad2deg(angle) < -90 or -90 < np.rad2deg(angle) < -45:
        if output > 180:     
            value = -1

    return  value

def ellipse_func(cx ,cy, x, y, a, b):
    p = (math.pow((x - cx), 2) / math.pow(a, 2)) + (math.pow((y - cy), 2) / math.pow(b, 2));
    return p

def bezierFunc(p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, work):
    bezier_ar = [[p0x, p0y]]
    t = 0.01                      
    t_increase = t
    t_counter = 1
    while t <= 1:
        temp_x = (pow( (1-t),3) * p0x ) + ( 3 * pow((1-t) , 2) * t * p1x ) + ( 3 * (1-t) * pow(t,2) * p2x) + ( pow(t,3) * p3x)
        temp_y = (pow( (1-t),3) * p0y ) + ( 3 * pow((1-t) , 2) * t * p1y ) + ( 3 * (1-t) * pow(t,2) * p2y) + ( pow(t,3) * p3y)
        bezier_ar.insert(t_counter, [temp_x, temp_y])
        t_counter += 1
        t = t+ t_increase

    
    
    new_bezier_array = [[p0x, p0y]]
    p_counter = 1
    new_b_counter = 0
    while p_counter < t_counter:
        vec_x = bezier_ar[p_counter][0] - new_bezier_array[new_b_counter][0]
        vec_y = bezier_ar[p_counter][1] - new_bezier_array[new_b_counter][1]
        vec_lengths = sqrt( pow(vec_x, 2) + pow(vec_y, 2))
        
        if vec_lengths >= 0:         
            new_b_counter += 1
            new_bezier_array.insert(new_b_counter, [bezier_ar[p_counter][0], bezier_ar[p_counter][1]])
        
        p_counter += 1    
    
    total_amount_of_points = new_b_counter
    current_point = 0
    counter = 10000
    while current_point <= total_amount_of_points:
        x = new_bezier_array[current_point][0]
        y = new_bezier_array[current_point][1]
    
        ranges = top_lidar.getRangeImage()
        initial_location = get_my_location()
        leave_point = initial_location
        Pen.setInkColor(pen_paint, 0xFFFFFF, 0.9)
        Pen.write(pen_paint, True)
        
        while ( robot.step(timestep) != -1 and
                distance_to(GOAL_POINT(x,y)) > OBJECT_HIT_DIST*1.5 and
                not any(r < OBJECT_HIT_DIST for r in ranges) ):
            ranges = top_lidar.getRangeImage()
            position = get_my_location()
            x_track_error = distance_to_line(leave_point, GOAL_POINT(x,y))
            heading_error = heading_difference(leave_point, GOAL_POINT(x,y))
            pure_pursuit(x_track_error, heading_error)
            
            counter += 1
            
            if (work == 1) or ( draw_everything == 1):
                if counter > paint_fq_bezier:
                    place_marker_purple(position)
                    counter = 0  
                     
        current_point += 1
        
    return

# ellipse 
def ellipseFunc(cx, cy, x2, y2, rx, ry, work):
    cx = float(cx)
    cy = float(cy)
    x2 = float(x2)
    y2 = float(y2)
    rx = float(rx)
    ry = float(ry) 
    is_cw = rotationCalculation(cx, cy)
    
    if is_cw == 1:
        is_cw = False
    else:
        is_cw = True
    
    px = cx
    py = cy-ry
    i = 1
    var = 2
    point_array = [[i, 0],[px, py]]
    point_found = False
    while i <= rx:
        k = 0
        point_found = False
        while point_found == False:
            var = ellipse_func(cx ,cy, (px+i), (py+k), rx, ry)
            if var <= 1:
                point_array.insert((i+1), [px+i, py+k])
                point_array[0][0] = i
                point_found = True
                
            else:
                k += 0.1         
        i += 1;
    
    point_array.insert((i+1), [px+i, py+k])
    point_array[0][0] = i+1
    quarter_point = point_array[0][0]
    
    j_max = int(point_array[0][0])
    j = 1    
    while j < j_max:
        px_new = point_array[(j_max-j)][0]
        py_new = 2*(cy - point_array[(j_max-j)][1]) + point_array[(j_max-j)][1]
        point_array.insert((j_max+j), [px_new, py_new])
        point_array[0][0] =point_array[0][0] +1
        j += 1
        
    halv_point = point_array[0][0]
    
    q_max = point_array[0][0]
    q = 1
    while q <= q_max:
        px_new = point_array[(q_max-q)][0] - ( 2* ( point_array[(q_max-q)][0] - cx) )
        py_new = point_array[(q_max-q)][1]
        point_array.insert((q_max+q), [px_new, py_new])
        point_array[0][0] =point_array[0][0] +1
        q += 1
    
    
    new_point_array = [[0, 0]]
    if cy == (y2 +ry):
       g = 1
       steps =  point_array[0][ 0]
       while g <= steps -1:
           new_point_array.insert((g), [point_array[g][0], point_array[g][1]])
           g += 1;
        
    elif cx == (x2 - rx):
        g = 0
        h = 1
        steps = (point_array[0][0] - quarter_point)
        while g <= steps:
            new_point_array.insert((g), [point_array[quarter_point+g][0], point_array[quarter_point+g][1]])
            g += 1
        while h <= quarter_point:
            new_point_array.insert((g), [point_array[h][0], point_array[h][1]])
            h += 1     
            g +=1          
       
    elif cy == (y2- ry):
        g = 0
        h = 1
        steps = (point_array[0][0] - halv_point)
        while g < steps:
           new_point_array.insert((g), [point_array[halv_point+g][0], point_array[halv_point+g][1]])
           g += 1
        while h <= halv_point:
           new_point_array.insert((g), [point_array[h][0], point_array[h][1]])
           h += 1
           g += 1            
            
    elif cx == (x2 + rx): 
        g = 0
        h = 1
        steps = (point_array[0][0] - (halv_point+quarter_point))
        while g < steps:
            new_point_array.insert((g), [point_array[halv_point + quarter_point + g][0], point_array[halv_point + quarter_point + g][1]])
            g += 1
        while h <= (quarter_point + halv_point):
            new_point_array.insert((g), [point_array[h][0], point_array[h][1]]) 
            h += 1   
            g +=1                  
    else: 
        print('Error with ellipse input')
        
    f = 1
    
    new_new_array = [[0, 0]]
    new_steps = (point_array[0][0])
    if is_cw == False:
        while f < new_steps:
             new_new_array.insert((f), [(new_point_array[new_steps - f][0]), (new_point_array[new_steps - f][1])])
             f += 1
    else:
        while f < new_steps:
             val_x = new_point_array[f][0]
             val_y = new_point_array[f][1]
             new_new_array.insert((f), [val_x, val_y])
             f += 1

    total_amount_of_points = point_array[0][0]
    current_point = 1
    
    while current_point <= total_amount_of_points:
        x = new_new_array[current_point][0]
        y = new_new_array[current_point][1]
    
        ranges = top_lidar.getRangeImage()
        initial_location = get_my_location()
        leave_point = initial_location
        Pen.setInkColor(pen_paint, 0xFFFFFF, 0.9)
        Pen.write(pen_paint, True)
        counter = 1000
        while ( robot.step(timestep) != -1 and
                distance_to(GOAL_POINT(x,y)) > OBJECT_HIT_DIST*2 and
                not any(r < OBJECT_HIT_DIST for r in ranges) ):
            ranges = top_lidar.getRangeImage()
            position = get_my_location()
            x_track_error = distance_to_line(leave_point, GOAL_POINT(x,y))
            heading_error = heading_difference(leave_point, GOAL_POINT(x,y))
            pure_pursuit(x_track_error, heading_error)
            
            if (work == 1) or ( draw_everything == 1):
                if counter > paint_fq_ellipse:
                   place_marker_red(position)
                   counter = 0  
                     
                counter += 1
            current_point += 2
        
    
    return

def circleMovement(x1, y1, x2, y2, rx, ry, work):
    mid_x = (x1+x2)/2
    mid_y = (y1+y2)/2
    rad_x = rx
    rad_y = ry
    
    v_x = x1 - x2
    v_y = y1 - y2
    mid_px = (v_x*2) + x2
    mid_py = (v_y*2) + y2
        
    imu_data = IMU.getRollPitchYaw()    
    
    angle_rotation = 1.2
    angle_rotation = rotationCalculation(x1, y1)     
    print('angle_rotation: ', angle_rotation)
    dir_control_val = angle_rotation       

    omkreds = 2 * float(rx) * pi
    full_rad = 6.28319
    angle_vel_circle = dir_control_val*(full_rad/omkreds) / 10 / TARGET_FORWARD_VELOCITY
        
    linear_velocity = WHEEL_RADIUS * MAX_WHEEL_VELOCITY
    
    start_time = robot.getTime()
    current_time = robot.getTime()
    
    angle_of_rotation = 6.28 
    distance_between_wheels = 0.090 
    rate_of_rotation = (2 * linear_velocity) / distance_between_wheels
    durationTurning = angle_of_rotation/rate_of_rotation
   
   
    rot_start_time = start_time 
    rot_end_time = rot_start_time + durationTurning
    
    ranges = top_lidar.getRangeImage()
    initial_location = get_my_location()
    leave_point = initial_location
    counter = 1000
    while ( robot.step(timestep) != -1 and
            distance_to(GOAL_POINT(mid_px,mid_py)) > OBJECT_HIT_DIST*2 and
            not any(r < OBJECT_HIT_DIST for r in ranges) ):   
        imu_data = IMU.getRollPitchYaw()
        ranges = top_lidar.getRangeImage()
        position = get_my_location()
        x_track_error = distance_to_line(leave_point, GOAL_POINT(mid_px,mid_py))
        heading_error = heading_difference(leave_point, GOAL_POINT(mid_px,mid_py))
        set_velocities(TARGET_FORWARD_VELOCITY, angle_vel_circle)#
        
        if (work == 1) or ( draw_everything == 1):
            if counter > paint_fq_circle:
                place_marker_blue(position)
                counter = 0  
                  
            counter += 1
            
    counter = 1000
    while ( robot.step(timestep) != -1 and
            distance_to(GOAL_POINT(x2,y2)) > OBJECT_HIT_DIST and
            not any(r < OBJECT_HIT_DIST for r in ranges) ):  
        ranges = top_lidar.getRangeImage()
        position = get_my_location()
        x_track_error = distance_to_line(leave_point, GOAL_POINT(x2,y2))
        heading_error = heading_difference(leave_point, GOAL_POINT(x2,y2))
        set_velocities(TARGET_FORWARD_VELOCITY, angle_vel_circle)#
        
        if (work == 1) or ( draw_everything == 1):
            if counter > paint_fq_circle:
                place_marker_blue(position)
                counter = 0  
                  
            counter += 1
            
    return
    

def driver_loop(x, y, x2, y2, type, rx, ry, prev_x, prev_y, b1x, b1y, b2x, b2y, prev_type, work):
    print('previous values', prev_x, prev_y)

    ranges = top_lidar.getRangeImage()
    initial_location = get_my_location()
    leave_point = initial_location
    Pen.setInkColor(pen_paint, 0xFFFFFF, 0.9)
    Pen.write(pen_paint, True)
    counter = 1000
    print('Goal point 1', GOAL_POINT(x,y))
    print('Goal point 2', GOAL_POINT(x2,y2))
    if type == 0:
        if (prev_x != x) and (prev_y != y):
            while ( robot.step(timestep) != -1 and
                    distance_to(GOAL_POINT(x,y)) > 2 * OBJECT_HIT_DIST and
                    not any(r < OBJECT_HIT_DIST for r in ranges) ) and type == 0:
                ranges = top_lidar.getRangeImage()
                position = get_my_location()
                x_track_error = distance_to_line(leave_point, GOAL_POINT(x,y))
                heading_error = heading_difference(leave_point, GOAL_POINT(x,y))
                pure_pursuit(x_track_error, heading_error)
                
                if (work == 1) or ( draw_everything == 1):
                    if counter > paint_fq_pattern:
                        place_marker_orange(position)
                        counter = 0  
                    
                    counter += 1
        else:
            print('didnt paint')
        
        counter = 1000        
        ranges = top_lidar.getRangeImage()
        initial_location = get_my_location()
        leave_point = initial_location  
        while ( robot.step(timestep) != -1 and
                distance_to(GOAL_POINT(x2,y2)) > OBJECT_HIT_DIST * 2 and
                not any(r < OBJECT_HIT_DIST for r in ranges) ) and type == 0:
            ranges = top_lidar.getRangeImage()
            position = get_my_location()
            x_track_error = distance_to_line(leave_point, GOAL_POINT(x2,y2))
            heading_error = heading_difference(leave_point, GOAL_POINT(x2,y2))
            pure_pursuit(x_track_error, heading_error)
            if (work == 1) or ( draw_everything == 1):
                if counter > paint_fq_line:
                    if (work == 1):
                        place_marker(position)
                    else:
                        place_marker_orange(position)
                    counter = 0                  
            counter += 1
        
 
    if (prev_x != x) and (prev_y != y):
        counter = 1000
        while ( robot.step(timestep) != -1 and
                distance_to(GOAL_POINT(x2,y2)) > OBJECT_HIT_DIST and
                not any(r < OBJECT_HIT_DIST for r in ranges) ) and type == 1:
            ranges = top_lidar.getRangeImage()
            position = get_my_location()
            x_track_error = distance_to_line(leave_point, GOAL_POINT(x2,y2))
            heading_error = heading_difference(leave_point, GOAL_POINT(x2,y2))
            pure_pursuit(x_track_error, heading_error)
            
            if (work == 1) or ( draw_everything == 1):
                if counter > paint_fq_travel:
                    place_marker_yellow(position)
                    counter = 0  
                      
            counter += 1
                
        while ( robot.step(timestep) != -1 and
                distance_to(GOAL_POINT(x2,y2)) > OBJECT_HIT_DIST and
                not any(r < OBJECT_HIT_DIST for r in ranges) ) and type == 2:
            ranges = top_lidar.getRangeImage()
            position = get_my_location()
            x_track_error = distance_to_line(leave_point, GOAL_POINT(x2,y2))
            heading_error = heading_difference(leave_point, GOAL_POINT(x2,y2))
            pure_pursuit(x_track_error, heading_error)

        if (work == 1) or ( draw_everything == 1):            
            if counter > paint_fq_travel:
                place_marker_yellow(get_my_location())
                counter = 0  
          
        counter += 1 
        
        while ( robot.step(timestep) != -1 and
                distance_to(GOAL_POINT(x,y)) > OBJECT_HIT_DIST and
                not any(r < OBJECT_HIT_DIST for r in ranges) ) and type == 4:
            ranges = top_lidar.getRangeImage()
            position = get_my_location()
            x_track_error = distance_to_line(leave_point, GOAL_POINT(x,y))
            heading_error = heading_difference(leave_point, GOAL_POINT(x,y))
            pure_pursuit(x_track_error, heading_error)
            
        if (work == 1) or ( draw_everything == 1):
            if counter > paint_fq_travel:
                place_marker_yellow(get_my_location())
                counter = 0  
                  
        counter += 1
    
    else:
        print('didnt paint')
            
    if distance_to(GOAL_POINT(x2,y2)) < OBJECT_HIT_DIST and type == 0:
        
        if (work == 1) or ( draw_everything == 1):
            position = get_my_location()
            place_marker(position)
    if  type == 1:
        print('Point reached now make circle')
        circleMovement(x, y, x2, y2, rx, ry, work) 
        
    if distance_to(GOAL_POINT(x2,y2)) < OBJECT_HIT_DIST and type == 2:
        ellipseFunc(float(x), float(y), float(x2), float(y2), float(rx), float(ry), work) 
    if type == 4:
        print('Point reached now make bezier')
        bezierFunc(float(x), float(y), float(b1x), float(b1y), float(b2x), float(b2y), float(x2), float(y2), work)    
        
    return

    

def mainDrive():

    robot.step(500)

    initial_location = get_my_location()
    leave_point = initial_location
    
    i = 0
    while True:
        ranges = top_lidar.getRangeImage()
        position = get_my_location()
        
        #df = pd.read_csv('finalfinal_logo_tt.csv')
        df = pd.read_csv('Final_soccer_test.csv')
        #df = pd.read_csv('phase2_football_field_new2.csv')
        #df = pd.read_csv('phase2_cubic_bezier_curve_new.csv')
        #df = pd.read_csv('phase2_football_field.csv')
        #df = pd.read_csv('phase2_polygone.csv')
        #df = pd.read_csv('phase2_polyline.csv')    
       
        #df = pd.read_csv('phase2_vertical.csv')
        #df = pd.read_csv('phase2_horizontal.csv')
        
        #df = pd.read_csv('testing2.csv')
        #df = pd.read_csv('circle_simu_test.csv')
        #df = pd.read_csv('ellipse_simu_test.csv')
        #df = pd.read_csv('FootballP1.csv')
        #df = pd.read_csv('foodball_big.csv')
        #df = pd.read_csv('fieldTest.csv')
        #df = pd.read_csv('simple_square.csv')
        #df = pd.read_csv('nicer.csv')
        #df = pd.read_csv('simple_fill.csv')
        #df = pd.read_csv('fill_3.csv')
        #df = pd.read_csv('fill_4.csv')
        #df = pd.read_csv('fill_rect.csv')
        #df = pd.read_csv('fill_circle.csv')
        #df = pd.read_csv('fill_ellipse_3.csv')
        #df = pd.read_csv('filled_ellipse_final_with_pattern.csv')
        #df = pd.read_csv('filled_circle_mod.csv')
        #df = pd.read_csv('line_phase1_1.csv')
        #df = pd.read_csv('fille_ellipse_final_hope.csv')
        #df = pd.read_csv('filled_ellipse_best.csv')
        #df = pd.read_csv('foodball_phase1_noPattern.csv')
        #df = pd.read_csv('simple_ellipse.csv')
        #df = pd.read_csv('circle_line_simulation.csv')        
        
        print('Total line segments:', len(df))
        while i < len(df) and robot.step(timestep) != -1:   
            print('Segment number:', i)  
            
            x = df.iloc[i,3]
            y = df.iloc[i, 4]
            x2 = df.iloc[i, 5]
            y2 = df.iloc[i, 6]
            type = df.iloc[i,2]
            rx = df.iloc[i,7]
            ry = df.iloc[i,8]
            b1x = df.iloc[i,9]
            b1y = df.iloc[i,10]
            b2x = df.iloc[i,11]
            b2y = df.iloc[i,12]
            work = df.iloc[i,14]
            
            ration = 1               # Ratio of the field || default = 1
            try:
                x  = float(x)/ration
                y  = float(y)/ration
                x2  = float(x2)/ration
                y2  = float(y2)/ration
                if type == 1 or type == 2:
                    rx  = float(rx)/ration
                    ry  = float(ry)/ration
                if type == 4:
                    b1x = float(b1x)/ration
                    b1y = float(b1y)/ration
                    b2x = float(b2x)/ration
                    b2y = float(b2y)/ration
                
                print('x, y, x2, y2, type, rx, ry, b1x, b1y, b2x, b2y')
                print(x, y, x2, y2, type, rx, ry, b1x, b1y, b2x, b2y)
                
            except:
                print("EXCEPTION: Found a nan values.")
                
            Point = GOAL_POINT(x,y)            
            
            prev_x = 0
            prev_y = 0
            prev_type = df.iloc[(i - 1),2]
            if (i > 0 and prev_type == 0):
                prev_x = df.iloc[ (i -1) ,3]
                prev_y = df.iloc[ (i -1) ,4]
            elif (i > 0 and prev_type == 1):
                prev_x = df.iloc[ (i -1) ,5]
                prev_y = df.iloc[ (i -1) ,6]
           
            print('prev_x', prev_x)    
            print('prev_y', prev_y)    
            driver_loop(x, y, x2, y2, type, rx, ry, prev_x, prev_y, b1x, b1y, b2x, b2y, prev_type, work)     
            
            i += 1
             
def main():
    while robot.step(timestep) != -1:
        mainDrive()
        
if __name__ == "__main__":
  
    main()

