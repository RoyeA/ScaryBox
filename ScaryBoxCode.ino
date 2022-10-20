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
    mp3.setVolume(15);
    mp3.playMp3FolderTrack(12);            
    myServo.write(0);
    track = 1;
    delay(300);
  }
}
void playScreem(){
  if ( track != 2 ){
    mp3.setVolume(20);
    mp3.playMp3FolderTrack(10);    
    myServo.write(90);
    track = 2;
    delay(300);        
  }
}
void playScaryMusic(){
  if ( track != 3){
    mp3.setVolume(17);
    mp3.playMp3FolderTrack(11);
    track=3;
    myServo.write(45);
    delay(300);
  }
}
void stopMusic(){
  if ( track != 4 ){
    mp3.stop();
    track=4;
    myServo.write(0);
    delay(300);
  }
}
void setup() 
{  
  mp3.begin();  
  // Set the UltraSonic sensor pins.
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  

  // Attach Servo to ServoPin
  myServo.attach(ServoPin);
}
#define BUFF 1
#define NEAR 2.5      // Value in inches
#define IN_RANGE 5.0  // Value in inches
#define FAR 7.0       // Value in inches

void loop() 
{
  float distance = distanceFT();
  if ( distance <= NEAR ){
    playScreem();
  }else if ( distance <= IN_RANGE && distance > (NEAR + BUFF )){
    playScaryMusic();
  }else if ( distance <= FAR && distance > ( IN_RANGE + BUFF ) ){
    playNonScrayMusic();
  }else if ( distance > (FAR+BUFF) ) {
    stopMusic();
  }  
  mp3.loop();
  delay(10);
}
