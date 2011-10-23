#include <cassert>
#include <qmessagebox.h>
#include <qfiledialog.h>

#include "zoneselector.h"


///////////////////////////////////////////////////////////////////////////////
int ZoneSelector::showMessage(const QString &m)
{
	QMessageBox msg(this);
	msg.setText(m);
	msg.exec();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
bool ZoneSelector::IplImage2QImage(const cv::Mat &iplImgMat, QImage &ref)
{
	IplImage iplImg = iplImgMat;
	int h = iplImg.height;
	int w = iplImg.width;
	int channels = iplImg.nChannels;
	QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
	char *data = iplImg.imageData;

	for (int y = 0; y < h; y++, data += iplImg.widthStep)
	{
		for (int x = 0; x < w; x++)
		{
			char r, g, b, a = 0;
			if (channels == 1)
			{
				r = data[x * channels];
				g = data[x * channels];
				b = data[x * channels];
			}
			else if (channels == 3 || channels == 4)
			{
				r = data[x * channels + 2];
				g = data[x * channels + 1];
				b = data[x * channels];
			}

			if (channels == 4)
			{
				a = data[x * channels + 3];
				qimg->setPixel(x, y, qRgba(r, g, b, a));
			}
			else
			{
				qimg->setPixel(x, y, qRgb(r, g, b));
			}
		}
	}
	ref = *qimg;
	delete qimg;


	return true;
}


///////////////////////////////////////////////////////////////////////////////
ZoneSelector::ZoneSelector(QWidget *parent)
    : QWidget(parent),
      mPointsVector(0)
{
	ui.setupUi(this);
	mMouseLabel = new MouseQLabel(this);
	mMouseLabel->setObjectName(QString::fromUtf8("img_label"));
	ui.verticalLayout->addWidget(mMouseLabel);
	mMouseLabel->setLabelX(ui.x_label);
	mMouseLabel->setLabelY(ui.y_label);

	QString filename = QFileDialog::getOpenFileName(this, "Image", ".", "*");

	if(filename.isEmpty()){
		return;
	}

	QImage img;
	img.load(filename);
	mMouseLabel->setImage(img);
}

///////////////////////////////////////////////////////////////////////////////
ZoneSelector::~ZoneSelector()
{


}



///////////////////////////////////////////////////////////////////////////////
void ZoneSelector::setImage(const cv::Mat &img)
{
	if(!mImage.empty()){
		mImage.release();
	}

	mImage = img.clone();
}


///////////////////////////////////////////////////////////////////////////////
void ZoneSelector::setPointsVector(std::vector<cv::Point2i> *vec)
{
	assert(mPointsVector == 0);
	mPointsVector = vec;
}


