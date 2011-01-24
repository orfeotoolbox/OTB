/*
 * VahineXml.h
 *
* Vahine Framework
 * Copyright (C) 2008, 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */

#ifndef __VAHINEXML_H
#define __VAHINEXML_H

#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include <string>
#include <list>

XERCES_CPP_NAMESPACE_USE

namespace vahine {

class XmlLog {
public:
  XmlLog(std::string moduleName);
  ~XmlLog();

  typedef DOMElement*        DOMElementPointer;
  typedef DOMDocument*       DOMDocumentPointer;
  typedef DOMImplementation* DOMImplementationPointer;

  void CreateElement(std::string name);

  void AppendAllToRoot();

  void AppendToSup();

  void CreateTextNode(std::string text);

  void CreateElementTextApp(std::string name, std::string text);

  void CreateAttribute(std::string name, std::string value);

  void SetFileName(std::string fileName);

  void Write();

private:

  std::string                  m_XmlFileName;
  DOMImplementationPointer     m_Implementation;
  DOMDocumentPointer           m_Document;
  DOMElementPointer            m_RootElement;
  std::list<DOMElementPointer> m_list;
};

} // end namespace vahine

#endif