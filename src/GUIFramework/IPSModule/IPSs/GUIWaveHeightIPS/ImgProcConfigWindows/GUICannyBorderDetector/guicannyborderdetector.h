#ifndef GUICANNYBORDERDETECTOR_H
#define GUICANNYBORDERDETECTOR_H

#include <QtGui/QWidget>
#include "ui_guicannyborderdetector.h"

#include "Frame.h"
#include "imgprocconfigwindows.h"
#include "ImageProcessor.h"

class GUICannyBorderDetector : public ImgProcConfigWindows
{
    Q_OBJECT

public:
    GUICannyBorderDetector(ImageGenerator *ig, QWidget *parent = 0);
    ~GUICannyBorderDetector();


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

	/* Sets the CannyBorderDetector ImageProcessor to apply to the image */
	void setCannyImgProcessor(ImageProcessor *ip);

	/* Sets the ImageRectifier processor, used to convert the actual captured
	 * image and rectify it
	 */
	void setPerspectiveRectifierIP(ImageProcessor *ip);

public slots:
	void onSlide1Change(int v);
	void onSlide2Change(int v);
	void onButtonCannyPressed(void);
	void onButtonShowRealPressed(void);


private:
	// Sets the values to the CannyBorderDetector ImageProcessor
	void setCannyImgProcData(void);
	void showImage(const cv::Mat &img);

private:
    Ui::GUICannyBorderDetectorClass ui;

    Frame							mFrame;
    QImage							mCannyImg;
    ImageProcessor					*mCannyImgProc;
    ImageProcessor					*mPerspectiveRectImgProc;

};

#endif // GUICANNYBORDERDETECTOR_H
