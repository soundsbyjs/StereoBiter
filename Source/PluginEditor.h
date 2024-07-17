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
class StereoBiterV2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    StereoBiterV2AudioProcessorEditor (StereoBiterV2AudioProcessor&);
    ~StereoBiterV2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StereoBiterV2AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoBiterV2AudioProcessorEditor)
};
