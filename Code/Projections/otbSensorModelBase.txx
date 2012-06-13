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


namespace otb
{

template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::SensorModelBase() : Superclass(OutputSpaceDimension, 0) //FIXME
{
  m_Model = SensorModelAdapter::New();
  m_AverageElevation = -32768; // used as a invalid value
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

/** Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
void
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::SetImageGeometry(const ImageKeywordlist& image_kwl)
{
  m_ImageKeywordlist = image_kwl;
  m_Model->CreateProjection(m_ImageKeywordlist);
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

template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
void
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::SetGeoidFile(const std::string& geoidFile)
{
  m_Model->SetGeoidFile(geoidFile);
}


template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
void
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::AddTiePoint(const InputPointType & inputPoint, const OutputPointType & outputPoint)
{
  itkExceptionMacro(<<"Method should be re-implemented in sub-classes (forward and inverse models)");
}

/** Clear tie points */
template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
void
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::ClearTiePoints()
{
  m_Model->ClearTiePoints();
}

/** Optimize sensor model */
template <class TScalarType,
    unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
double
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>
::Optimize()
{
  return m_Model->Optimize();
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
