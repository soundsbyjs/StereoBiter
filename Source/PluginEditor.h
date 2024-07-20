/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "juce_gui_basics/juce_gui_basics.h"

//==============================================================================
/**
*/
class StereoBiterV2AudioProcessorEditor  : public juce::AudioProcessorEditor,
										   public juce::Slider::Listener
{
public:
    StereoBiterV2AudioProcessorEditor (StereoBiterV2AudioProcessor&);
    ~StereoBiterV2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
	void sliderValueChanged(juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StereoBiterV2AudioProcessor& audioProcessor;

	juce::Image catgirl = juce::ImageCache::getFromMemory (BinaryData::catgirl_png, BinaryData::catgirl_pngSize);

	juce::Slider frequencySlider;
    juce::Label  frequencyLabel;
    juce::Slider lookbackSlider;
    juce::Label  lookbackLabel;

    // juce::Image catgirl = juce::ImageCache::getFromFile(juce::File("../catgirl.png"));

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoBiterV2AudioProcessorEditor)
};
