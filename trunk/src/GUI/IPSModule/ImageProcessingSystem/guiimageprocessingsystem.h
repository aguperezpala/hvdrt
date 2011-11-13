#ifndef GUIIMAGEPROCESSINGSYSTEM_H
#define GUIIMAGEPROCESSINGSYSTEM_H

#include <QtGui/QWidget>
#include <qdialog.h>

#include <auto_ptr.h>

#include "DebugUtil.h"
#include "GlobalDefines.h"
#include "ImageProcessingSystem.h"


class GUIImageProcessingSystem : public QDialog
{
    Q_OBJECT

public:
    GUIImageProcessingSystem(QWidget *parent = 0){}
    virtual ~GUIImageProcessingSystem(){};

    /* Returns the name of the IPS */
    virtual const std::string &getName(void) const
    {
    	ASSERT(mIPS);
    	return mIPS->getName();
    }

    /* Returns the info of the IPS */
    virtual const std::string &getInfo(void) const
    {
    	ASSERT(mIPS);
    	return mIPS->getInfo();
    }

    /* Initialize the Image Processing System.
	 * Returns errorCode
	 */
	virtual errCode initialize(void) = 0;

	/* Executes the IPS and blocks until it finish.
	 * Returns:
	 * 	errCode
	 */
	virtual errCode execute(void) = 0;


protected:
    ImageProcessingSystem				*mIPS;


};

#endif // GUIIMAGEPROCESSINGSYSTEM_H
