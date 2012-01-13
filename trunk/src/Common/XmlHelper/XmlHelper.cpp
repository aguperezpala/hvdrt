/*
 * XmlHelper.cpp
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

#include "XmlHelper.h"

#include <fstream>

#include "DebugUtil.h"


bool XmlHelper::readFileContent(const std::string &fName, std::string &contents)
{
        char *buffer;
        std::ifstream is;
        int fLength = 0;

        /* abrimos el archivo y lo leemos completo */
        is.open(fName.c_str(), std::ios::binary );
        if (!is.good()) {
                debug("Error openning file %s\n", fName.c_str());
                return false;
        }

        // obtenemos el tamaño y lemos todo
        is.seekg (0, std::ios::end);
        fLength = is.tellg();
        is.seekg (0, std::ios::beg);

        // pedimos memoria
        buffer = new char [fLength + 1];
        buffer[fLength] = '\0';
        // leemos
        is.read (buffer,fLength);

        if (is.fail()) {
                debug("error reading the file %s\n", fName.c_str());
                delete[] buffer;
                is.close();
                return false;
        }


        is.close();
        /*! FIXME: ineficiente! */
        contents = buffer;
        delete[] buffer;

        return true;
}


// Loads an xml document from a file. Returns 0 on error
TiXmlDocument *XmlHelper::loadFromFile(const std::string &fname)
{
	std::string content;
	if(!readFileContent(fname, content)){
		return 0;
	}

	TiXmlDocument *doc = new TiXmlDocument();
	doc->Parse(content.c_str());
	if(doc->Error()){
		debug("Error parsing the file %s\n", fname.c_str());
		delete doc;
		return 0;
	}

	// everything ok
	return doc;
}

/* Finds an element by attribute (it finds in the same "path length").
 * If no element is found 0 is returned
 */
TiXmlElement *XmlHelper::findByAttr(TiXmlElement *root, const char *attrName,
		const char *nameToFind)
{
	ASSERT(root);
	ASSERT(attrName);
	ASSERT(nameToFind);

	TiXmlElement *aux = root->FirstChildElement();
	while(aux){
		if(!aux->Attribute(attrName)){
			aux = aux->NextSiblingElement();
			continue;
		}
		std::string value = aux->Attribute(attrName);
		if(value == nameToFind){
			return aux;
		}

		aux = aux->NextSiblingElement();
	}

	return 0;
}








