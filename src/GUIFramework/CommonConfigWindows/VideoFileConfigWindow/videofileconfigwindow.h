#ifndef VIDEOFILECONFIGWINDOW_H
#define VIDEOFILECONFIGWINDOW_H

#include <QtGui/QWidget>
#include "ui_videofileconfigwindow.h"

#include "configwindow.h"
#include "ImageGenerator.h"
#include "framelabeldisplayer.h"

class VideoFileConfigWindow : public ConfigWindow
{
    Q_OBJECT

public:
    VideoFileConfigWindow(QWidget *parent = 0);
    virtual ~VideoFileConfigWindow();

    /* Set the ImageGenerator to be used.
     * This function checks if the ImageGenerator is using a video file */
    errCode setImageGenerator(ImageGenerator *);

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

	/* Function called when the user click on "Next Button" and the ConfigWindow
	 * will be hide and show the next one. This function shall return NO_ERROR
	 * if the ConfigWindow can be closed, or the error and the string error
	 * by param
	 */
	errCode finish(QString &error);

public slots:
	void onSlideChange(int);


private:
	/* Read the video file properties */
	void readVideoProperties(void);

private:
    Ui::VideoFileConfigWindowClass ui;

    ImageGenerator					*mImgGenerator;
    FrameLabelDisplayer				mDisplayer;
};

#endif // VIDEOFILECONFIGWINDOW_H
