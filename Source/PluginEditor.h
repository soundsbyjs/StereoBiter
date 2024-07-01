/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../lib/JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "./../lib/dRowAudio/dRowAudio.h"
#include "fileDropper.h"

//==============================================================================
/**
*/
class StereoBiterAudioProcessorEditor  : public juce::AudioProcessorEditor,
										 public juce::Slider::Listener
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
	juce::Slider midSlider;

    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager;
    juce::AudioSampleBuffer fileBuffer;
	void clearButtonClicked();
    int position;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StereoBiterAudioProcessor& audioProcessor;

	void sliderValueChanged(juce::Slider* slider) override;
	// drow::AudioFileDropTarget fileDropper;

	FileDropper fd;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoBiterAudioProcessorEditor)

};
