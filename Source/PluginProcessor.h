/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class StereoBiterV2AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    StereoBiterV2AudioProcessor();
    ~StereoBiterV2AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	void getStereoFieldRatio(juce::AudioBuffer<float> sidechain);

	void getAverageBufferHistory();
	void filoLookback(juce::AudioBuffer<float>* sidechainBuffer);

	//holds the number of buffers we keep in our history
	int bufferLookback = 5;

	// this is a stack
	juce::Array<float> bufferFilo;

	float stFieldRatio;


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoBiterV2AudioProcessor)
};

class BufferFilo
{
	juce::Array<float> filo[2];
	int numBuffers;
	int bufSize;
	int lastBuffer;
	void filoLookback(juce::AudioBuffer<float>* sidechainBuffer)
	{
		int size = filo[0].size();
		if(size < (numBuffers - bufSize))
		{
			for(int i = 0; i < sidechainBuffer->getNumSamples(); i++)
			{
				for(int j = 0; j <= 1; j++)
				{
					filo[j].add(sidechainBuffer->getSample(j, i));
				}
			}
		}
		for(int i = 0; i < bufSize * numBuffers; i += bufSize)
		{
			for(int i = bufSize * lastBuffer; )	
		}
	}
};

