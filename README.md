# motor_control_ROS
Controll BLDC motor using RoboTEQ controller in ROS

## Usage
The folder "boshen_ws" is a ROS work space containing roboteq driver package, and a self-defined package which sends commands to the roboteq nodes.
```bash
$ cd boshen_ws
$ catkin_make
$ source devel/setup.bash
$ roslaunch roboteq_driver example.launch port:=/dev/ttyACM0
$ roslaunch motor_controller motor_controller.launch

```

## Serial Command
In "serial_command/RoboteqInterface.cpp", there are libraries and functions which can send serial command (e.g. "!S 1 20") to controller through ROS.
