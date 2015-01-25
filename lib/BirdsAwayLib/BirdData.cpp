#include "BirdData.h" //include the declaration for this class
    
//<<constructor>> setup BirdData
BirdData::BirdData(){
}

BirdData::BirdData(char* wavFile, int dayWeight, int nightWeight, int duration, int minTimes){
  _wavFile = wavFile;
  _dayWeight = dayWeight;
  _nightWeight = nightWeight;
  _duration = duration;
  _minTimes = minTimes;
}

//<<destructor>>
BirdData::~BirdData(){/*nothing to destruct*/}
 
void BirdData::put(char* wavFile, int dayWeight, int nightWeight, int duration, int minTimes){
  _wavFile = wavFile;
  _dayWeight = dayWeight;
  _nightWeight = nightWeight;
  _duration = duration;
  _minTimes = minTimes;
}

// get _wavFile
char* BirdData::getWavFile(){
  return _wavFile;
}

// set _wavFile
void BirdData::setWavFile(char* wavFile){
  _wavFile = wavFile;
}

// get _dayWeight
int BirdData::getWeight(char when){
  if('N' == when) {
    return _nightWeight;
  }
  else {
    return _dayWeight;
  }
}

// set _dayWeight
void BirdData::setDayWeight(int dayWeight){
  _dayWeight = dayWeight;
}

// set _nightWeight
void BirdData::setNightWeight(int nightWeight){
  _nightWeight = nightWeight;
}

// get _duration
int BirdData::getDuration(){
  return _duration;
}

// set _duration
void BirdData::setDuration(int duration){
  _duration = duration;
}

// get _minTimes
int BirdData::getMinTimes(){
  return _minTimes;
}

// set _minTimes
void BirdData::setMinTimes(int minTimes){
  _minTimes = minTimes;
}
