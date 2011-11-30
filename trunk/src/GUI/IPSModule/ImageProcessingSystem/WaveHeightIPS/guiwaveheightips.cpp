#include <strstream>
#include "guiwaveheightips.h"


////////////////////////////////////////////////////////////////////////////////
bool GUIWaveHeightIPS::connectNewWidget(GUIConfiguratorDialog *w, const QString &info)
{
	ASSERT(w);

	if(mActualWin){
		// hide the old and remove it from the layout
		mActualWin->hide();
		ui.verticalLayout->removeWidget(mActualWin);

		// save to the default "auto save" config
		// TODO:
	}

	mActualWin = w;

	// set the text to the minimun size
	ui.info->setText(info);
	ui.info->setMaximumHeight(ui.info->font().pointSize()+30);

	// add the widget and show it
	ui.verticalLayout->addWidget(w);
	w->show();

	// get the size of the text
	int th = ui.info->size().height();

	// the new height of the widget
	int nh = mWindowHeight - th - 100; // the size of the top menu

	w->setMaximumSize(QSize(mWindowWidth - 30,nh));

	// disconnect the signals and connect it to this one
	QObject::disconnect(w, SIGNAL(doneSignal(int)),
			this, SLOT(widgetDone(int)));
	QObject::connect(w, SIGNAL(doneSignal(int)),
			this, SLOT(widgetDone(int)));

	// TODO: Load the config if we have to.

	return true;
}

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
	mImgGenConfWin.setImgGenerator(mWaveHIPS.getImageGenerator());
	connectNewWidget(&mImgGenConfWin, "Plese select the configuration"
			" used for the camera/file");
	//igc.exec();
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

	int err = mWaveHIPS.getImageGenerator()->captureFrame(mAuxFrame);
	if(err != NO_ERROR){
		debug("Error capturing a frame from the imageGenerator. It was "
				"configured?, error: %d\n", err);
		GUIUtils::showMessageBox("Error capturing a frame from the imageGenerator. "
				"It was configured?");
		return false;
	}

	// Transform perspective
	mTransformator->processData(mAuxFrame.data);

	// set the image to configure
	mCoordIntConWin.setImage(mAuxFrame.data);

	connectNewWidget(&mCoordIntConWin, "Select the midpoint where the height "
			"of standing water. This point will be used to analyze the wave "
			"height.");
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool GUIWaveHeightIPS::configurePerspectiveTransformation(void)
{
	static std::vector<cv::Point2i> points;

	// get an image from the image generator
	int err = mWaveHIPS.getImageGenerator()->captureFrame(mAuxFrame);
	if(err != NO_ERROR){
		debug("Error capturing a frame from the imageGenerator. It was "
				"configured?, error: %d\n", err);
		GUIUtils::showMessageBox("Error capturing a frame from the imageGenerator. "
				"It was configured?");
		return false;
	}

	points.clear();

	// set the image to the perspective transformator
	mPerspectivTranWin.setImage(mAuxFrame.data);
	mPerspectivTranWin.setPointsVector(&points);

	connectNewWidget(&mPerspectivTranWin, "Select the 4 points that form the "
			"rectangle, in the following order: TopLeft, TopRight, "
			"BottomLeft, BottomRight");

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


	// by now skipe
	widgetDone(0);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool GUIWaveHeightIPS::configureCanny(void)
{
	// get an image and apply the transformation by the transformator
	int err = mWaveHIPS.getImageGenerator()->captureFrame(mAuxFrame);
	if(err != NO_ERROR){
		debug("Error capturing a frame from the imageGenerator. It was "
				"configured?, error: %d\n", err);
		GUIUtils::showMessageBox("Error capturing a frame from the imageGenerator. "
				"It was configured?");
		return false;
	}
	if(mTransformator){
		mTransformator->processData(mAuxFrame.data);
	}

	// set and show the image
	mCannyCalcWin.setImage(mAuxFrame.data);

	connectNewWidget(&mCannyCalcWin, "Select the best parameters used to compute "
			"the border detection algorithm (Canny).");


	return true;
}


////////////////////////////////////////////////////////////////////////////////
bool GUIWaveHeightIPS::configureRealTimeDataDisplayer(void)
{

	// TODO: Here we have to show the RealTimeDataConfigurator (to set these values)
	mBridge.setRealTimeDataDisplayer(&mDataDisplayerWin);
	mDataDisplayerWin.setMaxYValue(30);
	mDataDisplayerWin.setMaxXValue(60);
	mDataDisplayerWin.setMeasureScale(50.0);
	mDataDisplayerWin.setTimeScale(20.0);

	mDataDisplayerWin.init();

	// show it now
	connectNewWidget(&mDataDisplayerWin, "Showing the real time results");
	return true;
}


////////////////////////////////////////////////////////////////////////////////
GUIWaveHeightIPS::GUIWaveHeightIPS(QWidget *parent, int windowW, int windowH)
    : GUIImageProcessingSystem(parent, windowW, windowH),
      mState(ST_BEGIN),
      mTransformator(0),
      mBridge(&mCoordsData),
      mImgGenConfWin(this),
      mCoordIntConWin(this),
      mPerspectivTranWin(this),
      mCannyCalcWin(this),
      mDataDisplayerWin(this),
      mActualWin(0)
{

	ui.setupUi(this);
	mIPS = &mWaveHIPS;

	showMaximized();
	activateWindow();
	raise();

	// hide everything
	mImgGenConfWin.hide();
	mCoordIntConWin.hide();
	mPerspectivTranWin.hide();
	mCannyCalcWin.hide();
	mDataDisplayerWin.hide();

	// Initialize everything
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
	// second: configure the perspective transformation
	// next: configure the middle point
	// third: set the zone that we will analyze and put to the mWaveHips
	// four: show and configure the RealTimeDataDisplayer
	// five: start to capture and show the results

//	return mWaveHIPS.execute();

	widgetDone(0);
	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::widgetDone(int err)
{

	// check the actual state and do what we have
	switch(mState){
	case ST_BEGIN:
	{
		// Done begin window, now show the ST_CONFIG_INPUT

		// Show the input config dialog
		if(!configureInput()){
			debug("Error configuring the input mode\n");
			// TODO: handle error here
			return;
		}
		mState = ST_CONFIG_INPUT;
	}
		break;
	case ST_CONFIG_INPUT:
	{
		// Done config input
		if(!configurePerspectiveTransformation()){
			debug("Error configuring the Perspective Transformation\n");
			// TODO: handle error here
			return;
		}

		mState = ST_CONFIG_PERSPECTIVE;
	}
		break;

	case ST_CONFIG_PERSPECTIVE:
	{
		// Perspective configuration done check the user input

		// get the transformator
		if(mTransformator){
			delete mTransformator;
		}

		mTransformator = mPerspectivTranWin.getPerspectiveTransformator();

		if(!mTransformator){
			debug("Error while trying to get the transformator\n");
			GUIUtils::showMessageBox("Error while trying to get the transformator");
			return;
		}


		// set the transformator to the WaveHIPS
		mWaveHIPS.setImgProcCalibrator(mTransformator);

		// get the size of the rectangle in mm and set the relation to the DataProcessor
		float size = mPerspectivTranWin.getRectangleSize();
		float relation = static_cast<float>(mAuxFrame.data.rows)/size;
		debug("Setting new relation (FrameHeight/RectangleHeight): %f pixels/mm\n",
				relation);
		mBridge.getDataProcessor()->setRelation(relation);

		// everything ok, go to the next state
		if(!configureMiddlePoint()){
			debug("Error configuring the middle point\n");
			//TODO: handle error here
			return;
		}

		mState = ST_CONFIG_MIDDLE_POINT;
	}
		break;
	case ST_CONFIG_MIDDLE_POINT:
	{
		// Done configuring middle point, now extract the data
		// get the middle point
		cv::Point p = mCoordIntConWin.getPoint();

		// transform the height of the point
		p.y = mAuxFrame.data.rows - p.y;
		// put the middle point to the data processor
		mBridge.getDataProcessor()->setMiddlePoint(p);

		// Set the analyze zone
		cv::Rect zone(p.x, 0, 3, mAuxFrame.data.rows);
		mCoordsInterpreter.setAnalyzeZone(zone);

		// Everything ok, show next window
		mState = ST_CONFIG_ANALYZE_ZONE;
		if(!configureAnalyzeZone()){
			debug("Error configuring the AnalyzeZone\n");
			// TODO: Erro handling here
			return;
		}


	}
		break;
	case ST_CONFIG_ANALYZE_ZONE:
	{
		// Do nothing by now

		if(!configureCanny()){
			// TODO: Handle error here
			return;
		}


		// everything ok go to next state
		mState = ST_CONFIG_CANNY;
	}
		break;
	case ST_CONFIG_CANNY:
	{
		// Canny params selected done, check it
		// now set the parameters to the borderDetector
		mBorderDetector.setThreshold1(mCannyCalcWin.getThreshold1());
		mBorderDetector.setThreshold2(mCannyCalcWin.getThreshold2());
		mBorderDetector.setL2Gradient(mCannyCalcWin.getL2Gradient());

		// go to next state
		if(!configureRealTimeDataDisplayer()){
			debug("Error configuring the RealTimeDataDisplayer\n");

			return;
		}

		// everything ok go to next state
		mState = ST_CONFIG_DATA_DISPLAYER;
	}
		break;
	case ST_CONFIG_DATA_DISPLAYER:
	{
		// Data displayer configured, start to receive data. check for errors to

		errCode e = mWaveHIPS.execute();
		if(e != NO_ERROR){
			std::stringstream ss;
			ss << e;
			GUIUtils::showMessageBox("Error executing the main system, "
					"error number: " + ss.str());
		}

		// everything ok, go to the last state
		mState = ST_DONE;

		// by now we call automatically
		widgetDone(0);
	}
		break;

	case ST_DONE:
	{
		// close the widget
		close();
	}
		break;

	default:

		break;
	}

}








