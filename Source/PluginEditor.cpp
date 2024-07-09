/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoBiterAudioProcessorEditor::StereoBiterAudioProcessorEditor (StereoBiterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    formatManager.registerBasicFormats();
	fd.getPointers(&fileBuffer, &formatManager, &audioProcessor.ratio);
	addAndMakeVisible(fd);

	fd.setSize(400, 300);
	fd.setTitle("Drop a sample in here.");


	// fd.getFilePointer(&fileBuffer);
	// addAndMakeVisible (openButton);
    // openButton.setButtonText ("Open...");
    // openButton.onClick = [this] { openButtonClicked(); };

    // addAndMakeVisible (clearButton);
    // clearButton.setButtonText ("Clear");
    // clearButton.onClick = [this] { clearButtonClicked(); };

	// addAndMakeVisible(midSlider);
	// midSlider.setRange(-100, 0, .1);
	// midSlider.setTextValueSuffix("this doesnt do anything rn");
	// midSlider.addListener(this);

	// addAndMakeVisible(fileDropper);

	
    setSize(400, 300);
}

void StereoBiterAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
	audioProcessor.midStrength = slider->getValue();	
}

StereoBiterAudioProcessorEditor::~StereoBiterAudioProcessorEditor()
{
}

void StereoBiterAudioProcessorEditor::clearButtonClicked()
{
	audioProcessor.ratio = 1.0f;
}
// ill come back to u
// void AudioProcessor::setProcessingPrecision 	( 	ProcessingPrecision 	newPrecision	)
//
//==============================================================================
void StereoBiterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
	//g.fill

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void StereoBiterAudioProcessorEditor::resized()
{
	openButton .setBounds (10, 10, getWidth() - 20, 20);
    clearButton.setBounds (10, 40, getWidth() - 20, 20);
	const int border = 120;
	midSlider.setBounds(border, 100, getWidth() - border, 20);

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void StereoBiterAudioProcessorEditor::openButtonClicked()
{
	//
	// juce::AudioAppComponent a;
    // shutdownAudio();                                                                            // [1]

    chooser = std::make_unique<juce::FileChooser> ("Select a Wave file shorter than 2 seconds to play...",
                                                   juce::File{},
                                                   "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode
                      | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
    {
        auto file = fc.getResult();

        if (file == juce::File{})
            return;

        std::unique_ptr<juce::AudioFormatReader> reader (formatManager.createReaderFor (file)); // [2]

        if (reader.get() != nullptr)
        {
            auto duration = (float) reader->lengthInSamples / reader->sampleRate;               // [3]

            fileBuffer.setSize ((int) reader->numChannels, (int) reader->lengthInSamples);  // [4]
            reader->read (&fileBuffer,                                                      
                          0,
                          (int) reader->lengthInSamples,
                          0,
                          true, 
                          true);
            position = 0;
			
        }
		if(fileBuffer.getNumSamples() != 0)
		{
			/*
			audioProcessor.sample = fileBuffer;
			audioProcessor.calculatedRatio = false;
			*/
			audioProcessor.getStereoFieldRatio(fileBuffer);
		}
    });
	
}
