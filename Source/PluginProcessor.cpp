/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>  // for std::isnan and std::isfinite

//==============================================================================
StereoBiterAudioProcessor::StereoBiterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

StereoBiterAudioProcessor::~StereoBiterAudioProcessor()
{
}

//==============================================================================
const juce::String StereoBiterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereoBiterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StereoBiterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StereoBiterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StereoBiterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StereoBiterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StereoBiterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereoBiterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StereoBiterAudioProcessor::getProgramName (int index)
{
    return {};
}

void StereoBiterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void StereoBiterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void StereoBiterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereoBiterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void StereoBiterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	/*
	if(!calculatedRatio)
	{
		getStereoFieldRatio(sample);
		calculatedRatio = true;
	}
	*/

    juce::ScopedNoDenormals noDenormals;
	
	int numSamples = buffer.getNumSamples();

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        // buffer.clear (i, 0, buffer.getNumSamples());
	
		float left, right, mid;
		for(int s = 0; s < numSamples; ++s)
		{
			left = buffer.getSample(0,s);
			right = buffer.getSample(1,s);
			float mid = (left + right) / sqrt(2);
			float side = (left - right) / sqrt(2);
			side *= ratio;
			left = (mid + side)/sqrt(2);
			right = (mid - side)/sqrt(2);
			buffer.setSample(0,s,left);
			buffer.setSample(1,s,right);
		}
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {	 
		auto* channelData = buffer.getWritePointer (channel);
    }
}

//==============================================================================
//
bool StereoBiterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StereoBiterAudioProcessor::createEditor()
{
    return new StereoBiterAudioProcessorEditor (*this);
}

//==============================================================================
void StereoBiterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StereoBiterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


float StereoBiterAudioProcessor::getAverage(juce::Array<float>& arr)
{
	float total = 0;
	for(auto i : arr)
		total += i;
	return total / arr.size();
}

void StereoBiterAudioProcessor::getStereoFieldRatio(juce::AudioSampleBuffer& sample)
{
	float totalMidEnergy = 0; 
	float totalSideEnergy = 0;
	juce::Array<float> ratios;
	for(int i = 0; i < sample.getNumSamples(); i++)
	{
		float mid = (sample.getSample(0, i) + (sample.getSample(1, i))) / sqrt(2);
		float side = (sample.getSample(0, i) - (sample.getSample(1, i))) / sqrt(2);
		/*
		totalMidEnergy += pow(fabs(mid), 2);
		totalSideEnergy += pow(fabs(side), 2);
		*/
		totalMidEnergy += pow(mid, 2);
		totalSideEnergy += pow(side, 2);
	}
	ratio = (totalSideEnergy/sample.getNumSamples()) / (totalMidEnergy/sample.getNumSamples());
	std::cout << " st field ratio: " << ratio << std::flush;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoBiterAudioProcessor();
}
