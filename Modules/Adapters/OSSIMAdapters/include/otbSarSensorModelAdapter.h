/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbSarSensorModelAdapter_h
#define otbSarSensorModelAdapter_h

#include <memory>

#include "otbDEMHandler.h"
#include "itkPoint.h"

namespace ossimplugins
{
class ossimSarSensorModel;
}

namespace otb
{

class ImageKeywordlist;

/**
 * \class SarSensorModelAdapter
 * \brief Wrapper class to access SarSensorModel specific methods
 *
 * This class is NOT intended to be used outside of OTB. Use the
 * InverseSensorModel and ForwardSensorModel. If you feel that you need to use
 * it directly, think again!
 *
 * \sa InverseSensorModel
 * \sa ForwardSensorModel
 * \ingroup Projection
 *
 *
 * \ingroup OTBOSSIMAdapters
 **/

class OTBOSSIMAdapters_EXPORT SarSensorModelAdapter: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef SarSensorModelAdapter         Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef std::auto_ptr<ossimplugins::ossimSarSensorModel> InternalModelPointer;

  using Point2DType = itk::Point<double,2>;
  using Point3DType = itk::Point<double,3>;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarSensorModelAdapter, itk::Object);

  /** Load model from keyword list */
  bool LoadState(const ImageKeywordlist& image_kwl);

  /** Save keyword list from model */
  bool SaveState(ImageKeywordlist & image_kwl);
  
  /** Is sensor model valid method. return false if the m_SensorModel is null*/
  bool IsValidSensorModel() const;

  /** Deburst metadata if possible and return lines to keep in image file */
  bool Deburst(std::vector<std::pair<unsigned long, unsigned long> > & lines,
	       std::pair<unsigned long,unsigned long> & samples, bool onlyValidSample=false);

  /** Burst extraction and return lines/samples to keep into image file (the required burst) */
  bool BurstExtraction(const unsigned int burst_index, std::pair<unsigned long,unsigned long> & lines, 
		       std::pair<unsigned long,unsigned long> & samples, bool allPixels=false);

  
  /** Deburst metadata if possible and prepare the burst concatenation */	
  bool DeburstAndConcatenate(std::vector<std::pair<unsigned long, unsigned long> > & linesBursts,
			     std::vector<std::pair<unsigned long,unsigned long> > & samplesBursts,
			     unsigned int & linesOffset, unsigned int first_burstInd,
			     bool inputWithInvalidPixels=false);


  /** Transform world point (lat,lon,hgt) to input image point
  (col,row) and YZ frame */
  bool WorldToLineSampleYZ(const Point3DType & inGeoPoint, Point2DType & cr, Point2DType & yz) const;

  /** Transform world point (lat,lon,hgt) to input image point
  (col,row) */
  bool WorldToLineSample(const Point3DType & inGEoPOint, Point2DType & cr) const;

/** Transform world point (lat,lon,hgt) to satellite position (x,y,z) and satellite velocity */
  bool WorldToSatPositionAndVelocity(const Point3DType & inGeoPoint, Point3DType & satellitePosition,  
				     Point3DType & satelliteVelocity) const;
  
  /** Transform line index to satellite position (x,y,z) and satellite velocity */
  bool LineToSatPositionAndVelocity(const double line, Point3DType &  satellitePosition,  
				    Point3DType & satelliteVelocity) const;

  /** Transform world point (lat,lon,hgt) to cartesian point (x,y,z) */
  static bool WorldToCartesian(const Point3DType & inGeoPoint, Point3DType & outCartesianPoint);

  static bool ImageLineToDeburstLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines, unsigned long imageLine, unsigned long & deburstLine);

  static void DeburstLineToImageLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines, unsigned long deburstLine, unsigned long & imageLine);

  
  
protected:
  SarSensorModelAdapter();
  virtual ~SarSensorModelAdapter() override;

private:
  SarSensorModelAdapter(const Self &) = delete;
  void operator =(const Self&) = delete;

  InternalModelPointer m_SensorModel;
};

} // namespace otb

#endif
