/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Timothy wallis                                            */
/*    Description:  Program intended for robot                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "string"

using namespace vex;
using namespace std;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
brain Brain = brain();
aivision::colordesc Red = aivision::colordesc(1, 210, 38, 48, 10.00, 0.5);
aivision::colordesc Blue = aivision::colordesc(2, 0, 119, 200, 10.00, 0.5);
aivision Camera = aivision(PORT5, Red, Blue);
inertial Inertial = inertial(PORT3);
motor driveMotor[2] = {motor(PORT1, true), motor(PORT10, false)};
smartdrive Drivetrain = smartdrive(driveMotor[0], driveMotor[1], Inertial);

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
    auton_track();
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
    auton_track();
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
void pre_auton(void)
{
  Inertial.calibrate(3);
  colorChoseUI();

  Brain.Screen.pressed(colorSelect);
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
