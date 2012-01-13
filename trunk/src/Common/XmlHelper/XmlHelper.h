/*
 * XmlHelper.h
 *
 *  Created on: 04/01/2012
 *      Author: agustin
 *
 *
 * OpenVC License
 *
 * Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
 * Copyright (C) 2009, Willow Garage Inc., all rights reserved.
 * Third party copyrights are property of their respective owners.
 *
 * This software is provided by the copyright holders and contributors "as is" and
 * any express or implied warranties, including, but not limited to, the implied
 * warranties of merchantability and fitness for a particular purpose are disclaimed.
 * In no event shall the Intel Corporation or contributors be liable for any direct,
 * indirect, incidental, special, exemplary, or consequential damages
 * (including, but not limited to, procurement of substitute goods or services;
 * loss of use, data, or profits; or business interruption) however caused
 * and on any theory of liability, whether in contract, strict liability,
 * or tort (including negligence or otherwise) arising in any way out of
 * the use of this software, even if advised of the possibility of such damage.
 */

#ifndef XMLHELPER_H_
#define XMLHELPER_H_

#include <string>

#include "tinyxml.h"


class TiXmlElement;
class TiXmlDocument;

class XmlHelper {
public:
	// Loads an xml document from a file. Returns 0 on error
	static TiXmlDocument *loadFromFile(const std::string &fname);

	/* Finds an element by attribute (it finds in the same "path length").
	 * If no element is found 0 is returned
	 */
	static TiXmlElement *findByAttr(TiXmlElement *root, const char *attrName,
			const char *nameToFind);


private:
	// reads a file content
	static bool readFileContent(const std::string &fName, std::string &contents);


};

#endif /* XMLHELPER_H_ */
