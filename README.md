# arduino-bird-scarer
Code for an Arduino project to scare birds away based upon initial ideas and development by Nick Allen in 2015. It uses several sound files played at random intervals.

This fork of the project uses the latest TMRpcm library and works as found here on the Arduino UNO R3 (and with mods for (Mega and Nano). 

I am running the sketch on an Arduino UNO release 3 board, using an SD Card and it's shield with a photocell and a 5v Class D amplifier with old Radio speakers. 

One benefit of using the TMRpcm library instead of the original SimpleSDAudio library is that it seems less heavy on use of RAM and also allows looping of Audio (WAV) clips which in turn avoids the Clicks/Pops found at the start of each recurrence  of an Audio playback using SimpleSDAudio.  The Author of TMRpcm library also has spent considerable time reducing the initial clicks when playing WAV files, which he says are in part caused by the METADATA in the sound files.  

tMRpcm is to be found here: https://github.com/TMRh20/TMRpcm

Just need to field test the results to scare the birds from my 2020 Cherry Crop!
