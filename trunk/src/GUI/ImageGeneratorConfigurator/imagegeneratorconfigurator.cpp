#include "imagegeneratorconfigurator.h"

ImageGeneratorConfigurator::ImageGeneratorConfigurator(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	QObject::connect(ui.camera_button,SIGNAL(clicked(bool)), this,
					SLOT(onSourceCameraClicked(void)));
	QObject::connect(ui.file_button,SIGNAL(clicked(bool)), this,
						SLOT(onSourceFileClicked(void)));

}

ImageGeneratorConfigurator::~ImageGeneratorConfigurator()
{

}


/* configures an ImageGenerator */
void ImageGeneratorConfigurator::setImgGenerator(ImageGenerator *imgGen)
{
	ASSERT(imgGen);
	mImgGen = imgGen;
	// add the tester
	debug("Antes\n");
	mFrameProcTester.reset(new FrameProcessorTester());
	mImgGen->addNewListener(mFrameProcTester.get());
	debug("Despues\n");

}

void ImageGeneratorConfigurator::onSourceCameraClicked(void)
{

}

void ImageGeneratorConfigurator::onSourceFileClicked(void)
{

}

void ImageGeneratorConfigurator::onSetResolutionClicked(void)
{

}

void ImageGeneratorConfigurator::closeEvent(QCloseEvent * event)
{
	debug("CLOSING\n");
	mImgGen->removeFrameListener(mFrameProcTester.get());
}
