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
#ifndef __otbVectorImageColorSpaceTransformFilter_h
#define __otbVectorImageColorSpaceTransformFilter_h


#include "itkUnaryFunctorImageFilter.h"
#include "itkIndex.h"

namespace otb
{

template<class TInput, class TOutput, class TFunctor >
class ITK_EXPORT VectorImageColorSpaceTransformFilter :
    public itk::UnaryFunctorImageFilter <TInput, TOutput,TFunctor >
{
public:

  typedef VectorImageColorSpaceTransformFilter          Self;
  typedef itk::UnaryFunctorImageFilter<TInput, TOutput, TFunctor>   Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  typedef typename TInput::InternalPixelType   InternalPixelType;
  typedef itk::Index<3> IndexType;

  itkNewMacro(Self);


  itkTypeMacro(Self, Superclass);

  typedef typename TInput::RegionType          RegionType;
  typedef typename TInput::Pointer            InputImagePointer;
  typedef typename TInput::PixelType           PixelType;
  



  void SetRGBIndex(const IndexType index)
  {

    this->GetFunctor().SetRGBIndex(index);
    this->Modified();


  }

  void SetMaxPixelValue(unsigned int value)
  {

    this->GetFunctor().SetMaxPixelValue(value);
    this->Modified();

  }

  void SetDelta(unsigned int delta)
  {
    this->GetFunctor().SetDelta(delta);
    this->Modified();

  }

  virtual void GenerateOutputInformation();
  virtual void BeforeThreadedGenerateData();



protected:

  VectorImageColorSpaceTransformFilter()
    {
    this->SetNumberOfRequiredInputs( 1 );
    }
  virtual ~VectorImageColorSpaceTransformFilter()
  {

  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const { }


private:

//  IndexType m_RGBIndex;

  VectorImageColorSpaceTransformFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImageColorSpaceTransformFilter.txx"
#endif

#endif
