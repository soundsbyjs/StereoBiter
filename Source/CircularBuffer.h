#pragma once
#include <JuceHeader.h>
#include <limits>

// good call
#define SENTINEL std::numeric_limits<float>::quiet_NaN()

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

	const static int MAXBUFFERSIZE = 500;

	int NumBlocks;
	//Averages &avgs[];
	Averages avgs[MAXBUFFERSIZE];
		
	int NumAverages; 
	float average = 1;
	bool isEmpty = true;
	float AverageTotal = 0;
	int totalNumSamples = 0;

	// additional average to be averaged into the average, lol
	// rolling average on a circular buffer! this is good computer science and dsp!!!!!!!!
	
	StereoBiterV2CircularBuffer()
	{
		NumAverages = 1;
		NumBlocks = 50;
		clear();
	}

	~StereoBiterV2CircularBuffer()
	{
			
	}
	/**
 	* @brief clear the buffer and change its size
 	*/
	void clear(int newNumAverages)
	{
		for(auto i : avgs)
			i.value = SENTINEL;
		NumAverages = newNumAverages;
		isEmpty = true;
	}
	/**
 	* @brief clear the buffer but keep its size 
 	*/
	void clear()
	{
		for(auto i : avgs)
			i.value = SENTINEL;
		isEmpty = true;
	}

	float circularAverage(juce::AudioBuffer<float> buf)
	{
		if(isEmpty)	
		{
			avgs[NumAverages - 1].value = getAverage(&buf);
			avgs[NumAverages - 1].numSamples = buf.getNumSamples();
			++NumAverages;
			average = avgs[NumAverages - 1].value;
			std::cout<<"buffer is empty, adding " << average << " to the buffer. NumAverages is " << NumAverages << std::endl;
			isEmpty = false;
		}
		else
		{
			// while the buffer is not completely populated we have to recalculate the average
			if(avgs[NumAverages - 1].value == SENTINEL)
			{
				avgs[NumAverages - 1].value = getAverage(&buf);
				avgs[NumAverages - 1].numSamples = buf.getNumSamples();

				std::cout<<"Buffer is not full. adding " << avgs[NumAverages - 1].value << " to the buffer. NumAverages is " << NumAverages << std::endl;
				AverageTotal = 0;
				totalNumSamples = 0;
				
				// calculate average of averages
				for(int i = 0; i < NumAverages; i++)	
				{
					AverageTotal += avgs[i].value * avgs[i].numSamples;
					totalNumSamples += avgs[i].numSamples;
				}
				std::cout<<"Average is " << average << std::endl << std::endl;

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

				std::cout<<"Buffer is now full. adding " << avgs[NumAverages - 1].value << " to the buffer. NumAverages is " << NumAverages << std::endl;

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
		return average;
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
