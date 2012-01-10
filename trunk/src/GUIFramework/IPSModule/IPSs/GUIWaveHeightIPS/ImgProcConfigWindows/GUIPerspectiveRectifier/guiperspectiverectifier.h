#ifndef GUIPERSPECTIVERECTIFIER_H
#define GUIPERSPECTIVERECTIFIER_H

#include <QtGui/QWidget>
#include "ui_guiperspectiverectifier.h"

#include <opencv2/core/core.hpp>

#include <vector>

#include "imgprocconfigwindows.h"
#include "ImageProcessor.h"
#include "zoomedlabel.h"

class GUIPerspectiveRectifier : public ImgProcConfigWindows
{
    Q_OBJECT

public:
    GUIPerspectiveRectifier(ImageGenerator *ig, QWidget *parent = 0);
    ~GUIPerspectiveRectifier();


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

	/* Function called when the user click on "Next Button" and the ConfigWindow
	 * will be hide and show the next one. This function shall return NO_ERROR
	 * if the ConfigWindow can be closed, or the error and the string error
	 * by param
	 */
	errCode finish(QString &error);

	/* Updates the image to be shown */
	void updateImg(void);

	/* Set the perspective ImageProcessor used to change the perspective.
	 * This image processor must be the "PerspectiveRectifier" and we will
	 * use the interface of this one
	 */
	void setPerspectiveRectifierIP(ImageProcessor *ip);

	/* Sets the WaveHeightAnalyzer to set the size of the rectangle in mms. */
	void setWaveHeightAnalyzerIP(ImageProcessor *ip);


public slots:

	void onPreviewClicked(void);
	void onClearClicked(void);



	/* We have to implement the signal used to advise when the actual ConfigWindow
	 * is finish automatically.
	 */
signals:
	// Advise when the configurator dialog is finish sending the errCode
	// associated and the info about the error associated.
	void doneSignal(int, QString info);


private:

	// sort the points in the next order: TopLeft, TopRight, BottomLeft, BottomRight
	void sortPoints(const std::vector<QPoint> &input, std::vector<QPoint> &output) const;

	// set the points to the ImageProcessor and returns the errorCode associated
	errCode setPointsToIP(void);

	// Set the rectangle size to the WaveHeightAnalyzer
	errCode setRectangleSizeToIP(void);

	// Auxiliar function used to get a double value from an xml
	double getValue(const TiXmlElement *elem, const char *attr, bool *ok = 0);
	void setValue(TiXmlElement *elem, const char *child, const char *attr,
			double value) const;


private:
    Ui::GUIPerspectiveRectifierClass ui;

    ZoomedLabel			mZoomedLabel;
    ImageProcessor		*mImgProc;
    ImageProcessor		*mWaveHAnalyzerIP;
    // size of the dest img
    int					mImgSizeX;
    int					mImgSizeY;

    Frame 				mFrame;
};

#endif // GUIPERSPECTIVERECTIFIER_H
