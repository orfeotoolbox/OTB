/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef ossimTerraSarXSarSensorModel_HEADER
#define ossimTerraSarXSarSensorModel_HEADER

#include "ossim/ossimSarSensorModel.h"
#include <string>

namespace ossimplugins
{

class OSSIM_PLUGINS_DLL ossimTerraSarXSarSensorModel : public ossimSarSensorModel
{
public:

#if ! (defined(BOOST_NO_DEFAULTED_FUNCTIONS) || defined(BOOST_NO_CXX1_DEFAULTED_FUNCTIONS))
  /** Constructor */
  ossimTerraSarXSarSensorModel() = default;

  /** Copy constructor */
  ossimTerraSarXSarSensorModel(ossimTerraSarXSarSensorModel const& m) = default;
  /** Move constructor */
  ossimTerraSarXSarSensorModel(ossimTerraSarXSarSensorModel && m) = default;

  /** Destructor */
  virtual ~ossimTerraSarXSarSensorModel() = default;
#endif

  //Not implemented yet
  /** References
   * TerraSAR-X Image Product Guide
   * SNAP source code (s1tbx-io/src/main/java/org/esa/s1tbx/io/terrasarx/TerraSarXProductDirectory.java)
  */
  void readAnnotationFile(const std::string & annotationXml, const std::string & geoXml);

protected:
  /*
  std::string theProductType;
  std::string theMode;
  std::string theSwath;
  std::string thePolarisation;
  */
};

} // end namespace

#endif
