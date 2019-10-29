#include <iostream>
#include "ros/ros.h"
#include <sstream>

// #include "roboteq_msgs/Command.h"
// #include "roboteq_msgs/Feedback.h"
#include "std_msgs/Float32.h"

#include "RoboteqInterface.cpp"

// class MotorController {
// public:
//     double cur_pos;
//     MotorController() {
//         cmd_sub_ = n_.subscribe("command_test", 1, &MotorController::cmd_callback, this);
//         fb_sub_ = n_.subscribe("/roboteq_driver/feedback", 1, &MotorController::fb_callback, this);
//         pub_ = n_.advertise<roboteq_msgs::Command>("cmd_1",1);
//     }
    
//     void cmd_callback(const std_msgs::Float32& cmd_in) {
//         roboteq_msgs::Command command;
//         command.mode = command.MODE_POSITION;

//         // "cur_pos" and "cmd_in.data" have big difference
//         int dir = ( cmd_in.data > cur_pos ) ? 1 : -1;
//         int step = 1;
//         double temp = cur_pos;
//         while ( abs(temp - cmd_in.data) > step ) {
//             temp += dir * step;
//             command.setpoint = temp / 360. * 4096. / 20.;
//             pub_.publish( command );
//             loop_rate.sleep();
//         }
//         command.setpoint = cmd_in.data / 360. * 4096. / 20.;
//         pub_.publish( command );
//     }

//     void fb_callback(const roboteq_msgs::Feedback& feedback) {
//         cur_pos = feedback.measured_position;
//         std::stringstream ss;
//         ss << "current position " << cur_pos << " degrees.";
//         ROS_INFO("%s", ss.str().c_str());
//     }
//     ros::NodeHandle n_;
//     ros::Subscriber cmd_sub_;
//     ros::Subscriber fb_sub_;
//     ros::Publisher pub_;
//     ros::Rate loop_rate = ros::Rate(1000);
// };


class MotorController {
public:
    ros::NodeHandle n_;
    ros::Subscriber cmd_sub_1;
    ros::Subscriber cmd_sub_2;
    RoboteqInterface interface;
    long enc_read_1, enc_read_2;
    int reverse1, reverse2;
    MotorController(std::string controllerName, std::string portName, int reverse1_, int reverse2_) 
        : interface(portName)
    {
        cmd_sub_1 = n_.subscribe("cmd_"+controllerName+"_aa", 1, &MotorController::cmd_callback_1, this);
        cmd_sub_2 = n_.subscribe("cmd_"+controllerName+"_fe", 1, &MotorController::cmd_callback_2, this);
        reverse1 = reverse1_;
        reverse2 = reverse2_;
    }
    void cmd_callback_1(const std_msgs::Float32& cmd_in) {
        double cur_pos = enc_read_1/4096.*360.;
        double des_pos = cmd_in.data * reverse1 * 5;
        // std::cout << "Boshen start debugging --------------------" << std::endl;
        if ( abs(cur_pos - des_pos) >= 20 ) {
            int dir = ( des_pos > cur_pos) ? 1:-1;
            int step = 5;
            double temp = cur_pos;
            while ( abs(temp - des_pos) > step ) {
                temp += dir * step;
                // interface.cmdOneChannel(temp/360.*4096/20., 1);
                // std::cout << temp << std::endl;
            }
            interface.cmdOneChannel(des_pos/360.*4096/20., 1);
            // std::cout << cmd_in.data << std::endl;
        }
    }
    void cmd_callback_2(const std_msgs::Float32& cmd_in) {
        double cur_pos = enc_read_2/4096.*360.;
        double des_pos = cmd_in.data * reverse2 * 25;
        if ( abs(cur_pos - des_pos) >= 20 ) {
            int dir = ( des_pos > cur_pos) ? 1:-1;
            int step = 50;
            double temp = cur_pos;
            while ( abs(temp - des_pos) > step ) {
                temp += dir * step;
                interface.cmdOneChannel(temp/360.*4096/20., 2);
            }
            interface.cmdOneChannel(des_pos/360.*4096/20., 2);
        }
    }
    
};

int main (int argc, char** argv) {
  	ros::init (argc, argv, "motor_controller");	
    MotorController MCObject_left("left", "/dev/ttyACM0", 1, 1);
    MotorController MCObject_right("right", "/dev/ttyACM1", 1, 1);

    // ros::Rate loop_rate(100);
    while (ros::ok()) {
        ros::spinOnce();
        MCObject_left.interface.readEncoderCh1( & MCObject_left.enc_read_1);
        MCObject_left.interface.readEncoderCh2( & MCObject_left.enc_read_2);
        MCObject_right.interface.readEncoderCh1( & MCObject_right.enc_read_1);
        MCObject_right.interface.readEncoderCh2( & MCObject_right.enc_read_2);

        std::cout << "-------------start-------------" << std::endl;
        std::cout << "Left Encoder Reading 1: " << MCObject_left.enc_read_1 / 4096. * 360. << std::endl;
        std::cout << "Left Encoder Reading 2: " << MCObject_left.enc_read_2 / 4096. * 360. << std::endl;
        std::cout << "-------------end-------------" << std::endl;
        // loop_rate.sleep();
    }
    return 0; 
}