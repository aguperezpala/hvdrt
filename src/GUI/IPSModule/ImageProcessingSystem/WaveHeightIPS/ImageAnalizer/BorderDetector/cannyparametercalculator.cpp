#include <cassert>

#include "DebugUtil.h"
#include "cannyparametercalculator.h"


///////////////////////////////////////////////////////////////////////////////
int CannyParameterCalculator::showMessage(const QString &m)
{
	QMessageBox msg(this);
	msg.setText(m);
	msg.exec();

	return 0;
}


void CannyParameterCalculator::showImage(const cv::Mat &img)
{
	if(img.data){
		QImage qImg;
		GUIUtils::IplImage2QImage(img, qImg);

		ui.image_label->setScaledContents(true);

		ui.image_label->setPixmap(QPixmap::fromImage(qImg));
		ui.image_label->setScaledContents(true);
	}
}


///////////////////////////////////////////////////////////////////////////////
CannyParameterCalculator::CannyParameterCalculator(QWidget *parent)
    : GUIConfiguratorDialog(parent, "CannyParameterCalculator")
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
	QObject::connect(ui.showReal_button,SIGNAL(clicked(bool)), this,
				SLOT(onButtonShowRealPressed(void)));
	QObject::connect(ui.done_button,SIGNAL(clicked(bool)), this,
				SLOT(onButtonDonePressed(void)));

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
	cv::GaussianBlur(mTransformedImage, mTransformedImage, cv::Size(7,7), 1.5, 1.5);
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
void CannyParameterCalculator::onButtonShowRealPressed(void)
{
	showImage(mOriginalImage);
}

///////////////////////////////////////////////////////////////////////////////
void CannyParameterCalculator::onButtonDonePressed(void)
{
	close();
}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////






