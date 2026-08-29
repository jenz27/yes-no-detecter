README - : Live Yes/No Detection


 
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


 
HOW TO RUN

----------

1. Build as a Windows Console App in Visual Studio, linking winmm.lib

   (already declared via #pragma comment in the source).

2. Run the executable.

3. When prompted, speak "Yes" or "No" clearly within the 3-second window.

4. The program prints the noise floor, average energy, average ZCR, and

   the final Yes/No decision, then plays the recording back.
 


------------------------------------------------




 
