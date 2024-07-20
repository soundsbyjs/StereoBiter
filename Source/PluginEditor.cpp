/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#define NYQUIST 44100/2

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoBiterV2AudioProcessorEditor::StereoBiterV2AudioProcessorEditor (StereoBiterV2AudioProcessor& p)
					// im pushing
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 500);

	addAndMakeVisible(frequencySlider);
	frequencySlider.setRange(0, NYQUIST);
	frequencyLabel.setText("HPF Frequency", juce::dontSendNotification);

	addAndMakeVisible(lookbackSlider);
	lookbackSlider.setRange(1, audioProcessor.cb.MAXBUFFERSIZE, 1);
	lookbackLabel.setText("Buffer Lookback", juce::dontSendNotification);

	addAndMakeVisible(filterButton);
}

StereoBiterV2AudioProcessorEditor::~StereoBiterV2AudioProcessorEditor()
{
}

void StereoBiterV2AudioProcessorEditor::buttonClicked(juce::Button *button)
{
	if (button == &filterButton)
    {
		if(button->getToggleState() == true)
			audioProcessor.filterOn = true;	
		else
			audioProcessor.filterOn = false;
	}
}
void StereoBiterV2AudioProcessorEditor::sliderValueChanged(juce::Slider* slider) 
{
	if(slider == &frequencySlider)	
	{
		// nothign cuz i haven't implemented it yet
	}
	else if(slider == &lookbackSlider)
	{
		audioProcessor.cb.clear(round(lookbackSlider.getValue()));
	}
}
//==============================================================================
void StereoBiterV2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.drawImage(catgirl, 0, 0, getWidth(), getHeight(), 0,0, catgirl.getWidth(), catgirl.getHeight(), false );
    g.setColour (juce::Colours::white);
}

void StereoBiterV2AudioProcessorEditor::resized()
{
	auto sliderLeft = 120;
    frequencySlider.setBounds (10, 310, getWidth() - 10 - 10, 20);
    lookbackSlider.setBounds (10, 340, getWidth() - 10 - 10, 20);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..Considering what I have seen his CPU usage at in ableton projects, I'm sure he has a battery backup for his investment.
}
