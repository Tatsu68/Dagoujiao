#pragma once

#include<vector>
#include<JuceHeader.h>

#include"MathUtils.h"
#include"FftWindow.h"

class MaxxCore {
    struct StatusInfo {
        int fftSize;
        int fftCross;
        juce::int64 numSamples;
        double sampleRate;
        int64_t analysisCnt;
        bool isAnalyzed;
    };
public:
    MaxxCore(int fftSize = 2048);
    ~MaxxCore();
private:
    int mFftSize, mFftCross, mFftDelta, mFreqDomainSize;
    int64_t mNumSamples = 0;
    double mSampleRate = 0;
    bool mIsAnalyzed = false;
    int64_t mAnalysisCnt = 0;
    std::vector<float> mTimeDatas[2], mAnalysisDatas[2];
    kiss_fftr_cfg mCfgFft, mCfgIfft;
    float* fftArrTime;
    kiss_fft_cpx* fftArrFreq;
    bool mIsSoften = false;
    int mSoftenKernelSize = 16;
    float mSoftenSigma = 4.f;
    // avg analyze
    std::vector<float> mAvgDatas[2];
    float mAvgWt[2];
    // juce parts
public:
    int setSoften(bool, int, float);
    int analyze(juce::File, juce::File);
    StatusInfo getStatus();
    int processTracks(juce::Array<juce::File>, juce::File);
    int processShots(juce::Array<juce::File>, juce::File);
private:
    juce::AudioFormatManager mFormatManager;
    juce::AudioBuffer<float> cool;

    int analyzeCore(float** pre, float** post, int n);
    int processTrackCore(juce::File, juce::File, bool);

};
