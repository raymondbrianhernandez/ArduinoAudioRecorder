/*
  Raymond Brian D. Hernandez
  October 29, 2016
*/

#include <SD.h>
#include <SPI.h>
#include <Button.h>
#include <TMRpcm.h>

//#define SD_ChipSelectPin 53  // example uses hardware SS pin 53 on Mega2560
#define SD_ChipSelectPin 4  // using digital pin 4 on arduino nano 328, can use other pins

int recPressed = 0;
int playPressed = 0;

TMRpcm audio;   // create an object for use in this sketch 

Button Stop = Button(5, true, true, 25); // create button stop using button library

void setup() {
  audio.setVolume(5);
  audio.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  
  pinMode(2, OUTPUT); // LED Start
  pinMode(3, OUTPUT); // LED Stop
  pinMode(7, INPUT_PULLUP); // Start/Stop Playback Pin 7
  pinMode(6, INPUT_PULLUP); // Start Recording Pin 2
  //pinMode(3, INPUT_PULLUP); // Stop Recording Pin 3

  Serial.begin(9600);

  if (!SD.begin(SD_ChipSelectPin)) {  
    return;
  }
  else{
    Serial.println("SD OK"); 
  }
  
  audio.CSPin = SD_ChipSelectPin; // Select Pin
}


void loop() {
 
  if (digitalRead(2) == LOW && recPressed == 0){ 
    audio.startRecording("test.wav",24000,A5); // Record at 24khz sample rate on pin A5
    digitalWrite(2, HIGH); 
    digitalWrite(3, LOW);
    recPressed = 1;
  }
    
  if (digitalRead(7) == LOW && playPressed == 0){
    audio.play("test.wav"); // Play the recording 
    digitalWrite(3, LOW); 
    digitalWrite(2, LOW); 
    playPressed = 1;
  }
  
  Stop.read();
  
  if (Stop.isPressed()) { // changed from 5 to 2
    if(recPressed == 1){
      audio.stopRecording("test.wav"); // Stop Recording
      recPressed = 0;
    }  
    else if(playPressed == 1){
      audio.stopPlayback(); // Stop all playback
      playPressed = 0;
    }
    //digitalWrite(9, LOW);   
    digitalWrite(3, LOW); 
    digitalWrite(2, HIGH);
  } 
  else if (Stop.pressedFor(5000)){
    SD.remove("test.wav"); // Delete file if stop button held down for 5 seconds.  
  }  
}
