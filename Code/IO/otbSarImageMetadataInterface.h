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
#ifndef __otbSarImageMetadataInterface_h
#define __otbSarImageMetadataInterface_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkMetaDataDictionary.h"

#include "otbMetaDataKey.h"
#include "otbImageKeywordlist.h"
#include "otbImageMetadataInterfaceBase.h"
#include "itkPointSet.h"

#include <string>


namespace otb
{
/** \class SarImageMetadataInterface
 *
 * \brief Class for SAR captor metadata reading.
 *
 */
class ITK_EXPORT SarImageMetadataInterface : public ImageMetadataInterfaceBase
{
public:

  typedef SarImageMetadataInterface     Self;
  typedef ImageMetadataInterfaceBase    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                      ImageType;
  typedef Superclass::MetaDataDictionaryType         MetaDataDictionaryType;
  typedef Superclass::VectorType                     VectorType;
  typedef Superclass::VariableLengthVectorType       VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType           ImageKeywordlistType;
  typedef itk::PointSet<double, 2>                   PointSetType;
  typedef PointSetType::Pointer                      PointSetPointer;

  /** GetIncidenceAnglePointSet */
  virtual PointSetPointer GetIncidenceAnglePointSet() const = 0;

protected:
  SarImageMetadataInterface();
  virtual ~SarImageMetadataInterface() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  SarImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#endif
