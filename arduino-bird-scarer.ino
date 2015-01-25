#include <SimpleSDAudio.h>
#include <BirdData.h>

const int SD_CARD_CS_PIN = 10;
const int PHOTOCELL_PIN = 0;

const unsigned int DATA_SIZE = 10;
const boolean DEBUG = false;


BirdData data[DATA_SIZE];

// Create static buffer 
#define BIGBUFSIZE (2*512)  // bigger than 2*512 is often only possible on Arduino megas!
uint8_t bigbuf[BIGBUFSIZE];

// helper function to determine free ram at runtime
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void(* resetFunc) (void) = 0;//declare reset function at address 0

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.print("Free Ram: "); Serial.flush();
  Serial.println(freeRam()); Serial.flush();
  
  // Setting the buffer manually for more flexibility
  SdPlay.setWorkBuffer(bigbuf, BIGBUFSIZE); 

  Serial.print("\nInitializing SimpleSDAudio V" SSDA_VERSIONSTRING " ..."); Serial.flush();
  
  // If your SD card CS-Pin is not at Pin 4, enable and adapt the following line:
  SdPlay.setSDCSPin(SD_CARD_CS_PIN);
  
    // Select between SSDA_MODE_FULLRATE or SSDA_MODE_HALFRATE (62.5kHz or 31.25kHz)
  // and the output modes SSDA_MODE_MONO_BRIDGE, SSDA_MODE_MONO or SSDA_MODE_STEREO
  if (!SdPlay.init(SSDA_MODE_HALFRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER)) {
    Serial.println("initialization failed. Things to check:"); Serial.flush();
    Serial.println("* is a card is inserted?"); Serial.flush();
    Serial.println("* Is your wiring correct?"); Serial.flush();
    Serial.println("* maybe you need to change the chipSelect pin to match your shield or module?"); Serial.flush();
    Serial.print("Error code: "); Serial.flush();
    Serial.println(SdPlay.getLastError()); Serial.flush();
    
    delay(10000);
    resetFunc();
    
    // while(1);
  } else {
    Serial.println("Wiring is correct and a card is present."); Serial.flush();
  }
    
  randomSeed(analogRead(1));

  Serial.println("Initializing SD card..."); Serial.flush();
  pinMode(10, OUTPUT);

  loadSounds();
}

void loop() {
  // Day(D) or Night(N)
  char when = 'D'; // 'N'; // null;
  // Determine the amount of light present to evaluate the day vs night settings.
  // We also build in some tolerence if the photocell is missing or broken so the mode defaults to D
  int pPin = analogRead(PHOTOCELL_PIN);
  Serial.print("Photocell (0-1023): "); Serial.flush();
  Serial.println(pPin); Serial.flush();

  if(pPin < 200) {
    when = 'N';
  }

  if(pPin > 30) {
    playRandomWavFile(when);
  }
  // else - It is really dark out, so we won't play any sounds right now. 
  // People want to sleep too. :-)
  
  // random amount of wait time between calls
  unsigned long waitTime = 10000;
  if('D' == when) {
    // 10 - 30 minutes
    waitTime = (random(600,1800) * 1000);
  }
  else if('N' == when){
    // 30 - 60 minutes
    waitTime = (random(1800,3600) * 1000);
  }
  if(DEBUG) {
    // override to 10 seconds
    waitTime = 10000;
  }
  Serial.print("waitTime: "); Serial.flush();
  Serial.println(waitTime); Serial.flush();
  delay(waitTime);
}

void loadSounds() {
  data[0].put("1.wav",5,5,7,4);
  data[1].put("2.wav",7,5,24,2);
  data[2].put("3.wav",8,5,22,2);
  //data[3].put("4.wav",16,10,16,2);  // 4.wav is broken, using 11 instead
  data[3].put("11.wav",16,10,15,3);
  data[4].put("5.wav",8,5,3,8);
  data[5].put("6.wav",2,22,17,2);
  data[6].put("7.wav",8,5,14,3);
  data[7].put("8.wav",17,10,15,3);
  //data[8].put("9.wav",7,5,3,7);  // 9.wav is broken, using 5 instead
  data[8].put("5.wav",7,5,3,8);
  data[9].put("10.wav",4,18,28,2);
  data[10].put("11.wav",18,10,15,3);
/*
  data[0] = &BirdData("blackbird-splutter.wav",5,5,30,2);
  data[1] = &BirdData("hawk1.wav",7,5,30,2);
  data[2] = &BirdData("red-tailed-hawk-scream.wav",8,5,30,2);
  data[3] = &BirdData("robin-distress.wav",16,10,30,2);
  data[4] = &BirdData("blackbird-warning.wav",8,5,30,2);
  data[5] = &BirdData("owl-agitated.wav",2,22,30,2);
  data[6] = &BirdData("red-tailed-hawk2.wav",8,5,30,2);
  data[7] = &BirdData("robin-warning.wav",17,10,30,2);
  data[8] = &BirdData("falcon.wav",7,5,30,2);
  data[9] = &BirdData("owl-contact.wav",4,18,30,2);
  data[10] = &BirdData("robin-alarm.wav",18,10,30,2);
*/
}

void playRandomWavFile(char when) {
  unsigned long rand = random(sumWeights(when));
  unsigned long maxTimesStart = 16;

  unsigned long runningWeights = 0;
  for(unsigned int w = 0; w < DATA_SIZE; w++) {
    runningWeights += data[w].getWeight(when);
    if(rand < runningWeights) {
      unsigned int maxTimes = (maxTimesStart+data[w].getMinTimes());
      playSound(data[w].getWavFile(), data[w].getDuration(), random(data[w].getMinTimes(), maxTimes));
      break;
    }
  }
}

long sumWeights(char when) {
  unsigned long weights = 0;
  for(unsigned int w = 0; w < DATA_SIZE; w++) {
    weights+=data[w].getWeight(when);
  }
  return weights;
}

void playSound(char* wavFile, int duration, long times){
  Serial.print("Playing ");
  Serial.print(wavFile);
  Serial.print(" ");
  Serial.print(times);
  Serial.println(" times. "); Serial.flush();
  Serial.print("duration: ");
  Serial.println(duration); Serial.flush();
    
  for(unsigned long i = 0; i < times; i++) {
    Serial.println(i); Serial.flush();
    // load the sound file
    SdPlay.setFile(wavFile);
    SdPlay.worker();
    // play the sound file
    SdPlay.play();
    Serial.flush();
    delay(duration * 1000);
    SdPlay.stop();
    int sdStatus = SdPlay.getLastError();
    if(sdStatus != 0) {
      Serial.print("SD Card Error Status: ");
      Serial.println(sdStatus); Serial.flush();
      resetFunc();
    }
  }
  
  Serial.print("Free Ram: "); Serial.flush();
  Serial.println(freeRam()); Serial.flush();

}

