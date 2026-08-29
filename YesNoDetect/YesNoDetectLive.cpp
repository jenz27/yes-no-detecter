// name - Bhavya Negi

// roll no - 230101025

// Assignment 2: Live microphone recording + Yes/No detection
 
#include <iostream>

#include <vector>

#include <cmath>

#include <string>

#include <windows.h>

#include <mmsystem.h>
 
#pragma comment(lib, "winmm.lib")
 
using namespace std;
 
 
// ---------------- CONSTANTS ----------------
 
const int SAMPLE_RATE = 16000;

const int RECORD_SECONDS = 3;
 
const int NUM_SAMPLES =

    SAMPLE_RATE * RECORD_SECONDS;
 
const int FRAME_SIZE = 400;

const int FRAME_SHIFT = 160;
 
 
// Microphone buffer

short waveBuffer[NUM_SAMPLES];
 
 
// ---------------- RECORD AUDIO ----------------
 
bool recordAudio()

{

    HWAVEIN hWaveIn;
 
    WAVEFORMATEX format;
 
    format.wFormatTag = WAVE_FORMAT_PCM;

    format.nChannels = 1;

    format.nSamplesPerSec = SAMPLE_RATE;

    format.nAvgBytesPerSec = SAMPLE_RATE * 2;

    format.nBlockAlign = 2;

    format.wBitsPerSample = 16;

    format.cbSize = 0;
 
 
    // Open microphone
 
    MMRESULT result =

        waveInOpen(
&hWaveIn,

            WAVE_MAPPER,
&format,

            0,

            0,

            CALLBACK_NULL

        );
 
 
    if (result != MMSYSERR_NOERROR)

    {

        cout << "ERROR: Could not open microphone."
<< endl;
 
        return false;

    }
 
 
    // Create recording header
 
    WAVEHDR waveHeader;
 
    waveHeader.lpData =

        (LPSTR)waveBuffer;
 
    waveHeader.dwBufferLength =

        NUM_SAMPLES * sizeof(short);
 
    waveHeader.dwBytesRecorded = 0;
 
    waveHeader.dwUser = 0;
 
    waveHeader.dwFlags = 0;
 
    waveHeader.dwLoops = 0;
 
 
    // Prepare header
 
    result =

        waveInPrepareHeader(

            hWaveIn,
&waveHeader,

            sizeof(WAVEHDR)

        );
 
 
    if (result != MMSYSERR_NOERROR)

    {

        cout << "ERROR: Could not prepare recording."
<< endl;
 
        waveInClose(hWaveIn);
 
        return false;

    }
 
 
    // Add buffer
 
    result =

        waveInAddBuffer(

            hWaveIn,
&waveHeader,

            sizeof(WAVEHDR)

        );
 
 
    if (result != MMSYSERR_NOERROR)

    {

        cout << "ERROR: Could not add recording buffer."
<< endl;
 
        waveInUnprepareHeader(

            hWaveIn,
&waveHeader,

            sizeof(WAVEHDR)

        );
 
        waveInClose(hWaveIn);
 
        return false;

    }
 
 
    // Start recording
 
    result =

        waveInStart(hWaveIn);
 
 
    if (result != MMSYSERR_NOERROR)

    {

        cout << "ERROR: Could not start recording."
<< endl;
 
        waveInUnprepareHeader(

            hWaveIn,
&waveHeader,

            sizeof(WAVEHDR)

        );
 
        waveInClose(hWaveIn);
 
        return false;

    }
 
 
    cout << endl;

    cout << "========================================"
<< endl;
 
    cout << "Recording for 3 seconds..."
<< endl;
 
    cout << "Please say YES or NO."
<< endl;
 
    cout << "========================================"
<< endl;
 
 
    Sleep(3000);
 
 
    // Stop recording
 
    waveInStop(hWaveIn);
 
    // IMPORTANT

    // Make sure buffer is completed
 
    waveInReset(hWaveIn);
 
 
    cout << "Bytes recorded: "
<< waveHeader.dwBytesRecorded
<< endl;
 
 
    // Cleanup
 
    waveInUnprepareHeader(

        hWaveIn,
&waveHeader,

        sizeof(WAVEHDR)

    );
 
    waveInClose(hWaveIn);
 
 
    cout << "Recording complete."
<< endl;
 
 
    return true;

}
 
 
// ---------------- ENERGY ----------------
 
double calculateEnergy(

    const vector<int>& frame)

{

    double energy = 0.0;
 
    for (int i = 0;

         i < frame.size();

         i++)

    {

        energy +=

            (double)frame[i] *

            frame[i];

    }
 
    return energy / frame.size();

}
 
 
// ---------------- ZCR ----------------
 
double calculateZCR(

    const vector<int>& frame)

{

    int zeroCrossings = 0;
 
 
    for (int i = 1;

         i < frame.size();

         i++)

    {

        if (

            (frame[i - 1] >= 0 &&

             frame[i] < 0)

            ||

            (frame[i - 1] < 0 &&

             frame[i] >= 0)

           )

        {

            zeroCrossings++;

        }

    }
 
 
    return

        (double)zeroCrossings /

        frame.size();

}
 
 
// ---------------- NOISE FLOOR ----------------
 
double computeNoiseFloor(

    const vector<int>& data)

{

    double sumSquares = 0;
 
    int count = 0;
 
 
    // First 0.25 seconds
 
    int noiseSamples = 4000;
 
 
    for (

        int i = 0;

        i < noiseSamples &&

        i < data.size();

        i++

       )

    {

        sumSquares +=

            (double)data[i] *

            data[i];
 
        count++;

    }
 
 
    if (count == 0)

        return 0;
 
 
    return sqrt(

        sumSquares / count

    );

}
 
 
// ---------------- SEGMENT WORD ----------------
 
pair<int, int> segmentWord(

    const vector<int>& data,

    double noiseFloor)

{

    int start = -1;
 
    int end = -1;
 
 
    // Threshold
 
    double threshold =

        noiseFloor + 200;
 
 
    cout << "Threshold: "
<< threshold
<< endl;
 
 
    // Find beginning
 
    for (

        int i = 0;

        i < data.size();

        i++

       )

    {

        if (

            abs(data[i]) >

            threshold

           )

        {

            start = i;

            break;

        }

    }
 
 
    // Find ending
 
    for (

        int i = data.size() - 1;

        i >= 0;

        i--

       )

    {

        if (

            abs(data[i]) >

            threshold

           )

        {

            end = i;

            break;

        }

    }
 
 
    return make_pair(start, end);

}
 
 
// ---------------- CLASSIFICATION ----------------
 
string classifyWord(

    double energy,

    double zcr)

{

    /*

       Assignment 2 rule:
 
       ZCR >= 0.11 -> YES

       ZCR <  0.11 -> NO

    */
 
    if (zcr >= 0.11)

        return "Yes";
 
    return "No";

}
 
 
// ---------------- MAIN ----------------
 
int main()

{

    cout << endl;
 
    cout << "========================================"
<< endl;
 
    cout << "     LIVE YES / NO DETECTION"
<< endl;
 
    cout << "========================================"
<< endl;
 
 
    // ---------------- RECORD ----------------
 
    bool success =

        recordAudio();
 
 
    if (!success)

    {

        return 1;

    }
 
 
    // ---------------- COPY DATA ----------------
 
    vector<int> data;
 
    for (

        int i = 0;

        i < NUM_SAMPLES;

        i++

       )

    {

        data.push_back(

            (int)waveBuffer[i]

        );

    }
 
 
    cout << "Samples available: "
<< data.size()
<< endl;
 
 
    // ---------------- NOISE ----------------
 
    double noiseFloor =

        computeNoiseFloor(data);
 
 
    cout << "Noise Floor: "
<< noiseFloor
<< endl;
 
 
    // ---------------- SEGMENT ----------------
 
    pair<int, int> segment =

        segmentWord(

            data,

            noiseFloor

        );
 
 
    int start = segment.first;
 
    int end = segment.second;
 
 
    cout << "Start: "
<< start
<< endl;
 
    cout << "End: "
<< end
<< endl;
 
 
    if (

        start == -1 ||

        end == -1 ||

        end <= start

       )

    {

        cout << endl;
 
        cout << "Could not detect speech."
<< endl;
 
        cout << "Try speaking louder or closer"
<< " to the microphone."
<< endl;
 
        return 0;

    }
 
 
    // ---------------- WORD ----------------
 
    vector<int> wordSegment(

        data.begin() + start,

        data.begin() + end + 1

    );
 
 
    cout << "Word samples: "
<< wordSegment.size()
<< endl;
 
 
    // ---------------- FEATURES ----------------
 
    double totalEnergy = 0;
 
    double totalZCR = 0;
 
    int frameCount = 0;
 
 
    for (

        int i = 0;

        i + FRAME_SIZE <=

        wordSegment.size();

        i += FRAME_SHIFT

       )

    {

        vector<int> frame(

            wordSegment.begin() + i,

            wordSegment.begin() +

            i + FRAME_SIZE

        );
 
 
        totalEnergy +=

            calculateEnergy(frame);
 
 
        totalZCR +=

            calculateZCR(frame);
 
 
        frameCount++;

    }
 
 
    if (frameCount == 0)

    {

        cout << "Not enough data."
<< endl;
 
        return 0;

    }
 
 
    // ---------------- AVERAGE ----------------
 
    double avgEnergy =

        totalEnergy /

        frameCount;
 
 
    double avgZCR =

        totalZCR /

        frameCount;
 
 
    cout << endl;
 
    cout << "========================================"
<< endl;
 
    cout << "Average Energy: "
<< avgEnergy
<< endl;
 
    cout << "Average ZCR: "
<< avgZCR
<< endl;
 
    cout << "========================================"
<< endl;
 
 
    // ---------------- CLASSIFY ----------------
 
    string decision =

        classifyWord(

            avgEnergy,

            avgZCR

        );
 
 
    cout << endl;
 
    cout << "The word is: "
<< decision
<< endl;
 
    cout << endl;
 
 
    return 0;

}
 