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
#ifndef __otbRadarsat2ImageMetadataInterface_h
#define __otbRadarsat2ImageMetadataInterface_h

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
  void CreateCalibrationLookupData(const short type);


protected:
  /* class constructor */
  Radarsat2ImageMetadataInterface();

  /* class desctructor */
  virtual ~Radarsat2ImageMetadataInterface() {}

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

  virtual ~Radarsat2CalibrationLookupData()
  {

  }

  void InitParameters(short type, int offset,  GainListType gains)
  {
    this->SetType(type);
    m_Offset = offset;
    m_Gains = gains;
  }

  double GetValue(const IndexValueType x, const IndexValueType itkNotUsed(y))
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

  void PrintSelf(std::ostream & os, itk::Indent indent) const
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
