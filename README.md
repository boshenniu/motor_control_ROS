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
