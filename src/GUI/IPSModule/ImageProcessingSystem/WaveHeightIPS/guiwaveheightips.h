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
#include "coordsinterpreterconfigurator.h"
#include "imagegeneratorconfigurator.h"
#include "guiperspectivetransformator.h"
#include "cannyparametercalculator.h"
#include "realtimedatadisplayer.h"
#include "CoordsInterpreter.h"
#include "PerspectiveTransformator.h"
#include "BorderDetector.h"
#include "CallBFunctor.h"
#include "DataProcessor.h"
#include "ImgPointPainter.h"

class GUIWaveHeightIPS : public GUIImageProcessingSystem
{
    Q_OBJECT

public:
    GUIWaveHeightIPS(QWidget *parent = 0,  int windowW = 800, int windowH = 600);
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

public slots:
	/* This slot is called when some config dialog or widget is done */
	void widgetDone(int);



protected:
	/* Function used to set the a new widget to show. */
	bool connectNewWidget(GUIConfiguratorDialog *w, const QString &info);


private:
	/* Creates the main ImageAnalyzer used by the WaveIPS */
	void createImageAnalyzer(void);

	/* Configure the input */
	bool configureInput(void);

	/* Configure the middle point */
	bool configureMiddlePoint(void);

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
		DataInterpreterBridge(CoordsInterpreter::Data *d) :
					mData(d),
					mDataDisplayer(0)
				{
					ASSERT(d);
					d->resize(3);
					mDataProcessor.setFileName("out.txt");
					mDataProcessor.setRelation(1.0f);
					mDataProcessor.setData(d);


				}
		virtual ~DataInterpreterBridge()
		{
		};

		void setRealTimeDataDisplayer(RealTimeDataDisplayer *r)
		{
			ASSERT(r);
			mDataDisplayer = r;
		}

		DataProcessor *getDataProcessor(void) {return &mDataProcessor;}

		void operator()(void)
		{
			mDataProcessor();
			if(mDataDisplayer){
				mDataDisplayer->addNewPoint(mDataProcessor.getLastTime(),
						mDataProcessor.getLastHeightCalculated()/10.0f);
			}
		}

	private:

		CoordsInterpreter::Data 	*mData;
		RealTimeDataDisplayer		*mDataDisplayer;
		DataProcessor				mDataProcessor;

	};


private:
	enum {
		ST_BEGIN = 0,
		ST_CONFIG_INPUT,
		ST_CONFIG_PERSPECTIVE,
		ST_CONFIG_MIDDLE_POINT,
		ST_CONFIG_ANALYZE_ZONE,
		ST_CONFIG_CANNY,
		ST_CONFIG_DATA_DISPLAYER,
		ST_DONE
	};

private:
    Ui::GUIWaveHeightIPSClass ui;

    // the actual state
    int							mState;

    // The real IPS
    WaveHeightIPS				mWaveHIPS;
    PerspectiveTransformator 	*mTransformator;

    // The ImageAnalyzer and the processors
    ImageAnalyzer				mImageAnalyzer;
    BorderDetector				mBorderDetector;
    CoordsInterpreter			mCoordsInterpreter;
    CoordsInterpreter::Data		mCoordsData;

    DataInterpreterBridge		mBridge;

    // Auxiliar frame used
    Frame						mAuxFrame;

    // All the windows used
    ImageGeneratorConfigurator		mImgGenConfWin;
    CoordsInterpreterConfigurator	mCoordIntConWin;
    GUIPerspectiveTransformator		mPerspectivTranWin;
    CannyParameterCalculator		mCannyCalcWin;
    RealTimeDataDisplayer			mDataDisplayerWin;

    // The widget that is actually showing
    GUIConfiguratorDialog			*mActualWin;


};

#endif // GUIWAVEHEIGHTIPS_H
