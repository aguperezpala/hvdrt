#include "guiwaveheightips.h"


////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::createImageAnalyzer(void)
{
	// adds the processors to the image analyzer
	mImageAnalyzer.addNewProcessor(&mBorderDetector);
	mImageAnalyzer.addNewProcessor(&mCoordsInterpreter);

	mCoordsInterpreter.setData(&mCoordsData);
	mCoordsInterpreter.setDataReadyCallBackF(&mBridge);


	mWaveHIPS.setImgAnalyzer(&mImageAnalyzer);
}

////////////////////////////////////////////////////////////////////////////////
bool GUIWaveHeightIPS::configureInput(void)
{
	ImageGeneratorConfigurator igc(this);
	igc.setImgGenerator(mWaveHIPS.getImageGenerator());
	igc.show();
	igc.exec();

	return true;
}


////////////////////////////////////////////////////////////////////////////////
bool GUIWaveHeightIPS::configureMiddlePoint(void)
{

	if(!mTransformator){
		debug("Error: No perspective transformator was set\n");
		GUIUtils::showMessageBox("Error: No Perspective transformator was set\n");
		return false;
	}

	CoordsInterpreterConfigurator cic(this);

	Frame f;
	int err = mWaveHIPS.getImageGenerator()->captureFrame(f);
	if(err != NO_ERROR){
		debug("Error capturing a frame from the imageGenerator. It was "
				"configured?, error: %d\n", err);
		GUIUtils::showMessageBox("Error capturing a frame from the imageGenerator. "
				"It was configured?");
		return false;
	}

	// Transform perspective
	mTransformator->processData(f.data);

	// set the image to configure
	cic.setImage(f.data);

	// show the window
	cic.show();
	cic.exec();

	// get the middle point
	cv::Point p = cic.getPoint();

	// transform the height of the point
	p.y = f.data.rows - p.y;
	// put the middle point to the data processor
	mBridge.getDataProcessor()->setMiddlePoint(p);

	// Set the analyze zone
	cv::Rect zone(p.x, 0, 3, f.data.rows);
	mCoordsInterpreter.setAnalyzeZone(zone);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool GUIWaveHeightIPS::configurePerspectiveTransformation(void)
{
	GUIPerspectiveTransformator pt(this);
	std::vector<cv::Point2i> points;

	// get an image from the image generator
	Frame f;
	int err = mWaveHIPS.getImageGenerator()->captureFrame(f);
	if(err != NO_ERROR){
		debug("Error capturing a frame from the imageGenerator. It was "
				"configured?, error: %d\n", err);
		GUIUtils::showMessageBox("Error capturing a frame from the imageGenerator. "
				"It was configured?");
		return false;
	}

	// set the image to the perspective transformator
	pt.setImage(f.data);
	pt.setPointsVector(&points);

	// show and get let the user to set the perspective transformation
	pt.show();
	pt.exec();


	// get the transformator
	if(mTransformator){
		delete mTransformator;
	}
	mTransformator = pt.getPerspectiveTransformator();

	if(!mTransformator){
		debug("Error while trying to get the transformator\n");
		GUIUtils::showMessageBox("Error while trying to get the transformator");
		return false;
	}

	// set the transformator to the WaveHIPS
	mWaveHIPS.setImgProcCalibrator(mTransformator);

	// get the size of the rectangle in mm and set the relation to the DataProcessor
	float size = pt.getRectangleSize();
	float relation = static_cast<float>(f.data.rows)/size;
	debug("Setting new relation (FrameHeight/RectangleHeight): %f pixels/mm\n",
			relation);
	mBridge.getDataProcessor()->setRelation(relation);

	return true;
}


////////////////////////////////////////////////////////////////////////////////
bool GUIWaveHeightIPS::configureAnalyzeZone(void)
{
	// BY NOW WE ARE SETTING THIS PARAMS IN CONFIGURE MIDDLE POINT FUNCTION


	// by now we will use all the zone of height but 3 pixels of width at the
	// middle
//	Frame f;
//	int err = mWaveHIPS.getImageGenerator()->captureFrame(f);
//	if(err != NO_ERROR){
//		debug("Error setting the analyze zone\n");
//		return false;
//	}
////	cv::Point p1(f.data.cols/2,0);
////	cv::Point p2(3,f.data.rows);
////	cv::Rect zone(p1,p2);
//
//	cv::Rect zone(f.data.cols/2, 0, 3, f.data.rows);
//
//	mCoordsInterpreter.setAnalyzeZone(zone);




	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool GUIWaveHeightIPS::configureCanny(void)
{
	CannyParameterCalculator cc(this);


	// get an image and apply the transformation by the transformator
	Frame f;
	int err = mWaveHIPS.getImageGenerator()->captureFrame(f);
	if(err != NO_ERROR){
		debug("Error capturing a frame from the imageGenerator. It was "
				"configured?, error: %d\n", err);
		GUIUtils::showMessageBox("Error capturing a frame from the imageGenerator. "
				"It was configured?");
		return false;
	}
	if(mTransformator){
		mTransformator->processData(f.data);
	}

	// set and show the image
	cc.setImage(f.data);
	cc.show();
	cc.exec();

	// now set the parameters to the borderDetector
	mBorderDetector.setThreshold1(cc.getThreshold1());
	mBorderDetector.setThreshold2(cc.getThreshold2());
	mBorderDetector.setL2Gradient(cc.getL2Gradient());

	return true;
}


////////////////////////////////////////////////////////////////////////////////
bool GUIWaveHeightIPS::configureRealTimeDataDisplayer(void)
{
	if(!mDataDisplayer.get()){
		mDataDisplayer.reset(new RealTimeDataDisplayer());
	}
	// TODO: Here we have to show the RealTimeDataConfigurator (to set these values)
	mBridge.setRealTimeDataDisplayer(mDataDisplayer.get());
	mDataDisplayer->setMaxYValue(30);
	mDataDisplayer->setMaxXValue(60);
	mDataDisplayer->setMeasureScale(50.0);
	mDataDisplayer->setTimeScale(20.0);

	mDataDisplayer->init();

	mDataDisplayer->show();
	//mDataDisplayer.exec();

	return true;
}


////////////////////////////////////////////////////////////////////////////////
GUIWaveHeightIPS::GUIWaveHeightIPS(QWidget *parent)
    : GUIImageProcessingSystem(parent),
      mTransformator(0),
      mBridge(&mCoordsData)
{

	ui.setupUi(this);
	mIPS = &mWaveHIPS;

	createImageAnalyzer();
}

////////////////////////////////////////////////////////////////////////////////
GUIWaveHeightIPS::~GUIWaveHeightIPS()
{
	delete mTransformator;
}

////////////////////////////////////////////////////////////////////////////////
errCode GUIWaveHeightIPS::initialize(void)
{
	// we will configure the ImageGenerator
//	mImgGenWin.setImgGenerator(mWaveHIPS.getImageGenerator());


	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode GUIWaveHeightIPS::execute(void)
{
	// TODO: we probably want to load all the configurations from a file (xml)


	// first: configure the image generator
	if(!configureInput()){
		debug("Error configuring the input mode\n");
		return INCOMPLETE_CONFIGURATION;
	}

	// second: configure the perspective transformation
	if(!configurePerspectiveTransformation()){
		debug("Error configuring the Perspective Transformation\n");
		return INCOMPLETE_CONFIGURATION;
	}

	// next: configure the middle point
	if(!configureMiddlePoint()){
		debug("Error configuring the middle point\n");
		return INCOMPLETE_CONFIGURATION;
	}

	// third: set the zone that we will analyze and put to the mWaveHips
	if(!configureAnalyzeZone()){
		debug("Error configuring the AnalyzeZone\n");
		return INCOMPLETE_CONFIGURATION;
	}
	if(!configureCanny()){
		debug("Error configuring the configureCanny\n");
		return INCOMPLETE_CONFIGURATION;
	}

	// four: show and configure the RealTimeDataDisplayer
	if(!configureRealTimeDataDisplayer()){
		debug("Error configuring the RealTimeDataDisplayer\n");
		return INCOMPLETE_CONFIGURATION;
	}


	// five: start to capture and show the results



	return mWaveHIPS.execute();
}








