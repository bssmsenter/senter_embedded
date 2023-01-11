const int trigPin = D1;
const int echoPin = D2;
long duration;
float distanceCm;
float distanceInch;
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701 
void setup()
{
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop(){

  // Clears the trigPin
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
  if(distanceCm <=200){
    Serial.print("Distance (cm): ");
    int height = 200-(int)distanceCm;
    Serial.print(height);
    Serial.print(":");
    Serial.println(distanceCm);
   
  }
  else{
    Serial.println("out");
  }
  delay(500);
}
