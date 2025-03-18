#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
motor LeftDriveSmart = motor(PORT1, ratio18_1, false);
motor RightDriveSmart = motor(PORT10, ratio18_1, true);
inertial DrivetrainInertial = inertial(PORT3);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 319.19, 320, 40, mm, 1);

controller Controller1 = controller(primary);


// generating and setting random seed
void initializeRandomSeed(){
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  // Combine these values into a single integer
  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;

  // Set the seed
  srand(seed);
}

bool vexcode_initial_drivetrain_calibration_completed = false;
void calibrateDrivetrain() {
  wait(200, msec);
  Brain.Screen.print("Calibrating");
  Brain.Screen.newLine();
  Brain.Screen.print("Inertial");
  DrivetrainInertial.calibrate();
  while (DrivetrainInertial.isCalibrating()) {
    wait(25, msec);
  }
  vexcode_initial_drivetrain_calibration_completed = true;
  // Clears the screen and returns the cursor to row 1, column 1.
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
}

void vexcodeInit() {

  // Calibrate the Drivetrain
  calibrateDrivetrain();

  //Initializing random seed.
  initializeRandomSeed(); 
}


// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}



// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      // stop the motors if the brain is calibrating
      if (DrivetrainInertial.isCalibrating()) {
        LeftDriveSmart.stop();
        RightDriveSmart.stop();
        while (DrivetrainInertial.isCalibrating()) {
          wait(25, msec);
        }
      }
      
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis1
      // right = Axis3 - Axis1
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1) {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1) {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(fwd);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(fwd);
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);

#pragma endregion VEXcode Generated Robot Configuration

// Include the V5 Library
  
// Allows for easier use of the VEX Library


#include "string"

using namespace std;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
aivision::colordesc Red = aivision::colordesc(1, 210, 38, 48, 10.00, 0.5);
aivision::colordesc Blue = aivision::colordesc(2, 0, 119, 200, 10.00, 0.5);
aivision Camera = aivision(PORT5, Red, Blue);
motor Intake = motor(PORT16);
motor Ramp = motor(PORT9);
//TODO
motor_group Intake_Ramp = motor_group(Ramp, Intake);
digital_out Pnuematics = digital_out(Brain.ThreeWirePort.A);
//END TODO
int i = 0;
//Ai vision range
int x_prime = (160/2) - 15;
int x_dblprime = (160/2) + 15;

                          //Blue            Red
color CompColor[2] = {color(0x0000FF), color(0xFF0000)};
string side;
int TEXT_PADDING = 20;
bool ColorSelected = true;
bool SideChosing = true;
bool valve = true;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/
void auton_track(){
  while(true){
    if(i){
      Camera.takeSnapshot(Blue);
    }else if(i){
      Camera.takeSnapshot(Red);
    }
    if(Camera.objectCount > 0){
      if(Camera.objects[0].centerX < x_prime){
        Drivetrain.turn(left);
      }else if(Camera.objects[0].centerX > x_dblprime){
        Drivetrain.turn(right);
      }else{
        Drivetrain.stop();
      }
    }
  }
}
void selectChoseUI(){
  Brain.Screen.setPenColor("#000000");
  Brain.Screen.setFillColor("#FFFFFF");
  Brain.Screen.drawRectangle((SYSTEM_DISPLAY_WIDTH / 2) - (Brain.Screen.getStringWidth("Right") + TEXT_PADDING), (SYSTEM_DISPLAY_HEIGHT / 2) - Brain.Screen.getStringHeight("Right") * 2, Brain.Screen.getStringWidth("Right") + TEXT_PADDING, Brain.Screen.getStringHeight("Right") + (TEXT_PADDING * 2));
  Brain.Screen.setCursor(7, 19);
  Brain.Screen.print("Right");
  Brain.Screen.setCursor(7, 31);
  Brain.Screen.setFillColor("#FFFFFF");
  Brain.Screen.drawRectangle((SYSTEM_DISPLAY_WIDTH / 2) + (Brain.Screen.getStringWidth("Left")) + (TEXT_PADDING / 2), (SYSTEM_DISPLAY_HEIGHT / 2) - Brain.Screen.getStringHeight("Left") * 2, Brain.Screen.getStringWidth("Left") + (TEXT_PADDING), Brain.Screen.getStringHeight("Left") + (TEXT_PADDING * 2));
  Brain.Screen.print("Left");
}

//TODO: Complete alogrithm
void sideSelected()
{
  if(SideChosing){
    if(
      //Right
      Brain.Screen.xPosition() >= (SYSTEM_DISPLAY_WIDTH / 2) - (Brain.Screen.getStringWidth("Right") + TEXT_PADDING) && 
      Brain.Screen.xPosition() <= Brain.Screen.getStringWidth("Right") + TEXT_PADDING && 
      Brain.Screen.yPosition() >= (SYSTEM_DISPLAY_HEIGHT / 2) - Brain.Screen.getStringHeight("Right") * 2 &&
      Brain.Screen.yPosition() <= Brain.Screen.getStringHeight("Right") + (TEXT_PADDING * 2)
    ){
      side = "Right";
      Brain.Screen.clearScreen("#000000");

    }
  }
}
void colorSelect()
{
  
  if(ColorSelected){
  if (Brain.Screen.xPosition() >= 185 && Brain.Screen.xPosition() <= 235 && Brain.Screen.yPosition() >= 115 && Brain.Screen.yPosition() <= 155)
  {
    //Blue
    ColorSelected = false;
    Brain.Screen.setPenColor(CompColor[i]);
    Brain.Screen.setFillColor(CompColor[i]);
    Brain.Screen.drawRectangle(0, 0, SYSTEM_DISPLAY_WIDTH, SYSTEM_DISPLAY_HEIGHT);
    selectChoseUI();
    Brain.Screen.pressed(sideSelected);
  }
  else if (
      Brain.Screen.xPosition() >= 280 && Brain.Screen.xPosition() <= 325 && Brain.Screen.yPosition() >= 115 && Brain.Screen.yPosition() <= 155)
  {
    // Red
    i++;
    ColorSelected = false;
    Brain.Screen.setPenColor(CompColor[i]);
    Brain.Screen.setFillColor(CompColor[i]);
    Brain.Screen.drawRectangle(0, 0, SYSTEM_DISPLAY_WIDTH, SYSTEM_DISPLAY_HEIGHT);
    selectChoseUI();
    Brain.Screen.pressed(sideSelected);
  }
  }else{
    return;
  }
}

void colorChoseUI()
{
  Brain.Screen.setPenColor("#372e2d");
  Brain.Screen.setFillColor("#372e2d");
  Brain.Screen.drawRectangle(0, 0, SYSTEM_DISPLAY_WIDTH, SYSTEM_DISPLAY_HEIGHT);
  Brain.Screen.setPenColor("#000000");
  Brain.Screen.setFillColor("#FFFFFF");
  Brain.Screen.drawRectangle((SYSTEM_DISPLAY_WIDTH / 2) - (Brain.Screen.getStringWidth("Blue") + TEXT_PADDING), (SYSTEM_DISPLAY_HEIGHT / 2) - Brain.Screen.getStringHeight("Blue") * 2, Brain.Screen.getStringWidth("Blue") + TEXT_PADDING, Brain.Screen.getStringHeight("Blue") + (TEXT_PADDING * 2));
  Brain.Screen.setCursor(7, 20);
  Brain.Screen.print("Blue");
  Brain.Screen.setCursor(7, 30);
  Brain.Screen.setFillColor("#FFFFFF");
  Brain.Screen.drawRectangle((SYSTEM_DISPLAY_WIDTH / 2) + (Brain.Screen.getStringWidth("Red")) + (TEXT_PADDING / 2), (SYSTEM_DISPLAY_HEIGHT / 2) - Brain.Screen.getStringHeight("Red") * 2, Brain.Screen.getStringWidth("Red") + (TEXT_PADDING), Brain.Screen.getStringHeight("Red") + (TEXT_PADDING * 2));
  Brain.Screen.print("Red");
}

//Pnuematic controls
void pnuemintialize(){
  if(valve){
    Pnuematics.set(true);
  }else if(!valve){
    Pnuematics.set(false);
  }
}
void pnuemrelease(){
  if(valve){
    valve = false;
  }else if(!valve){
    valve = true;
  }
}
//Ramp
void RampBegin(){
  Intake_Ramp.spin(fwd);
}
void RampStop(){
  Intake_Ramp.stop();
}
//Rest of competition template
void pre_auton(void)
{
  Intake_Ramp.setVelocity(100, percent);
  colorChoseUI();

  Brain.Screen.pressed(colorSelect);
  Controller1.ButtonA.pressed(pnuemintialize);
  Controller1.ButtonA.released(pnuemrelease);
  Controller1.ButtonL1.pressed(RampBegin);
  Controller1.ButtonL1.released(RampStop);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void)
{
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void)
{
  // User control code here, inside the loop
  while (1)
  {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main()
{
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true)
  {
    wait(100, msec);
  }
}
