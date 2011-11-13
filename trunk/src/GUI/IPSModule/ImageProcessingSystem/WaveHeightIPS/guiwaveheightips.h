#ifndef GUIWAVEHEIGHTIPS_H
#define GUIWAVEHEIGHTIPS_H

#include <QtGui/QWidget>
#include "ui_guiwaveheightips.h"

#include <fstream>
#include <auto_ptr.h>

#include "DebugUtil.h"
#include "GlobalDefines.h"
#include "WaveHeightIPS.h"
#include "Timestamp.h"
#include "Frame.h"
#include "GUIUtils.h"
#include "guiimageprocessingsystem.h"
#include "imagegeneratorconfigurator.h"
#include "guiperspectivetransformator.h"
#include "cannyparametercalculator.h"
#include "realtimedatadisplayer.h"
#include "CoordsInterpreter.h"
#include "PerspectiveTransformator.h"
#include "BorderDetector.h"
#include "CallBFunctor.h"

class GUIWaveHeightIPS : public GUIImageProcessingSystem
{
    Q_OBJECT

public:
    GUIWaveHeightIPS(QWidget *parent = 0);
    ~GUIWaveHeightIPS();

    /* Initialize the Image Processing System.
	 * Returns errorCode
	 */
	virtual errCode initialize(void);

	/* Executes the IPS and blocks until it finish.
	 * Returns:
	 * 	errCode
	 */
	virtual errCode execute(void);

private:
	/* Creates the main ImageAnalyzer used by the WaveIPS */
	void createImageAnalyzer(void);

	/* Configure the input */
	bool configureInput(void);

	/* configure the perspective transformation */
	bool configurePerspectiveTransformation(void);

	/* Select analyze zone */
	bool configureAnalyzeZone(void);

	/* Configure the canny parameter values */
	bool configureCanny(void);

	/* Configure and show the RealTimeDataDisplayer */
	bool configureRealTimeDataDisplayer(void);



private:
	class DataInterpreterBridge : public CallBFunctor{
	public:
		DataInterpreterBridge(RealTimeDataDisplayer *r,
				CoordsInterpreter::Data *d) :
					mData(d),
					mDataDisplayer(r)
				{
					ASSERT(d);
					ASSERT(r);

					// open the save file
					mSaveFile.open("out.txt", std::ios::out);
				}
		virtual ~DataInterpreterBridge()
		{
			mSaveFile.close();
		};

		void operator()(void)
		{
			double deltaTime = mTimeStamp.getDiffTimestamp();

			for(int i = mData->size()-1; i >= 0; --i){
				std::vector<int> &p =(*mData)[i];
				for(int j = p.size() - 1; j >= 0; --j){
					// TODO: fix this, this is an error, we are adding a lot
					// of points to the same time
					mDataDisplayer->addNewPoint(deltaTime, p[j]);
					mSaveFile << deltaTime << "\t" << p[j] << std::endl;
				}

			}

			// probably we want to save this data to some file

		}

	private:
		Timestamp					mTimeStamp;
		CoordsInterpreter::Data 	*mData;
		RealTimeDataDisplayer		*mDataDisplayer;
		std::ofstream				mSaveFile;

	};


private:
    Ui::GUIWaveHeightIPSClass ui;

    // The real IPS
    WaveHeightIPS				mWaveHIPS;
    PerspectiveTransformator 	*mTransformator;

    // The ImageAnalyzer and the processors
    ImageAnalyzer				mImageAnalyzer;
    BorderDetector				mBorderDetector;
    CoordsInterpreter			mCoordsInterpreter;
    CoordsInterpreter::Data		mCoordsData;

    // The data interpreter
    RealTimeDataDisplayer		mDataDisplayer;
    DataInterpreterBridge		mBridge;




};

#endif // GUIWAVEHEIGHTIPS_H
