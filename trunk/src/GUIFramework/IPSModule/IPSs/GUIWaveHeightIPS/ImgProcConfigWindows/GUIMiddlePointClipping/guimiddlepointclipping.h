#ifndef GUIMIDDLEPOINTCLIPPING_H
#define GUIMIDDLEPOINTCLIPPING_H

#include <QtGui/QWidget>
#include "ui_guimiddlepointclipping.h"

#include <opencv2/core/core.hpp>

#include "Frame.h"
#include "imgprocconfigwindows.h"
#include "ImageProcessor.h"
#include "zoomedlabel.h"


class GUIMiddlePointClipping : public ImgProcConfigWindows
{
    Q_OBJECT

public:
    GUIMiddlePointClipping(ImageGenerator *ig, QWidget *parent = 0);
    virtual ~GUIMiddlePointClipping();

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

   	/* Sets the MiddlePointClipping ImageProecssor */
	void setPerspectiveRectifierIP(ImageProcessor *ip);

   	/* Sets the MiddlePointClipping ImageProecssor */
   	void setMiddlePointClippingIP(ImageProcessor *ip);

   	/* Sets the WaveGHeightAnalyzer ImageProcessor */
   	void setWaveHeightAnalyzerIP(ImageProcessor *ip);


public slots:
	void onClearClicked(void);

private:
	// updates the frame and set the correct content.
	void refreshFrame(void);

	// save the data to the MiddlePointClipping IP
	errCode saveMiddlePointClippingData(void);

	// save the data to the WaveHEightAnalyzer IP
	errCode saveWaveHeightAnalyzerData(void);


private:
    Ui::GUIMiddlePointClippingClass ui;

    ZoomedLabel			mZoomedLabel;
	ImageProcessor		*mPerspectiveIP;
	ImageProcessor		*mMiddlePointCIP;
	ImageProcessor		*mWaveHeightAIP;

	Frame				mFrame;

};

#endif // GUIMIDDLEPOINTCLIPPING_H
