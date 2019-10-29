#include "RoboteqInterface.h"
#include <string.h> 
#include <iostream>
using namespace boost;

//-- public methods --//
RoboteqInterface::RoboteqInterface(std::string portName)
        :port(io), timeout(io), reader(port,10)
{
        // open port
        port.open(portName);
        port.set_option(asio::serial_port_base::baud_rate(115200));
}

RoboteqInterface::~RoboteqInterface() { port.close(); }

void RoboteqInterface::cmdOneChannel(float value, int channel)
{
        char buffer[11], c;
        int i = 0;

        std::string cmd = "!G " + std::to_string(channel) + " " + std::to_string(value) + "\r\n";
	// std::cout << cmd;

        asio::write(port,asio::buffer(cmd.c_str(),cmd.length()));
        
        bool equalFound = false;

        while(reader.read_char(c)) 
        {
                printf("%c",c);
                if(equalFound && c != '\n') {
                        if(i>=11) { return; }
                        buffer[i] = c;
                        i++;
                        //printf("%c",c);
                }
                if(c == '=') { equalFound = true; }
                if(c == '!') { equalFound = false; }
        }
}

void RoboteqInterface::cmdChannels(float ch1, float ch2)
{
        char buffer[11], c;
        int i = 0;

        std::string cmd1 = "!G 1 " + std::to_string(ch1) + "\r\n";
        std::string cmd2 = "!G 2 " + std::to_string(ch2) + "\r\n";

	std::cout << cmd1;

        asio::write(port,asio::buffer(cmd1.c_str(),cmd1.length()));
        asio::write(port,asio::buffer(cmd2.c_str(),cmd2.length()));
        
        bool equalFound = false;

        while(reader.read_char(c)) 
        {
                printf("%c",c);
                if(equalFound && c != '\n') {
                        if(i>=11) { return; }
                        buffer[i] = c;
                        i++;
                        //printf("%c",c);
                }
                if(c == '=') { equalFound = true; }
                if(c == '!') { equalFound = false; }
        }
}

void RoboteqInterface::setPPR(int PPR)
{
        char buffer[4], c;

        std::string cmd1 = "^EPPR 1 " + std::to_string(PPR) + "\r\n";
        std::string cmd2 = "^EPPR 2 " + std::to_string(PPR) + "\r\n";

        asio::write(port,asio::buffer(cmd1.c_str(),cmd1.length()));
        asio::write(port,asio::buffer(cmd2.c_str(),cmd2.length()));
        bool equalFound = false;
        int i = 0;

        while(reader.read_char(c)) 
        {
                //printf("%c",c);
                if(equalFound && c != '\n') {
                        if(i>=11) { return; }
                        buffer[i] = c;
                        i++;
                        //printf("%c",c);
                }
                if(c == '=') { equalFound = true; }
                if(c == '!') { equalFound = false; }
        }
}

bool RoboteqInterface::readEncoderCh1(long* enc1)
{
        bool readSuccess = true;
        char cmd[] = "?CSS 1\r\n";

        asio::write(port,asio::buffer(cmd,sizeof(cmd)));
        readSuccess = readEncoder(enc1);
        return readSuccess;
}

bool RoboteqInterface::readEncoderCh2(long* enc2)
{
        bool readSuccess = true;
        char cmd[] = "?CSS 2\r\n";
        asio::write(port,asio::buffer(cmd,sizeof(cmd)));
        readSuccess = readEncoder(enc2);
        return readSuccess;
}

void RoboteqInterface::setParams()
{
	char ch1_encoder_mode[] = "^EMOD 1 18";
	char ch2_encoder_mode[] = "^EMOD 2 34";

        asio::write(port,asio::buffer(ch1_encoder_mode,sizeof(ch1_encoder_mode)));
        asio::write(port,asio::buffer(ch2_encoder_mode,sizeof(ch2_encoder_mode)));
        usleep(100);

        char ch1_close[] = "^MMOD 1 1";
        char ch2_close[] = "^MMOD 2 1";
        
        asio::write(port,asio::buffer(ch1_close,sizeof(ch1_close)));
        asio::write(port,asio::buffer(ch2_close,sizeof(ch2_close)));
        
}

void RoboteqInterface::resetEncoders()
{
	char ch1_open[] = "^MMOD 1 0";
	char ch2_open[] = "^MMOD 2 0";
	char ch1_close[] = "^MMOD 1 1";
        char ch2_close[] = "^MMOD 2 1";
        char cmd1[] = "!C 1 0\r\n";
        char cmd2[] = "!C 2 0\r\n";
        asio::write(port,asio::buffer(ch1_open,sizeof(ch1_open)));
        asio::write(port,asio::buffer(ch2_open,sizeof(ch2_open)));
                usleep(100);
        asio::write(port,asio::buffer(cmd1,sizeof(cmd1)));
        asio::write(port,asio::buffer(cmd2,sizeof(cmd2)));
		usleep(100);
        asio::write(port,asio::buffer(ch1_close,sizeof(ch1_close)));
        asio::write(port,asio::buffer(ch2_close,sizeof(ch2_close)));
}

bool RoboteqInterface::readBattery(float* batt)
{
		char buffer[4], c;
		char cmd[] = "?V 2\r\n";
		int i = 0;
		bool equalFound = false;
		float value = 0;
		
		asio::write(port,asio::buffer(cmd,sizeof(cmd)));
		
		//printf("Reading: ");
		while(reader.read_char(c))
		{
			//printf("%c",c);
			if(equalFound && c != '\n')
			{
				if(i>=11) { return false; }
				buffer[i] = c;
				i++;
			}
			if(c == '=') { equalFound = true; }
			if(c == '!') { equalFound = false; }
		}
		//printf("\n");
		*batt = atof(buffer)/10.0F;
		return true;
}

//-- private methods --//
bool RoboteqInterface::readEncoder(long* encVal)
{
        char buffer1[11], c;
        int i = 0;
        bool equalFound = false;

        //printf("Reading: ");
        while(reader.read_char(c)) 
        {
                //printf("%c",c);
                if(equalFound && c != '\n') {
                        if(i>=11) { return false; }
                        buffer1[i] = c;
                        i++;
                        //printf("%c",c);
                }
                if(c == '=') { equalFound = true; }
                if(c == '!') { equalFound = false; }
        }
        //printf("\n");

        *encVal = atol(buffer1);
        return true;
}

// adds length of input int to cmd length(7)
int RoboteqInterface::getCmdLength(int motorSpeed)
{
        motorSpeed = getAbs(motorSpeed);
        if(motorSpeed < 10) { return 8; }
        else if(motorSpeed < 100) { return 9; }
        else if(motorSpeed < 1000) { return 10; }
        else { return 11; }
}

int RoboteqInterface::getAbs(int input)
{
        if(input < 0) { return -1*input; }
        return input;
}
