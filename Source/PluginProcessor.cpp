/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoBiterV2AudioProcessor::StereoBiterV2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
			.withInput("Input", juce::AudioChannelSet::stereo())
    		.withInput("Sidechain", juce::AudioChannelSet::stereo())
    		.withOutput("Output", juce::AudioChannelSet::stereo())
                       )
#endif
{
}

StereoBiterV2AudioProcessor::~StereoBiterV2AudioProcessor()
{
}

//==============================================================================
const juce::String StereoBiterV2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereoBiterV2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StereoBiterV2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StereoBiterV2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StereoBiterV2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StereoBiterV2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StereoBiterV2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereoBiterV2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StereoBiterV2AudioProcessor::getProgramName (int index)
{
    return {};
}

void StereoBiterV2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void StereoBiterV2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void StereoBiterV2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereoBiterV2AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
    {
        // the sidechain can take any layout, the main bus needs to be the same on the input and output
        return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet()
                 && ! layouts.getMainInputChannelSet().isDisabled();
    }
#endif

void StereoBiterV2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

	auto mainInputOutput = getBusBuffer (buffer, true, 0);                                  // [5]
	auto sideChainInput  = getBusBuffer (buffer, true, 1);

	juce::ScopedNoDenormals noDenormals;
	
	int numSamples = buffer.getNumSamples();
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    // If there are more output channels than input channels, clear the extra channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, numSamples);
    }
    getStereoFieldRatio(sideChainInput);
    float left, right, mid;
    for (int s = 0; s < numSamples; ++s)
    {
            left = buffer.getSample(0, s);
            right = buffer.getSample(1, s);
            float mid = (left + right) / sqrt(2);
            float side = (left - right) / sqrt(2);
            side *= stFieldRatio;
            left = (mid + side) / sqrt(2);
            right = (mid - side) / sqrt(2);
            buffer.setSample(0, s, left);
            buffer.setSample(1, s, right);
    }

	for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {	 
		auto* channelData = buffer.getWritePointer (channel);
    }

}

// ok so now this has to run every audio buffer
void StereoBiterV2AudioProcessor::getStereoFieldRatio(juce::AudioBuffer<float> sidechain)
{
	float totalMidEnergy = 0;
	float totalSideEnergy = 0;
	float left, right;
	for(float s = 0; s < sidechain.getNumSamples(); s++)
	{
		left = sidechain.getSample(0,s);
		right = sidechain.getSample(1,s);
		float mid = (left + right) / sqrt(2);
		float side = (left - right) / sqrt(2);
		totalMidEnergy += pow(fabs(mid), 2);
		totalSideEnergy += pow(fabs(side), 2);
	}
	stFieldRatio = (totalSideEnergy/sidechain.getNumSamples()) / (totalMidEnergy/sidechain.getNumSamples());
}

void StereoBiterV2AudioProcessor::getAverageBufferHistory()
{
	
}

void StereoBiterV2AudioProcessor::filoLookback(juce::AudioBuffer<float>* sidechainBuffer)
{
	int finalIndex = ;
	for(int i = 0; i < sidechainBuffer->getNumSamples(); i++)
	{
	}
}

//==============================================================================
bool StereoBiterV2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StereoBiterV2AudioProcessor::createEditor()
{
    return new StereoBiterV2AudioProcessorEditor (*this);
}

//==============================================================================
void StereoBiterV2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StereoBiterV2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoBiterV2AudioProcessor();
}


