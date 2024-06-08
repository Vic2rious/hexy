import time
import math
import keyboard
from servo import Servo, servo2040
from servo import ServoCluster, servo2040

# Select all pins
pins = list(range(servo2040.SERVO_1, servo2040.SERVO_18 + 1))

# Create the servo cluster
cluster = ServoCluster(0, 0, pins)

# Leg and joint counts
legCount = 6
jointCount = 3

# Define Joint class
class Joint:
    def __init__(self, servoNum, offset):
        self.servoNum = servoNum
        self.offset = offset
        self.angle = offset
    
    def enable(self):
        cluster.enable(self.servoNum)
        cluster.value(self.servoNum, self.offset)
        
    def set_angle(self, angle):
        cluster.value(self.servoNum, angle + self.offset)
        self.angle = angle + self.offset
    
    def disable(self):
        cluster.disable(self.servoNum)

# Leg part lengths in mm
coxaLength = 51
femurLength = 65
tibiaLength = 121

# Servo offsets
coxaOffset = 10
femurOffset = 20
tibiaOffset = 20

# Define joints for each leg
joints = [
    [Joint(servo2040.SERVO_1, coxaOffset + 40), Joint(servo2040.SERVO_2, femurOffset + 40), Joint(servo2040.SERVO_3, tibiaOffset + 0 )],
    [Joint(servo2040.SERVO_4, coxaOffset + 0 ), Joint(servo2040.SERVO_5, femurOffset - 30), Joint(servo2040.SERVO_6, tibiaOffset + 40)],
    [Joint(servo2040.SERVO_7, coxaOffset + 60), Joint(servo2040.SERVO_8, femurOffset + 10), Joint(servo2040.SERVO_9, tibiaOffset - 30)],
    [Joint(servo2040.SERVO_10, coxaOffset + 30), Joint(servo2040.SERVO_11, femurOffset + 20), Joint(servo2040.SERVO_12, tibiaOffset - 30)],
    [Joint(servo2040.SERVO_13, coxaOffset + 20), Joint(servo2040.SERVO_14, femurOffset + 0 ), Joint(servo2040.SERVO_15, tibiaOffset - 10)],
    [Joint(servo2040.SERVO_16, coxaOffset + 30), Joint(servo2040.SERVO_17, femurOffset - 30), Joint(servo2040.SERVO_18, tibiaOffset - 20)]
]

def disableAll():
    # Disable all joints
    for leg in joints:
        for joint in leg:
            joint.disable()

def reset_leg_positions(seconds):
    """Set default angles for all joints."""
    for leg in joints:
        leg[0].enable()
        leg[1].enable()
        leg[2].enable()
        time.sleep(seconds)
#     print("leg_reset complete") 

def get_up(forward, height, order_first):
    """Move the legs to an elevated position for walking."""
    def get_135():
        for i in range(0, legCount, 2):
            joints[i][1].set_angle(-height if forward else 0)
            
    def get_246():
        for i in range(0, legCount, 2):
            joints[i + 1][1].set_angle(0 if forward else -height)
    
    if order_first:
        get_135()    
        time.sleep(0.3)
        get_246()
    else:
        get_246()    
        time.sleep(0.3)
        get_135()

def forward_cycle(speed, height, forward, seconds):
    """Perform a forward walking cycle."""
    while not keyboard.is_pressed('q'):
        get_up(forward, height, not forward)
        time.sleep(seconds)
        for i in range(0, legCount, 2):
            direction = -1 if i > 2 else 1
            joints[i][0].set_angle(-speed * direction)
            direction = -1 if i + 1 > 2 else 1
            joints[i + 1][0].set_angle(speed * direction)
        time.sleep(seconds)

        forward = not forward
        get_up(forward, height, not forward)
        time.sleep(seconds)

        for i in range(0, legCount, 2):
            direction = -1 if i > 2 else 1
            joints[i][0].set_angle(speed * direction)
            direction = -1 if i + 1 > 2 else 1
            joints[i + 1][0].set_angle(-speed * direction)
        time.sleep(seconds)

def spin_cycle(speed, height, forward, seconds):
    """Perform a spin cycle."""
    get_up(forward, 0, forward)
    get_up(forward, height, forward)
    time.sleep(seconds)
    for i in range(0, legCount, 2):
        
        joints[i][0].set_angle(-speed if forward else speed)
        
        joints[i + 1][0].set_angle(speed if forward else -speed)
    time.sleep(seconds)

    
    get_up(not forward, height, forward)
    time.sleep(seconds)

    for i in range(0, legCount, 2):
        
        joints[i][0].set_angle(speed if forward else -speed)
       
        joints[i + 1][0].set_angle(-speed if forward else speed)
    time.sleep(seconds)
    get_up(not forward, 0, not forward)

def dance(speed, height):
    """Perform a dancing motion cycle (robot should probably be in the air)."""
    angle_range = 30  # Define how much the leg should move during spin
    for leg in range(legCount):
        joints[leg][0].enable()
        joints[leg][1].enable()
        joints[leg][2].enable()

    for step in range(angle_range):
        for leg in range(legCount):
            joints[leg][0].set_angle(math.sin(math.radians(step * speed)) * height)
            joints[leg][1].set_angle(math.cos(math.radians(step * speed)) * height)
            joints[leg][2].set_angle(math.sin(math.radians(step * speed)) * height)
            time.sleep(0.05)

    for step in range(angle_range, 0, -1):
        for leg in range(legCount):
            joints[leg][0].set_angle(math.sin(math.radians(step * speed)) * height)
            joints[leg][1].set_angle(math.cos(math.radians(step * speed)) * height)
            joints[leg][2].set_angle(math.sin(math.radians(step * speed)) * height)
            time.sleep(0.05)
    
# height for walking and spinning
height = -30
low_bound = 10
high_bound = 25

def main():
    reset_leg_positions(0.5)
    print("Leg positions reset.")
    try:
        while True:
            yPos = int(input("Enter speed and direction (- for reverse, 0 to stop): "))
            if yPos == 99:
                speed = int(input("Enter speed for spinning: "))
                speed = max(-high_bound, min(-low_bound, speed)) if speed < low_bound else max(low_bound, min(high_bound, speed))
                spin_cycle(speed, height, speed > 0, 0.5)
            elif yPos != 0:
                speed = abs(yPos)
                speed = max(low_bound, min(high_bound, speed))
                forward_cycle(speed, height, yPos > 0, 0.5)
                reset_leg_positions(0.3)
            else:
                print("Stopping movement and resetting legs.")
                reset_leg_positions(0.3)
            time.sleep(0.1)
    except KeyboardInterrupt:
        print("Program stopped by user.")
    finally:
        disableAll()

if __name__ == "__main__":
    main()
    
# reset_leg_positions(0.5)

# get_up(True, -40)
# time.sleep(2)
# disableAll()
