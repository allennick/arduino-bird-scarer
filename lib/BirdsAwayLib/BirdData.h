/*Library by Xander Industries 2014 - 2015

Contributors:

  Nick Allen

*/

#ifndef BirdData_h
#define BirdData_h

// Include this if you need access to Arduino controls
#include <Arduino.h>

class BirdData {
  private:
    char* _wavFile;
    volatile int _dayWeight;
    volatile int _nightWeight;
    volatile int _duration;
    volatile int _minTimes;
  public:
    BirdData();
    BirdData(char* wavFile, int dayWeight, int nightWeight, int duration, int minTimes);
    ~BirdData();
    void put(char* wavFile, int dayWeight, int nightWeight, int duration, int minTimes);
    char* getWavFile();
    int getWeight(char when);
    int getDuration();
    int getMinTimes();
    void setWavFile(char* wavFile);
    void setDayWeight(int dayWeight);
    void setNightWeight(int dayWeight);
    void setDuration(int duration);
    void setMinTimes(int minTimes);
};
 
#endif