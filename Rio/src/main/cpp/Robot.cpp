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
    TalonSRX right1{1};
    TalonSRX right2{2};
    TalonSRX left1{3};
    TalonSRX left2{4};
    frc::Joystick Controls{5};

public:
    Robot(){
        HAL_SendConsoleLine("____________________________INIT ROBOT CODE____________________________\n----------- Dashes Are Not More Appealing -----------");
        setData("Socialbot", "Firestorm Robotics", 6341);
    }

    void Init(){

    }

    void BeginTeleop(){

    }

    void TeleopLoop(){
        right1.Set((-Controls.GetX()/2 + Controls.GetY())*(Controls.GetThrottle() * 0.5 + 0.5));
        right2.Set((-Controls.GetX()/2 + Controls.GetY())*(Controls.GetThrottle() * 0.5 + 0.5));
        left1.Set((-Controls.GetX()/2 + -Controls.GetY())*(Controls.GetThrottle() * 0.5 + 0.5));
        left2.Set((-Controls.GetX()/2 + -Controls.GetY())*(Controls.GetThrottle() * 0.5 + 0.5));
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
