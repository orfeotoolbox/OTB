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

  class Radarsat2CalibrationLookupData : public SarCalibrationLookupData
  {

  public:
    std::vector<float> list;
    int offset;

    std::string GetNameOfClass()
    {
      return "Radarsat2CalibrationLookupData";
    }


    Radarsat2CalibrationLookupData()
      : SarCalibrationLookupData()
      , offset(0)
    {

    }

    Radarsat2CalibrationLookupData(const short type)
      : SarCalibrationLookupData(type)
      , offset(0)
    {

    }

    virtual ~Radarsat2CalibrationLookupData()
    {

    }

    virtual double GetValue(int x, int /*y*/)
    {
      double lutVal = 1.0;

      const size_t pos =  x + offset;
      if(pos  < list.size())
        {
        lutVal = list[pos];
        }
      else
        {
//      itkExceptionMacro( << "error: (pos < list.size() )" << pos << " < " << list.size())
        }

      return lutVal;
    }

  };



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
  SarCalibrationLookupData* GetCalibrationLookupData(const short type);

  bool HasCalibrationLookupData() const
  {
    return true;
  }


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

} // end namespace otb

#endif
