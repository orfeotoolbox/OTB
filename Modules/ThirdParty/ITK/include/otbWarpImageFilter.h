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
#ifndef __otbWarpImageFilter_h
#define __otbWarpImageFilter_h

#include "vcl_deprecated_header.h"

#include "itkWarpImageFilter.h"

namespace otb
{

/** \class WarpImageFilter
 *  \brief Deprecated VectorImage enabled WarpImageFilter
 *
 *  WarpImageFilter is deprecated and provided for backward
 *  compatibility only. Full support of VectorImage is now provided by
 *  itk::WarpImageFilter, which offers strictly the same API.
 *
 *  \sa itk::WarpImageFilter
 */
template <
  class TInputImage,
  class TOutputImage,
  class TDisplacementField
  >
class ITK_EXPORT WarpImageFilter :
    public itk::WarpImageFilter<TInputImage, TOutputImage,TDisplacementField>
{
public:
  /** Standard class typedefs. */
  typedef WarpImageFilter                                                   Self;
  typedef itk::WarpImageFilter<TInputImage,TOutputImage,TDisplacementField> Superclass;
  typedef itk::SmartPointer<Self>                                           Pointer;
  typedef itk::SmartPointer<const Self>                                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro( WarpImageFilter, itk::WarpImageFilter );

protected:
  WarpImageFilter() {};
  virtual ~WarpImageFilter() {};
};

} // end namespace otb

#endif
