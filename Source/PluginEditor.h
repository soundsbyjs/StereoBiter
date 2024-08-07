/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class StereoBiterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    StereoBiterAudioProcessorEditor (StereoBiterAudioProcessor&);
    ~StereoBiterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
	void openButtonClicked();

private:
	juce::TextButton openButton;
    juce::TextButton clearButton;

    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager;
    juce::AudioSampleBuffer fileBuffer;
    int position;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StereoBiterAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoBiterAudioProcessorEditor)

};
