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

#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimKeywordNames.h>

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

  /*!
   * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
   * KWL files. Returns true if successful.
   */
  virtual bool saveState(ossimKeywordlist& kwl,
			 const char* prefix=NULL) const override;

  virtual bool loadState(ossimKeywordlist const& kwl,
			 const char* prefix=NULL) override;

  //Not implemented yet
  /** References
   * TerraSAR-X Image Product Guide
   * SNAP source code (s1tbx-io/src/main/java/org/esa/s1tbx/io/terrasarx/TerraSarXProductDirectory.java)

   This class works for TSX-1 and PAZ-1 mission
  */
  bool readAnnotationFile(const std::string & annotationXml, const std::string & geoXml);

  bool open(const ossimFilename& file);

private:
  bool readSceneCornerCoord(const ossimRefPtr<ossimXmlDocument> xmlDoc,
			    unsigned int numberOfRows,
			    unsigned int numberOfColumns);
  bool readOrbitVector(const ossimRefPtr<ossimXmlDocument> xmlDoc);
  bool readDopplerRate(const ossimRefPtr<ossimXmlDocument> xmlDoc);
  bool readDopplerCentroid(const ossimRefPtr<ossimXmlDocument> xmlDoc, const std::string polarisation="HH");
  bool readGCPs(const std::string & geoXml);

  ossimKeywordlist   theProductKwl;
  std::string m_imageName;

};

} // end namespace

#endif
