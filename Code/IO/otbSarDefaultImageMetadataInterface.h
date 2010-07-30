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
#ifndef __otbSarDefaultImageMetadataInterface_h
#define __otbSarDefaultImageMetadataInterface_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbMacro.h"

#include "otbSarImageMetadataInterface.h"

namespace otb
{
/** \class SarDefaultImageMetadataInterface
 *
 * \brief Creation of an "otb" DefaultImageMetadataInterface that gets metadata.
 *
 */
class ITK_EXPORT SarDefaultImageMetadataInterface : public SarImageMetadataInterface
{
public:

  typedef SarDefaultImageMetadataInterface Self;
  typedef SarImageMetadataInterface    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarDefaultImageMetadataInterface, SarImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;


  /** Get the imaging acquisition day from the ossim metadata */
  int GetDay() const
  {
    itkExceptionMacro("GetDay not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition month from the ossim metadata */
  int GetMonth() const
  {
    itkExceptionMacro("GetMonth not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition year from the ossim metadata */
  int GetYear() const
  {
    itkExceptionMacro("GetYear not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition hour from the ossim metadata */
  int GetHour() const
  {
    itkExceptionMacro("GetHour not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition minute from the ossim metadata */
  int GetMinute() const
  {
    itkExceptionMacro("GetMinute not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production day from the ossim metadata */
  int GetProductionDay() const
  {
    itkExceptionMacro("GetProductionDay not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production month from the ossim metadata */
  int GetProductionMonth() const
  {
    itkExceptionMacro("GetProductionMonth not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production year from the ossim metadata */
  int GetProductionYear() const
  {
    itkExceptionMacro("GetProductionYear not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  /** Get Incidence Angle  from the ossim metadata*/
  PointSetPointer GetIncidenceAnglePointSet() const
  {
    itkExceptionMacro("GetIncidenceAnglePointSet not implemented in SarDefaultImageMetadataInterface, no captor type found");
  }

  bool CanRead() const
  {
    // This clas is the default one, it has to be able to call every metadata
    return true;
  }

  unsigned int GetDefaultRBand() const
  {
    return 0;
  }

  unsigned int GetDefaultGBand() const
  {
    return 1;
  }

  unsigned int GetDefaultBBand() const
  {
    return 2;
  }

protected:
  SarDefaultImageMetadataInterface(){};
  virtual ~SarDefaultImageMetadataInterface() {}

private:

  SarDefaultImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
