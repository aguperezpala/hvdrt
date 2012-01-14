#ifndef GUIREALTIMEDATADISPLAYER_H
#define GUIREALTIMEDATADISPLAYER_H

#include <QtGui/QWidget>
#include <qgraphicsitem.h>
#include "ui_guirealtimedatadisplayer.h"

#include "imgprocconfigwindows.h"
#include "ImageProcessor.h"

// reserve 5 min * 60 seconds * 25 fps
#define RTDD_RESERVE_NUM_ITEMS		5*60*25


class GUIRealTimeDataDisplayer : public ImgProcConfigWindows
{
    Q_OBJECT

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
	void onChangeScaleClicked(void);
	void onStartCapturingClicked(void);
	void onShowLinesToggled(bool);


private:
	// create the axis
	void createAxis(void);

	// create a line between 2 points
	inline void createNewLine(float x1, float y1, float x2, float y2)
	{
		QGraphicsLineItem *l = mScene.addLine(x1, y1, x2, y2);
		if(!mShowLines){
			l->hide();
		}
		mLines.push_back(l);
	}

	// create a point using the new scale
	void createNewPoint(float x, float y);

	// Redraw every object using the old scale and assuming that the
	// mXAxisScale and mYAxisScale are the new scales
	void redrawAll(float oldXScale, float oldYScale);

	// read the Axis values
	bool readAxisScales(float &xScale, float &yScale);

	// update the axis size given a new point
	void updateAxis(float x, float y);



private:
	typedef std::vector<QGraphicsEllipseItem *> PointsVector;
	typedef std::vector<QGraphicsLineItem *> 	LinesVector;

    Ui::GUIRealTimeDataDisplayerClass ui;

    QGraphicsScene					mScene;
    // Axis scales
    float							mXAxisScale;
	float							mYAxisScale;
	float							mOldXAxisScale;
	float							mOldYAxisScale;

	PointsVector					mPoints;
	LinesVector						mLines;
	bool							mShowLines;
	CallbackFunctor					*mCallbackFunct;

	QGraphicsLineItem 				*mXAxisLine;
	QGraphicsLineItem 				*mYAxisLine;

};

#endif // GUIREALTIMEDATADISPLAYER_H
