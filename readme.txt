README - Assignment 2: Live Yes/No Detection

Name: Bhavya Negi

Roll No: 230101025
 
WHAT CHANGED FROM ASSIGNMENT 1

-------------------------------

Assignment 1 (YesNoDetection.cpp) read a pre-recorded amplitude text file

from disk using readAmplitudeData(), then ran it through the detection

pipeline.
 
Assignment 2  replaces that file-reading

step with live microphone input:
 
1. recordAudio()

   - Adapted from live_recording.cpp (StartRecord()).

   - Opens the default microphone via waveInOpen(), records 3 seconds of

     16 kHz / 16-bit / mono audio directly into an in-memory buffer

     (waveBuffer), instead of reading samples from a text file.
 
2. main()

   - Calls recordAudio() first.

   - Copies the captured samples from waveBuffer into a vector<int>,

     the same data type the rest of the original pipeline expects.

   - Everything after this point is UNCHANGED from Assignment 1:

     computeNoiseFloor(), segmentWord(), calculateEnergy(),

     calculateZCR(), and classifyWord() all run exactly as before,

     just on live audio instead of file data.
 
3. saveToTextFile() [new, optional]

   - Writes the captured samples to recorded_input.txt after each run.

   - Not required for detection to work; included only so a recording

     can be inspected/debugged after the fact.
 
4. playAudio()

   - Adapted from live_recording.cpp (PlayRecord()).

   - Plays back the 3-second recording after classification, so you can

     confirm what was actually captured.
 
WHAT DID NOT CHANGE

--------------------

- Feature extraction (energy, ZCR) and the classification rule

  (ZCR >= 0.17 -> "Yes", else "No") are identical to Assignment 1.

- Frame size (400 samples / 25 ms) and frame shift (160 samples / 10 ms)

  are unchanged.
 
WHAT IS NOT INCLUDED

---------------------

- live_recording.cpp and the original YesNoDetection.cpp are not needed

  in this submission; their relevant logic has been merged directly into

 YesNoDetection_Live.cpp, which is self-contained.

- ambience.txt is not required, since calculateDCShift() (the only

  function that used it) is not called anywhere in main().
 
HOW TO RUN

----------

1. Build as a Windows Console App in Visual Studio, linking winmm.lib

   (already declared via #pragma comment in the source).

2. Run the executable.

3. When prompted, speak "Yes" or "No" clearly within the 3-second window.

4. The program prints the noise floor, average energy, average ZCR, and

   the final Yes/No decision, then plays the recording back.
 
THRESHOLD UPDATE (Assignment 1 -> Assignment 2)

------------------------------------------------

The ZCR threshold (0.17) was tuned on the Assignment 1 pre-recorded

files. Live microphone input produced different ZCR values in testing

(observed avgZCR of approximately 0.11), which fell below 0.17 and

caused every input to be classified as "No".
 
The threshold in classifyWord() has been updated from 0.17 to 0.11 to

reflect this. Note: this value was set based on a single observed

data point and has not yet been validated across multiple "Yes" and

"No" samples, so it may need further tuning once more live test runs

are done.
 