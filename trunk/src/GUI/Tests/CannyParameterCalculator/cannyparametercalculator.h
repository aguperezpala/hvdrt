#ifndef CANNYPARAMETERCALCULATOR_H
#define CANNYPARAMETERCALCULATOR_H

#include <QtGui/QWidget>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>

#include "ui_cannyparametercalculator.h"

class CannyParameterCalculator : public QWidget
{
    Q_OBJECT

public:
    CannyParameterCalculator(QWidget *parent = 0);
    ~CannyParameterCalculator();

public slots:
	void onSlide1Change(int v);
	void onSlide2Change(int v);
	void onButtonCannyPressed(void);
	void onButtonLoadPressed(void);

private:
	/* Load the image into the label */
	bool IplImage2QImage(const cv::Mat &iplImg, QImage &ref);
	void showImage(const cv::Mat &img);

	/* Error message */
	int showMessage(const QString &msg);

private:
    Ui::CannyParameterCalculatorClass ui;

    // the source image
    cv::Mat							mOriginalImage;
    cv::Mat							mTransformedImage;


};

#endif // CANNYPARAMETERCALCULATOR_H
