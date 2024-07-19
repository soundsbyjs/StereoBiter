/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoBiterV2AudioProcessorEditor::StereoBiterV2AudioProcessorEditor (StereoBiterV2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (300, 300);
}

StereoBiterV2AudioProcessorEditor::~StereoBiterV2AudioProcessorEditor()
{
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
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..Considering what I have seen his CPU usage at in ableton projects, I'm sure he has a battery backup for his investment.
}
