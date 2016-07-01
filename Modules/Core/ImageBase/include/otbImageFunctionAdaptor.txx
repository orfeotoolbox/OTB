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
#ifndef otbImageFunctionAdaptor_txx
#define otbImageFunctionAdaptor_txx

#include "otbImageFunctionAdaptor.h"

namespace otb
{
template< class TInternalImageFunctionType, class TOutputPrecision >
ImageFunctionAdaptor< TInternalImageFunctionType, TOutputPrecision >
::ImageFunctionAdaptor()
{
  m_InternalImageFunction = InternalImageFunctionType::New();
  m_Converter = ConverterType::New();
}

template< class TInternalImageFunctionType, class TOutputPrecision >
void
ImageFunctionAdaptor< TInternalImageFunctionType, TOutputPrecision >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Internal Image Function: " << m_InternalImageFunction << std::endl;
}

template< class TInternalImageFunctionType, class TOutputPrecision >
typename ImageFunctionAdaptor< TInternalImageFunctionType, TOutputPrecision >::OutputType
ImageFunctionAdaptor< TInternalImageFunctionType, TOutputPrecision >
::EvaluateAtIndex(const IndexType& index) const
{
  OutputType result;
  this->GetInternalImageFunction()->SetInputImage(this->GetInputImage());
  InternalImageFunctionOutputType tmpResult = this->GetInternalImageFunction()->EvaluateAtIndex(index);
  result = m_Converter->Convert(tmpResult);

  return result;
}


} // end namespace otb

#endif
