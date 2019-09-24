#include <iostream>
#include "ros/ros.h"
#include <sstream>

#include "roboteq_msgs/Command.h"
#include "roboteq_msgs/Feedback.h"
#include "std_msgs/Float32.h"

class MotorController {
public:
    double cur_pos;
    MotorController() {
        cmd_sub_ = n_.subscribe("command_test", 1, &MotorController::cmd_callback, this);
        fb_sub_ = n_.subscribe("/roboteq_driver/feedback", 1, &MotorController::fb_callback, this);
        pub_ = n_.advertise<roboteq_msgs::Command>("cmd_1",1);
    }
    
    void cmd_callback(const std_msgs::Float32& cmd_in) {
        roboteq_msgs::Command command;
        command.mode = command.MODE_POSITION;

        // "cur_pos" and "cmd_in.data" have big difference
        int dir = ( cmd_in.data > cur_pos ) ? 1 : -1;
        int step = 1;
        double temp = cur_pos;
        while ( abs(temp - cmd_in.data) > step ) {
            temp += dir * step;
            command.setpoint = temp / 360. * 4096. / 20.;
            pub_.publish( command );
            loop_rate.sleep();
        }
        command.setpoint = cmd_in.data / 360. * 4096. / 20.;
        pub_.publish( command );
    }

    void fb_callback(const roboteq_msgs::Feedback& feedback) {
        cur_pos = feedback.measured_position;
        std::stringstream ss;
        ss << "current position " << cur_pos << " degrees.";
        ROS_INFO("%s", ss.str().c_str());
    }
    ros::NodeHandle n_;
    ros::Subscriber cmd_sub_;
    ros::Subscriber fb_sub_;
    ros::Publisher pub_;
    ros::Rate loop_rate = ros::Rate(1000);
};

int main (int argc, char** argv) {
  	ros::init (argc, argv, "motor_controller");	
    MotorController MCObject;
    ros::spin();
    return 0; 
}