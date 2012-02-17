#ifndef GUIWAVEHEIGHTIPS_H
#define GUIWAVEHEIGHTIPS_H

#include <QtGui/QWidget>
#include "ui_guiwaveheightips.h"
#include <qthread.h>

#include <fstream>
#include <auto_ptr.h>

#include "DebugUtil.h"
#include "GlobalDefines.h"
#include "WaveHeightIPS.h"
#include "WaveHeightException.h"
#include "Timestamp.h"
#include "Frame.h"
#include "GUIUtils.h"

// All the windows used and the window manager
#include "configwindowmanager.h"

#include "guiimageprocessingsystem.h"
#include "guicannyborderdetector.h"
#include "guimiddlepointclipping.h"
#include "guiperspectiverectifier.h"
#include "guirealtimedatadisplayer.h"
#include "cameraconfigwindow.h"
#include "videofileconfigwindow.h"

#include "DataDisplayerBridge.h"
#include "dataanalyzewindow.h"



class GUIWaveHeightIPS : public GUIImageProcessingSystem
{
    Q_OBJECT

public:
    GUIWaveHeightIPS(QWidget *parent = 0,  int windowW = 800, int windowH = 600);
    ~GUIWaveHeightIPS();

    /* Returns the name of the IPS */
    virtual const std::string &getName(void) const;

    /* Returns the info of the IPS */
    virtual const std::string &getInfo(void) const;


    /* Initialize the Image Processing System.
	 * Returns errorCode
	 */
	virtual errCode initialize(void);

	/* Executes the IPS and blocks until it finish.
	 * Returns:
	 * 	errCode
	 */
	virtual errCode execute(void);

protected:
    /* The close event will emitt the "ClosingSignal()" used by the parent class */
    void closeEvent(QCloseEvent *);

private:
	/* We will define a class that will handle the events of the
	 * RealTimeDataDisplayer (event StartCapturing and StopCapturing) that
	 * will start/stops the IPS
	 */
	class RealTimeDDEventReceiver : public GUIRealTimeDataDisplayer::CallbackFunctor,
	 public QThread {
	public:
		RealTimeDDEventReceiver(WaveHeightIPS *ips);
		virtual ~RealTimeDDEventReceiver(void);

		void operator()(int);
		void run();

	private:
		WaveHeightIPS		*mIPS;
		errCode				mError;
	};


private:
	// Configure the WaveHeightIPS
	void configureIPS(void) throw (WaveHeightException);

	// Configure the windows
	void configureCannyWin(void) throw (WaveHeightException);
	void configureMiddlePointWin(void) throw (WaveHeightException);
	void configurePerspectiveWin(void) throw (WaveHeightException);
	void configureRealTimeDDWin(void) throw (WaveHeightException);
	void configureVideoFileConfWin(void) throw (WaveHeightException);
	void configureCameraConfigWin(void) throw (WaveHeightException);

	// Configure the window manager
	void configureWindowManager(bool fromFile) throw (WaveHeightException);

	// Add the session xml info to a XmlElement. If the info already exists
	// is repleaced
	void addXmlSessionInfo(TiXmlElement *root);

	// Fills the GUI session section from an xml element. Returns errCode
	errCode fillGuiFromXml(const TiXmlElement *elem);


	// Interface handling

	// Enable/Disable the options of the session
	void enableOptions(bool enable);

	// Check the fields of the session. Returns true if there are ok, false otherwise
	bool checkFields(void);

	// clear all the fields
	void clearFields(void);


private slots:
	void onWinMngrClose(void);
	void onLoadSessionClicked(void);
	void onNewSessionClicked(void);
	void onStartClicked(void);
	void onOutClicked(void);
	void onAnalyzeClicked(void);


private:
	Ui::GUIWaveHeightIPSClass ui;

	WaveHeightIPS						mWaveHeightIPS;
	ConfigWindowManager					mConfigWinMngr;

	// Config windows used
	GUICannyBorderDetector				mCannyWin;
	GUIMiddlePointClipping				mMiddlePointWin;
	GUIPerspectiveRectifier				mPerspectiveWin;
	GUIRealTimeDataDisplayer			mRealTimeDDWin;
	VideoFileConfigWindow				mVideoFileWin;
	CameraConfigWindow					mCameraConfWin;

	RealTimeDDEventReceiver				mEventReceiver;
	DataDisplayerBridge					mDataDisplayerBridge;

	QString								mInputPath;
	TiXmlDocument						*mDocument;

	DataAnalyzeWindow					mDataAnalyzeWin;
	errCode								mLastError;
};

#endif // GUIWAVEHEIGHTIPS_H
