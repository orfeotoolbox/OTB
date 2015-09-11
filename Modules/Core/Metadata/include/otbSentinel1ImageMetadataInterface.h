/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSentinel1ImageMetadataInterface_h
#define __otbSentinel1ImageMetadataInterface_h

#include "otbSarImageMetadataInterface.h"


namespace otb
{

struct Sentinel1CalibrationStruct {

public:
  double timeMJD;
  int line;
  std::vector<int> pixels;
  std::vector<float> vect;
};

class Sentinel1CalibrationLookupData : public SarCalibrationLookupData
{

public:
  double firstLineTime;
  double lastLineTime;
  int numOfLines;
  int count;
  std::vector<Sentinel1CalibrationStruct> calibrationVectorList;
  double lineTimeInterval;


  Sentinel1CalibrationLookupData(double ft, double lt,
                                 int lines, int c,
                                 std::vector<Sentinel1CalibrationStruct> vlist)
      : SarCalibrationLookupData()
  {
    firstLineTime = ft;
    lastLineTime = lt;
    numOfLines = lines;
    count = c;
    calibrationVectorList = vlist;
    lineTimeInterval = (lt - ft) / ((lines - 1) * 1.0f);
  }

  Sentinel1CalibrationLookupData(short type, double ft, double lt,
                                 int lines, int c,
                                 std::vector<Sentinel1CalibrationStruct> vlist)
    : SarCalibrationLookupData(type)
  {
    firstLineTime = ft;
    lastLineTime = lt;
    numOfLines = lines;
    count = c;
    calibrationVectorList = vlist;
    lineTimeInterval = (lt - ft) / ((lines - 1) * 1.0f);
  }

  virtual ~Sentinel1CalibrationLookupData()
  {

  }

  virtual double GetValue(int x, int y)
  {
    const int calVecIdx = GetVectorIndex(y);
    const Sentinel1CalibrationStruct vec0 = calibrationVectorList[calVecIdx];
    const Sentinel1CalibrationStruct vec1 = calibrationVectorList[calVecIdx + 1];
    const double azTime = firstLineTime + y * lineTimeInterval;
    const double muY = (azTime - vec0.timeMJD) /(vec1.timeMJD - vec0.timeMJD);
    const int pixelIdx = GetPixelIndex(x, calibrationVectorList[calVecIdx]);
    const double muX = ((x * 1.0f) - (vec0.pixels[pixelIdx]* 1.0f)) / ((vec0.pixels[pixelIdx + 1] - vec0.pixels[pixelIdx]) * 1.0f);
    const double lutVal = (1.0f - muY) * ((1.0f - muX) * (1.0f * vec0.vect[pixelIdx]) + muX * (1.0f * vec0.vect[pixelIdx + 1])) +
      muY * ((1.0f - muX) * (vec1.vect[pixelIdx] * 1.0f) + muX *   (1.0f * vec1.vect[pixelIdx + 1]));
    return lutVal;
  }

  int GetVectorIndex(int y)
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

  int GetPixelIndex(int x, const Sentinel1CalibrationStruct& calVec)
  {
    const int size = calVec.pixels.size();
    for (int i = 0; i < size; i++)
      {
      if (x < calVec.pixels[i])
        {
        return i - 1;
        }
      }
    return size - 2;
  }
};


/** \class Sentinel1ImageMetadataInterface
 *
 * \brief Creation of an "otb" Sentinel1ImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */

class ITK_ABI_EXPORT Sentinel1ImageMetadataInterface : public SarImageMetadataInterface
{
public:

  typedef Sentinel1ImageMetadataInterface    Self;
  typedef SarImageMetadataInterface         Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


  /** Run-time type information (and related methods). */
  itkTypeMacro(Sentinel1ImageMetadataInterface, SarImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;
  typedef Superclass::RealType                  RealType;

  /** Get the imaging production day from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable */
  int GetProductionDay() const;

  /** Get the imaging production month from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable */
  int GetProductionMonth() const;

  /** Get the imaging production year from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable */
  int GetProductionYear() const;

  /** check sensor ID */
  bool CanRead() const;

  int GetDay() const;

  int GetMonth() const;

  int GetYear() const;

  int GetHour() const;

  int GetMinute() const;

  UIntVectorType GetDefaultDisplay() const;

  /*SarImageMetadataInterface pure virutals rituals */
  double GetPRF() const;

  double GetRSF() const;

  double GetRadarFrequency() const;

  double GetCenterIncidenceAngle() const;

  /*get lookup data for calulating backscatter */
  SarCalibrationLookupData* GetCalibrationLookupData(const short type);

  bool HasCalibrationLookupData() const
  {
    return true;
  }


protected:

  /* class ctor */
  Sentinel1ImageMetadataInterface();

  /* class dtor */
  virtual ~Sentinel1ImageMetadataInterface() {}

private:

  Sentinel1ImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

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

} // end namespace otb

#endif
