#include <Ps3Controller.h>
#include "smorphi.h"
int player = 0;
int battery = 0;

#define LED_PIN 2

Smorphi robot;

//Store data analog from joystick left
int leftX = 0;
int leftY = 0;

//Store data analog from joystick right
int rightX = 0;
int rightY = 0;

float max_position_joystick = 128.0;

float linearX = 0, linearY = 0, angular = 0;
int scale = 1;
void command() {


  if (Ps3.event.button_down.square) {
    // Serial.println("O Shape");
    robot.O();
  } else if (Ps3.event.button_down.cross) {
    // Serial.println("I SHAPE");
    robot.I();
  } else if (Ps3.event.button_down.circle) {
    // Serial.println("L SHAPE");
    robot.L();
  } else if (Ps3.event.button_down.triangle) {
    // Serial.println("J shape");
    robot.J();
  } else if (Ps3.event.button_down.select) {
    // Serial.println("T shape");
    robot.T();
  } else if (Ps3.event.button_down.start) {
    // Serial.println("S shape");
    robot.S();
  } else if (Ps3.event.button_down.l2) {
    // Serial.println("Z shape");
    robot.Z();
  } else if (Ps3.event.button_down.up) {
    // Serial.println("Forward");
    robot.MoveForward(100);
  } else if (Ps3.event.button_down.right) {
    // Serial.println("right");
    robot.MoveRight(100);
  } else if (Ps3.event.button_down.left) {
    // Serial.println("left");
    robot.MoveLeft(100);
  } else if (Ps3.event.button_down.down) {
    // Serial.println("Backaward");
    robot.MoveBackward(100);
  } else {
    // Serial.println(abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly));
    if (abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2) {
      // Serial.print("Moved the left stick:");
      leftX = Ps3.data.analog.stick.lx;
      leftY = Ps3.data.analog.stick.ly;

      //get Linear speed
      linearX = (leftX / max_position_joystick) * robot.sm_max_curve_speed;
      linearY = (leftY / max_position_joystick) * robot.sm_max_curve_speed;

      Serial.println("Linear: " + String(linearX) + "| " + String(linearY));
      robot.sm_velocity_handler(-linearY, -linearX, 0);
      robot.sm_pwm_handler(3);
      robot.MotorDirection();
    } else if (abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2) {
      rightX = Ps3.data.analog.stick.rx;
      rightY = Ps3.data.analog.stick.ry;
  
      //Get angular speed
      angular = (rightX / max_position_joystick) * 10.0;
      // Serial.println("Angular: " + String(angular));

      robot.sm_velocity_handler(0, 0, -angular);
      robot.sm_pwm_handler(2);
      robot.MotorDirection();
      // robot.sm_velocity_handler(linearY, linearX, angular);
      // robot.sm_pwm_handler(3);
      // robot.MotorDirection();
    } else if (Ps3.event.button_up.down || Ps3.event.button_up.up || Ps3.event.button_up.left || Ps3.event.button_up.right) {
      // Serial.println("No click");
      robot.stopSmorphi();
      // robot.sm_reset_M1();
      // robot.sm_reset_M2();
      // robot.sm_reset_M3();
      // robot.sm_reset_M4();
    }
  }
  robot.sm_reset_M1();
  robot.sm_reset_M2();
  robot.sm_reset_M3();
  robot.sm_reset_M4();
}




void notify() {
  //Smorphi Algorithm
  Serial.println(robot.sm_getShape());
  command();

  // //---------------------- Battery events ---------------------
  if (battery != Ps3.data.status.battery) {
    battery = Ps3.data.status.battery;
    Serial.print("The controller battery is ");
    if (battery == ps3_status_battery_charging) Serial.println("charging");
    else if (battery == ps3_status_battery_full) Serial.println("FULL");
    else if (battery == ps3_status_battery_high) Serial.println("HIGH");
    else if (battery == ps3_status_battery_low) Serial.println("LOW");
    else if (battery == ps3_status_battery_dying) Serial.println("DYING");
    else if (battery == ps3_status_battery_shutdown) Serial.println("SHUTDOWN");
    else Serial.println("UNDEFINED");
  }
}

void onConnect() {
  Serial.println("Connected.");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  //Need to change

  Ps3.begin("78:dd:08:08:d9:62");
  // Ps3.begin("98:b6:a1:f5:98:f3");

  Serial.println("Ready.");
  Serial.println(Ps3.isConnected());
  while (!Ps3.isConnected()) {
    //   Serial.println("Not Connect");
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
  Serial.println("Connect");
  robot.BeginSmorphi();
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  // static int sensor_1 = robot.module1_sensor_status(0);
  // Serial.println(sensor_1);
}
