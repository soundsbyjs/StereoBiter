#include "../lib/JuceLibraryCode/JuceHeader.h"

class FileDropper : public juce::FileDragAndDropTarget, public juce::Component
{
public:
	bool isDraggingOver = false;
	juce::GlowEffect g;
	juce::AudioSampleBuffer* sample;
	juce::AudioFormatManager* formatManager;
	double* ratio;
	juce::Image catgirl;

	FileDropper()
    {
        setSize(400, 300); // Optionally set a size for your component
		juce::File catgirlFile("../img/catgirl.png");
		catgirl = juce::ImageFileFormat::loadFrom(catgirlFile);
    }
	~FileDropper()
	{
		// leak my memory see if i care
		// the cousin that stole the laptop:
	}

	void getPointers(juce::AudioSampleBuffer* sample, juce::AudioFormatManager* formatManager, double* ratio)
	{
		// pointer to a pointer points to a pointer, lol
		this->sample = sample;
		this->formatManager = formatManager;
		this->ratio = ratio;
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
		std::cout<<sample->getNumSamples()<<std::flush;
		getStereoFieldRatio(*sample);
	}

	void getStereoFieldRatio(juce::AudioSampleBuffer& sample)
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
		*ratio = (totalSideEnergy/sample.getNumSamples()) / (totalMidEnergy/sample.getNumSamples());
		std::cout << " st field ratio: " << *ratio << std::flush;
	}

	void paint(juce::Graphics& g) override
	{
		if (isDraggingOver)
		{
			g.fillAll(juce::Colours::lightblue); // Change color to light blue when dragging
		}
		else
		{
			g.fillAll(juce::Colours::black); // Default background color
		}
		if (catgirl.isValid())
        {
			std::cout << "image is valid" << std::flush;
            g.drawImageWithin(catgirl, 10, 10, getWidth() - 20, getHeight() - 20, juce::RectanglePlacement::centred);
        }

        g.setColour(juce::Colours::white);
        g.setFont(20.0f);
        g.drawFittedText("Drag and drop files here :3", getLocalBounds(), juce::Justification::centred, 1);

	}
};
