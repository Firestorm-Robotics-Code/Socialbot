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

#include <frc/Joystick.h>
#include <stdio.h>
#include <unistd.h>
#include <atomic>
#include <cstring>

#include "ModularRobot.hpp"
#include "c_str_man.hpp"


class Robot : public ModularRobot{
private:
    std::atomic<bool> m_exit{false}; // Multithreaded variable. This is why the code doesn't die!
    TalonSRX right1{2};
    TalonSRX right2{3};
    TalonSRX left1{4};
    TalonSRX left2{1};
    frc::Joystick Controls{5};

public:
    Robot(){
        HAL_SendConsoleLine("____________________________INIT ROBOT CODE____________________________");
        HAL_SendConsoleLine("----------- Dashes Are Not More Appealing -----------");
        setData("Socialbot", "Firestorm Robotics", 6341);
    }

    void Init(){

    }

    void BeginTeleop(){

    }

    void TeleopLoop(){
        /* NORMAL   */ right1.Set(ControlMode::PercentOutput, (Controls.GetX()/2 + Controls.GetY())*(Controls.GetThrottle() * 0.5 + 0.5));
        /* NORMAL   */ right2.Set(ControlMode::PercentOutput, (Controls.GetX()/2 + Controls.GetY())*(Controls.GetThrottle() * 0.5 + 0.5));
        /* INVERTED */  left1.Set(ControlMode::PercentOutput, -(-Controls.GetX()/2 + Controls.GetY())*(Controls.GetThrottle() * 0.5 + 0.5));
        /* INVERTED */  left2.Set(ControlMode::PercentOutput, -(-Controls.GetX()/2 + Controls.GetY())*(Controls.GetThrottle() * 0.5 + 0.5));
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

    }
};

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
