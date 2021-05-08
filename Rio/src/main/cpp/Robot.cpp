// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

// This used to include Robot.h, now it doesn't because I prefer to write my own class.

#include <frc/DriverStation.h>
#include <frc/livewindow/LiveWindow.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <hal/DriverStation.h>
#include <networktables/NetworkTable.h>
#include <ctre/Phoenix.h>
#include <atomic>

#include <unistd.h>
#include <frc/Joystick.h>
#include <frc/RobotBase.h>

/* CAN ID setup for Socialbot:
-------------------------------
Right motor (master, slave): 1, 2
Left motor (master, slave): 3, 4
*/

class Robot : public frc::RobotBase{
private:
    std::atomic<bool> m_exit{false}; // Don't worry about it!
    TalonSRX Right{1}; // TalonSRX motor type, right master
    TalonSRX Left{3}; // TalonSRX motor type, left master
    VictorSPX RightSlave{2}; // VictorSPX motor type, right slave
    VictorSPX LeftSlave{4}; // VictorSPX motor type, left slave

public:
    Robot(){
    }
    void RobotInit() {
        RightSlave.Follow(Right); // Make it follow the right, and thus do whatever right does
        LeftSlave.Follow(Left);
    }

    void Disabled() {}

    void Autonomous() {}

    void Teleop() {
      while(true){
        for (int i = 0; i < 500; i++) {
          Right.Set(ControlMode::PercentOutput, 0.1);
          Left.Set(ControlMode::PercentOutput, 0.1);
        }
        usleep(3500000);
        for (int i = 0; i < 500; i++) {
          Right.Set(ControlMode::PercentOutput, -0.5);
          Left.Set(ControlMode::PercentOutput, 0.5);
        }
      }
        // Lets break it down:
        // Right.Set is the set call on the Right Motor. That sets the speed/power of the motor
        // ControlMode::PercentOutput is the percent-output mode, which allows you to set the speed of the motor based on the maximum speed. Its quite deadly actually.
        // 0.1 is a safe speed

        // So, space cadets, can you make this drive based on joystick input? The necessary tab is in your browser!
    }

    void Test() {}

    void StartCompetition() {
        auto& lw = *frc::LiveWindow::GetInstance();

        RobotInit();

        // Tell the DS that the robot is ready to be enabled
        HAL_ObserveUserProgramStarting();

        while (!m_exit) {
            if (IsDisabled()) {
                m_ds.InDisabled(true);
                Disabled();
                m_ds.InDisabled(false);
                while (IsDisabled()) {
                    m_ds.WaitForData();
                }
            } else if (IsAutonomous()) {
                m_ds.InAutonomous(true);
                Autonomous();
                m_ds.InAutonomous(false);
                while (IsAutonomousEnabled()) {
                    m_ds.WaitForData();
                }
            } else if (IsTest()) {
                lw.SetEnabled(true);
                frc::Shuffleboard::EnableActuatorWidgets();
                m_ds.InTest(true);
                Test();
                m_ds.InTest(false);
                while (IsTest() && IsEnabled()) {
                    m_ds.WaitForData();
                }
                lw.SetEnabled(false);
                frc::Shuffleboard::DisableActuatorWidgets();
            } else {
                m_ds.InOperatorControl(true);
                Teleop();
                m_ds.InOperatorControl(false);
                while (IsOperatorControlEnabled()) {
                    m_ds.WaitForData();
                }
            }
        }
    }

    void EndCompetition() {
        m_exit = true;
    }
};

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
