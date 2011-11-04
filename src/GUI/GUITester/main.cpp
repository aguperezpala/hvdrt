

#include <QtGui>
#include <QApplication>


#include <opencv2/core/core.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpumat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "DebugUtil.h"
#include "ImageGenerator.h"
#include "imagegeneratorconfigurator.h"


static void testImageGeneratorConfig(QApplication *app)
{
	ImageGeneratorConfigurator igc;
	ImageGenerator ig;

	igc.setImgGenerator(&ig);
	igc.show();
	app->exec();
	debug("Aa\n");

	return;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    testImageGeneratorConfig(&a);
}
