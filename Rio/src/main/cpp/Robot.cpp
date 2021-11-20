// Lots of FRC/Wpi crap to dredge through, but this is the 21st century and when we see a smelly swamp we build things on it.
/*





Ah, a nice copyright-free walkway! Careful not to fall in the bog.
________________________________________________________________________________________
*/

#include <frc/DriverStation.h>
#include <frc/livewindow/LiveWindow.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <hal/DriverStation.h>
#include <networktables/NetworkTable.h>
#include <ctre/Phoenix.h>
#include "rev/CANSparkMax.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <frc/Joystick.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <atomic>
#include <cstring>
#include <thread>
#include <fcntl.h>


#include "ModularRobot.hpp"
//#include "c_str_man.hpp"


class Robot : public ModularRobot{
private:
    std::atomic<bool> m_exit{false}; // Multithreaded variable. This is why the code doesn't die!
    TalonSRX right1{2};
    TalonSRX right2{3};
    TalonSRX left1{4};
    TalonSRX left2{1};
    frc::Joystick Controls{5};
    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;
    long right1Pos = 0;
    long right2Pos = 0;
    long left1Pos = 0;
    long left2Pos = 0;

public:
    Robot(){
        HAL_SendConsoleLine("____________________________INIT ROBOT CODE____________________________");
        HAL_SendConsoleLine("----------- Dashes Are Not More Appealing -----------");
        setData("Socialbot", "Firestorm Robotics", 6341);
    }

    void Init(){
        /*listenfd = socket(AF_INET, SOCK_STREAM, 0);
        memset(&serv_addr, '0', sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(5801); // Just gonna be quick and dumb with this one.
        bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        listen(listenfd, 10);
        connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
        fcntl(connfd, F_SETFL, O_NONBLOCK);*/
        right1Pos = right1.GetSelectedSensorPosition();
        right2Pos = right2.GetSelectedSensorPosition();
        left1Pos = left1.GetSelectedSensorPosition();
        left2Pos = left2.GetSelectedSensorPosition();
    }

    void BeginTalonPID(TalonSRX *_talon, long position, uint8_t slot = 0, uint8_t timeout = 30, float maxSpeed = 0.5){
        _talon->SetSelectedSensorPosition(position, slot, timeout);

        /* choose the sensor and sensor direction */
        _talon->ConfigSelectedFeedbackSensor(
                FeedbackDevice::CTRE_MagEncoder_Relative, slot, timeout);
        _talon->SetSensorPhase(true);

        /* set the peak and nominal outputs, 12V means full */
        _talon->ConfigNominalOutputForward(0, 30);
        _talon->ConfigNominalOutputReverse(0, 30);
        _talon->ConfigPeakOutputForward(maxSpeed, 30);
        _talon->ConfigPeakOutputReverse(-maxSpeed, 30);

        /* set closed loop gains in slot0 */
        _talon->Config_kF(slot, 0.0, timeout);
        _talon->Config_kP(slot, 0.1, timeout);
        _talon->Config_kI(slot, 0.0, timeout);
        _talon->Config_kD(slot, 0.0, timeout);
    }

    void BeginTeleop(){
        BeginTalonPID(&right1, right1Pos);
        BeginTalonPID(&left1, left1Pos);
        BeginTalonPID(&right2, right2Pos);
        BeginTalonPID(&left2, left2Pos);
    }

    void moveStandard(uint32_t change){
        int32_t signedVersion = change;// - 2147483648; // Make it signed so the robot can go backwards.
        right1Pos += signedVersion;
        right2Pos += signedVersion;
        left1Pos += signedVersion;
        left2Pos += signedVersion;
    }

    void moveMecanum(uint32_t change){
        int32_t signedVersion = change - 2147483648; // Make it signed so the robot can go backwards.
        right1Pos += change;
        right2Pos -= change;
        left1Pos -= change;
        left2Pos += change;
    }

    void TeleopLoop(){
        char* buffer = "000000000";
        floattostring(right1Pos.GetSelectedSensorPosition());
        HAL_SendConsoleLine()
        usleep(1000000);
        /*(right1.Set(ControlMode::Position, right1Pos + (Controls.GetY() * 4096));
        left1.Set(ControlMode::Position, (left1Pos + (Controls.GetY() * 4096)));
        usleep(5000);*/
        /*char buff;
        read(connfd, &buff, 1); // Get one byte
        switch (buff){
            case 1: // This is a move command.
                char* byWhichAmount = "00000000"; // Literally no reason for this to be anything, it's going to be overwritten in a second.
                int totalRead = 0;
                while (totalRead < 8){
                    read(connfd, byWhichAmount + totalRead, 8); // Bit of pointer magic here; because it's a pointer, we can shift up in memory by the number of bytes it's read and not lose data.
                }
                uint32_t yChange = byWhichAmount[0] + byWhichAmount[1] * 256 + byWhichAmount[2] * 65536 + byWhichAmount[3] * 16777216; // To the coders that just threw up: go screw yourselves. This is much faster than the alternative.
                uint32_t xChange = byWhichAmount[4] + byWhichAmount[5] * 256 + byWhichAmount[6] * 65536 + byWhichAmount[7] * 16777216; // Stop that, that's new carpeting!
                moveStandard(yChange);
                moveMecanum(xChange);
                write(connfd, "0", 1);
                break;
        }*/
    }

    void TestLoop(){
        /* NORMAL   */ right1.Set(ControlMode::PercentOutput, 0.3);
        /* NORMAL   */ right2.Set(ControlMode::PercentOutput, 0.3);
        /* INVERTED */  left1.Set(ControlMode::PercentOutput, 0.3);
        /* INVERTED */  left2.Set(ControlMode::PercentOutput, 0.3);
    }

    void BeginTest(){

    }

    void AutonomousLoop(){

    }

    void CleanUpTest(){
        right1.Set(ControlMode::PercentOutput, 0);
        right2.Set(ControlMode::PercentOutput, 0);
        left1.Set(ControlMode::PercentOutput, 0);
        left2.Set(ControlMode::PercentOutput, 0);
    }
};

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
