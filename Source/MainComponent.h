#pragma once

#include <JuceHeader.h>
#include "MaxxCore.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component,
    public juce::Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* buttonThatWasClicked) override;

private:

    const juce::String kSuperDescriptText =
        juce::String("Make your BMS as loud as a big barkin' dog!\n") +
        juce::String("First, analyze the pre & post mastering audio file.\n") +
        juce::String("Then select and process all your tracks.\n") +
        juce::String("Make sure all these files have all the same sample rate and length.\n") +
        juce::String("Keep the Soften option on and tweak the kernel size & the sigma value to achieve a better result.\n") +
        juce::String("\n") +
        juce::String(u8"あなたのBMSを、デカ犬吠えるように！\n") +
        juce::String(u8"まずマスタリング前とマスタリング済データを分析しよう。\n") +
        juce::String(u8"その後、トラックを選択して処理しよう。\n") +
        juce::String(u8"すべでの音声データのサンプリングレートと標本数を一致してください。\n") +
        juce::String(u8"最優結果のため、Softenをオンにして、カーネルサイズやシグマ値を調整したほうがいい。\n") +
        juce::String("\n") +
        juce::String(u8"让您的BMS像大狗叫一样响亮！\n") +
        juce::String(u8"首先分析母带前后的音频文件。\n") +
        juce::String(u8"然后选择分轨并进行处理。\n") +
        juce::String(u8"请保证所有的音频数据采样率和长度一致。\n") +
        juce::String(u8"为了达到最佳效果，请保持Soften开启、适当调整Kernel大小和sigma值。\n") +
        juce::String("\nby Kolaa");
    //==============================================================================
    // Your private member variables go here...
    std::unique_ptr<MaxxCore> mMaxx;
    juce::String mPreDir, mPostDir, mAnalysisStatusStr;
    int mAnStat = -1;
    int mProcStat = -1;

    juce::FileChooser mChooserPre = juce::FileChooser(
        "Select Pre",
        juce::File(),
        "*.wav",
        true,
        true

    );
    juce::FileChooser mChooserPost = juce::FileChooser(
        "Select Post",
        juce::File(),
        "*.wav",
        true,
        true

    );
    juce::FileChooser mChooserTrack = juce::FileChooser(
        "Add Track(s)",
        juce::File(),
        "*.wav",
        true,
        true

    );
    juce::FileChooser mChooserSave = juce::FileChooser(
        "Save To:",
        juce::File(),
        "*.wav",
        true,
        true

    );


    juce::TextButton mBtnLoadPre{ "Load Pre-master" };
    juce::TextButton mBtnLoadPost{ "Load Post-master" };
    juce::TextButton mBtnAnalyze{ "Analyze" };
    juce::TextButton mBtnSelectTracks{ "Select Tracks" };
    juce::TextButton mBtnProcess{ "Process" };
    juce::TextButton mBtnProcessShot{ "Process Shot" };
    juce::TextEditor mAnalysisIndicator;
    
    juce::ToggleButton mTglSoften {"Soften"};
    juce::Slider mSldKernel;
    juce::Slider mSldSigma;
    juce::ComboBox mComboFftOrder;

    juce::TreeView mTracksTree;
    juce::ListBox mListBoxTracks;
    
    void updateAnalysisStatus();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
