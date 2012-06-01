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
#ifndef __otbGeometriesProjectionFilter_h
#define __otbGeometriesProjectionFilter_h

#include "otbGeometriesToGeometriesFilter.h"

namespace otb
{
class ITK_EXPORT GeometriesProjectionFilter : public GeometriesToGeometriesFilter
  {
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef GeometriesProjectionFilter    Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  //@}

  /**\name I/O typedefs */
  //@{
  typedef SuperClass::InputGeometriesType     InputGeometriesType;
  typedef SuperClass::InputGeometriesPointer  InputGeometriesPointer;
  typedef SuperClass::OutputGeometriesType    OutputGeometriesType;
  typedef SuperClass::OutputGeometriesPointer OutputGeometriesPointer;
  //@}

  /**\name Standard macros */
  //@{
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometriesProjectionFilter, GeometriesToGeometriesFilter);
  //@}

protected:
  GeometriesProjectionFilter();
  virtual ~GeometriesProjectionFilter();

  virtual DoProcessLayer(ogr::Layer const& source, ogr::Layer & destination);

  };
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGeometriesProjectionFilter.txx"
#endif

#endif // __otbGeometriesProjectionFilter_h
