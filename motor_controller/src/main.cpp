#include <iostream>
#include "ros/ros.h"
#include <sstream>

#include "roboteq_msgs/Command.h"

int main (int argc, char** argv) {
  	ros::init (argc, argv, "motor_controller");
  	ros::NodeHandle nh;

    ros::Publisher desired_motor_position_pub = nh.advertise<roboteq_msgs::Command>("cmd_1",1000);

    ros::Rate loop_rate(1); // 1 fps
    
    int count = 0;
    while ( ros::ok() ) {
        std::stringstream ss;
        ss << "hello world " << count;

        ROS_INFO("%s", ss.str().c_str());

        roboteq_msgs::Command command;
        command.mode = command.MODE_POSITION;
        command.setpoint = (count % 2 == 0) ? 20 : -20;
        // command.setpoint = 20;

        // command.setpoint = count * 20;

        desired_motor_position_pub.publish( command );
        
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }

    return 0; 

}