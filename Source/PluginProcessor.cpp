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
	this->bufSize = samplesPerBlock;
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
	juce::Random random;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	auto mainInputOutput = getBusBuffer (buffer, true, 0);
	// auto sideChainInput  = getBusBuffer (buffer, true, 1);
	auto sideChainInput = juce::AudioBuffer<float>(2, buffer.getNumSamples());

		
    for (auto sample = 0; sample < sideChainInput.getNumSamples(); ++sample)
	{
		float left = random.nextFloat() * 0.25f - 0.125f;
		float right = -1 * left - .001f;
        sideChainInput.setSample(0, sample, left);
		sideChainInput.setSample(1, sample, right);
	}	

	// this checks if the sidechain input is active
	// should only do anything if the input is stereo
	if(sideChainInput.getNumChannels() == 2)	
	{
		cb.circularAverage(sideChainInput);
		float left, right, mid;
		for (int s = 0; s < buffer.getNumSamples(); ++s)
		{
			left = buffer.getSample(0, s);
			right = buffer.getSample(1, s);
			float mid = (left + right) / sqrt(2);
			float side = (left - right) / sqrt(2);
			side *= cb.average;
			left = (mid + side) / sqrt(2);
			right = (mid - side) / sqrt(2);
			buffer.setSample(0, s, left);
			buffer.setSample(1, s, right);
		}
	}
	else
	{
		for (int s = 0; s < buffer.getNumSamples(); ++s)
		{
			//                                          HA
			for(int c = 0; c < buffer.getNumChannels(); c++)
			{
				buffer.setSample(c, s, mainInputOutput.getSample(0, s));
			}
		}
	}

	for(int channel = 0; channel < totalNumInputChannels; ++channel)
    {	 
		auto* channelData = buffer.getWritePointer (channel);
    }
}

void StereoBiterV2AudioProcessor::getAverageBufferHistory()
{
	
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

bool StereoBiterV2AudioProcessor::sidechainActive()
{
	return true;
}
