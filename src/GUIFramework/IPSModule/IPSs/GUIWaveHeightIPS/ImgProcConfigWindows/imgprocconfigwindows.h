#ifndef IMGPROCCONFIGWINDOWS_H
#define IMGPROCCONFIGWINDOWS_H

#include <QtGui/QWidget>

#include "configwindow.h"
#include "ImageGenerator.h"

class ImgProcConfigWindows : public ConfigWindow
{
    Q_OBJECT

public:
    ImgProcConfigWindows(ImageGenerator *ig,
    		QWidget *parent = 0, const std::string &name = "unnamed") :
    	ConfigWindow(parent, name),
    	mImgGenerator(ig)
    {
    	ASSERT(ig);
    }

    virtual ~ImgProcConfigWindows() {};

    // sets the Image generator to be used
    void setImageGenerator(ImageGenerator *ig)
    {
    	ASSERT(ig);
    	mImgGenerator = ig;
    }

	/* Function used to load the configurations from a xml file
	 * Returns:
	 * 	errCode
	 */
	virtual errCode loadConfig(const TiXmlElement *)
	{
		debug("Unimplemented\n");
		return FEATURE_NOT_SUPPORTED;
	}

	/* Save the data to an xml and return it.
	 * Returns:
	 * 	0			on Error
	 * 	xml			on success */
	virtual std::auto_ptr<TiXmlElement> getConfig(void) const
	{
		debug("unimplemented\n");
		return std::auto_ptr<TiXmlElement>(0);
	}

	/* Function used to retrieve the info about the ConfigWindow. This is
	 * an optional function.
	 */
	virtual QString getInfo(void) const {return QString("");}

	/* Function called when the window get visible */
	virtual void windowVisible(void) {};

	/* Function called when the window get invisible */
	virtual void windowInvisible(void) {};

	/* Function called when the user click on "Next Button" and the ConfigWindow
	 * will be hide and show the next one. This function shall return NO_ERROR
	 * if the ConfigWindow can be closed, or the error and the string error
	 * by param
	 */
	virtual errCode finish(QString &error) = 0;

	/* We have to implement the signal used to advise when the actual ConfigWindow
	 * is finish automatically.
	 */
signals:
	// Advise when the configurator dialog is finish sending the errCode
	// associated and the info about the error associated.
	void doneSignal(int, QString info);

protected:
    ImageGenerator		*mImgGenerator;
};

#endif // IMGPROCCONFIGWINDOWS_H
