#include "guiwaveheightips.h"





////////////////////////////////////////////////////////////////////////////////
GUIWaveHeightIPS::GUIWaveHeightIPS(QWidget *parent)
    : GUIImageProcessingSystem(parent)
{
	ui.setupUi(this);
	mIPS = &mWaveHIPS;
}

////////////////////////////////////////////////////////////////////////////////
GUIWaveHeightIPS::~GUIWaveHeightIPS()
{

}

////////////////////////////////////////////////////////////////////////////////
errCode GUIWaveHeightIPS::initialize(void)
{
	// we will configure the ImageGenerator
	mImgGenWin.setImgGenerator(mWaveHIPS.getImageGenerator());


	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode GUIWaveHeightIPS::execute(void)
{
	// first: configure the image generator

	// second: configure the perspective transformation

	// third: set the zone that we will analyze and put to the mWaveHips

	// four: show and configure the RealTimeDataDisplayer

	// five: start to capture and show the results

	// six: probably we want to save the results in the hard disk for a later
	// use

	return NO_ERROR;
}





