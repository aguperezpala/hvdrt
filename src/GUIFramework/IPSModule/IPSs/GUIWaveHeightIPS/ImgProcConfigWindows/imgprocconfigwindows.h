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

protected:
    ImageGenerator		*mImgGenerator;
};

#endif // IMGPROCCONFIGWINDOWS_H
