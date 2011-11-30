#ifndef GUIPERSPECTIVETRANSFORMATOR_H
#define GUIPERSPECTIVETRANSFORMATOR_H

#include <QtGui/QWidget>
#include <qimage.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qscrollarea.h>
#include <qdialog.h>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

#include "ui_guiperspectivetransformator.h"
#include "MouseQLabel.h"
#include "PerspectiveTransformator.h"
#include "GUIConfiguratorDialog.h"
#include "GUIUtils.h"


class GUIPerspectiveTransformator : public GUIConfiguratorDialog
{
    Q_OBJECT

public:
    GUIPerspectiveTransformator(QWidget *parent = 0);
    ~GUIPerspectiveTransformator();

    /* Sets the image where the user have to select the points */
    void setImage(const cv::Mat &img);

    /* Set the vector where it will be pushed the points selected in the image */
    void setPointsVector(std::vector<cv::Point2i> *vec);

    /* Function used to extract the ImageProcessor generated.
     * Returns 0 on error
     * NOTE: This allocates memory that must be removed after
     */
    PerspectiveTransformator *getPerspectiveTransformator(void);

    /* Returns the size of the rectangle in mm */
    float getRectangleSize(void){return mRectSizeY;}

    /* Function used to load the configurations from a xml file
	 * Returns:
	 * 	errCode
	 */
	virtual errCode loadConfig(TiXmlElement *);

	/* Save the data to an xml and return it.
	 * Returns:
	 * 	0			on Error
	 * 	xml			on success */
	virtual std::auto_ptr<TiXmlElement> getConfig(void);

public slots:
    void onClearClicked(void);
    void onDoneClicked(void);
    void onPreviewClicked(void);

private:
    int showMessage(const QString &m);

private:
    Ui::GUIPerspectiveTransformatorClass 		ui;
    // The image to analyze
    cv::Mat						mImage;
    // Vector of points
    std::vector<cv::Point2i> 	*mPointsVector;
    MouseQLabel					*mMouseLabel;
    QScrollArea					mScrollArea;

    PerspectiveTransformator 	*mTransformator;

    float						mRectSizeY;


};

#endif // GUIPERSPECTIVETRANSFORMATOR_H
