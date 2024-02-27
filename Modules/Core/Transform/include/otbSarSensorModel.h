/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#include "otbLagrangianOrbitInterpolator.h"
#include "otbImageMetadata.h"
#include "otbSARMetadata.h"
#include "otbGeocentricTransform.h"

#include "itkPoint.h"
#include <string>
#include <tuple>

namespace otb
{

class SarSensorModel
{
public:

  /*-------------------------------[ Typedefs ]--------------------------------*/
  using Point2DType  = itk::Point<double, 2>;
  using Point3DType  = itk::Point<double, 3>;

  using Vector3DType = itk::Point<double, 3>;

  using TimeType     = MetaData::TimePoint;
  using DurationType = MetaData::Duration;

  using OrbitIterator = std::vector<Orbit>::const_iterator;

  /*-------------------------------[ Special functions ]-----------------------*/
  SarSensorModel(
      std::string          productType,
      SARParam             sarParam,
      Projection::GCPParam gcps,
      unsigned int polynomial_degree = 8);

  explicit SarSensorModel(const ImageMetadata & imd, unsigned int polynomial_degree = 8);
  virtual ~SarSensorModel() = default;

  SarSensorModel(const SarSensorModel&) = delete; // non construction-copyable
  SarSensorModel& operator=(const SarSensorModel&) = delete; // non copyable

  /*-------------------------------[ Result types ]----------------------------*/
  /** Aggregate type to collect Zero Doppler related information (excl'd range
   * time). */
  struct ZeroDopplerInfo
  {
    TimeType     azimuthTime;
    // double       rangeTime;
    Point3DType  sensorPos;
    Vector3DType sensorVel;
  };

  /** Aggregate type to collect line sample (col,row) and coordinates in YZ
   * frame. */
  struct LineSampleYZ
  {
    Point2DType col_row;
    Point2DType yz;
  };

  /*-------------------------------[ Doppler related functions ]---------------*/
  /** Transform world point (lat,lon,hgt) to input image point (col,row) */
  void WorldToLineSample(const Point3DType& inGeoPoint,
                         Point2DType& outLineSample) const;


  /** Transform world point (lat,lon,hgt) to input image point (col,row) and YZ
   * frame.
   *
   * Unlike `Doppler0ToLineSampleYZ`, this function takes care of searching
   * doppler 0 information related to the ground point `inGeoPoint`. However,
   * intermediary data will be hidden and lost. Any call to other `WorldTo...`
   * function will search for the same doppler0 information again and again...
   *
   * As such, prefer using `otb::Projection::WorldToEcef`, `ZeroDopplerLookup`,
   * and `Doppler0ToLineSampleYZ` manually if you need several of these
   * information.
   *
   * \param[in] inGeoPoint  ground point in WGS84 (lat, lon, hgt) coordinates
   * \param[out] cr         associated col+row
   * \param[out] yz         associated coordinates in YZ frame
   *
   * \throw itk::ExceptionObject if there aren't enough OrbitStateVector
   * samples to search in the orbit.
   */
  void WorldToLineSampleYZ(const Point3DType& inGeoPoint, Point2DType& cr, Point2DType& yz) const;

  /**
   * Compute input image point (col,row) and YZ frame from Doppler0 precomputed
   * information.
   *
   * Unlike `WorldToLineSampleYZ`, this function starts from data already
   * computed and cached by user code.
   *
   * The main use case is when doppler0 information is already requested by
   * user code as it could be directly injected to compute col/row/Y/Z instead
   * of searching again for the doppler0 data related to `ecefGround` point.
   * \param[in] ecefGround  ground point in XYZ cartesian coordinates
   * \param[in] zdi         zero doppler information (azimuth time, sensor
   *                        position + velocity)
   * \param[in] rangeTime   associated range time
   *
   * \pre the `zdi` data and `rangeTime| shall have been obtained with
   * `ZeroDopplerLookup` and `CalculateRangeTime` on the same `ecefGround`
   * point.
   *
   * \return input image point (col,row) and YZ frame.
   * \throw None
   * \see `WorldToLineSampleYZ`
   */
  LineSampleYZ Doppler0ToLineSampleYZ(
      Point3DType const& ecefGround, ZeroDopplerInfo const& zdi, double rangeTime) const;

  /**
   * Calculate range time from pre computed information.
   * For user code that may or may not need range time information
   * independently of other information related to the zero doppler.
   * \param[in] ecefGround  ground point in XYZ cartesian coordinates
   * \param[in] sensorPos   sensor position associated to `ecefGround`
   *
   * \return the related range time
   * \pre `sensorPos` shall match ground point.
   * \throw None
   */
  double CalculateRangeTime(
      Point3DType const& ecefGround, Point3DType const& sensorPos) const;

  /** Transform world point (lat,lon,hgt) to satellite position (x,y,z) and satellite velocity.
   *
   * \warning this function also computes the azimuthTime and the range time,
   * but it only return sensor position and velocity.
   * Prefer `WorldToAzimuthRangeTime` or the more fine grained `ZeroDopplerLookup` function.
   * \return true, always...
   * \throw itk::ExceptionObject if there aren't enough OrbitStateVector
   * samples to search in the orbit.
   */
  bool WorldToSatPositionAndVelocity(const Point3DType& inGeoPoint, Point3DType& satellitePosition, Point3DType& satelliteVelocity) const;

  /** Transform line index to satellite position (x,y,z) and satellite velocity */
  bool LineToSatPositionAndVelocity(double line, Point3DType& satellitePosition, Point3DType& satelliteVelocity) const;

  /**
   * Helper function that returns every zero doppler related information for
   * the input ground point in WGS84 coordinates (lat, lon, hgt).
   *
   * Actually, it returns everything but the ground point in ECEF coordinates.
   *
   * Alternately, user code can chose to use the more fine grained functions
   * - `otb::Projection::WorldToEcef()`,
   * - `ZeroDopplerLookup()`
   * - `CalculateRangeTime()`
   *
   * \param[in] inGeoPoint   ground point in WGS84 (lat, lon, hgt) coordinates
   * \param[out] azimuthTime azimuth time at zero doppler
   * \param[out] rangeTime   range time at zero doppler
   * \param[out] sensorPos   sensor position at zero doppler
   * \param[out] sensorVel   sensor velocity at zero doppler
   *
   * \return true, always...
   * \throw itk::ExceptionObject if there aren't enough OrbitStateVector
   * samples to search in the orbit.
   * \see `otb::Projection::WorldToEcef()`, `ZeroDopplerLookup()`,
   * `CalculateRangeTime()`.
   */
  bool WorldToAzimuthRangeTime(
      const Point3DType& inGeoPoint,
      TimeType & azimuthTime,
      double & rangeTime,
      Point3DType& sensorPos,
      Vector3DType& sensorVel) const;

  void LineSampleHeightToWorld(const Point2DType& imPt,
                               double heightAboveEllipsoid,
                               Point3DType& worldPt) const;

  void LineSampleToWorld(const Point2DType& imPt,
                         Point3DType& worldPt) const;

  /**
   * Computes Zero Doppler information.
   * Given a ground position, estimates the associated azimuth time of the zero
   * doppler.
   *
   * Internally, this function calls `ZeroDopplerTimeLookupInternal()` but only
   * return the azimuth time.
   *
   * \param[in] ecefGround  Ground position in ECEF coordinates
   * \return Associated azimuth time.
   * \throw itk::ExceptionObject if orbit record vector contains less than 2
   * elements.
   */
  TimeType ZeroDopplerTimeLookup(Point3DType const& ecefGround) const;

  /**
   * Internal function that looks for zero doppler.
   * Given a ground position, estimates the associated azimuth time of the zero
   * doppler.
   *
   * This internal function also provides iterators to OrbitStateVector
   * samples in between which the sensor has been the closest to the ground
   * point. These iterators will then permit to directly interpolate the sensor
   * position and velocity (without looking for the OSV sample which is the
   * closest from ground point, a second time).
   *
   * \internal the actual estimation is done by searching the two OSV for which
   * the scalar product between ground->sensor and sensor velocity changes
   * sign. Then the azimuth time is roughly estimated with a simple cross
   * multiplication.
   *
   * \return Associated azimuth time.
   * \return plus a pair of `OrbitIterator`s to the OrbitStateVector samples
   * that are around the azimuth found.
   * \throw itk::ExceptionObject if there aren't enough OrbitStateVector
   * samples to search in the orbit.
   */
  std::tuple<TimeType, OrbitIterator, OrbitIterator>
    ZeroDopplerTimeLookupInternal(Point3DType const& ecefGround) const;

  /**
   * Computes Zero Doppler information.
   * Given a ground position, looks for the associated azimuth time, and the
   * associated position and velocity of the sensor.
   *
   * \param[in] inEcefPoint  Ground position in ECEF coordinates
   *
   * \return {azimuthTime, sensor position, sensor velocity} at zero doppler
   * \throw itk::ExceptionObject if orbit record vector contains less than 2
   * elements.
   */
  ZeroDopplerInfo ZeroDopplerLookup(Point3DType const& inEcefPoint) const;


  /*-------------------------------[ Burst related functions ]-----------------*/
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
    * This method will estimate the overlap area between two bursts and return the
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


private:
  void OptimizeTimeOffsetsFromGcps();

  /**
   * Linear search of the OrbitStateVector which is the closest from ground
   * point.
   *
   * This function is time consuming and best avoided is possible.
   * \return an `OrbitIterator` on the closest OSV (from the ground), with a
   * supposition on the Lagrangian polynomial degree.
   * \throw None
   */
  OrbitIterator searchLagrangianNeighbourhood(TimeType azimuthTime) const;

   /**
    * Interpolate sensor position and velocity at given azimuth time
    * using lagragian interpolation of orbital records.
    *
   * This overload will search (in O(N)) for the closest OrbitStateVector
   * from ground point thanks to `searchLagrangianNeighbourhood`.
   * This overload is time consuming and best avoided is possible.
   *
    * \param[in] azimuthTime The time at which to interpolate
   * \return interpolated sensor {position, velocity}
    */
  std::pair<Point3DType, Vector3DType> interpolateSensorPosVel(TimeType azimuthTime) const;

  /**
   * Interpolate sensor position and velocity at given azimuth time
   * using lagragian interpolation of orbital records.
   *
   * This overload will exploit the last OSV sample known to be before the
   * `azimuthTime`.
   * Prefer this overload when possible.
   *
   * \param[in] azimuthTime The time at which to interpolate
   * \return interpolated sensor {position, velocity}
   */
  std::pair<Point3DType, Vector3DType> interpolateSensorPosVel(
      TimeType  azimuthTime,
      OrbitIterator itrecord1) const;

   /**
    * Convert azimuth time to fractional line.
    *
    * \param[in] azimuthTime The azimuth time to convert
    * \return The estimated fractional line
    */
  double AzimuthTimeToLine(const TimeType & azimuthTime) const;

  double SlantRangeToGroundRange(double slantRange,
                                 const TimeType & azimuthTime) const;


  double ApplyCoordinateConversion(double in,
                                 const TimeType& azimuthTime,
                                 const std::vector<CoordinateConversionRecord> & records) const;

  const GCP & findClosestGCP(const Point2DType& imPt, const Projection::GCPParam & gcpParam) const;

  Point3DType projToSurface(const GCP & gcp,
                            const Point2DType & imPt,
                            std::function<double(double, double)> heightFunction) const;

  TimeType LineToAzimuthTime(double line) const;

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

  // True if the input product is a ground product
  bool m_IsGrd;

  // Let's use a single precision for Lagrangian interpolation that is decided
  // once, when the SarSensorModel is instantiated.
  LagrangianOrbitInterpolator m_OrbitInterpolator;
  unsigned int                m_polynomial_degree;

  otb::GeocentricTransform<otb::TransformDirection::INVERSE, double>::Pointer m_EcefToWorldTransform;
  otb::GeocentricTransform<otb::TransformDirection::FORWARD, double>::Pointer m_WorldToEcefTransform;
};

}

#endif
