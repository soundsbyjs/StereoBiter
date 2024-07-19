#pragma once
#include <JuceHeader.h>

// we do a lil computer science
// ok, I need a circular buffer so that I can keep a running average
// this way we won't end up with really clicky shit going on

// number of blocks in the circular buffer
#define NumBlocks 50
#define SENTINEL -10000 // dont send a signal in that's wider than this! lol

class StereoBiterV2CircularBuffer
{
public:
	class Averages
	{
	  public:
		float value;
		// length of the buffer can be variable
		int numSamples;
	};

	Averages avgs[NumBlocks];
		
	int NumAverages; 
	float average = 1;
	bool isEmpty = true;
	float AverageTotal = 0;
	int totalNumSamples = 0;

	// additional average to be averaged into the average, lol
	// rolling average on a circular buffer! this is good computer science and dsp!!!!!!!!
	
	StereoBiterV2CircularBuffer()
	{
		for(int i = 0; i < NumBlocks; i++)
		{
			avgs[i].value = SENTINEL;
		}
		NumAverages = 1;
	}
	void circularAverage(juce::AudioBuffer<float> buf)
	{
		if(isEmpty)	
		{
			avgs[NumAverages - 1].value = getAverage(&buf);
			avgs[NumAverages - 1].numSamples = buf.getNumSamples();
			++NumAverages;
			average = avgs[NumAverages - 1].value;
			isEmpty = false;
		}
		else
		{
			// while the buffer is not completely populated we have to recalculate the average
			if(avgs[NumAverages - 1].value == SENTINEL)
			{
				avgs[NumAverages - 1].value = getAverage(&buf);
				avgs[NumAverages - 1].numSamples = buf.getNumSamples();
				AverageTotal = 0;
				totalNumSamples = 0;
				
				// calculate average of averages
				for(int i = 0; i < NumAverages; i++)	
				{
					AverageTotal += avgs[i].value * avgs[i].numSamples;
					totalNumSamples += avgs[i].numSamples;
				}

				if(totalNumSamples != 0 || AverageTotal != 0)
					average = AverageTotal / totalNumSamples;
				else
					average = 1.0f;
			}
			// buffer is full
			else
			{
				avgs[NumAverages - 1].value = getAverage(&buf);
				avgs[NumAverages - 1].numSamples = buf.getNumSamples();

				AverageTotal = 0;
				totalNumSamples = 0;

				for(int i = 0; i < NumBlocks; i++)
				{
					AverageTotal += avgs[i].value * avgs[i].numSamples;
					totalNumSamples += avgs[i].numSamples;
				}

				if(totalNumSamples != 0)
					average = AverageTotal / totalNumSamples;
				else
					average = 1.0f;

			}

			if(NumAverages == NumBlocks)
				NumAverages = 1;
			else
				++NumAverages;
		}
	}
	float getAverage(juce::AudioBuffer<float>* buf)
	{
		float totalMidEnergy = 0;
		float totalSideEnergy = 0;
		float left, right;
		for(float s = 0; s < buf->getNumSamples(); s++)
		{
			left = buf->getSample(0,s);
			right = buf->getSample(1,s);
			float mid = (left + right) / sqrt(2);
			float side = (left - right) / sqrt(2);
			totalMidEnergy += pow(fabs(mid), 2);
			totalSideEnergy += pow(fabs(side), 2);
		}
		// calling getNumSamples again is inefficient, but i dont care!
		return (totalSideEnergy/buf->getNumSamples()) / (totalMidEnergy/buf->getNumSamples());
	}
};
