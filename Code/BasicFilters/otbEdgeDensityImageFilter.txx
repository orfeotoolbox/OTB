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

#include "otbEdgeDensityImageFilter.h"


namespace otb
{
/**---------------------------------------------------------
 * Constructor
 ----------------------------------------------------------*/
template <class TInputImage , class TEdgeDetector, class TDensityCount , class TOutputImage>
EdgeDensityImageFilter<TInputImage, TEdgeDetector, TDensityCount, TOutputImage>
::EdgeDensityImageFilter()
{
  this->SetNumberOfRequiredInputs( 1 );
  
  m_NeighborhoodRadius = 1;
  m_Detector =  DetectorType::New();
  m_DensityImageFilter = DensityImageType::New();
}


/*---------------------------------------------------------
 * Destructor.c
 ----------------------------------------------------------*/
template <class TInputImage , class TEdgeDetector, class TDensityCount , class TOutputImage>
EdgeDensityImageFilter<TInputImage, TEdgeDetector, TDensityCount, TOutputImage>
::~EdgeDensityImageFilter()
{}

/**
 * threaded Generate Data
 */

/**
* ThreadedGenerateData Performs the pixel-wise addition
*/
template <class TInputImage , class TEdgeDetector, class TDensityCount , class TOutputImage>
void
EdgeDensityImageFilter<TInputImage, TEdgeDetector, TDensityCount, TOutputImage>
//::GenerateData()
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread, int threadId )
{
  typename Superclass::OutputImagePointer      outputImage = this->GetOutput();
  InputImagePointerType ptr = const_cast<InputImageType *>(this->GetInput());
  if (!ptr)
    return ;

  /** Apply Canny Detector*/
  m_Detector->SetInput(ptr);
  

  /** Compute density on the binaruzed Image */
  m_DensityImageFilter->SetInput(m_Detector->GetOutput());
  m_DensityImageFilter->SetNeighborhoodRadius(m_NeighborhoodRadius);

  /** updating the output*/
  m_DensityImageFilter->GraftOutput(this->GetOutput());
  m_DensityImageFilter->Update();
  this->GraftOutput(m_DensityImageFilter->GetOutput());
}


/**
 * Set Detector
 */
template <class TInputImage , class TEdgeDetector, class TDensityCount , class TOutputImage>
void
EdgeDensityImageFilter<TInputImage, TEdgeDetector, TDensityCount, TOutputImage>
::SetDetector(DetectorType* detector)
{
  m_Detector = detector;
}


/**
 * Get Detector
 */
template <class TInputImage , class TEdgeDetector, class TDensityCount , class TOutputImage>
typename EdgeDensityImageFilter<TInputImage, TEdgeDetector, TDensityCount, TOutputImage> 
::DetectorType *
EdgeDensityImageFilter<TInputImage, TEdgeDetector, TDensityCount, TOutputImage> 
::GetDetector()
{
  return m_Detector;
}



/*----------------------------------------------------------------
  PrintSelf
  -----------------------------------------------------------------*/
template <class TInputImage , class TEdgeDetector, class TDensityCount , class TOutputImage>
void
EdgeDensityImageFilter<TInputImage, TEdgeDetector, TDensityCount, TOutputImage>  
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Neighborhood Radius " << m_NeighborhoodRadius   << std::endl;
}

}/** end namesapce otb*/
