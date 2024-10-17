#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    mMaxx = std::make_unique<MaxxCore>();

    
    setSize (600, 640);


    mBtnLoadPre.setBounds(20, 20, 100, 30);
    mBtnLoadPre.addListener(this);
    addAndMakeVisible(mBtnLoadPre);

    mBtnLoadPost.setBounds(140, 20, 100, 30);
    mBtnLoadPost.addListener(this);
    addAndMakeVisible(mBtnLoadPost);

    mBtnAnalyze.setBounds(260, 20, 100, 30);
    mBtnAnalyze.addListener(this);
    addAndMakeVisible(mBtnAnalyze);
    
    mBtnSelectTracks.setBounds(20, 70, 100, 30);
    mBtnSelectTracks.addListener(this);
    addAndMakeVisible(mBtnSelectTracks);

    mBtnProcess.setBounds(140, 70, 100, 30);
    mBtnProcess.addListener(this);
    addAndMakeVisible(mBtnProcess);

    mBtnProcessShot.setBounds(260, 70, 100, 30);
    mBtnProcessShot.addListener(this);
    addAndMakeVisible(mBtnProcessShot);

    mTglSoften.setBounds(20, 120, 100, 30);
    mTglSoften.addListener(this);
    mTglSoften.setToggleState(true, false);
    addAndMakeVisible(mTglSoften);

    mSldKernel.setBounds(140, 120, 200, 90);
    mSldKernel.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSldKernel.setTextBoxStyle(juce::Slider::TextBoxRight, false,60,20);
    mSldKernel.setTooltip("Gaussian Kernel Size");
    mSldKernel.setRange(1.0, 64.0, 1.0);
    mSldKernel.setValue(16, juce::dontSendNotification);
    addAndMakeVisible(mSldKernel);

    mSldSigma.setBounds(360, 120, 200, 90);
    mSldSigma.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSldSigma.setTextBoxStyle(juce::Slider::TextBoxRight, false,60,20);
    mSldSigma.setTooltip("Gaussian Sigma");
    mSldSigma.setRange(0.1, 8.0, 0.1);
    mSldSigma.setValue(2.0, juce::dontSendNotification);
    addAndMakeVisible(mSldSigma);


    mAnalysisIndicator.setBounds(20, 220, 560, 400);
    mAnalysisIndicator.setReadOnly(true);
    mAnalysisIndicator.setMultiLine(true);
    mAnalysisIndicator.setText(kSuperDescriptText, false);
    addAndMakeVisible(mAnalysisIndicator);


}

MainComponent::~MainComponent()
{
}


//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);


}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
}


void MainComponent::buttonClicked(juce::Button *btn) {
    mProcStat = -1;
    if (btn == &mBtnLoadPre) {
        mChooserPre.browseForFileToOpen();
        mPreDir = mChooserPre.getResult().getFullPathName();
    }
    if (btn == &mBtnLoadPost) {
        mChooserPost.browseForFileToOpen();
        mPostDir = mChooserPost.getResult().getFullPathName();
    }
    if (btn == &mBtnAnalyze) {
        mMaxx->setSoften(mTglSoften.getToggleState(), mSldKernel.getValue(), mSldSigma.getValue());
        mAnStat = mMaxx->analyze(
            mChooserPre.getResult(),
            mChooserPost.getResult()
        );
    }
    if (btn == &mBtnSelectTracks) {
        mChooserTrack.browseForMultipleFilesToOpen();
    }
    if (btn == &mBtnProcess) {
        mChooserSave.browseForDirectory();
        mProcStat = mMaxx->processTracks(mChooserTrack.getResults(), mChooserSave.getResult());
    }
    if (btn == &mBtnProcessShot) {
        mChooserSave.browseForDirectory();
        mProcStat = mMaxx->processShots(mChooserTrack.getResults(), mChooserSave.getResult());
    }
    if (btn == &mTglSoften) {

    }

    updateAnalysisStatus();
}

void MainComponent::updateAnalysisStatus()
{
    juce::String text = "";
    text += "Pre: ";
    text += mPreDir.isNotEmpty()?mPreDir:"[not selected]";
    text += "\nPost: ";
    text += mPostDir.isNotEmpty() ? mPostDir : "[not selected]";
    text += "\nAnalysis: ";
    switch (mAnStat) {
    case 0:
        text += "ok";
        break;
    case -1:
        text += "not yet";
        break;
    default:
        text += "failed! code: ";
        text += juce::String(mAnStat);
        break;
    }
    text += "\nNumber of selected tracks: " + juce::String(mChooserTrack.getResults().size());
    text += "\nProcess: ";
    switch (mProcStat) {
    case 0:
        text += "success";
        break;
    case -1:
        text += "not yet";
        break;
    default:
        text += "failed! code: ";
        text += juce::String(mProcStat);
        break;
    }
    mAnalysisIndicator.setText(text);
    
}
