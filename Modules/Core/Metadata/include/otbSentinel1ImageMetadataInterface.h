/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSentinel1ImageMetadataInterface_h
#define otbSentinel1ImageMetadataInterface_h

#include "otbSarImageMetadataInterface.h"


namespace otb
{
/** \class Sentinel1ImageMetadataInterface
 *
 * \brief Creation of an "otb" Sentinel1ImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */

class OTBMetadata_EXPORT Sentinel1ImageMetadataInterface : public SarImageMetadataInterface
{
public:
  typedef Sentinel1ImageMetadataInterface Self;
  typedef SarImageMetadataInterface       Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


  /** Run-time type information (and related methods). */
  itkTypeMacro(Sentinel1ImageMetadataInterface, SarImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;
  typedef Superclass::RealType                 RealType;
  typedef Superclass::LookupDataPointerType    LookupDataPointerType;

  /** Get the imaging production day from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable
   * \deprecated
   */
  int GetProductionDay() const override;

  /** Get the imaging production month from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable
   * \deprecated
   */
  int GetProductionMonth() const override;

  /** Get the imaging production year from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable
   * \deprecated
   */
  int GetProductionYear() const override;

  /** check sensor ID */
  bool CanRead() const override;

  int GetDay() const override;

  int GetMonth() const override;

  int GetYear() const override;

  int GetHour() const override;

  int GetMinute() const override;

  UIntVectorType GetDefaultDisplay() const override;

  /*SarImageMetadataInterface pure virutals rituals */
  double GetPRF() const override;

  double GetRSF() const override;

  double GetRadarFrequency() const override;

  double GetCenterIncidenceAngle() const override;

  /*get lookup data for calculating backscatter */
  void CreateCalibrationLookupData(const short type) override;

protected:
  /* class ctor */
  Sentinel1ImageMetadataInterface();

  /* class dtor */
  ~Sentinel1ImageMetadataInterface() override
  {
  }

private:
  Sentinel1ImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;

  /* Helper function to parse date and time into a std::vector<std::string>
   * using boost::split() expect date time in yyyy-mm-ddThh:mm:ss.ms
   * the date-time string is to be found in keywordlist with key 'key'
   * fills argument dateFields of type std::vector<std::string> which is mutable!
   * TODO: move this method into base class
   */
  void ParseDateTime(const char* key, std::vector<int>& dateFields) const;

  mutable std::vector<int> m_ProductionDateFields;
  mutable std::vector<int> m_AcquisitionDateFields;
};


struct Sentinel1CalibrationStruct
{
public:
  double              timeMJD;
  double              deltaMJD; // time difference to previous MJD in the list
  int                 line;
  std::vector<int>    pixels;
  std::vector<double> deltaPixels;
  std::vector<float>  vect;
};

class Sentinel1CalibrationLookupData : public SarCalibrationLookupData
{
public:
  /** Standard typedefs */
  typedef Sentinel1CalibrationLookupData Self;
  typedef SarCalibrationLookupData       Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(Sentinel1CalibrationLookupData, SarCalibrationLookupData);

  typedef itk::IndexValueType IndexValueType;

  Sentinel1CalibrationLookupData() : firstLineTime(0.), lastLineTime(0.), numOfLines(0), count(0), lineTimeInterval(0.)
  {
  }

  ~Sentinel1CalibrationLookupData() override
  {
  }

  void InitParameters(short type, double ft, double lt, int lines, int c, std::vector<Sentinel1CalibrationStruct> const& vlist)
  {
    firstLineTime         = ft;
    lastLineTime          = lt;
    numOfLines            = lines;
    count                 = c;
    calibrationVectorList = vlist;
    this->SetType(type);
    lineTimeInterval = (lt - ft) / ((lines - 1) * 1.0);
  }

  double GetValue(const IndexValueType x, const IndexValueType y) const override
  {
    const int calVecIdx = GetVectorIndex(y);
    assert(calVecIdx >= 0 && calVecIdx < count - 1);
    const Sentinel1CalibrationStruct& vec0     = calibrationVectorList[calVecIdx];
    const Sentinel1CalibrationStruct& vec1     = calibrationVectorList[calVecIdx + 1];
    const double                      azTime   = firstLineTime + y * lineTimeInterval;
    const double                      muY      = (azTime - vec0.timeMJD) / vec1.deltaMJD;
    const int                         pixelIdx = GetPixelIndex(x, calibrationVectorList[calVecIdx]);
    const double                      muX      = (x - vec0.pixels[pixelIdx]) / vec0.deltaPixels[pixelIdx + 1];
    const double                      lutVal =
        (1 - muY) * ((1 - muX) * vec0.vect[pixelIdx] + muX * vec0.vect[pixelIdx + 1]) + muY * ((1 - muX) * vec1.vect[pixelIdx] + muX * vec1.vect[pixelIdx + 1]);
    return lutVal;
  }

  int GetVectorIndex(int y) const
  {
    for (int i = 1; i < count; i++)
    {
      if (y < calibrationVectorList[i].line)
      {
        return i - 1;
      }
    }
    return -1;
  }

  int GetPixelIndex(int x, const Sentinel1CalibrationStruct& calVec) const
  {
    const int                        size = calVec.pixels.size();
    std::vector<int>::const_iterator wh   = std::upper_bound(calVec.pixels.begin(), calVec.pixels.end(), x);
    return wh == calVec.pixels.end() ? size - 2 : std::distance(calVec.pixels.begin(), wh) - 1;
  }

private:
  Sentinel1CalibrationLookupData(const Self&) = delete;

  void operator=(const Self&) = delete;

  double                                  firstLineTime;
  double                                  lastLineTime;
  int                                     numOfLines;
  int                                     count;
  std::vector<Sentinel1CalibrationStruct> calibrationVectorList;
  double                                  lineTimeInterval;
};


} // end namespace otb

#endif
