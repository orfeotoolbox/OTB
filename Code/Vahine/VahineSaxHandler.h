/*
 * VahineSaxHandler.h
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

#ifndef __VAHINESAXHANDLER_H
#define __VAHINESAXHANDLER_H

#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/sax/InputSource.hpp>

#include <stack>
#include <vector>
#include <string>

XERCES_CPP_NAMESPACE_USE

namespace vahine {

class XmlError {
public:
  XmlError(){
  };
  XmlError(std::string m){
    msg =m;
  };
private:
  std::string msg;
}; //  class exception

class XmlTagWithChar {
private:
  std::string value;
public:
  XmlTagWithChar() {
  };
  void addChars(const char* buf, unsigned int len) {
    value += std::string(buf, len);
  }

  std::string getValue() {
    return value;
  }
};

class InverseParam : public XmlTagWithChar {
private:
  bool activ;
public:
  InverseParam(bool a){
    activ = a;
  }
  bool isActiv() {
    return activ;
  }
  std::string getDescription(){
    return getValue();
  }
};

class Band : public XmlTagWithChar {
private:
  bool activ;
public:
  Band(bool a) {
    activ = a;
  }
  bool isActiv() {
    return activ;
  }
  // convert string to double
  const double getLength() {
    return strtod(getValue().c_str(), NULL);
  }
  unsigned int number;
};

class WaveMask {
public:
  typedef std::vector<Band> BandList;
  enum ApplyValue {
    UNKNOWN,
    IMAGE,
    LEARNDATA,
    };

  WaveMask() : isMaskDefaultOn(true) {
  }
  ~WaveMask() {
  };
  BandList     bandList;
  bool         isMaskDefaultOn;
  unsigned int numberOfBand;
  ApplyValue   applyOn;

};

class SpatialMask : public XmlTagWithChar {
public:
  unsigned int band;
  std::string getDescription(){
    return getValue();
  }
};

class GrsirParam {
public:
  typedef std::vector<InverseParam> InverseParamList;
  typedef WaveMask::BandList        BandList;
  typedef std::vector<double>       RegulationList;

  GrsirParam() : regulation_auto(true){
  }
  ~GrsirParam(){
  }

  // file name list
  std::string image;
  std::string learning;
  std::string learning_noise;
  std::string learning_param;
  std::string image_param;
  std::string spatial_mask;

  InverseParamList inverse;
  RegulationList   regulation;
  bool             regulation_auto;
  SpatialMask      spatialMask;
  WaveMask         waveMask;
};

class VahineSAXHandler : public HandlerBase {
private:
  bool        charsValid;
  GrsirParam& grsir;
  std::string fileName;
  std::string tmp;
  // List of tags which can be found in the config XML file.
  enum ConfigTag {
    UNKNOWN,
    GRSIR,
    MASKWAVE,
    BAND,
    BANDS,
    FILE,
    INVERSE,
    PARAMETER,
    REGULATION,
    MASKSPATIAL
    };
  // The tag of the current read node.
  std::stack<ConfigTag> currentNodes;
  bool checkConfig();

public:
  VahineSAXHandler(GrsirParam& g) : grsir(g), charsValid(false) {
  }
  void startElement(const XMLCh* const, AttributeList&);

  void endElement(const XMLCh* const name);

  void characters(const XMLCh* const buf, unsigned int len);

  void fatalError(const SAXParseException&);

};

} //end namespace vahine
#endif /* VAHINESAXHANDLER_H_ */