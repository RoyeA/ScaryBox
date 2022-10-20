#include <Servo.h>
#include <SoftwareSerial.h>
#include <DFMiniMp3.h>

#define Echo 4
#define Trig 5
#define ServoPin 3

class Mp3Notify;
typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3;



// return the distance in CM
float distanceFT() {
  static float lastDist = -1;
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float distance = pulseIn(Echo, HIGH);  
  if ( lastDist == -1 )
    lastDist = distance;
  else if ( abs(lastDist - distance ) <= lastDist * .1 )
    lastDist = distance;
  else if ( lastDist > distance )
    lastDist = lastDist*.9;
  else 
    lastDist = lastDist*1.1;
  return lastDist / 1740.0 ; // return distance in FT;

} 

class Mp3Notify
{
public:  
  static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode)
  {}
  static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track)
  {
    Serial.println("OnPlayFinished");
    mp3.playMp3FolderTrack(track);   
  }
  static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  {}
  static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  {}
  static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
  {}
};


SoftwareSerial secondarySerial(10, 11); // RX, TX - the MP3 player pins
// instance a DFMiniMp3 object, 
DFMiniMp3<SoftwareSerial,Mp3Notify> mp3(secondarySerial);

// instance of Servo.
Servo myServo;

int track = -1;
void playNonScrayMusic(){
  if ( track != 1){
    myServo.write(0);
    delay(500);
    mp3.setVolume(18);
    mp3.playMp3FolderTrack(1);                
    track = 1;
    
  }
}
void playScreem(){
  if ( track != 2 ){
    myServo.write(90);
    delay(500);      
    mp3.setVolume(18);
    mp3.playMp3FolderTrack(10);        
    track = 2;
      
  }
}
void stopMusic(){
  if ( track != 4 ){
    myServo.write(0);
    delay(500);
    mp3.stop();
    track=4;    
    
  }
}
void setup() 
{  
  // Serial.begin(9600);
  // Serial.println("Initializing...");
  mp3.begin();  
  // Set the UltraSonic sensor pins.
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  

  // Attach Servo to ServoPin
  myServo.attach(ServoPin);
}
#define IN_RANGE 4.0  // Value in FT

void loop() 
{
  float distance = distanceFT();
  // Serial.println(distance);
  if ( distance <= IN_RANGE){
    playScreem();
  }else{
    playNonScrayMusic();
  }  
  // delay(100);
  mp3.loop();  
}
