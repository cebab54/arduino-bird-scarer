#include "BirdData.h" //include the declaration for this class
    
//<<constructor>> setup BirdData
BirdData::BirdData(){
}

BirdData::BirdData(char* wavNr, int dayWeight, int nightWeight, int duration, int minTimes){
  _wavNr = wavNr;
  _dayWeight = dayWeight;
  _nightWeight = nightWeight;
  _duration = duration;
  _minTimes = minTimes;
}

//<<destructor>>
BirdData::~BirdData(){/*nothing to destruct*/}
 
void BirdData::put(char* wavNr, int dayWeight, int nightWeight, int duration, int minTimes){
  _wavNr = wavNr;
  _dayWeight = dayWeight;
  _nightWeight = nightWeight;
  _duration = duration;
  _minTimes = minTimes;
}

// get _wavNr
char* BirdData::getWavNr(){
  return _wavNr;
}

// set _wavNr
void BirdData::setWavNr(char* wavNr){
  _wavNr = wavNr;
}

// get _dayWeight
int BirdData::getWeight(char when){
  if('N' == when) {
    return _nightWeight;
  } else {
  // either day or dusk should return dayweight
    return _dayWeight;
  }
}

// set _dayWeight (Day or Dusk)
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
