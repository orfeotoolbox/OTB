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
  typedef Superclass::LookupDataPointerType LookupDataPointerType;

  /** Get the imaging production day from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable */
  int GetProductionDay() const ITK_OVERRIDE;

  /** Get the imaging production month from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable */
  int GetProductionMonth() const ITK_OVERRIDE;

  /** Get the imaging production year from the ossim metadata : DATASET_PRODUCTION_DATE metadata variable */
  int GetProductionYear() const ITK_OVERRIDE;

  /** check sensor ID */
  bool CanRead() const ITK_OVERRIDE;

  int GetDay() const ITK_OVERRIDE;

  int GetMonth() const ITK_OVERRIDE;

  int GetYear() const ITK_OVERRIDE;

  int GetHour() const ITK_OVERRIDE;

  int GetMinute() const ITK_OVERRIDE;

  UIntVectorType GetDefaultDisplay() const ITK_OVERRIDE;

  /*SarImageMetadataInterface pure virutals rituals */
  double GetPRF() const ITK_OVERRIDE;

  double GetRSF() const ITK_OVERRIDE;

  double GetRadarFrequency() const ITK_OVERRIDE;

  double GetCenterIncidenceAngle() const ITK_OVERRIDE;

  /*get lookup data for calulating backscatter */
  void CreateCalibrationLookupData(const short type) ITK_OVERRIDE;

protected:

  /* class ctor */
  Sentinel1ImageMetadataInterface();

  /* class dtor */
  ~Sentinel1ImageMetadataInterface() ITK_OVERRIDE {}

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


  /** Standard typedefs */
  typedef Sentinel1CalibrationLookupData   Self;
  typedef SarCalibrationLookupData         Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(Sentinel1CalibrationLookupData, SarCalibrationLookupData);

  typedef itk::IndexValueType IndexValueType;

  Sentinel1CalibrationLookupData()
    : firstLineTime(0.)
    , lastLineTime(0.)
    , numOfLines(0)
    , count(0)
    , lineTimeInterval(0.)
  {

  }

  ~Sentinel1CalibrationLookupData() ITK_OVERRIDE
  {

  }

  void InitParameters(short type, double ft, double lt,
                      int lines, int c,
                      std::vector<Sentinel1CalibrationStruct> vlist)
  {
    firstLineTime = ft;
    lastLineTime = lt;
    numOfLines = lines;
    count = c;
    calibrationVectorList = vlist;
    this->SetType(type);
    lineTimeInterval = (lt - ft) / ((lines - 1) * 1.0);
  }

  double GetValue(const IndexValueType x, const IndexValueType y) ITK_OVERRIDE
  {
    const int calVecIdx = GetVectorIndex(y);
    const Sentinel1CalibrationStruct vec0 = calibrationVectorList[calVecIdx];
    const Sentinel1CalibrationStruct vec1 = calibrationVectorList[calVecIdx + 1];
    const double azTime = firstLineTime + y * lineTimeInterval;
    const double muY = (azTime - vec0.timeMJD) /(vec1.timeMJD - vec0.timeMJD);
    const int pixelIdx = GetPixelIndex(x, calibrationVectorList[calVecIdx]);
    const double muX = (static_cast<float>(x) - static_cast<float>(vec0.pixels[pixelIdx])) / (static_cast<float>(vec0.pixels[pixelIdx + 1] - vec0.pixels[pixelIdx]));
    const double lutVal = (1 - muY) * ((1 - muX) * vec0.vect[pixelIdx] + muX * vec0.vect[pixelIdx + 1]) +
      muY * ((1 - muX) * vec1.vect[pixelIdx] + muX *   vec1.vect[pixelIdx + 1]);
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

private:

  Sentinel1CalibrationLookupData(const Self&); //purposely not implemented

  void operator =(const Self&); //purposely not implemented

  double firstLineTime;
  double lastLineTime;
  int numOfLines;
  int count;
  std::vector<Sentinel1CalibrationStruct> calibrationVectorList;
  double lineTimeInterval;

};


} // end namespace otb

#endif
