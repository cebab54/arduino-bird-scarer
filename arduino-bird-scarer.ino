#include <SD.h>             // need to include the SD library
#include <SPI.h>
#include <TMRpcm.h>         // and also need to include the TMRpcm Audio library
#include <pcmConfig.h>      // edit pcmConfig.h in Libraries/TMRpcm folder to set specific options e.g. uncomment DISABLE_SPEAKER2 and HANDLE_TAGS and then re-compile
#include <BirdData.h>

#define SD_ChipSelectPin 4      // amend digital pin number if applicable
#define LDR_ChipSelectPin 0     // amend analogue pin number if applicable
#define DIGI_LineoutSelectPin 9 // choose one of pins 5,6,11 or 46 on the Mega, or pin 9 on Uno, Nano, etc
#define AUDIO_Volume 1          // amend output pin volume to suit your audio setup (range is 0-7). generally found that 3 is maximum without distortion or too much noise from an Amplifier
//
// SD Card should be max 8Gb and formatted to FAT16
// Sound clips should be saved using Audacity (Freeware) as mono 'wav' clips sampled at 32000Hz
// Sound clips should NOT contain any METADATA (Reset and Clear all fields) this helps to avoid click/pop at start of playback
// In Audacity, "File/Export/Export as WAV". File should be renamed as a number and the filetype deleted/removed
// Export clips using dropdown option "Save as Type" set to: 'Other uncompressed files' and with
// Header set to: "WAV (Microsoft)"; and Encoding set to: "unsigned 8-bit PCM".
//
Sd2Card card;               // SD Card should not be greater than 4Gb, if it is createa 4GB Partition and format that using FAT16 always using 'Overwrite format' (NOT Quick/Fast Format) preferably formatted using SD Association's "SD Card Formatter" (Freeware)
TMRpcm audio;

const unsigned int DATA_SIZE = 11;
const boolean DEBUG = false;
BirdData data[DATA_SIZE];

// helper function to determine free ram at runtime
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void(* resetFunc) (void) = 0; //declare reset function at address 0

void setup() {
  audio.speakerPin = DIGI_LineoutSelectPin;
  audio.setVolume(AUDIO_Volume);  // reset output volume
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  #if defined (DISABLE_SPEAKER2)
    Serial.println("SP2 is Disabled");
  #endif
  #if defined (HANDLE_TAGS)
    Serial.println("HANDLE_TAGS is enabled");
  #endif
  Serial.print("Free Ram: ");
  Serial.println(freeRam()); Serial.flush();

  Serial.println("Initializing SD card...");
  // if (!card.init(SPI_HALF_SPEED, SD_ChipSelectPin)) {
  // check if an SD card is present and can be initialized:
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD Card initialization failed:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  } else {
    Serial.print("Card type found is: ");
    switch (card.type()) {
      case SD_CARD_TYPE_SD1:
        Serial.println("SD1");
        break;
      case SD_CARD_TYPE_SD2:
        Serial.println("SD2");
        break;
      case SD_CARD_TYPE_SDHC:
        Serial.println("SDHC");
        break;
      default:
        Serial.println("Unknown");
    }
  }

  randomSeed(analogRead(LDR_ChipSelectPin)); // READ lIGHT LEVEL
}

void loop(void) {

  // Next determine the amount of light present to evaluate the day vs night settings.
  // We also build in some tolerance if the photocell is missing or broken so default to T
  // Day(D) or Twighlight(T) or Night(N)
  char when = 'T'; // set default to Twilight (Dusk/Dawn) not to be too intrusive at night

  int LPin = analogRead(LDR_ChipSelectPin);
  Serial.print("Photocell Reading (0-1023) is ");
  Serial.println(LPin); Serial.flush();
  
  // set random amount of wait time between birdcall clip loops
  unsigned long waitTime = 10000; // initially set to 10 seconds
  if (LPin > 200) {
    when = 'D';
    // Daytime set 5 - 25 minutes
    waitTime = (random(300, 1500) * 1000);
    loadSoundsDay();  // Load Daytime defined sound clips
  }
  else if (LPin > 30) {
    when = 'T';
    // Twilight or Dusk/Dawn set 30 - 60 minutes
    waitTime = (random(1800, 3600) * 1000);
    loadSoundsDawnDusk(); // Load DawnDusk defined sound clips
  }
  else {
    when = 'N';
    // Night Time set 90 - 160 minutes
    waitTime = (random(5400, 9600) * 1000);
    loadSoundsNight(); // Load Night Time defined sound clips
  }
  if (DEBUG) {
    // override to 10 seconds//
    waitTime = 10000;
  }


  playRandomWavFile(when);
  int mins = int(waitTime / 60000);
  int secs = int((waitTime - (mins * 60000)) / 1000);
  Serial.print("Waiting for: ");
  Serial.print(mins);
  Serial.print(" min(s) ");
  Serial.print(secs);
  Serial.println(" sec{s)");
  Serial.println("------------------=-----------------");
  Serial.flush();
  delay(waitTime);

  Serial.print("Free Ram: ");
  Serial.println(freeRam()); Serial.flush();
  return;
}

void loadSoundsDay() {
  // edit to load Daytime sound clips, see next line and the details in files.xls
  // (Wav name [mustbe an integer] excl extension, Daytime Weighting, Nighttime Weighting, Clip duration in seconds, minimum number of repeats)
  data[0].put("1",18,5,5,6);
  data[1].put("2",12,5,21,2);
  data[2].put("3",12,5,21,2);
  data[3].put("4",12,5,1,20);
  data[4].put("5",18,5,13,4);
  data[5].put("6",9,5,13,3);
  data[6].put("14",14,8,10,5);
  data[7].put("15",15,8,83,1);
  data[8].put("20",10,5,86,1);
  data[9].put("21",18,5,41,2);
//  data[10].put("22",18,5,58,3);
  data[10].put("26",18,18,22,2);
  return;
}

void loadSoundsDawnDusk() {
  // edit to load Dawn/Dusk sound clips, see next line and the details in files.xls
  // (Wav name [mustbe an integer] excl extension, Daytime Weighting, Nighttime Weighting, Clip duration in seconds, minimum number of repeats)
  data[0].put("6",5,18,13,3);
  data[1].put("14",18,5,10,5);
  data[2].put("15",5,18,83,1);
  data[3].put("18",5,15,65,1);
  data[4].put("19",7,16,89,1);
  data[5].put("5",18,5,68,2);
  data[6].put("22",5,15,58,1);
  data[7].put("24",5,18,32,3);
  data[8].put("25",5,18,65,1);
  data[9].put("15",5,18,83,1);
//  data[10].put("22",5,15,58,1);
  data[10].put("26",18,18,22,2);
  return;
}

void loadSoundsNight() {
  // edit to load Night time sound clips, see next line and the details in files.xls
  // (Wav name [mustbe an integer] excl extension, Daytime Weighting, Nighttime Weighting, Clip duration in seconds, minimum number of repeats)
  data[0].put("6",5,18,14,3);
  data[1].put("18",5,15,65,2);
  data[2].put("19",7,16,89,1);
  data[3].put("23",5,18,4,4);
  data[4].put("24",5,18,32,2);
  data[5].put("25",5,18,68,1);
  data[6].put("6",5,18,14,4);
  data[7].put("18",5,15,65,1);
  data[8].put("19",5,18,89,1);
  data[9].put("24",5,18,32,3);
//  data[10].put("25",5,18,65,1);
  data[10].put("26",18,18,22,2);
  return;
}

void playRandomWavFile(char when) {
  unsigned long maxTimesStart = 6;
  unsigned long rand = random(sumWeights(when));
  unsigned long runningWeights = 0;
  if (when == 'N') {
    // repeat less at night
    maxTimesStart = 2;
  }
  for (unsigned int w = 0; w < DATA_SIZE; w++) {
    runningWeights += data[w].getWeight(when);
    if (rand < runningWeights) {
      unsigned int maxTimes = (maxTimesStart + data[w].getMinTimes());
      playSound(data[w].getWavNr(), data[w].getDuration(), random(data[w].getMinTimes(), maxTimes), when);
      break;
    }
  }
  return;
}

long sumWeights(char when) {
  unsigned long weights = 0;
  for (unsigned int w = 0; w < DATA_SIZE; w++) {
    weights += data[w].getWeight(when);
  }
  return weights;
}

void playSound(char* wavNr, long duration, long times, char when) {
  duration = duration * 1000;  // set length of clip as duration in Milliseconds
  Serial.print("Output volume is set to: ");
  Serial.println(AUDIO_Volume);
  Serial.flush();
  //
  // if using active speaker or amplifier use 100nf and 100K resistor in series from digital pin output connected to audio line input
  // FRom Arduino -------||-------\/\/\/\/-----o Line in O---\/\/\/\/----GND
  //       100Nf     8K ohms                    2K ohms
  //
  // Arduino voltage = 5v > Line in on Amp = 1v nominal
  //
  // Use pins 9 and 10 to feed separate line inputs to two channel amplifier, each channel wired as above
  // If pin 10 is used then make sure that SPEAKER2 is not disabled in pcmConfig.h in the TMRpcm Library
  // Better use a 16Bit DAC to create an analogue signal(s) to feed amplifier
  //

  wavNr = "26";
  times = 2;
  duration = 22000;
  
  if (when != 'N') {  // if Night sounds required then delete this line 227 and lines 256-260 inclusive
    Serial.print("Playing ");
    Serial.print(wavNr);
    Serial.print(" as a wav file.");
    Serial.print(" ");
    Serial.print(times);
    Serial.println(" time(s); ");
    Serial.print("Sound clip duration is ");
    Serial.print(duration / 1000);
    Serial.println(" seconds.");
//    Serial.print("Clip has been randomly selected from the '");
//    Serial.print(when);
//    Serial.println("' dataset.");
    Serial.flush();
    audio.loop(1);  // set loop play function on, this avoids the pop that can otherwise occur between the clips
    audio.play(wavNr);  // start loop playing
    for (unsigned long i = 0; i < times; i++) {
      // print progress of timing loop
      Serial.print("Looping - this clip is no. ");
      Serial.print(i + 1);
      Serial.print(" of ");
      Serial.println(times);
      Serial.flush();
      delay(duration); // wait for clip to finish before continuing
    }
    delay(100);
    audio.loop(0);  // set play loop function off
    Serial.println("Now finished current clip output.");
    Serial.flush();
    delay(100);
  } else {
    Serial.println("Night mode: No sounds to loop.");
    Serial.flush();
    delay(100);
  }
  return;
}
