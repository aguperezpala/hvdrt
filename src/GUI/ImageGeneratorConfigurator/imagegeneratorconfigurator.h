#ifndef IMAGEGENERATORCONFIGURATOR_H
#define IMAGEGENERATORCONFIGURATOR_H

#include <QtGui/QWidget>
#include <qdialog.h>
#include "ui_imagegeneratorconfigurator.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>

#include <auto_ptr.h>

#include "DebugUtil.h"
#include "ImageGenerator.h"
#include "FrameListener.h"

class ImageGeneratorConfigurator : public QDialog
{
    Q_OBJECT

public:
    ImageGeneratorConfigurator(QWidget *parent = 0);
    ~ImageGeneratorConfigurator();

    /* configures an ImageGenerator */
    void setImgGenerator(ImageGenerator *);


    /* When the windows close */
    virtual void closeEvent(QCloseEvent * event);


public slots:
    void onSourceCameraClicked(void);
    void onSourceFileClicked(void);
    void onSetParamettersClicked(void);
    void onGetParamettersClicked(void);
    void onDoneClicked(void);



private:
    class FrameProcessorTester : public FrameListener {
    	public:
    		FrameProcessorTester()
    		{
    			cv::namedWindow("ImgGenTester");
    		}
    		~FrameProcessorTester()
    		{
    			cv::destroyWindow("ImgGenTester");
    		}


    		errCode processFrame(Frame &frame)
    		{
    			cv::imshow("ImgGenTester", frame.data);

    			return NO_ERROR;
    		}
    };


private:
    /* Function used to show the resolution of the ImageGenerator */
    void showResolution(void);

    /* set resolution */
    bool setResolution(double width, double height);

    /* get properties */
    void getProperties(void);


private:
    Ui::ImageGeneratorConfiguratorClass ui;

    ImageGenerator							*mImgGen;
    std::auto_ptr<FrameProcessorTester> 	mFrameProcTester;
};

#endif // IMAGEGENERATORCONFIGURATOR_H
