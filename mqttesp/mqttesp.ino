  #include <ESP8266WiFi.h>
  #include <PubSubClient.h>
  #include <sstream>
  
  const char* ssid = "bssm_free";
  const char* password =  "bssm_free";

  const char* mqttServer = "broker.hivemq.com";
  const int mqttPort = 1883;
  // const char* mqttUser = "yourMQTTuser";
  // const char* mqttPassword = "yourMQTTpassword";
  const char* topic_pub = "bssmheavn/2";
  const char* topic_sub = "bssmheavn/#";

  const int trigPin = D1;
  const int echoPin = D2;
  long duration;
  float distanceCm;
  float distanceInch;
  //define sound speed in cm/uS
  #define SOUND_SPEED 0.034
  #define CM_TO_INCH 0.393701 

  int count;
  #define UPDATE_INTERVAL 5000L
  unsigned long prevUpdateTime = 0L;

  WiFiClient espClient;
  PubSubClient mqttClient(espClient);


  void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
  
    Serial.print("Message: ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }

    Serial.println();
    Serial.println("-----------------------");
  }


  void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
  
    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.setCallback(callback);
  
    while (!mqttClient.connected()) {
      Serial.println("Connecting to MQTT...");
      
      if (mqttClient.connect("minse")) {
        Serial.println("connected");
      } else {
        Serial.print("failed with state ");
        Serial.print(mqttClient.state());
        delay(2000);
      }
    }

    mqttClient.subscribe(topic_sub);
    mqttClient.publish(topic_pub, "ESP32 logged in");
    prevUpdateTime = millis();

    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  }

  void loop() {
    //시분할 서브루틴
    mqttClient.loop();

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculate the distance
    distanceCm = duration * SOUND_SPEED/2;//초음파센서
    int height = 29-(int)distanceCm;
    if(height>1 && count==0){
      Serial.print("Distance (cm): ");
      Serial.println(height);
      unsigned long currentTime = millis();
      if(currentTime > prevUpdateTime + UPDATE_INTERVAL) {
        int i = 0;
        if(height>15){
          Serial.println("adult");
          i = 0;
        }
        else{
          Serial.println("child");
          i = 1;
        }
        std::stringstream st;
        st<<i;
        mqttClient.publish(topic_pub, st.str().c_str());
        prevUpdateTime = currentTime;
      }
      if(height>15){
      Serial.println("adult");
      }
      else{
      Serial.println("child");
      }
      count = 1;
    }
    else if(count == 1){
      count = 0;
    }

    delay(200);
  }