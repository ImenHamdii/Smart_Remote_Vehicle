#include <WiFi.h>
#include <WebServer.h>

// Replace with your Wi-Fi network credentials
const char* ssid = "HUAWEI Y9 Prime 2019";
const char* password = "imen1234";

// Motor GPIO Pins
#define ENA   4
#define IN_1  0
#define IN_2  2
#define IN_3  12
#define IN_4  13
#define ENB   15
#define Light 16

// PWM Channel Configurations
#define PWM_CHANNEL_ENA  0
#define PWM_CHANNEL_ENB  1
#define PWM_FREQUENCY    5000
#define PWM_RESOLUTION   8  // 8-bit resolution (0-255)

// Global Variables
int speedCar = 150;  // Default speed (0 to 255)
int speed_low = 60;
WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Initialize motor and light pins
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(Light, OUTPUT);

  // Configure PWM channels
  ledcSetup(PWM_CHANNEL_ENA, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_ENB, PWM_FREQUENCY, PWM_RESOLUTION);

  // Attach PWM channels to motor pins
  ledcAttachPin(ENA, PWM_CHANNEL_ENA);
  ledcAttachPin(ENB, PWM_CHANNEL_ENB);

  // Connect to Wi-Fi
  connectToWiFi();

  // Set up web server routes
  server.on("/", handleRequest);
  server.onNotFound(handleRequest);
  server.begin();
}

void loop() {
  server.handleClient();

  String command = server.arg("State");
  if (command == "F") goForward();
  else if (command == "B") goBack();
  else if (command == "L") goLeft();
  else if (command == "R") goRight();
  else if (command == "W") {
    digitalWrite(Light, HIGH);
    Serial.println("Light ON");
  } else if (command == "w") {
    digitalWrite(Light, LOW);
    Serial.println("Light OFF");
  } else if (command == "S") stopRobot();

  if (server.hasArg("Speed")) {
    int sliderSpeed = server.arg("Speed").toInt();
    speedCar = constrain(sliderSpeed, 0, 255);  // Ensure speed stays within valid range
    Serial.println("Speed updated: " + String(speedCar));
  }
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // Wait until the device connects to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Display local IP assigned by router
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP());
}

void handleRequest() {
  if (server.hasArg("State") || server.hasArg("Speed")) {
    if (server.hasArg("State")) {
      Serial.println("Received State: " + server.arg("State"));
    }
    if (server.hasArg("Speed")) {
      Serial.println("Received Speed: " + server.arg("Speed"));
    }
    server.send(200, "text/html", "Command received");
  } else {
    server.send(200, "text/html", "ESP32 Web Server is running.");
  }
}

void setMotorSpeed(int channel, int speed) {
  ledcWrite(channel, speed);
}

void goForward() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  setMotorSpeed(PWM_CHANNEL_ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  setMotorSpeed(PWM_CHANNEL_ENB, speedCar);
}

void goBack() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  setMotorSpeed(PWM_CHANNEL_ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  setMotorSpeed(PWM_CHANNEL_ENB, speedCar);
}

void goLeft() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  setMotorSpeed(PWM_CHANNEL_ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  setMotorSpeed(PWM_CHANNEL_ENB, speedCar);
}

void goRight() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  setMotorSpeed(PWM_CHANNEL_ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  setMotorSpeed(PWM_CHANNEL_ENB, speedCar);
}

void goForwordRight(){
      
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(PWM_CHANNEL_ENA, speed_low);
 
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      setMotorSpeed(PWM_CHANNEL_ENB, speedCar);
   }

void goForwordLeft(){
      
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(PWM_CHANNEL_ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(PWM_CHANNEL_ENB, speed_low);
  }

void goBackRight(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(PWM_CHANNEL_ENA, speed_low);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(PWM_CHANNEL_ENB, speedCar);
  }

void goBackLeft(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(PWM_CHANNEL_ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(PWM_CHANNEL_ENB, speed_low);
  }

void stopRobot() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  setMotorSpeed(PWM_CHANNEL_ENA, 0);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  setMotorSpeed(PWM_CHANNEL_ENB, 0);
}
