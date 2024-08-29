#include "MaxxCore.h"

MaxxCore::MaxxCore(int size) {
	mFftSize = size;
	mFftCross = mFftSize / 2;
	mFftDelta = mFftSize - mFftCross;
	mFreqDomainSize = mFftSize / 2 + 1;
	mFormatManager.registerBasicFormats();

	mCfgFft = kiss_fftr_alloc(mFftSize, 0, NULL, NULL);
	mCfgIfft = kiss_fftr_alloc(mFftSize, 1, NULL, NULL);

}

MaxxCore::~MaxxCore()
{
	kiss_fftr_free(mCfgFft);
	kiss_fftr_free(mCfgIfft);
}
int MaxxCore::setSoften(bool isSoften, int kernelSize, float sigma)
{
	mIsSoften = isSoften;
	mSoftenKernelSize = kernelSize;
	mSoftenSigma = sigma;
	return 0;
}
int MaxxCore::analyze(juce::File pre, juce::File post) {
	mIsAnalyzed = false;
	mAnalysisCnt = 0;
	for (int i = 0; i < 2; i++) {
		mTimeDatas[i].clear();
		mAnalysisDatas[i].clear();
	}
	auto readerPre = mFormatManager.createReaderFor(pre);
	if (!readerPre)
		return 1; // fail to open pre
	auto srcPre = juce::AudioFormatReaderSource(readerPre, true);
	auto lenPre = srcPre.getTotalLength();
	auto srPre = readerPre->sampleRate;
	

	auto readerPost = mFormatManager.createReaderFor(post);
	if (!readerPost)
		return 2; // fail to open post
	auto srcPost = juce::AudioFormatReaderSource(readerPost, true);
	auto lenPost = srcPost.getTotalLength();
	auto srPost = readerPost->sampleRate;

	if (srPre != srPost) return 10; //different sample rate
	mSampleRate = srPre;
	mNumSamples = lenPre < lenPost ? lenPre : lenPost;

	if (mNumSamples < mFftDelta)
		return 1001;//too short

	juce::AudioBuffer<float> bufPre,bufPost;
	bufPre.setSize(2, mFftSize);
	bufPost.setSize(2, mFftSize);

	auto arrTimePre = new kiss_fft_scalar[mFftSize];
	auto arrTimePost = new kiss_fft_scalar[mFftSize];
	auto arrFreqPre = new kiss_fft_cpx[mFreqDomainSize];
	auto arrFreqPost = new kiss_fft_cpx[mFreqDomainSize];

	auto mSoftenTmp = new float[mFreqDomainSize];

	bool failed = false;

	for (int64_t i = 0; i < mNumSamples; i+=mFftDelta) {
		bool isFinal = i + mFftDelta >= mNumSamples;
		auto cnt = isFinal ? mNumSamples - i : mFftSize;
		if (isFinal) {
			bufPre.clear();
			bufPost.clear();
		}
		readerPre->read(&bufPre, 0,
			(int)cnt,i, true,true);
		readerPost->read(&bufPost, 0,
			(int)cnt,i, true,true);


		for (int j = 0; j < 2; j++) {
			fwHann(bufPre.getWritePointer(j), mFftSize, i>0,true);
			fwHann(bufPost.getWritePointer(j), mFftSize, i>0,true);
			kiss_fftr(mCfgFft, bufPre.getReadPointer(j), arrFreqPre);
			kiss_fftr(mCfgFft, bufPost.getReadPointer(j), arrFreqPost);
			for (int k = 0; k < mFreqDomainSize; k++) {
				float anRatio = 1;
				auto magPreK = getComplexMag(arrFreqPre[k].r, arrFreqPre[k].i);
				auto magPostK = getComplexMag(arrFreqPost[k].r, arrFreqPost[k].i);
				if (!juce::approximatelyEqual(magPreK, 0.0f)) {
					anRatio = magPostK / magPreK;
				}
				//Soften
				if (mIsSoften) {
					mSoftenTmp[k] = log2f(anRatio);
				}
				else mAnalysisDatas[j].push_back(anRatio);
			}
			if (mIsSoften){
				gaussian(mSoftenTmp, mFreqDomainSize, mSoftenKernelSize, mSoftenSigma);
				for (int k = 0; k < mFreqDomainSize; k++) {
					mAnalysisDatas[j].push_back(exp2f(mSoftenTmp[k]));
				}
			}
		}
		mAnalysisCnt++;
	}

	delete[]arrTimePre, arrTimePost, arrFreqPre, arrFreqPost, mSoftenTmp;

	mIsAnalyzed = true;
	return 0;
}

MaxxCore::StatusInfo MaxxCore::getStatus()
{
	return StatusInfo{
		mFftSize,
		mFftCross,
		mNumSamples,
		mSampleRate,
		mAnalysisCnt,
		mIsAnalyzed
	};
}

int MaxxCore::analyzeCore(float** pre, float** post, int n)
{
	return 0;
}

int MaxxCore::processTracks(juce::Array<juce::File> files, juce::File destInfo)
{
	if (files.isEmpty()) return 9; // no file selected
	for (auto i = files.begin(); i < files.end(); i++) {
		auto trackResult = processTrackCore(*i, destInfo);
		if (trackResult) return trackResult;
	}
	return 0;
}

int MaxxCore::processTrackCore(juce::File track, juce::File destInfo)
{
	auto trackName = juce::String("Dagoujiao_") + track.getFileName();
	auto reader = mFormatManager.createReaderFor(track);
	if (!reader)
		return 1; // fail to open
	auto src = juce::AudioFormatReaderSource(reader, true);
	auto len = src.getTotalLength();
	auto sr = reader->sampleRate;
	if (sr != mSampleRate)
		return 10; // sample rates are different
	juce::WavAudioFormat wavFormat;
	auto destFile = destInfo.getChildFile(trackName);
	if (destFile.exists()) {
		if (!destFile.deleteFile()) return 21; // fail to delete track;
	}
	auto os = destFile.createOutputStream();
	if (!os)
		return 11; //can't create output
	auto writer = wavFormat.createWriterFor(
		os.get(),
		sr,
		2,
		reader->bitsPerSample,
		juce::StringPairArray(),
		0);
	if (!writer)
		return 12; // can't create writer

	juce::AudioBuffer<float> buf;
	juce::AudioBuffer<float> bufDest;
	juce::AudioBuffer<float> bufCross;

	buf.setSize(2, mFftSize);
	buf.clear();
	bufDest.setSize(2, mFftSize);
	bufDest.clear();
	bufCross.setSize(2, mFftCross);
	bufCross.clear();

	auto arrTime = new kiss_fft_scalar[mFftSize];
	auto arrFreq = new kiss_fft_cpx[mFreqDomainSize];
	int64_t mAnIndex = 0;

	auto anFullCnt = mAnalysisCnt * mFreqDomainSize;

	for (int64_t i = 0; i < mNumSamples; i += mFftDelta) {
		bool isFinal = i + mFftDelta >= mNumSamples;
		auto cnt = isFinal ? mNumSamples - i: mFftSize;
		auto writeCnt = cnt - mFftCross;

		reader->read(&buf, 0,
			(int)cnt, i, true, true);
		bufDest.clear();
		for (int j = 0; j < 2; j++) {

			//copy buf
			memcpy(bufDest.getWritePointer(j), bufCross.getReadPointer(j), sizeof(float) * mFftCross);

			fwHann(buf.getWritePointer(j), mFftSize);
			kiss_fftr(mCfgFft, buf.getReadPointer(j), arrFreq);



			for (int k = 0; k < mFreqDomainSize; k++) {
				if (mAnIndex + k>= anFullCnt) break;
				
				arrFreq[k] = resizeCpx(arrFreq[k], mAnalysisDatas[j][mAnIndex + k]);
				
			}
			kiss_fftri(mCfgIfft, arrFreq, buf.getWritePointer(j));

			// addin the result

			for (int k = 0; k < mFftSize; k++) {
				bufDest.getWritePointer(j)[k] += buf.getReadPointer(j)[k];
				bufDest.getWritePointer(j)[k] /= (mFftSize);
			}
			// copy tmp buf
			memcpy(bufCross.getWritePointer(j), buf.getReadPointer(j) + mFftDelta, sizeof(float) * mFftCross);
		}
		mAnIndex+=mFreqDomainSize;

		// write cnt
		writer->writeFromAudioSampleBuffer(bufDest, 0, writeCnt);

	}
	writer->flush();
	os.reset();
	delete[] arrTime, arrFreq;

	return 0;
}


