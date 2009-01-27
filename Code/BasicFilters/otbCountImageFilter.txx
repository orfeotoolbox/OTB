/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.

Copyright (c) CS systèmes d'information. All rights reserved.
See CSCopyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbCountImageFilter.h"
#include "itkImageRegionIterator.h"


namespace otb
{
  /**---------------------------------------------------------
   * Constructor
   ----------------------------------------------------------*/
  template <class TInputImage , class TDetector, class TCount, class TOutputImage>
  CountImageFilter<TInputImage , TDetector, TCount, TOutputImage>
  ::CountImageFilter()
  {
    m_CountImageFunction = CountImageFunctionType::New();
    m_NeighborhoodRadius = 1;
  }


 /*---------------------------------------------------------
  * Destructor.c
  ----------------------------------------------------------*/
  template <class TInputImage , class TDetector, class TCount, class TOutputImage>
  CountImageFilter<TInputImage, TDetector, TCount, TOutputImage >
  ::~CountImageFilter()
  {}

 /*-------------------------------------------------------
  * Generate Data
  --------------------------------------------------------*/

  template <class TInputImage , class TDetector, class TCount, class TOutputImage>
  void
  CountImageFilter<TInputImage, TDetector, TCount, TOutputImage >
  ::GenerateData(void)
  {
    InputImagePointerType ptr = const_cast<InputImageType *>(this->GetInput());
    
    OutputImagePointerType outputImage = this->GetOutput();
    
    /** Update the radius for the CountImageFunction */
    m_CountImageFunction->SetNeighborhoodRadius(this->GetNeighborhoodRadius());
    
    itk::ImageRegionIterator<InputImageType> 
                       itInput(ptr, ptr->GetLargestPossibleRegion());
    
    itk::ImageRegionIterator<OutputImageType> 
                       itOutput(outputImage, outputImage->GetLargestPossibleRegion());
    
    CountMethodType CountMethod;
    
    itInput.GoToBegin();
    itOutput.GoToBegin();
    
    while(!itInput.IsAtEnd() && !itOutput.IsAtEnd())
      {
	IndexType index = itInput.GetIndex();
	itOutput.Set(m_CountImageFunction->EvaluateAtIndex(index));
	
	++itInput;
	++itOutput;
      }
           

  }/** End of GenerateData()*/



  /**
   * Set Detector 
   */
  template <class TInputImage , class TDetector, class TCount, class TOutputImage>
  void
  CountImageFilter<TInputImage, TDetector, TCount, TOutputImage >
  ::SetDetector(DetectorType* detector)
  {
    m_CountImageFunction->SetDetector(detector);
  }
  
  
  /**
   * Get Detector 
   */
  template <class TInputImage , class TDetector, class TCount, class TOutputImage>
  typename CountImageFilter< TInputImage ,  TDetector,  TCount,  TOutputImage >
  ::DetectorType *
  CountImageFilter< TInputImage , TDetector, TCount, TOutputImage >
  ::GetDetector()
  {
    return m_CountImageFunction->GetDetector();
  }
  


  /*----------------------------------------------------------------
    PrintSelf
    -----------------------------------------------------------------*/
  template <class TInputImage , class TDetector, class TCount, class TOutputImage>
  void
  CountImageFilter< TInputImage ,  TDetector,  TCount,  TOutputImage >
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "Neighborhood Radius " << m_NeighborhoodRadius   << std::endl;
  }

}/** end namesapce otb*/
