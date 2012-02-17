#ifndef GUIREALTIMEDATADISPLAYER_H
#define GUIREALTIMEDATADISPLAYER_H

#include <QtGui/QWidget>
#include <qgraphicsitem.h>
#include "ui_guirealtimedatadisplayer.h"

#include "ImageGenerator.h"
#include "configwindow.h"
#include "ImageProcessor.h"

#include "framelabeldisplayer.h"
#include "RealTimePlot.h"

// reserve 5 min * 60 seconds * 25 fps
#define RTDD_RESERVE_NUM_ITEMS		5*60*25


class GUIRealTimeDataDisplayer : public ConfigWindow
{
    Q_OBJECT

    static const int FRAME_UPDATE_TIME		=	500; //ms

public:

    class CallbackFunctor {
    public:
    	virtual void operator()(int) = 0;
    };

    // Events used to advise throw the callback functor
    enum {
    	EVENT_START_CAPTURING,
    	EVENT_STOP_CAPTURING,
    };

public:
    GUIRealTimeDataDisplayer(ImageGenerator *ig, QWidget *parent = 0);
    virtual ~GUIRealTimeDataDisplayer();

    /* Function used to load the configurations from a xml file
  	 * Returns:
  	 * 	errCode
  	 */
  	errCode loadConfig(const TiXmlElement *);

  	/* Save the data to an xml and return it.
  	 * Returns:
  	 * 	0			on Error
  	 * 	xml			on success */
  	std::auto_ptr<TiXmlElement> getConfig(void) const;

  	/* Function used to retrieve the info about the ConfigWindow. This is
  	 * an optional function.
  	 */
  	QString getInfo(void) const;

  	/* Function called when the window get visible */
  	void windowVisible(void);

  	/* Function called when the window get invisible */
	void windowInvisible(void);

  	/* Function called when the user click on "Next Button" and the ConfigWindow
  	 * will be hide and show the next one. This function shall return NO_ERROR
  	 * if the ConfigWindow can be closed, or the error and the string error
  	 * by param
  	 */
  	errCode finish(QString &error);

  	/* Adds a new point to be drawn */
  	void addPoint(double timeSecs, double mmHeight);

  	/* Clear all the points on the windows */
  	void clearPoints(void);

  	/* Sets the callbacks functors to advise when we want to start/stop capturing
  	 * The events passed are:
  	 * 	EVENT_START_CAPTURING,
     *	EVENT_STOP_CAPTURING,
  	 */
  	void setEventCallbackFunctor(CallbackFunctor *);

public slots:
	void onStartCapturingClicked(void);
	void onFrameChecboxToggled(bool);
	void onPloterChecboxToggled(bool);

protected:
    virtual void timerEvent(QTimerEvent *e);

private:
	void showFrame(void);
	void enableTimers(void);
	void disableTimers(void);

private:

    Ui::GUIRealTimeDataDisplayerClass ui;

	CallbackFunctor					*mCallbackFunct;
	RealTimePlot					mPlotter;
	FrameLabelDisplayer				mFrameDisplayer;
	int								mTimerId;
	ImageGenerator					*mImgGenerator;

};

#endif // GUIREALTIMEDATADISPLAYER_H
