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
#ifndef __otbConcatenateVectorImageFilter_txx
#define __otbConcatenateVectorImageFilter_txx

#include "otbConcatenateVectorImageFilter.h"
#include "itkImageRegionIterator.h"

namespace otb {
  /**
   * Constructor.
   */
  template <class TInputImage1,class TInputImage2,class TOutputImage>
  ConcatenateVectorImageFilter<TInputImage1,TInputImage2,TOutputImage>
  ::ConcatenateVectorImageFilter()
  {
    this->SetNumberOfRequiredInputs(2);
  }
  /**
   * Destructor.
   */
  template <class TInputImage1,class TInputImage2,class TOutputImage>
  ConcatenateVectorImageFilter<TInputImage1,TInputImage2,TOutputImage>
  ::~ConcatenateVectorImageFilter()
  {}
  /**
   * Set The first input image.
   * \param image The first input image.
   */
  template <class TInputImage1,class TInputImage2,class TOutputImage>
  void
  ConcatenateVectorImageFilter<TInputImage1,TInputImage2,TOutputImage>
  ::SetInput1(const TInputImage1 * image)
  {
    this->SetNthInput(0,const_cast<TInputImage1 *>(image));
  }
  /**
   * Set The second input image.
   * \param image The second input image.
   */
  template <class TInputImage1,class TInputImage2,class TOutputImage>
  void
  ConcatenateVectorImageFilter<TInputImage1,TInputImage2,TOutputImage>
  ::SetInput2(const TInputImage2 * image)
  {
    this->SetNthInput(1,const_cast<TInputImage2 *>(image));
  }
  /**
   * Get the first input image.
   * \return The first input image.
   */
  template <class TInputImage1,class TInputImage2,class TOutputImage>
  typename ConcatenateVectorImageFilter<TInputImage1,TInputImage2,TOutputImage>
  ::InputImage1Type *
  ConcatenateVectorImageFilter<TInputImage1,TInputImage2,TOutputImage>
  ::GetInput1(void)
  {
    return const_cast<InputImage1Type *>(this->GetInput(0));
  }
  /**
   * Get the second input image.
   * \return The second input image.
   */
  template <class TInputImage1,class TInputImage2,class TOutputImage>
  typename ConcatenateVectorImageFilter<TInputImage1,TInputImage2,TOutputImage>
  ::InputImage2Type *
  ConcatenateVectorImageFilter<TInputImage1,TInputImage2,TOutputImage>
  ::GetInput2(void)
  {
    return const_cast<InputImage2Type *>(this->GetInput(1));
  }
  /**
   * Main computation method.
   */
  template <class TInputImage1,class TInputImage2,class TOutputImage>
  void
  ConcatenateVectorImageFilter<TInputImage1,TInputImage2,TOutputImage>
  ::GenerateData()
  {
    // retrieves inputs and output pointer
    InputImage1PointerType input1 = this->GetInput1();
    InputImage2PointerType input2 = this->GetInput2();
    OutputImagePointerType output = this->GetOutput();

    // Check the requested regions
    typename InputImage1Type::RegionType region1 = input1->GetRequestedRegion();
    typename InputImage2Type::RegionType region2 = input2->GetRequestedRegion();

    if(region1!=region2)
      {
  itkExceptionMacro(<<"InputImage1 and InputImage2 have different requested regions.");
      }

    // Iterators typedefs
    typedef itk::ImageRegionIterator<InputImage1Type> Input1IteratorType;
    typedef itk::ImageRegionIterator<InputImage2Type> Input2IteratorType;
    typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;

    // Initialize output
    output->SetRegions(region1);
    output->SetNumberOfComponentsPerPixel(input1->GetNumberOfComponentsPerPixel()
            +input2->GetNumberOfComponentsPerPixel());
    output->Allocate();

    // Iterators declaration
    Input1IteratorType input1It(input1,region1);
    Input2IteratorType input2It(input2,region1);
    OutputIteratorType outputIt(output,region1);

    input1It.GoToBegin();
    input2It.GoToBegin();
    outputIt.GoToBegin();

    // Iterate through the pixel
    while(!input1It.IsAtEnd()
    &&!input2It.IsAtEnd()
    &&!outputIt.IsAtEnd())
      {
  // define an output pixel
  typename OutputImageType::PixelType output;
  // Retrieve the size of each input pixel
  unsigned int l1 = input1It.Get().GetSize();
  unsigned int l2 = input2It.Get().GetSize();
  // Set the output pixel size
  output.SetSize(l1+l2);
  // Loop through each band of the first image
  for(unsigned int i=0;i<l1;i++)
    {
      // Fill the output pixel
      output[i]=static_cast<typename OutputImageType::InternalPixelType>(input1It.Get()[i]);
    }
  // Loop though each band of the second image
  for(unsigned int i = 0;i<l2;i++)
    {
      // Fill the output pixel
      output[i+l1]=static_cast<typename OutputImageType::InternalPixelType>(input2It.Get()[i]);
    }
  // Set the output pixel
  outputIt.Set(output);
  // Increment the iterator
  ++input1It;
  ++input2It;
  ++outputIt;
      }
  }
  /**
   * PrintSelf method.
   */
  template <class TInputImage1,class TInputImage2,class TOutputImage>
  void
  ConcatenateVectorImageFilter<TInputImage1,TInputImage2,TOutputImage>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }
} // end namespace otb
#endif
