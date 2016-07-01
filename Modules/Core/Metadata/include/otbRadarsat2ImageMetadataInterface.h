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
#ifndef otbRadarsat2ImageMetadataInterface_h
#define otbRadarsat2ImageMetadataInterface_h

#include "otbSarImageMetadataInterface.h"

namespace otb
{
/** \class Radarsat2ImageMetadataInterface
 *
 * \brief Creation of an "otb" Radarsat2ImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */

class ITK_ABI_EXPORT Radarsat2ImageMetadataInterface : public SarImageMetadataInterface
{
public:

  typedef Radarsat2ImageMetadataInterface    Self;
  typedef SarImageMetadataInterface         Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Radarsat2ImageMetadataInterface, SarImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;
  typedef Superclass::LookupDataPointerType LookupDataPointerType;
//  typedef Radarsat2CalibrationLookupData::Pointer          LookupDataPointerType;

  /*ImageMetadataInterfaceBase pure virtuals */
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
  /* class constructor */
  Radarsat2ImageMetadataInterface();

  /* class desctructor */
  ~Radarsat2ImageMetadataInterface() ITK_OVERRIDE {}

private:
  Radarsat2ImageMetadataInterface(const Self &); //purposely not implemented
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


class Radarsat2CalibrationLookupData : public SarCalibrationLookupData
{

public:

  /** Standard typedefs */
  typedef Radarsat2CalibrationLookupData   Self;
  typedef SarCalibrationLookupData         Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(Radarsat2CalibrationLookupData, SarCalibrationLookupData);

  typedef itk::IndexValueType IndexValueType;

  typedef std::vector<float> GainListType;


  Radarsat2CalibrationLookupData()
    : m_Offset(0)
  {

  }

  ~Radarsat2CalibrationLookupData() ITK_OVERRIDE
  {

  }

  void InitParameters(short type, int offset,  GainListType gains)
  {
    this->SetType(type);
    m_Offset = offset;
    m_Gains = gains;
  }

  double GetValue(const IndexValueType x, const IndexValueType itkNotUsed(y)) ITK_OVERRIDE
  {
    double lutVal = 1.0;

    const size_t pos =  x + m_Offset;
    if(pos  < m_Gains.size())
      {
      lutVal = m_Gains[pos];
      }
    else
      {
      //itkExceptionMacro( << "error: (pos < list.size() )" << pos << " < " << list.size())
      }
    return lutVal;
  }

  void PrintSelf(std::ostream & os, itk::Indent indent) const ITK_OVERRIDE
  {
    os << indent << " offset:'" << m_Offset << "'" << std::endl;
    os <<  " referenceNoiseLevel.gain: " << std::endl;
    std::vector<float>::const_iterator it = m_Gains.begin();
    while (it != m_Gains.end())
      {
      os << (*it) << " ";
      ++it;
      }
    os << std::endl;

    Superclass::PrintSelf(os, indent);
  }

private:

  Radarsat2CalibrationLookupData(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  GainListType m_Gains;
  int m_Offset;


};

} // end namespace otb

#endif
