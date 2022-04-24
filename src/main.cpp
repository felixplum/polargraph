
#include <consts.h>
#include <secrets.h>
#include <Arduino.h>
// #include <string.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <stepper_ctrl.h>
#include <trajectory_ctrl.h>

ESP8266WebServer server(80);
TrajectoryController trajectory_controller;
PointBuffer* buffer = trajectory_controller.getBuffer();

void blink(int num, int length_ms) {
  for (int i = 0; i < num; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(length_ms);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(length_ms);
  }
}


void handleRoot() {
}

bool isValid() {
    if (server.hasArg("plain") == false){
          server.send(200, "text/plain", "Body not received");
          return false;
    }
    return true;
}

void handleIncrementAngle() {
      if (!isValid()) return;
        trajectory_controller.steppers[0].incrementAngle(server.arg("plain").toFloat(), 1000000);
      String ret = "OK";
      server.send(400, "text/plain", ret);
      // Serial.println(ret);
}

void handleSetSpeed() {
 
      if (!isValid()) return;
      trajectory_controller.setMaxAngularVelocity(server.arg("plain").toFloat());
      String ret = "OK";
      //ret.concat(angle_actual);
      server.send(200, "text/plain", ret);
      // Serial.println(ret);
}

void handleSetInit() {
 
      if (!isValid()) return;
      String msg = server.arg("plain");
      int delim_idx = msg.indexOf(";");
      if (delim_idx == -1) {
        server.send(400, "text/plain", "");
        return;
      }
      String x0, y0;
      x0 = msg.substring(0, delim_idx);
      y0 = msg.substring(delim_idx+1, msg.length());
      trajectory_controller.init(x0.toFloat(), y0.toFloat());
      server.send(200, "text/plain", "OK");
      // Serial.println(x0);
      // Serial.println(y0);
}

void handleGetStatus() {
 
      if (!isValid()) return;
      Point pnt = trajectory_controller.getCurrentLengths();
      String ret = "l0: ";
      ret.concat(pnt.x);
      ret.concat(" l1: ");
      ret.concat(pnt.y);
      server.send(200, "text/plain", ret);
}

void handleAddPoints() {
      if (!isValid()) return;
      const char* input_str = server.arg("plain").c_str();
      
      int num_points_before = buffer->num_points;
      buffer->addPointsFromList(input_str);
      int num_points_added = buffer->num_points - num_points_before;
      int num_points_free = buffer->NUM_POINTS_MAX - buffer->num_points;
      // trajectory_controller.is_active = true;
      String ret = "";
      ret.concat(num_points_added);
      ret.concat(";");
      ret.concat(num_points_free);
      server.send(200, "text/plain", ret);
}

void setup(void){
  // Pin Modes
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_PULSE_LM, OUTPUT);

  pinMode(PIN_DIR_LM, OUTPUT);
  pinMode(PIN_PULSE_RM, OUTPUT);
  pinMode(PIN_DIR_RM, OUTPUT);


  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);

  // Init server
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  // Define callbacks
  server.on("/", handleRoot);
  server.on("/incrementAngle", handleIncrementAngle);
  server.on("/getStatus", handleGetStatus);
  server.on("/setInit", handleSetInit);
  server.on("/setSpeed", handleSetSpeed);
  server.on("/addPoints", handleAddPoints);

  server.begin();
  Serial.println("HTTP server started");
  blink(3, 500);

  trajectory_controller.init(0,0);
  // trajectory_controller.is_active = true;
}

void loop(void){
  server.handleClient();
  trajectory_controller.update();
  // motor_left.updateStepper();
}
