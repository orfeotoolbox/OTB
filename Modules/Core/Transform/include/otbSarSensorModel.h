/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSarSensorModel_h
#define otbSarSensorModel_h

#include "otbImageMetadata.h"
#include "otbSARMetadata.h"
#include "otbGeocentricTransform.h"

#include "itkPoint.h"

namespace otb
{

class SarSensorModel
{
public:

  SarSensorModel(const std::string & productType,
                 const SARParam & sarParam,
                 const Projection::GCPParam & gcps);

  SarSensorModel(const ImageMetadata & imd);
  virtual ~SarSensorModel() = default;

  SarSensorModel(const SarSensorModel&) = delete; // non construction-copyable
  SarSensorModel& operator=(const SarSensorModel&) = delete; // non copyable

  using Point2DType = itk::Point<double, 2>;
  using Point3DType = itk::Point<double, 3>;

  using Vector3DType = itk::Point<double, 3>;

  using TimeType = MetaData::TimePoint;
  using DurationType = MetaData::Duration;

  /** Transform world point (lat,lon,hgt) to input image point
  (col,row) */
  void WorldToLineSample(const Point3DType& inGeoPoint,
                         Point2DType& outLineSample) const;


  /** Transform world point (lat,lon,hgt) to input image point
  (col,row) and YZ frame */
  void WorldToLineSampleYZ(const Point3DType& inGeoPoint, Point2DType& cr, Point2DType& yz) const;

  /** Transform world point (lat,lon,hgt) to satellite position (x,y,z) and satellite velocity */
  bool WorldToSatPositionAndVelocity(const Point3DType& inGeoPoint, Point3DType& satellitePosition, Point3DType& satelliteVelocity) const;

  /** Transform line index to satellite position (x,y,z) and satellite velocity */
  bool LineToSatPositionAndVelocity(double line, Point3DType& satellitePosition, Point3DType& satelliteVelocity) const;

  bool WorldToAzimuthRangeTime(const Point3DType& inGeoPoint, 
                                          TimeType & azimuthTime, 
                                          double & rangeTime, 
                                          Point3DType& sensorPos, 
                                          Vector3DType& sensorVel) const;

  void LineSampleHeightToWorld(const Point2DType& imPt,
                               double heightAboveEllipsoid,
                               Point3DType& worldPt) const;


  void LineSampleToWorld(const Point2DType& imPt,
                         Point3DType& worldPt) const;

  /** Deburst metadata if possible and return lines to keep in image file */
  bool Deburst(std::vector<std::pair<unsigned long, unsigned long>>& lines,
               std::pair<unsigned long, unsigned long>& samples,
               bool onlyValidSample = false);


  /**
    * This method will perform an extraction of one burst. It will return the
    * lines and samples to extract in the image file.
    * \return true if the extraction operation succeeded. No changes is
    * made to the object if the operation fails.
    * \param burst_index Index of Burst.
    * \param lines A container for the lines to keep in the
    * standalone burst.
    * \param samples A container for the samples to keep in the
    * standalone burst.
    */
  bool BurstExtraction(const unsigned int burst_index, std::pair<unsigned long,unsigned long> & lines,
      std::pair<unsigned long,unsigned long> & samples, bool allPixels = false);

   /**
    * This method will perform a deburst and concatenation operation, and return the
    * vector of lines and the vector of samples to keep in the 
    * image file. The lines and samples represents start/size into each independent bursts. 
    * Note that the deburst operation has no effect if theBurstRecords
    * contains a single burst. Otherwise it will merge burst together
    * into a single burst, and update GCPs accordingly.
    * \return true if the deburst operation succeeded. No changes is
    * made to the object if the operation fails.
    * \param lines A container for the lines ranges to keep in the
    * deburst image.
    * \param samples A container for the samples ranges to keep in the
    * deburst image.
    * \param lines A Boolean to indicate only valids samples are required.
    */
   bool DeburstAndConcatenate(std::vector<std::pair<unsigned long,unsigned long> >& linesBursts, 
            std::vector<std::pair<unsigned long,unsigned long> >& samplesBursts,
            unsigned int & linesOffset, unsigned int first_burstInd,
            bool inputWithInvalidPixels=false);

   /**
    * This method will estime the overlap area between two bursts and return the
    * vector of lines and the vector of samples (with two elements : Burst Up and Burst Low). 
    * Note that this operation has no effect if theBurstRecords
    * contains a single burst. 
    * \return true if this operation succeeded. No changes is
    * made to the object if the operation fails.
    * \param linesUp A container for the lines ranges to keep into the first Burst
    * \param linesLow A container for the lines ranges to keep into the second Burst
    * \param samplesUp A container for the samples ranges to keep into the first Burst.
    * \param samplesDown A container for the samples ranges to keep into the second Burst.
    * \param burstIndUp Index of the first Burst
    * \param inputWithInvalidPixels A Boolean to indicate if invalids pixels are into inputs.
    */
  bool Overlap(std::pair<unsigned long, unsigned long>& linesUp, std::pair<unsigned long, unsigned long>& linesLow,
               std::pair<unsigned long, unsigned long>& samplesUp, std::pair<unsigned long, unsigned long>& samplesLow, unsigned int burstIndUp,
               bool inputWithInvalidPixels = false);

   /** Update a ImageMetadata object with the stored SarParam and GCPs, possibly modified from the 
    * original metadata by the SarSensorModel
    * \param imd The ImageMetadata to be updated
     */ 
   void UpdateImageMetadata(ImageMetadata & imd);

  /** 
    * This is a helper function to convert deburst line to input image
    * line
    * \param lines The vector of lines range to keep
    * \param imageLine The input deburst line
    * \param deburstLine The output original image line
  */
  static void DeburstLineToImageLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines, 
                                              unsigned long deburstLine, 
                                              unsigned long & imageLine);

  /**
    * This is a helper function to convert image line to deburst image
    * line.
    * \param lines The vector of lines range to keep
    * \param imageLine The input image line
    * \param deburstLine The output deburst line
    * \return True if imageLine is within a preserved range, false otherwise
    */
  static bool ImageLineToDeburstLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines,
                              unsigned long imageLine,
                              unsigned long & deburstLine);

protected:

private:
  void OptimizeTimeOffsetsFromGcps();


  bool ZeroDopplerLookup(const Point3DType& inEcefPoint, 
                                          TimeType & azimuthTime, 
                                          Point3DType& sensorPos, 
                                          Vector3DType& sensorVel) const;


   /**
    * Interpolate sensor position and velocity at given azimuth time
    * using lagragian interpolation of orbital records.
    *
    * \param[in] azimuthTime The time at which to interpolate
    * \param[out] sensorPos Interpolated sensor position
    * \param[out] sensorvel Interpolated sensor velocity
    * \param[in] deg Degree of lagragian interpolation
    */
   void interpolateSensorPosVel(const TimeType & azimuthTime, 
                                Point3DType& sensorPos, 
                                Vector3DType& sensorVel, 
                                unsigned int deg = 8) const;

   /**
    * Convert azimuth time to fractional line.
    *
    * \param[in] azimuthTime The azimuth time to convert
    * \param[out] The estimated fractional line
    */
  void AzimuthTimeToLine(const TimeType & azimuthTime, 
                          double & line) const;

  void SlantRangeToGroundRange(double slantRange, 
                               const TimeType & azimuthTime, 
                               double & groundRange) const;


  void ApplyCoordinateConversion(double in,
                                 const TimeType& azimuthTime,
                                 const std::vector<CoordinateConversionRecord> & records,
                                 double & out) const;


  const GCP & findClosestGCP(const Point2DType& imPt, const Projection::GCPParam & gcpParam) const;

  Point3DType projToSurface(const GCP & gcp,
                            const Point2DType & imPt,
                            std::function<double(double, double)> heightFunction) const;

  void LineToAzimuthTime(double line, TimeType & azimuthTime) const;

  /** Coordinate transformation from ECEF to geographic */
  itk::Point<double, 3> EcefToWorld(const itk::Point<double, 3> & ecefPoint) const;

  /** Coordinate transformation from geographic to ECEF */
  itk::Point<double, 3> WorldToEcef(const itk::Point<double, 3> & worldPoint) const;


  std::string m_ProductType;
  Projection::GCPParam m_GCP;
  SARParam m_SarParam;

  TimeType m_FirstLineTime;
  TimeType m_LastLineTime;

  DurationType m_AzimuthTimeOffset;
  double m_RangeTimeOffset; // Offset in seconds

  // Speed of light 
  static constexpr double C = 299792458;

  // True if the input product is a ground product
  bool m_IsGrd;

  otb::GeocentricTransform<otb::TransformDirection::INVERSE, double>::Pointer m_EcefToWorldTransform;
  otb::GeocentricTransform<otb::TransformDirection::FORWARD, double>::Pointer m_WorldToEcefTransform;
};

}

#endif