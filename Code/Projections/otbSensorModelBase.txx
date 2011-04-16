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
#ifndef __otbSensorModelBase_txx
#define __otbSensorModelBase_txx

#include "otbSensorModelBase.h"

#include "otbMacro.h"
#include "itkExceptionObject.h"


#include "base/ossimKeywordlist.h"

namespace otb
{

template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::SensorModelBase() : Superclass(OutputSpaceDimension, 0) //FIXME
{
  m_Model = SensorModelWrapper::New();
    m_AverageElevation = -32768;
}

template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::~SensorModelBase()
{}

/// Get the Geometry Keyword list
template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
ImageKeywordlist
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::GetImageGeometryKeywordlist(void) const
{
  return m_ImageKeywordlist;
}

/// Get the Geometry Keyword list
template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
ossimKeywordlist
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::GetOssimKeywordlist(void)
{
  ossimKeywordlist geom;
  m_ImageKeywordlist.convertToOSSIMKeywordlist(geom);

  return geom;
}

/// Get the ossim model
template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
ossimProjection*
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::GetOssimModel(void)
{

  return m_Model->GetOssimModel(); // FIXME 
}

/** Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
bool
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::SetImageGeometry(const ImageKeywordlist& image_kwl)
{
  m_ImageKeywordlist = image_kwl;
  return m_Model->CreateProjection(m_ImageKeywordlist);
}

/** Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
bool
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::SetImageGeometry(const ossimKeywordlist& geom_kwl)
{
  m_ImageKeywordlist.Clear();
  m_ImageKeywordlist.SetKeywordlist(geom_kwl);
  return m_Model->CreateProjection(m_ImageKeywordlist);
}

template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
void 
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::SetDEMDirectory(const std::string& directory)
{
  m_Model->SetDEMDirectory(directory);
}


/**
 * PrintSelf method
 */
template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
void
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Model: " << m_Model << std::endl;
  os << indent << "Keywordlist: " << m_ImageKeywordlist << std::endl;
}

} // namespace otb

#endif
