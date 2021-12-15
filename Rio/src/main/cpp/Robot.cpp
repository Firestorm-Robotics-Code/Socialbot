// Lots of FRC/Wpi legalese crap to dredge through, but this is the 21st century and when we see a smelly swamp we build things on it.
/*





Ah, a nice copyright-free building, atop a smelly swamp!
                                             ________________________________________________
                                            |                                                |
                                            |   ________                          ________   |
                                            |  |        |                        |        |  |
                                            |  |   n    |                        |   Y    |  |
                                            |  |   |    |                        |   |    |  |
                                            |  |  \_]   |                        |  \_]   |  |
                                            |  |mmmmmmmm|                        |mmmmmmmm|  |
                                            |                                                |
                                            |                                                |
                                            |   ________                          ________   |
                                            |  |        |                        |   V    |  |
                                            |  |   U    |                        |  FVF   |  |
                                            |  |   |    |                        |   |    |  |
                                            |  |  \_]   |                        |  \_]   |  |
                                            |  |mmmmmmmm|                        |mmmmmmmm|  |
                                            |                                                |
                                            |                                                |
                                            |   ________                          ________   |
                                            |  |        |                        |        |  |
                                            |  |  ooo   |                        |   0    |  |
                                            |  |   |    |                        |   |    |  |
                                            |  |  \_]   |                        |  \_]   |  |
                                            |  |mmmmmmmm|                        |mmmmmmmm|  |
                                            |                                                |
                                            |                                                |
                                            |   ________                          ________   |
                                            |  |  zZz   |                        |  XX    |  |
                                            |  |  zZz   |                        |   XX   |  |
                                            |  |   |    |                        |   |    |  |
                                            |  |  \_]   |                        |  \_]   |  |
                                            |  |mmmmmmmm|        _______         |mmmmmmmm|  |
                                            |                   |       |                    |
                                            |                   |       |                    |
                                            |                   |       |                    |
                                            |                   |     O |                    |
                                            |                   |       |                    |
                                            |                   |       |                    |
                                            --------------------------------------------------
Distribution includes (but is not possible to put the Notice in a larger work of which you may at your option offer warranty protection in exchange for a fee. You may always continue to use the Work by You to the interfaces of, the Licensor except as expressly stated in Sections 2(a) and 2(b) above, Recipient receives no rights or licenses to their respective portions thereof. Deploy" means: (a) to sublicense, distribute or transfer NetHack except as disclosed pursuant to Section 3.4(a) above, Contributor believes that Contributor's Modifications are derived, directly or indirectly infringes any patent where such claim is resolved (such as a whole.

If identifiable sections of this License, shall survive. Termination Upon Assertion of Patent Infringement. If you do not forfeit any of the original test modes be preserved. If you make it clear that any such warranty or additional liability.

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
#include <frc/Notifier.h>
#include <units/time.h>
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
    TalonSRX right2{4};
    TalonSRX left1{3};
    TalonSRX left2{1};
    frc::Joystick Controls{5};
    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;
    long right1Pos = 0;
    long right2Pos = 0;
    long left1Pos = 0;
    long left2Pos = 0;
    frc::Notifier *PIDLoop;

public:
    Robot(){
        HAL_SendConsoleLine("____________________________INIT ROBOT CODE____________________________");
        HAL_SendConsoleLine("----------- Dashes Are Not More Appealing -----------");
        HAL_SendConsoleLine("FRC driver station is poopy, gotta write an alternative. This gon' be fun.")
        setData("Socialbot", "Firestorm Robotics", 6341);
    }

    void RunPIDTasks(){
        moveMecanum(Controls.GetY()*1024 + Controls.GetTrigger() * 1024 + 2147483648);
        right1.Set(ControlMode::Position, right1Pos);
        left1.Set(ControlMode::Position, left1Pos);
        right2.Set(ControlMode::Position, right2Pos);
        left2.Set(ControlMode::Position, left2Pos);
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
        /*right1Pos = right1.GetSelectedSensorPosition();
        right2Pos = right2.GetSelectedSensorPosition();
        left1Pos = left1.GetSelectedSensorPosition();
        left2Pos = left2.GetSelectedSensorPosition();*/
        PIDLoop = new frc::Notifier(&Robot::RunPIDTasks, this);
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
        left1.SetInverted(true);
        left2.SetInverted(true);
        PIDLoop -> StartPeriodic((units::millisecond_t)200);
    }

    void moveStandard(uint32_t change){
        int32_t signedVersion = change - 2147483648; // Make it signed so the robot can go backwards.
        right1Pos += signedVersion;
        right2Pos += signedVersion;
        left1Pos += signedVersion;
        left2Pos += signedVersion;
    }

    void moveMecanum(uint32_t change){
        int32_t signedVersion = change;// - 2147483648; // Make it signed so the robot can go backwards.
        right1Pos += change;
        right2Pos -= change;
        left1Pos -= change;
        left2Pos += change;
    }

    void TeleopLoop(){
        char buff;
        read(connfd, &buff, 1); // Get one byte
        switch (buff){
            case 1: // This is a move command.
                char* byWhichAmount = "00000000"; // Literally no reason for this to be anything, it's going to be overwritten in a second.
                // However, if it isn't something, we'll end up with a segfault because the pointer out of bounds bla bla bla.
                // *Dreams of Arduino Mega*
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
        }
    }

    void TestLoop(){

    }

    void BeginTest(){
        /* NORMAL   */ right1.Set(ControlMode::PercentOutput, 0.3);
        usleep(1000000);
        right1.Set(ControlMode::PercentOutput, 0);
        /* NORMAL   */ right2.Set(ControlMode::PercentOutput, 0.3);
        usleep(1000000);
        right2.Set(ControlMode::PercentOutput, 0);
        /* INVERTED */  left1.Set(ControlMode::PercentOutput, 0.3);
        usleep(1000000);
        left1.Set(ControlMode::PercentOutput, 0);
        /* INVERTED */  left2.Set(ControlMode::PercentOutput, 0.3);
        usleep(1000000);
        left2.Set(ControlMode::PercentOutput, 0);
    }

    void AutonomousLoop(){

    }

    void CleanUpTest(){
        right1.Set(ControlMode::PercentOutput, 0);
        right2.Set(ControlMode::PercentOutput, 0);
        left1.Set(ControlMode::PercentOutput, 0);
        left2.Set(ControlMode::PercentOutput, 0);
    }

    void CleanUpTeleop(){
        PIDLoop -> Stop();
    }
};

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
