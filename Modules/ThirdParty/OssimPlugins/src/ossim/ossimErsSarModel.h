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


#ifndef ossimErsSarModel_H
#define ossimErsSarModel_H 1

#include <otb/JSDDateTime.h>
#include <ossimGeometricSarSensorModel.h>

#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>


namespace ossimplugins
{

class PlatformPosition;
class SensorParams;
class RefPoint;
class ErsSarLeader;
/**
 * @brief This class is able to direct localisation and indirect localisation using the ErsSar sensor model
 *
 */
class OSSIM_PLUGINS_DLL ossimErsSarModel : public ossimGeometricSarSensorModel
{
public:
  /**
   * @brief Constructor
   */
  ossimErsSarModel();

  /**
   * @brief Destructor
   */
  virtual ~ossimErsSarModel();

  /**
   * @brief Method to return the class name.
   * @return The name of this class.
   */
  virtual ossimString getClassName() const;

  /**
   * @brief Returns pointer to a new instance, copy of this.
   */
  virtual ossimObject* dup() const;

  /**
   * @brief This function associates an image column number to a slant range when the image is georeferenced (ground projected)
   * @param col Column coordinate of the image point
   */
  virtual double getSlantRangeFromGeoreferenced(double col) const;

  /**
  * @brief Method to instantiate model from the leader file.
  * @param file
  * @return true on success, false on error.
  */
  bool open(const ossimFilename& file);

  /**
   * @brief Method to save object state to a keyword list.
   * @param kwl Keyword list to save to.
   * @param prefix added to keys when saved.
   * @return true on success, false on error.
   */
  virtual bool saveState(ossimKeywordlist& kwl,
                         const char* prefix = 0) const;

  /**
   * @brief Method to the load (recreate) the state of the object from a
   * keyword list. Return true if ok or false on error.
   * @return true if load OK, false on error
   */
  virtual bool loadState(const ossimKeywordlist &kwl, const char *prefix = 0);

protected:
  virtual bool InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix);
  virtual bool InitSensorParams(const ossimKeywordlist &kwl, const char *prefix);
  virtual bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix);
  /**
  * @brief Initializes the Slant Range for each Ground Range data sets : theNumberSRGR,theSRGRCoeffset,_srgr_update,thePixelSpacing,_isProductGeoreferenced
  */
  virtual bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix);

private:
  /**
   *  @brief Slant Range for each Ground Range (SRGR) number of coefficients sets
   */
  int   theNumberSRGR;
  /**
   * @brief SRGR coefficient sets
   */
  double theSRGRCoeffset[1][3];
  /**
   * @brief Pixel spacing
   */
  double thePixelSpacing;

  /**
   * @brief List of metadata contained in the Leader file
   */
  ErsSarLeader *theErsSarleader;



  virtual bool isErsLeader(const ossimFilename& file) const;
  virtual ossimFilename findErsLeader(const ossimFilename& file) const;

  TYPE_DATA

};

}
#endif
