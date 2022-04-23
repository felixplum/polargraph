
#include <consts.h>
#include <secrets.h>
#include <Arduino.h>
// #include <string.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// #include <stepper_ctrl.h>
#include <trajectory_ctrl.h>

ESP8266WebServer server(80);
TrajectoryController trajectory_controller;
// StepperController motor_left;


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

// void handleIncrementAngle() { //Handler for the body path
 
//       if (server.hasArg("plain") == false){ // Check if body received
//             server.send(200, "text/plain", "Body not received");
//             return;
//       }
//       // motor_left.incrementAngle(server.arg("plain").toFloat());
//       String ret = "OK";
//       server.send(200, "text/plain", ret);
//       Serial.println(ret);
// }

void handleSetSpeed() { //Handler for the body path
 
      if (server.hasArg("plain") == false){ // Check if body received
            server.send(200, "text/plain", "Body not received");
            return;
      }
      trajectory_controller.setMaxAngularVelocity(server.arg("plain").toFloat());
      String ret = "OK";
      //ret.concat(angle_actual);
      server.send(200, "text/plain", ret);
      // Serial.println(ret);
}

void handleAddPoints() {
      if (server.hasArg("plain") == false){ // Check if body received
            server.send(200, "text/plain", "Body not received");
            return;
      };
      // trajectory_controller.is_active = false;

      const char* input_str = server.arg("plain").c_str();
      PointBuffer* buffer = trajectory_controller.getBuffer();
      int num_points_before = buffer->num_points;
      buffer->addPointsFromList(input_str);
      int num_points_added = buffer->num_points - num_points_before;

      String ret = "";
      ret.concat(num_points_added);
      //ret.concat(angle_actual);
      server.send(200, "text/plain", ret);
      // Serial.println(ret);
      // trajectory_controller.is_active = true;

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
  // server.on("/incrementAngle", handleIncrementAngle);
  server.on("/setSpeed", handleSetSpeed);
  server.on("/addPoints", handleAddPoints);

  server.begin();
  Serial.println("HTTP server started");
  blink(3, 500);

  // Test logic to fill buffer
  // delay(1000);
  // PointBuffer* buffer = trajectory_controller.getBuffer();
  // for (int i = 0; i < 1000; i++) {
  //   Point pnt;
  //   pnt.x = (float)i / 1000.;
  //   pnt.y = 0.5*(1+sin(pnt.x*2*3.141));
  //   pnt.z = 0;
  //   buffer->addPoint(pnt);
  // }
  trajectory_controller.init(0,0);
  trajectory_controller.is_active = true;
}

void loop(void){
  server.handleClient();
  trajectory_controller.update();
}
