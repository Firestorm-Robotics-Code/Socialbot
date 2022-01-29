// Lots of FRC/Wpi legalese crap to dredge through, but this is the 21st century and when we see a smelly swamp we build things on it.
/*





You see how nice things can be built upon the swamps of copyright?
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
 ___________________________________________--------------------------------------------------_____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________
|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  |
|__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________|
Distribution includes (but is not possible to put the Notice in a larger work of which you may at your option offer warranty protection in exchange for a fee. You may always continue to use the Work by You to the interfaces of, the Licensor except as expressly stated in Sections 2(a) and 2(b) above, Recipient receives no rights or licenses to their respective portions thereof. Deploy" means: (a) to sublicense, distribute or transfer NetHack except as disclosed pursuant to Section 3.4(a) above,
Contributor believes that Contributor's Modifications are derived, directly or indirectly infringes any patent where such claim is resolved (such as a whole.

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
#include <frc/Compressor.h>
#include <frc/Solenoid.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <atomic>
#include <cstring>
#include <thread>
#include <fcntl.h>
#include <frc/DigitalInput.h>
#include <AHRS.h>
#include <frc/SPI.h>
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableInstance.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableValue.h"
#include <cmath>


#include "ModularRobot.hpp"
//#include "httpserver.hpp"

// Note that in any case where user and driverstation are at odds, driverstation wins. The person at the driverstation computer can, thus, at any time, override mode and similar.
//#include "site.hpp"


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
    frc::Solenoid up{5, 0};
    frc::Compressor comp{5};
    frc::Solenoid down{5, 1};
    frc::DigitalInput button{2};
    //HTTPServer *server;
    AHRS navx {frc::SPI::Port::kMXP};
    std::shared_ptr<NetworkTable> table = nt::NetworkTableInstance::GetDefault().GetTable("limelight");

public:

    Robot(){
        HAL_SendConsoleLine("Please connect to http://roboRIO-6341-FRC.local:5801 for web console (Firefox, potentially Chrome). Please see Tyler, the incredibly smart, if you have a problem.");
        HAL_SendConsoleLine("Note that it is unnecessary to use the web console, it simply adds more features. You can also control it with cURL, but you'll need the manual for that.");
        setData("Socialbot", "Firestorm Robotics", 6341);
    }

    /*void respond(HTTPARGS){

    }*/

//    void disconnect(Client *client){
        /*// Comment gate. Add an asterisk in front of the first forward slash to close, remove said asterisk to open.
        // The idea is (quite idiotically) to comment out code cleanly by toggling one character.
        free(client -> state); // Prevent memleak.
        //*/
    //}

    static void Periodic(Robot *self){
        /*//
        if (self -> mode == 3){
            self -> right1.Set(ControlMode::Position, self -> right1Pos);
            self -> left1.Set(ControlMode::Position, self -> left1Pos);
            self -> right2.Set(ControlMode::Position, self -> right2Pos);
            self -> left2.Set(ControlMode::Position, self -> left2Pos);
        }
        HAL_SendConsoleLine("Yo");//*/
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
        //PIDLoop = new frc::Notifier(&Robot::RunPIDTasks, this);
        //HAL_SendConsoleLine("Got connection");
        /*BeginTalonPID(&right1, 0);
        BeginTalonPID(&left1, 0);
        BeginTalonPID(&right2, 0);
        BeginTalonPID(&left2, 0);*/
        left1.SetInverted(true);
        left2.SetInverted(true);
        /*server = new HTTPServer (5801, [this](HTTPARGS){
            this -> respond(HTTPPASSARGS);
        }, [this](Client *client){
            this -> disconnect(client);
        }, 3); // Initialize it. This method is a poor coding practice but I'm not going to bother improving it, that's just how awesome my ethics are.*/
        setPeriodicDelay(200000);
        navx.Calibrate();
        navx.ResetDisplacement();
    }

    void BeginTalonPID(TalonSRX *_talon, long position, uint8_t slot = 0, uint8_t timeout = 30, float maxSpeed = 0.6){
        _talon->ConfigFactoryDefault();

        _talon->SetSelectedSensorPosition(position, slot, timeout);

        /* choose the sensor and sensor direction */
        _talon->ConfigSelectedFeedbackSensor(
                FeedbackDevice::CTRE_MagEncoder_Absolute, slot, timeout);
        _talon->SetSensorPhase(true);

        /* set the peak and nominal outputs, 12V means full */
        _talon->ConfigNominalOutputForward(0, timeout);
        _talon->ConfigNominalOutputReverse(0, timeout);
        _talon->ConfigPeakOutputForward(maxSpeed, timeout);
        _talon->ConfigPeakOutputReverse(-maxSpeed, timeout);

        /* set closed loop gains in slot0 */
        _talon->Config_kF(slot, 0.0, timeout);
        _talon->Config_kP(slot, 0.1, timeout);
        _talon->Config_kI(slot, 0.0, timeout);
        _talon->Config_kD(slot, 0.0, timeout);
        printf("Begunt motor\n");
    }

    void BeginTeleop(){
        BeginTalonPID(&right1, 1);
        BeginTalonPID(&right2, 1);
        BeginTalonPID(&left1, 1);
        BeginTalonPID(&left2, 1);
        printf("Begunt teleop\n");
        navx.ResetDisplacement();
    }

    void moveForwards(long change){
        right1Pos += change;
        right2Pos += change;
        left1Pos += change;
        left2Pos += change;
    }

    void moveSideways(long change){
        right1Pos += change;
        right2Pos -= change;
        left1Pos -= change;
        left2Pos += change;
    }

    void moveTo(long x, long y){
        right1Pos = (y + x * 2) + right1Pos; // X = mecanum, which loses half of power to friction (approx.)
        right2Pos = (y - x * 2) + right2Pos;
        left1Pos = (y - x * 2) + left1Pos;
        left2Pos = (y + x * 2) + left2Pos;
    }

    bool triggy = true; // We want the first press to go up.
    bool state = false; // Off

    void TeleopLoop(){
        ///
        double controlY = -1 * Controls.GetY() * (Controls.GetThrottle() + 1) / 2; // Speed limiter, the throttle can be -1 to 1 so this makes it work
        double controlX = Controls.GetX() * (Controls.GetThrottle() + 1) / 2;
        double right1mov = controlY;
        double left1mov = controlY;
        double right2mov = controlY;
        double left2mov = controlY;

        if (!Controls.GetTrigger()){
            right1mov -= 2 * controlX;
            left1mov += 2 * controlX;
            right2mov += 2 * controlX;
            left2mov -= 2 * controlX;
        }
        else{
            right1mov += controlX;
            left1mov -= controlX;
            right2mov += controlX;
            left2mov -= controlX;
        }

        if (!Controls.GetRawButton(2)){ // Trigger makes it turn. Works well after my tests, so whatever your problem is, it isn't that
            if (triggy){
                triggy = false;
                if (state){
                    HAL_SendConsoleLine("Rising.");
                    up.Set(true);
                    down.Set(false);
                }
                else{
                    HAL_SendConsoleLine("Dropping.");
                    down.Set(true);
                    up.Set(false);
                }
                state = !state; // Flip it. First press goes up, second press goes down, etc.
            }
        }
        else{
            triggy = true;
        }
        left1Pos += left1mov * 2048;
        left2Pos += left2mov * 2048;
        right1Pos += right1mov * 2048;
        right2Pos += right2mov * 2048;
        //*/
        /*//
        if (Controls.GetTrigger()){
            const double minSpeed = 0;
            const double maxSpeed = 0.3;
            double wrongness = minSpeed + ((0.1 - navx.GetDisplacementX()) * 10) * (maxSpeed - minSpeed); // 1 when it's 360 degrees away, and then * 0.5 so the max is 0.5.
            right1.Set(ControlMode::PercentOutput, wrongness);
            right2.Set(ControlMode::PercentOutput, wrongness);
            left1.Set(ControlMode::PercentOutput, -wrongness);
            left2.Set(ControlMode::PercentOutput, -wrongness);
        }
        else{
            double wrongness = Controls.GetY(); // 1 when it's 360 degrees away, and then * 0.5 so the max is 0.5.
            right1.Set(ControlMode::PercentOutput, wrongness);
            right2.Set(ControlMode::PercentOutput, wrongness);
            left1.Set(ControlMode::PercentOutput, -wrongness);
            left2.Set(ControlMode::PercentOutput, -wrongness);
        }
        //*/
        /*//
        printf("Displacement X: %f\nDisplacement Y: %f\nDisplacement Z: %f\n", navx.GetDisplacementX(), navx.GetDisplacementY(), navx.GetDisplacementZ());
        usleep(1000000);
        //*/
        /*//
        double right1mov = 0;
        double right2mov = 0;
        double left1mov = 0;
        double left2mov = 0;
        if (Controls.GetTrigger()){
            double targetOffsetAngle_Horizontal = table->GetNumber("tx",0.0);
            double targetOffsetAngle_Vertical = table->GetNumber("ty",0.0);
            double targetArea = table->GetNumber("ta",0.0);
            double targetSkew = table->GetNumber("ts",0.0);
            double hasTarget = table -> GetNumber("tv", 0);
            double speed = (targetOffsetAngle_Horizontal / 20) * 0.4;
            unsigned int botHeightInches = 44;
            unsigned int targetHeightInches = 66;
            double distance = (targetHeightInches - botHeightInches) / tan(targetOffsetAngle_Vertical);
            printf("Distance to target: %f\n", distance);
            usleep(1000000);

            if (hasTarget){
                right1mov = speed;
                right2mov = speed;
                left1mov = -speed;
                left2mov = -speed;
                double followerSpeed = (4 - targetArea) * 0.2;
                right1mov += followerSpeed;
                right2mov += followerSpeed;
                left1mov += followerSpeed;
                left2mov += followerSpeed;
            }
        }
        else {
            right1mov = Controls.GetY() + -(Controls.GetX() / 2);
            right2mov = Controls.GetY() + -(Controls.GetX() / 2);
            left1mov = Controls.GetY() + (Controls.GetX() / 2);
            left2mov = Controls.GetY() + (Controls.GetX() / 2);
            right1mov *= (Controls.GetThrottle() + 1)/2;
            right2mov *= (Controls.GetThrottle() + 1)/2;
            left1mov *= (Controls.GetThrottle() + 1)/2;
            left2mov *= (Controls.GetThrottle() + 1)/2;
        }
        right1.Set(ControlMode::PercentOutput, right1mov);
        right2.Set(ControlMode::PercentOutput, right2mov);
        left1.Set(ControlMode::PercentOutput, left1mov);
        left2.Set(ControlMode::PercentOutput, left2mov);
        //*/
    }

    void TeleopPeriodic(){
        right1.Set(ControlMode::Position, right1Pos);
        right2.Set(ControlMode::Position, right2Pos);
        left1.Set(ControlMode::Position, left1Pos);
        left2.Set(ControlMode::Position, left2Pos);
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

    }
};

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
