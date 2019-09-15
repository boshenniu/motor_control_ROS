#ifndef ROBOTEQ_H
#define ROBOTEQ_H

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <stdlib.h>
#include <sdr_control/blocking_reader.h>
#include <string.h>

class RoboteqInterface {
        public:
                RoboteqInterface(std::string portName);
                ~RoboteqInterface();

                void cmdChannels(float ch1, float ch2);
                bool readEncoderCh1(long* enc1);
                bool readEncoderCh2(long* enc2);
                void resetEncoders();
                bool readBattery(float* batt);
                void setPPR(int PPR);
		void setParams();
                
                // boost asio serial stuff
                boost::asio::io_service io;
                boost::asio::serial_port port;
                boost::asio::deadline_timer timeout;

                // serial reading
                blocking_reader reader;
        private:
                int getCmdLength(int motorSpeed);
                int getAbs(int input);
                bool readEncoder(long* encVal);
};

#endif

