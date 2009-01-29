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

#include "otbPointSetToDensityImageFilter.h"
#include "itkImageRegionIterator.h"


namespace otb
{
  /**---------------------------------------------------------
   * Constructor
   ----------------------------------------------------------*/
  template <class TInputPointSet , class TOutputImage  >
  PointSetToDensityImageFilter<  TInputPointSet ,  TOutputImage  >
  ::PointSetToDensityImageFilter()
  {
    m_Radius = 1;
  }


 /*---------------------------------------------------------
  * Destructor.c
  ----------------------------------------------------------*/
  template <class TInputPointSet , class TOutputImage  >
  PointSetToDensityImageFilter< TInputPointSet ,  TOutputImage >
  ::~PointSetToDensityImageFilter()
  {}

 /*-------------------------------------------------------
  * Generate Data
  --------------------------------------------------------*/
  template <class TInputPointSet , class TOutputImage  >
  void
  PointSetToDensityImageFilter<  TInputPointSet ,  TOutputImage>
  ::GenerateData(void)
  {
    this->AllocateOutputs();
    
    PointSetDensityFunctionPointerType densityComputeFunction = PointSetDensityFunctionType::New();
    densityComputeFunction->SetPointSet(const_cast<PointSetType*>(this->GetInput()));
    densityComputeFunction->SetRadius(m_Radius);
      
    /** Point*/
    InputType   pCenter;
    IndexType index;
    itk::ImageRegionIterator<OutputImageType> itOut(this->GetOutput(),
						    this->GetOutput()->GetLargestPossibleRegion());
    itOut.GoToBegin();

    while(!itOut.IsAtEnd())
      {
	index = itOut.GetIndex();
	pCenter[0] = index[0];
	pCenter[1] = index[1];

	itOut.Set(densityComputeFunction->Evaluate(pCenter));
	++itOut;
      }
  }/** End of GenerateData()*/

/*-------------------------------------------------------
  * Generate Data
  --------------------------------------------------------*/
  template <class TInputPointSet , class TOutputImage  >
  void
  PointSetToDensityImageFilter<  TInputPointSet ,  TOutputImage>
  ::GenerateOutputInformation(void)
  {
    //Superclass::GenerateOutputInformation();
    typename  Superclass::OutputImagePointer outputPtr = this->GetOutput();

    if ( !outputPtr )
    {
    return;
    }
    
  typename OutputImageType::RegionType region;
  IndexType start ;
  start.Fill(0);
  
  region.SetSize(this->GetSize());
  region.SetIndex(start);
  
  outputPtr->SetOrigin(this->GetOrigin());
  outputPtr->SetSpacing(this->GetSpacing());
  outputPtr->SetRegions( region );

  
  
  }/** End of GenerateoutputInformation*/
  

  /*----------------------------------------------------------------
    PrintSelf
    -----------------------------------------------------------------*/
  template <class TInputPointSet , class TOutputImage  >
  void
  PointSetToDensityImageFilter< TInputPointSet ,  TOutputImage >
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }
}
