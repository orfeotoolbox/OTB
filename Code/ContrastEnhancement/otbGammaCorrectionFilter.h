/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbGammaCorrectionFilter_h
#define __otbGammaCorrectionFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace otb
{
template<class TInput, class TOutput, class TFunctor >
class ITK_EXPORT GammaCorrectionFilter :
    public itk::UnaryFunctorImageFilter <TInput, TOutput,TFunctor >
{
public:

  typedef GammaCorrectionFilter                                     Self;
  typedef itk::UnaryFunctorImageFilter<TInput, TOutput, TFunctor>   Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;

  typedef typename TInput::InternalPixelType   InternalPixelType;
  typedef itk::Index<3> IndexType;

  itkNewMacro(Self);

  itkTypeMacro(Self, Superclass);

  typedef typename TInput::RegionType          RegionType;
  typedef typename TInput::Pointer            InputImagePointer;
  typedef typename TInput::PixelType           PixelType;

  void SetGamma(double gamma)
  {
    this->GetFunctor().SetGamma(gamma);
    this->Modified();
  }

  virtual void GenerateOutputInformation();
  virtual void BeforeThreadedGenerateData();

protected:

  GammaCorrectionFilter()
    {
    this->SetNumberOfRequiredInputs( 1 );
    }
  virtual ~GammaCorrectionFilter() {  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const { }

private:

  GammaCorrectionFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGammaCorrectionFilter.txx"
#endif

#endif
