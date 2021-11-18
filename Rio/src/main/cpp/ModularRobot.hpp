// Highly modular FRC robot class with threading support
// Works like a standard robot at face value, but has lots of extra cool functionality
// Such as the ability to add modules which can run in threads if you want them to
// As well as extra functionality for internet servers

#include <frc/RobotBase.h>
#include <vector>
#include <thread>
#include <atomic>
#include "c_str_man.hpp"
#include <string>

class ModularRobot;

class Module{ // Not much of a base class, but it serves a purpose!
protected:
    ModularRobot* myRobot;
public:
    void beginModule(ModularRobot *robot){
        myRobot = robot;
        init(robot);
    }
    virtual void init(ModularRobot* robot){
    }
    virtual void run(unsigned long long tick){

    }
};

class ModularRobot : public frc::RobotBase{
private:
    std::atomic<bool> m_exit{false};
    std::vector<Module> modules;
    const char* RobotName;
    const char* TeamName;
    int TeamNumber;
    int mode = -1; // 0 = disabled, 1 = autonomous, 2 = test, 3 = teleop
    unsigned long long tick; // Number of iterations since robot began
    unsigned long long localTick; // Number of iterations since current operation-mode began
public:
    void println(const char* data){
        HAL_SendConsoleLine(data);
    }
    void setData(const char* robotname, const char* teamname, int teamnumber){
        RobotName = robotname;
        TeamNumber = teamnumber;
        TeamName = teamname;
    }

    virtual void Init(){

    }

    virtual void BeginDisabled(){

    }

    virtual void DisabledLoop(){

    }

    virtual void CleanUpDisabled(){

    }

    virtual void BeginTeleop(){

    }

    virtual void TeleopLoop(){

    }

    virtual void CleanUpTeleop(){

    }

    virtual void BeginTest(){

    }

    virtual void TestLoop(){

    }

    virtual void CleanUpTest(){

    }

    virtual void BeginAutonomous(){

    }

    virtual void AutonomousLoop(){

    }

    virtual void CleanUpAutonomous(){

    }

    virtual void Loop(){

    } // User mainloop

    virtual void ItsOver(){

    }

    void addModule(Module module){
        modules.push_back(module);
        module.beginModule(this); // Module init function should take a pointer to a ModularRobot
    }

    void loop(){
        for (Module i : modules){
            i.run(tick);
        }
        Loop();
    }

    void StartCompetition(){ // "Final" keeps it from being overriden so stupid people don't damage the program
        Init();
        HAL_SendConsoleLine(constchar_concat_many(6, RobotName, " by ", TeamName, " (FRC ", "6341", ") is now turning on!"));
        HAL_InitializeDriverStation();
        HAL_ObserveUserProgramStarting();
        while (!m_exit){ // Restructured from the old uglies. This one gives easy-peasy mainlooping without our ugly-mugly turdy-purdy stinky-winky infinite while loop
            loop(); // General mainloop
            if (IsDisabled()){ // Disabled tasks
                HAL_ObserveUserProgramDisabled();
                if (mode != 0){
                    HAL_SendConsoleLine("Begin Disable mode");
                    localTick = 0; // Reset the local tick counter
                    BeginDisabled();
                }
                if (mode == 1){
                    CleanUpAutonomous();
                }
                else if (mode == 2){
                    CleanUpTest();
                }
                else if (mode == 3){
                    CleanUpTeleop();
                }
                DisabledLoop();
                mode = 0;
            }
            else if (IsAutonomous()){ // Autonomous tasks
                HAL_ObserveUserProgramAutonomous();
                if (mode != 1){
                    HAL_SendConsoleLine("Begin Autonomous mode");
                    localTick = 0; // Reset the local tick counter
                    BeginAutonomous();
                }
                if (mode == 0){
                    CleanUpDisabled();
                }
                else if (mode == 2){
                    CleanUpTest();
                }
                else if (mode == 3){
                    CleanUpTeleop();
                }
                AutonomousLoop();
                mode = 1;
            }
            else if (IsTest()){ // Test tasks
                HAL_ObserveUserProgramTest();
                if (mode != 2){
                    HAL_SendConsoleLine("Never gonna give you up, never gonna let you down, never gonna run around and hurt you");
                    localTick = 0; // Reset the local tick counter
                    BeginTest();
                }
                if (mode == 0){
                    CleanUpDisabled();
                }
                else if (mode == 1){
                    CleanUpAutonomous();
                }
                else if (mode == 3){
                    CleanUpTeleop();
                }
                TestLoop();
                mode = 2;
            }
            else{ // Teleop tasks
                HAL_ObserveUserProgramTeleop();
                if (mode != 3){
                    HAL_SendConsoleLine("Begin Teleop mode");
                    localTick = 0; // Reset the local tick counter
                    BeginTeleop();
                }
                if (mode == 0){
                    CleanUpDisabled();
                }
                else if (mode == 1){
                    CleanUpAutonomous();
                }
                else if (mode == 2){
                    CleanUpTest();
                }
                TeleopLoop();
                mode = 3;
            }
            tick++; // Update the tick counters
            localTick++;
        }
        ItsOver();
    }
    void EndCompetition() {
        m_exit = true;
    }
};