#include "../lib/JuceLibraryCode/JuceHeader.h"

class FileDropper : public juce::FileDragAndDropTarget, public juce::Component
{
public:
	bool isDraggingOver = false;
	juce::GlowEffect g;
	juce::AudioSampleBuffer* sample;
	juce::AudioFormatManager* formatManager;

	FileDropper()
    {
        setSize(400, 300); // Optionally set a size for your component
    }
	~FileDropper()
	{
		// leak my memory see if i care
		// the cousin that stole the laptop:
	}

	void getPointers(juce::AudioSampleBuffer* sample, juce::AudioFormatManager* formatManager)
	{
		// pointer to a pointer points to a pointer, lol
		this->sample = sample;
		this->formatManager = formatManager;
	}
    bool isInterestedInFileDrag(const juce::StringArray& files) override 
	{ 
		return true; 
	}

    void fileDragEnter(const juce::StringArray& files, int x, int y) override
	{
		isDraggingOver = true;
		repaint();
	}

    void fileDragMove(const juce::StringArray& files, int x, int y) override
	{
		repaint();
	}

    void fileDragExit(const juce::StringArray& files) override
	{
		isDraggingOver = false;
		repaint();
	}

    void filesDropped(const juce::StringArray& files, int x, int y) override 
	{  
		isDraggingOver = false;
		toSampleBuffer(files);
		repaint();

	}
	
	int position;
	// takes in a file and returns a sample buffer
	void toSampleBuffer(const juce::StringArray& files)
	{
		// ok so we have a pointer to the pointer to the samplebuffer in plugineditor	
		// now we have to make that sample point to the file in question
		// only operate on the first file if it exists
		
		juce::File file(files[0]);
		
		std::unique_ptr<juce::AudioFormatReader> reader (formatManager->createReaderFor (file)); // [2]
			
		if (reader.get() != nullptr)
        {
            auto duration = (float) reader->lengthInSamples / reader->sampleRate;               // [3]

            sample->setSize ((int) reader->numChannels, (int) reader->lengthInSamples);  // [4]
            reader->read (sample,                                                      
                          0,
                          (int) reader->lengthInSamples,
                          0,
                          true, 
                          true);
            position = 0;
        }
	}

	void paint(juce::Graphics& g) override
	{
		if (isDraggingOver)
		{
			g.fillAll(juce::Colours::lightblue); // Change color to light blue when dragging
		}
		else
		{
			g.fillAll(juce::Colours::lightgrey); // Default background color
		}

        g.setColour(juce::Colours::black);
        g.setFont(20.0f);
        g.drawFittedText("Drag and drop files here", getLocalBounds(), juce::Justification::centred, 1);
	}
};
