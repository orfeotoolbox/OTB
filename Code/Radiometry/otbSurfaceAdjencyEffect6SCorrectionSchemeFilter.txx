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

#ifndef _otbSurfaceAdjencyEffect6SCorrectionSchemeFilter_txx
#define _otbSurfaceAdjencyEffect6SCorrectionSchemeFilter_txx

#include "otbSurfaceAdjencyEffect6SCorrectionSchemeFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"
#include "otbImage.h"


namespace otb
{

template <class TInputImage, class TOutputImage>
SurfaceAdjencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::SurfaceAdjencyEffect6SCorrectionSchemeFilter()
{ 
  m_WindowRadius = 1;
  m_UpwardTransmission = 1.;
  m_UpwardDirectTransmission = 1.;
  m_UpwardDiffusTransmission = 1.;   
}


template <class TInputImage, class TOutputImage>
void 
SurfaceAdjencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData ()
{
  // get pointers to the input and output
  typename InputImageType::Pointer inputPtr = const_cast< TInputImage * >( this->GetInput() );
  typename OutputImageType::Pointer outputPtr = const_cast< TOutputImage * >( this->GetOutput() );

  outputPtr->SetNumberOfComponentsPerPixel(inputPtr->GetNumberOfComponentsPerPixel());

  // Check Matrix ponderation value validity.
  if(std::floor(static_cast<double>(m_PonderationValues[0].Rows())) != 2*m_WindowRadius+1 
     ||  std::floor(static_cast<double>(m_PonderationValues[0].Cols())) != 2*m_WindowRadius+1)
    {
      itkExceptionMacro( << "The ponderation values matrix have to have a size equal to 2*WindowRadius+1."); 
    }
  // Check the validity of the ponderation container.
  if(m_PonderationValues.size()==inputPtr->GetNumberOfComponentsPerPixel() && m_PonderationValues.size()!=1)
    {
      for (unsigned int i=0; i<m_PonderationValues.size()-1;i++)
	{
	  // Check the matrix size parity. It has to be odd.
	  if(std::floor(static_cast<double>(m_PonderationValues[i].Rows())/2.) == static_cast<double>(m_PonderationValues[i].Rows())/2. 
	     &&  std::floor(static_cast<double>(m_PonderationValues[i].Cols())/2.) == static_cast<double>(m_PonderationValues[i].Cols())/2)
	  {
	      itkExceptionMacro( << "Each ponderation values matrix has to have an odd size (and 0 as central value)."); 
	  }
	  if(m_PonderationValues[i].Rows()!=m_PonderationValues[i+1].Rows() || m_PonderationValues[i].Cols()!=m_PonderationValues[i+1].Cols())
	    {
	      itkExceptionMacro( << "Each ponderation values matrix has to have the same number of components."); 
	    }
	}
    }
  else
    {
      itkExceptionMacro( << "Each channel has to have its own ponderation values vector (or 1 vector apply to every channel).");  
    }

  // Functor initialisation
  double tempInvUpwardDirectTransmission = 1./m_UpwardDirectTransmission;
  this->GetFunctor().SetUpwardTransmissionRatio(m_UpwardTransmission*tempInvUpwardDirectTransmission);
  this->GetFunctor().SetDiffusRatio(m_UpwardDiffusTransmission*tempInvUpwardDirectTransmission);
  this->GetFunctor().SetPonderationValues(m_PonderationValues);
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
SurfaceAdjencyEffect6SCorrectionSchemeFilter<TInputImage, TOutput>
::PrintSelf( std::ostream& os, itk::Indent indent) const
{
  os << indent << "Radius : " << m_WindowRadius << std::endl;
  os << indent << "Upward direct transmission : " << m_UpwardDirectTransmission << std::endl;
  os << indent << "Upward transmission : " << m_UpwardTransmission << std::endl;
  os << indent << "Upward diffus transmission : " << m_UpwardDiffusTransmission << std::endl;

}

} // end namespace otb

#endif
