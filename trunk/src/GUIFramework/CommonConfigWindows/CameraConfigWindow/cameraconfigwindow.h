#ifndef CAMERACONFIGWINDOW_H
#define CAMERACONFIGWINDOW_H

#include <QtGui/QWidget>
#include "ui_cameraconfigwindow.h"
#include "configwindow.h"
#include "ImageGenerator.h"

#include "framelabeldisplayer.h"
#include "Frame.h"


#define QV4L_COMMAND_PATH	"qv4l2"

class CameraConfigWindow : public ConfigWindow
{
    Q_OBJECT

    static const int FRAME_TIME_UPDATE		=	30;	//ms

public:
    CameraConfigWindow(QWidget *parent = 0);
    virtual ~CameraConfigWindow();

    /* Set the ImageGenerator to be used
     * This function checks if the ImageGenerator is using a camera
     */
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


protected slots:
	void onqv4lClicked(void);


    virtual void timerEvent(QTimerEvent *e);


private:
    Ui::CameraConfigWindowClass ui;

    ImageGenerator				*mImgGenerator;
    FrameLabelDisplayer			mFrameDisplayer;
    int							mTimerId;
    Frame						mFrame;
};

#endif // CAMERACONFIGWINDOW_H
