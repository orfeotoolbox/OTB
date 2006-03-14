/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   09 mars 2006
  Role      :   Filter detection of contours 
  $Id$ 

=========================================================================*/
#ifndef __otbTouziEdgeDetector_txx
#define __otbTouziEdgeDetector_txx

#include "otbTouziEdgeDetector.h"

#include "itkDataObject.h"
#include "itkExceptionObject.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
//#include "itkOffset.h"
#include "itkProgressReporter.h"
#include <math.h>

#define M_PI 3.14159265358979323846

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
TouziEdgeDetector<TInputImage, TOutputImage>::TouziEdgeDetector()
{
  m_Radius.Fill(1);
}

template <class TInputImage, class TOutputImage>
void TouziEdgeDetector<TInputImage, TOutputImage>::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the input and output
  typename Superclass::InputImagePointer inputPtr   =  const_cast< TInputImage * >( this->GetInput() );
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
  
  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( m_Radius );

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    
    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << static_cast<const char *>(this->GetNameOfClass())
        << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

/**
 * Set up state of filter before multi-threading.
 * InterpolatorType::SetInputImage is not thread-safe and hence
 * has to be set up before ThreadedGenerateData
 */
template <class TInputImage, class TOutputImage>
void 
TouziEdgeDetector<TInputImage,TOutputImage>
::BeforeThreadedGenerateData()
{
//  std::cout << " BeforeThreadedGenerateData"<<std::endl;

  typename OutputImageType::RegionType region;    
  typename OutputImageType::Pointer     output = this->GetOutput();

  m_DirectionOuputImage = OutputImageType::New();

  region.SetSize(output->GetLargestPossibleRegion().GetSize());
  region.SetIndex(output->GetLargestPossibleRegion().GetIndex());
  m_DirectionOuputImage->SetRegions( region );
  m_DirectionOuputImage->SetOrigin(output->GetOrigin());
  m_DirectionOuputImage->SetSpacing(output->GetSpacing());
  m_DirectionOuputImage->Allocate();

}

template <class TInputImage, class TOutputImage>
const typename TouziEdgeDetector<TInputImage,TOutputImage>::OutputImageType *
TouziEdgeDetector<TInputImage,TOutputImage>
::GetOutputDirection()
{
	this->Update();
	return 	static_cast< const OutputImageType *> (m_DirectionOuputImage);
}


template< class TInputImage, class TOutputImage>
void TouziEdgeDetector< TInputImage, TOutputImage>
::ThreadedGenerateData(	
			const 	OutputImageRegionType& 		outputRegionForThread,
                       	int 	threadId
		       )
{
  unsigned int i;
  itk::ZeroFluxNeumannBoundaryCondition<InputImageType> 	nbc;
  itk::ConstNeighborhoodIterator<InputImageType> 		bit;
  typename itk::ConstNeighborhoodIterator<InputImageType>::OffsetType	off;
  itk::ImageRegionIterator<OutputImageType> 			it;
  itk::ImageRegionIterator<OutputImageType> 			it_dir;
  
  
  // Allocate output
  typename OutputImageType::Pointer     output = this->GetOutput();
  typename InputImageType::ConstPointer input  = this->GetInput();
  typename OutputImageType::Pointer     outputDir = m_DirectionOuputImage;
    
  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType 		faceList;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator 	fit;

  itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
  faceList = bC(input, outputRegionForThread, m_Radius);


  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  typename TInputImage::IndexType     bitIndex;

  // Initialisations
  // ---------------
  // Number of direction
  const int NB_DIR = 4;
  // Number of half window of the filter	  
  const int NB_WIN = 2;  
  // Definition of the 4 directions
  double Theta[NB_DIR];
  
  Theta[0] = 0.;
  Theta[1] = M_PI / 2. ;
  Theta[2] = M_PI / 4. ;
  Theta[3] = 3*M_PI / 4. ;
  

  // contains for the 4 directions the sum of the pixels belonging to each half window
  double Sum[NB_DIR][NB_WIN];
  // Mean of half window 1
  double M1;
  // Mean of half window 2
  double M2;
  // Result of the filter for each direction
  double R_theta[NB_DIR];
  double Sum_R_theta = 0;
  // Intensity of the contour
  double I_contour; 
  // Direction of the contour
  double Dir_contour = 0; 
  // sign of the contour
  int sign;

  // position of the pixel of the window
  int x;
  int y;
  // position of the central pixel of the window
  int xc;
  int yc;

 
    
  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit=faceList.begin(); fit != faceList.end(); ++fit)
    { 
    bit = itk::ConstNeighborhoodIterator<InputImageType>(m_Radius, input, *fit);
    unsigned int neighborhoodSize = bit.Size();
      
    it = itk::ImageRegionIterator<OutputImageType>(output, *fit);
    it_dir = itk::ImageRegionIterator<OutputImageType>(outputDir, *fit);
    
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();
    
//std::cout <<"***" << neighborhoodSize << std::endl; 


    while ( ! bit.IsAtEnd() )
      {
//std::cout << bit.GetIndex() << std::endl;

      // Position of the central pixel
      bitIndex = bit.GetIndex();
      
      xc = bitIndex[0];
      yc = bitIndex[1];

      // Initialisations
      for (int dir=0; dir<NB_DIR; dir++)
        {
        for (int m=0; m<NB_WIN; m++) 
          Sum[dir][m] = 0.;
        }
        
      Dir_contour = 0.;
      Sum_R_theta = 0.;
      
      
      // Loop on the window of the filter  
      for (i = 0; i < neighborhoodSize; ++i)
        {

//std::cout << "---"<< i <<" "<< bit.GetIndex(i)<< std::endl;

        bitIndex = bit.GetIndex(i);
        x = bitIndex[0];
        y = bitIndex[1];
 
//std::cout << "---"<< i <<" "<< static_cast<double>(bit.GetPixel(i))<< std::endl;     
      
        // We determine for each direction with which half window the pixel belongs. 
         
        // Vertical direction 
        if ( x < xc )
      	  Sum[0][0] += static_cast<double>(bit.GetPixel(i));
        else if ( x > xc )
          Sum[0][1] += static_cast<double>(bit.GetPixel(i));
          
        // Horizontal direction
        if ( y < yc )
      	  Sum[1][0] += static_cast<double>(bit.GetPixel(i));
        else if ( y > yc )
          Sum[1][1] += static_cast<double>(bit.GetPixel(i));
      
        // Diagonal direction 1
        if ( (y-yc) < -(x-xc) )
      	  Sum[2][0] += static_cast<double>(bit.GetPixel(i));
        else if ( (y-yc) > -(x-xc) )
          Sum[2][1] += static_cast<double>(bit.GetPixel(i));
      
        // Diagonal direction 2
        if ( (y-yc) > (x-xc) )
      	  Sum[3][0] += static_cast<double>(bit.GetPixel(i));
        else if ( (y-yc) < (x-xc) )
          Sum[3][1] += static_cast<double>(bit.GetPixel(i));
          
        } // end of the loop on the pixels of the window           

//std::cout << static_cast<double>(Sum[0][0])/ double(neighborhoodSize) << std::endl;    
           
      // Loop on the 4 directions
      for ( int dir=0; dir<NB_DIR; dir++ )
        {
        // Calculation of the averages of the 2 half windows	
        M1 = Sum[dir][0] / double(neighborhoodSize);
        M2 = Sum[dir][1] / double(neighborhoodSize);
     	
        // Calculation of the intensity of the contour
        if (( M1 != 0 ) && (M2 != 0)) 
          R_theta[dir] = static_cast<double>( 1 - MIN( (M1/M2), (M2/M1) ) );
        else
	  R_theta[dir] = 0.;
	 
        // Determination of the sign of contour
        if ( M1 > M2 ) 
          sign = +1;
        else
          sign = -1;

        Dir_contour += sign * Theta[dir] * R_theta[dir];        
        Sum_R_theta += R_theta[dir];
	 
//std::cout << "dir " << dir << " " << Sum[dir][0] <<" "<< Sum[dir][1] <<" "<< R_theta[dir] << std::endl; 
//std::cout << "dir " << dir << " " << sign*Theta[dir] <<" "<< R_theta[dir]<<" "<< Dir_contour << std::endl; 
      		
        } // end of the loop on the directions
      
      // Determination of the maximum intensity of the contour
      I_contour = R_theta[0];
      
      for (int dir=1; dir<NB_DIR; dir++)
        I_contour = static_cast<double>( MAX( I_contour, R_theta[dir] ) );
 
//std::cout << "IC = " << I_contour << std::endl; 

      // Assignment of this value to the output pixel
      it.Set( static_cast<OutputPixelType>(I_contour) );  

      // Determination of the direction of the contour
      if ( Sum_R_theta != 0. )	
        Dir_contour = Dir_contour / Sum_R_theta;

//std::cout << Dir_contour <<" "<< Sum_R_theta << std::endl;
      
      // Assignment of this value to the "outputdir" pixel
      it_dir.Set( static_cast<OutputPixelType>(Dir_contour) );
                     
      ++bit;
      ++it;
      ++it_dir;
      progress.CompletedPixel();  
      
      }
      
    }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void 
TouziEdgeDetector<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Radius: " << m_Radius << std::endl;
}


} // end namespace otb


#endif
