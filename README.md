# motor_control_ROS
Controll BLDC motor using RoboTEQ controller in ROS

## Usage
"motor_controller" is a package which sends commands to the roboteq driver nodes.

"roboteq/roboteq_driver" is a modified package of the driver of the roboteq controller.

```bash
$ cd boshen_ws/src # go to the "src" folder of your work space
$ git clone https://github.com/boshenniu/motor_control_ROS.git
$ cd ..
$ catkin_make
$ source devel/setup.bash
$ roslaunch roboteq_driver example.launch port:=/dev/ttyACM0
$ roslaunch motor_controller motor_controller.launch

```

## Serial Command
In "serial_command/RoboteqInterface.cpp", there are libraries and functions which can send serial command (e.g. "!S 1 20") to controller through ROS. These libraries are not used now.

## To Be Done (Oct 10)
Note: start by validate that the "position control" mode can work. Turn on the controller, validate that both channels are "close loop position" mode (when power cycle, the mode might be randomly changed. However, this doesn't appear recently). Go to "run" section in RoboRun, drag the bar to change the motor position. If not work: 1) "stop" and manually rotate the encoders to "0" and restart; 2) go to "diagnosis" and click "motor and sensor set up" for both channel. After that, you should be able to spin the motor in position mode.
### Unstable issue while motor is spinning
"Short" and losing control when change the desired position. Two causes: 1) physical connection (mitigated by reconnect??); 2) handle large difference (e.g. change from 0 to 1000). 

1. Try different position control mode (e.g. "close loop position relative", "close loop position tracking", "close loop position count". All these three modes have individual sections in the User Manual). (on the GUI)
2. Tune the PID gains. (on the GUI)

### Two motor channel integration to ROS
The command for position control for the two motors are "!G 1 xxx" and "!G 2 xxx". Our purpose is to send the same thing to serial using ROS

1. Existing code: very messy code cloned from a online repo. I debugged and changed to make it send correct command to one channel. When both motors are connected, it doesn't work. I believe this is a software and am debugging now, though the code is too confusing.
2. Functions from Grant. These include functions for sending serial commands, getting encoder feedback. Now I have some environment issue when compiling them.

## Update (Oct 15)
The "short" is caused by current surge generated when inputting new command. "!G 1 100" will generated ~10A.
1. Reduce AMP limit.
2. Reduce P gain to ~0.1.
3. Try "close loop position relative" and "close loop position count".
