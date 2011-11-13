#include <cassert>

#include "cannyparametercalculator.h"


///////////////////////////////////////////////////////////////////////////////
int CannyParameterCalculator::showMessage(const QString &m)
{
	QMessageBox msg(this);
	msg.setText(m);
	msg.exec();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
bool CannyParameterCalculator::IplImage2QImage(const cv::Mat &iplImgMat, QImage &ref)
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

void CannyParameterCalculator::showImage(const cv::Mat &img)
{
	if(img.data){
		QImage qImg;
		IplImage2QImage(img, qImg);

		// gets the resolution to show the image
		if(img.rows > ui.image_label->minimumHeight() || img.cols > ui.image_label->minimumWidth()){
			// have to set new proportion
			qImg = qImg.scaled(ui.image_label->minimumWidth(), ui.image_label->minimumHeight(), Qt::KeepAspectRatio);
		}

		ui.image_label->setPixmap(QPixmap::fromImage(qImg));
	}
}


///////////////////////////////////////////////////////////////////////////////
CannyParameterCalculator::CannyParameterCalculator(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	// connect the signals
	QObject::connect(ui.threshole1_slider,SIGNAL(valueChanged(int)), this,
			SLOT(onSlide1Change(int)));
	QObject::connect(ui.threshole2_slider,SIGNAL(valueChanged(int)), this,
			SLOT(onSlide2Change(int)));

	QObject::connect(ui.cannyBuild_button,SIGNAL(clicked(bool)), this,
				SLOT(onButtonCannyPressed(void)));
	QObject::connect(ui.loadImg_button,SIGNAL(clicked(bool)), this,
				SLOT(onButtonLoadPressed(void)));

	ui.threshole1_text->setText("10");
	ui.threshole2_text->setText("80");

}


///////////////////////////////////////////////////////////////////////////////
CannyParameterCalculator::~CannyParameterCalculator()
{

}

///////////////////////////////////////////////////////////////////////////////
void CannyParameterCalculator::setImage(const cv::Mat &img)
{
	ui.loadImg_button->setEnabled(false);
	ui.loadImg_button->setVisible(false);

	mOriginalImage = img.clone();
	mTransformedImage = mOriginalImage.clone();
	showImage(mTransformedImage);
}


///////////////////////////////////////////////////////////////////////////////
void CannyParameterCalculator::onSlide1Change(int v)
{
	QString num;
	num.setNum(v);
	ui.threshole1_text->setText(num);
}

///////////////////////////////////////////////////////////////////////////////
void CannyParameterCalculator::onSlide2Change(int v)
{
	QString num;
	num.setNum(v);
	ui.threshole2_text->setText(num);
}

///////////////////////////////////////////////////////////////////////////////
void CannyParameterCalculator::onButtonCannyPressed(void)
{
	if(!mOriginalImage.data){
		showMessage("Seleccione una imagen primero");
		return;
	}

	// get the params
	bool ok = false;
	double th1 = ui.threshole1_text->text().toDouble(&ok);
	if(!ok){
		showMessage("Threshole1 debe ser valor numerico");
		return;
	}

	double th2 = ui.threshole2_text->text().toDouble(&ok);
	if(!ok){
		showMessage("Threshole2 debe ser valor numerico");
		return;
	}

	mTransformedImage = mOriginalImage.clone();

	cvtColor(mTransformedImage, mTransformedImage, CV_BGR2GRAY);
	cv::GaussianBlur(mTransformedImage, mTransformedImage, cv::Size(7,7), 5.0, 5.0);
	Canny(mTransformedImage, mTransformedImage, th1, th2, 3, ui.l2Gradient_checkBox->isChecked());

	mThreshold1 = th1;
	mThreshold2 = th2;
	mL2Gradient = ui.l2Gradient_checkBox->isChecked();

	showImage(mTransformedImage);
}

///////////////////////////////////////////////////////////////////////////////
void CannyParameterCalculator::onButtonLoadPressed(void)
{
	QString filename = QFileDialog::getOpenFileName(this, "Image", ".", "*");

	if(filename.isEmpty()){
		return;
	}

	mOriginalImage = cv::imread(filename.toAscii().data());

	if(!mOriginalImage.data){
		showMessage("Error abriendo la imagen");
		return;
	}

	// copy to the image to be showed
	mTransformedImage = mOriginalImage.clone();
	showImage(mTransformedImage);

}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////






