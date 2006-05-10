/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   30 mars 2006
  Role      :   Filtre de suppression des pixels isoles par direction  
  $Id: $ 

=========================================================================*/
#ifndef __otbPixelSuppressionByDirectionImageFilter_txx
#define __otbPixelSuppressionByDirectionImageFilter_txx

#include "otbPixelSuppressionByDirectionImageFilter.h"

#include "itkDataObject.h"
#include "itkExceptionObject.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkConstantBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"
#include <math.h>

#define M_PI 3.14159265358979323846

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>::PixelSuppressionByDirectionImageFilter()
{
	
  m_Radius.Fill(1);
  m_AngularBeam = static_cast<double>(0.);

}

template <class TInputImage, class TOutputImage>
void 
PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>
::SetInputImage(const InputImageType *input)
{
  this->SetInput(0,input);	
}

template <class TInputImage, class TOutputImage>
void 
PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>
::SetInputImageDirection(const InputImageType *input)
{
  this->SetInput(1,input);	
}

template <class TInputImage, class TOutputImage>
const 
typename PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>::InputImageType *
PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>
::GetInputImage(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TInputImage * >
    (this->GetInput(0) );
}

template <class TInputImage, class TOutputImage>
const 
typename PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>::InputImageType *
PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>
::GetInputImageDirection(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TInputImage * >
    (this->GetInput(1) );
}

template <class TInputImage, class TOutputImage>
void PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the input and output
  typename Superclass::InputImagePointer inputPtr   =  const_cast< TInputImage * >( this->GetInputImageDirection() );
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


template< class TInputImage, class TOutputImage>
void PixelSuppressionByDirectionImageFilter< TInputImage, TOutputImage>::ThreadedGenerateData(	
			const 	OutputImageRegionType& 		outputRegionForThread,
                       	int 	threadId
				)
{
  unsigned int i; 
  itk::ConstantBoundaryCondition<InputImageType> 		cbc;
  const InputPixelType cvalue = 255;
  cbc.SetConstant(cvalue);
   
  itk::ConstNeighborhoodIterator<InputImageType> 		bit; 
  itk::ImageRegionConstIterator<InputImageType> 		itin;
  itk::ImageRegionIterator<OutputImageType> 			itout;
  
  
  // Allocate output
  typename OutputImageType::Pointer     output = this->GetOutput();
  typename InputImageType::ConstPointer input  = this->GetInputImage();
  typename InputImageType::ConstPointer inputDirection  = this->GetInputImageDirection();
  
  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType 		faceList;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator 	fit;


  itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
  faceList = bC(inputDirection, outputRegionForThread, m_Radius);


  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  typename TInputImage::IndexType     bitIndex;
  
  //---------------------------------------------------------------------------

  InputPixelType PixelValue;
    
  // Location of the central pixel in the input image
  int Xc, Yc;

  // Pixel location in the system axis of the region  
  int x, y;
  
  // Pixel location in the system axis of the region after rotation of theta
  // where theta is the direction of the cantral pixel  
  double xt, yt;
  
  // Distance between pixel (x,y) and the central pixel of the region
  double DistanceXY;
   
  // Pixel Direction  
  double ThetaXcYc, Thetaxtyt, ThetaXY ;
  
  // Angular tolerance on the direction of the central pixel
  double MinThetaXcYc, MaxThetaXcYc; 
  

  //---------------------------------------------------------------------------
  
  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit=faceList.begin(); fit != faceList.end(); ++fit)
    { 
    bit = itk::ConstNeighborhoodIterator<InputImageType>(m_Radius, inputDirection, *fit);
    unsigned int neighborhoodSize = bit.Size();
      
    itin = itk::ImageRegionConstIterator<InputImageType>(input, *fit);  
    itout = itk::ImageRegionIterator<OutputImageType>(output, *fit);
    
    bit.OverrideBoundaryCondition(&cbc);
    bit.GoToBegin();
 
 
    while ( ! bit.IsAtEnd() )
      {
      	
      // Location of the central pixel of the region in the input image
      bitIndex = bit.GetIndex();
      
      Xc = bitIndex[0];
      Yc = bitIndex[1];
    
      // Get Pixel Direction from the image of directions
      ThetaXcYc = static_cast<double>( bit.GetCenterPixel() );

      // Pixel intensity in the input image      
      PixelValue = itin.Get();

      bool IsLine = false;
      
      // Loop on the region 
      for (i = 0; i < neighborhoodSize; ++i)
        {

        // Pixel location in the system axis of the region (Xc,Yc) -> (0,0)        	
	bitIndex = bit.GetIndex(i);
	
        x = bitIndex[0] - Xc;
        y = bitIndex[1] - Yc;
        
        // No calculation on the central pixel  
        if (( x == 0 ) and ( y == 0 )) 
           continue;

	// Distance between pixel (x,y) and central pixel 
//	DistanceXY = static_cast<double>(sqrt(x*x+y*y));
		
	
	// If the pixel (x,y) is inside the circle of radius m_Radius
//	if ( DistanceXY <= m_Radius[0] )
//	   {
	   
// 	   // Rotation of the system axis in the direction of the central pixel (thetaXcYc)
// 	   // (x,y) -> (xt,yt)
// 	   TRANSITION_MATRIX( x, y, ThetaXcYc, xt, yt );

   	
//    	   // The absolute value of yt/xt is used to bring back the pixel (xt,yt)
//    	   // in positive co-ordinates 
//    	   // => thetaxtyt is included in the interval [0;PI/2]
// 	   if ( xt != 0. )
// 	      Thetaxtyt = atan( fabs(yt/xt) );
// 	   else
// 	      Thetaxtyt = M_PI/2.;

	   
// 	   // Then, we test if the pixel is included in the interval [0;m_AngularBeam] 
// 	   if ( (0. <= Thetaxtyt) and
// 	   	(Thetaxtyt <= m_AngularBeam) )
// 	      {
// 	      // The pixel is located in the direction of the central pixel
// 	      // Get the direction of the pixel (X,Y) in the image of diretions
// 	      ThetaXY = static_cast<double>(bit.GetPixel(i));
	      
// 	      // Angular tolerance on the direction of the central pixel
// 	      MinThetaXcYc = ThetaXcYc - m_AngularBeam;
// 	      MaxThetaXcYc = ThetaXcYc + m_AngularBeam;	

// 	      // Test if the pixel belongs to a line   
// 	      if ( (MinThetaXcYc <= ThetaXY) &&
// 	      	   (ThetaXY <= MaxThetaXcYc ) )
	      	   
// 		 IsLine = true;
//	   }

	   Thetaxtyt = atan2(y,x) - M_PI/2.0;
	   if(Thetaxtyt < 0)
	     Thetaxtyt = M_PI + Thetaxtyt;

//	   std::cout << sin(Thetaxtyt) << " " << sin(ThetaXcYc) << " " << sin(bit.GetPixel(i)) << std::endl;
	   if( (fabs(sin(Thetaxtyt-ThetaXcYc)) <= sin(m_AngularBeam)) && (fabs(sin(bit.GetPixel(i)-ThetaXcYc)) <= sin(m_AngularBeam)) )
	     IsLine = true;
//	   }
	      

	}

       // end of the loop on the pixels of the region 
  

      // Assignment of this value to the output pixel
      if (IsLine == true)
         itout.Set( static_cast<OutputPixelType>(PixelValue) );
      else
         itout.Set( static_cast<OutputPixelType>(0.) );  
                 
      
      ++bit;
      ++itin;
      ++itout;
      progress.CompletedPixel();  
      
      }
      
    }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void 
PixelSuppressionByDirectionImageFilter<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Radius: " << m_Radius << std::endl;
}


} // end namespace otb


#endif
