/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   14 mars 2006
  Role      :   Filter of detection of linear features 
  $Id$ 

=========================================================================*/
#ifndef __otbLineCorrelationDetector_txx
#define __otbLineCorrelationDetector_txx

#include "otbLineCorrelationDetector.h"

#include "itkDataObject.h"
#include "itkExceptionObject.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkProgressReporter.h"
#include <math.h>

#define M_PI 3.14159265358979323846

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class InterpolatorType >
LineCorrelationDetector<TInputImage, TOutputImage, InterpolatorType>::LineCorrelationDetector()
{
  m_Radius.Fill(1);
  m_LengthLine = 1;
  m_WidthLine = 0;
  m_FaceList.Fill(0);
  m_Interpolator = InterpolatorType::New(); 
}

template <class TInputImage, class TOutputImage, class InterpolatorType>
void LineCorrelationDetector<TInputImage, TOutputImage, InterpolatorType>::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
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

  // Define the size of the region by the radius
  m_Radius[0] = static_cast<unsigned int>(3*m_WidthLine + 2); 
  m_Radius[1] = m_LengthLine ;
  
  // Define the size of the facelist by taking into account the rotation of the region
  m_FaceList[0] = static_cast<unsigned int>( sqrt((m_Radius[0]*m_Radius[0])+ (m_Radius[1]*m_Radius[1]))+1 );
  m_FaceList[1] = m_FaceList[0];


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
template <class TInputImage, class TOutputImage, class InterpolatorType>
void 
LineCorrelationDetector< TInputImage, TOutputImage, InterpolatorType>
::BeforeThreadedGenerateData()
{

  typename OutputImageType::RegionType  region;    
  typename OutputImageType::Pointer     output = this->GetOutput();

  m_DirectionOuputImage = OutputImageType::New();

  region.SetSize(output->GetLargestPossibleRegion().GetSize());
  region.SetIndex(output->GetLargestPossibleRegion().GetIndex());
  m_DirectionOuputImage->SetRegions( region );
  m_DirectionOuputImage->SetOrigin(output->GetOrigin());
  m_DirectionOuputImage->SetSpacing(output->GetSpacing());
  m_DirectionOuputImage->Allocate();

}

template <class TInputImage, class TOutputImage, class InterpolatorType>
const typename LineCorrelationDetector< TInputImage, TOutputImage, InterpolatorType>::OutputImageType *
LineCorrelationDetector< TInputImage, TOutputImage, InterpolatorType>
::GetOutputDirection()
{
	this->Update();
	return 	static_cast< const OutputImageType *> (m_DirectionOuputImage);
}

template< class TInputImage, class TOutputImage, class InterpolatorType>
void LineCorrelationDetector< TInputImage, TOutputImage, InterpolatorType>
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
  itk::ImageRegionIterator<OutputImageType> 			itdir;
  
  // Allocate output
  typename InputImageType::ConstPointer input  = this->GetInput();
  typename OutputImageType::Pointer     output = this->GetOutput();
  typename OutputImageType::Pointer     outputDir = m_DirectionOuputImage;

  
  m_Interpolator->SetInputImage(input);
   
  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType 		faceList;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator 	fit;


  itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
  faceList = bC(input, outputRegionForThread, m_FaceList);


  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  typename TInputImage::IndexType     bitIndex;
  typename InterpolatorType::ContinuousIndexType Index;


  // --------------------------------------------------------------------------
  
  // Number of direction
  const int NB_DIR = 4;
  // Number of zone	  
  const int NB_ZONE = 3;  
  // Definition of the 4 directions
  double Theta[NB_DIR];
  
  Theta[0] = 0.;
  Theta[1] = M_PI / 4. ;
  Theta[2] = M_PI / 2. ;
  Theta[3] = 3*M_PI / 4. ;

  // Number of the zone 
  unsigned int zone;
  
  // Pixel numbers in each zone
  const int NbPixelZone = (2*m_WidthLine+1)*(2*m_LengthLine+1);
  
  double InvNbPixel = static_cast<double>(1. / NbPixelZone); 
   
  // Contains for the 4 directions the sum of the pixels belonging to each zone
  double Sum[NB_DIR][NB_ZONE];
  double Sum2[NB_DIR][NB_ZONE];
  
  // Mean of zone 1, 2 and 3
  double M1, M2, M3;
  
  // Standard deviation
  double sigma1, sigma2, sigma3;
  
  // Result of the filter for each direction
  double rho12_theta[NB_DIR];
  double rho13_theta[NB_DIR];
  // Result for each direction
  double rho_theta[NB_DIR];
  double Sum_rho_theta = 0.;
  // Intensity of the linear feature
  double rho;
  
  // Direction of detection
  double Direction = 0.; 

  // Pixel location in the input image
  int X, Y;
  // Value of the pixel
  double ValXY = 0.;
  
  // Pixel location after rotation in the system axis of the region  
  double xout, yout;
  
   // Pixel location in the input image after rotation
  TPoint point;
   
  // location of the central pixel in the input image
  int Xc, Yc;

  // location of the central pixel between zone 1 and 2 and between zone 1 and 3
  int Xc12, Xc13;
  
  //---------------------------------------------------------------------------
 
  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit=faceList.begin(); fit != faceList.end(); ++fit)
    { 
    bit = itk::ConstNeighborhoodIterator<InputImageType>(m_Radius, input, *fit);
    unsigned int neighborhoodSize = bit.Size();
      
    it = itk::ImageRegionIterator<OutputImageType>(output, *fit);
    itdir = itk::ImageRegionIterator<OutputImageType>(outputDir, *fit);
    
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();


    while ( ! bit.IsAtEnd() )
      {
//std::cout << "Xc,Yc " << bit.GetIndex() << std::endl;

      // Initialisations
      for (unsigned int dir=0; dir<NB_DIR; dir++)
        {
        for (unsigned int z=0; z<NB_ZONE; z++)
          { 
          Sum[dir][z] = 0.;
          Sum2[dir][z] = 0.;
          }
        }
        
     
      // Location of the central pixel of the region
      bitIndex = bit.GetIndex();
      
      Xc = bitIndex[0];
      Yc = bitIndex[1];
      
      // Location of the central pixel between zone 1 and zone 2
      Xc12 = Xc - m_WidthLine - 1;
      
      // Location of the central pixel between zone 1 and zone 3
      Xc13 = Xc + m_WidthLine + 1;
          
      // Loop on the region 
      for (i = 0; i < neighborhoodSize; ++i)
        {
//std::cout << "---"<< i <<" "<< bit.GetIndex(i)<< std::endl;
//std::cout << "val(X,Y) "<< static_cast<double>(bit.GetPixel(i)) << " " << std::endl;

        bitIndex = bit.GetIndex(i);
        X = bitIndex[0];
        Y = bitIndex[1];

      
        // We determine in the vertical direction with which zone the pixel belongs. 
         
        if ( X < Xc12 )
      	  zone = 1;
        else if ( ( Xc12 < X ) && ( X < Xc13 ) )
          zone = 0;
        else if ( X > Xc13 )
          zone = 2;
        else
          continue;
    
        ValXY = static_cast<double>(bit.GetPixel(i));
        
        Sum[0][zone] += ValXY;
        Sum2[0][zone] += ValXY*ValXY;
         
        // Loop on the 3 other directions
        for (unsigned int dir=1; dir<NB_DIR; dir++ )
          {      
            ROTATION( (X-Xc), (Y-Yc), Theta[dir], xout, yout);
            
            Index[0] = static_cast<CoordRepType>(xout + Xc);
            Index[1] = static_cast<CoordRepType>(yout + Yc);
                        
//std::cout << "X' Y' "<< (xout + Xc) << " " << (yout + Yc) << std::endl;
//std::cout << "val(X',Y') "<< static_cast<double>(m_Interpolator->EvaluateAtContinuousIndex( Index )) << std::endl;
           
            ValXY = static_cast<double>(m_Interpolator->EvaluateAtContinuousIndex( Index ));
             
            Sum[dir][zone] += ValXY;
            Sum2[dir][zone] += ValXY*ValXY;
          
          }      

        } // end of the loop on the pixels of the region 
        
      rho = -1.;
      Direction = 0.;
      Sum_rho_theta = 0.;
      
      double NbPixel = static_cast<double>(NbPixelZone);          
           
      // Loop on the 4 directions
      for (unsigned int dir=0; dir<NB_DIR; dir++ )
        {
        		
        // Calculation of the mean for the 3 zones	
        M1 = Sum[dir][0] * InvNbPixel;
        M2 = Sum[dir][1] * InvNbPixel;
        M3 = Sum[dir][2] * InvNbPixel;
        
        // Calculation of the standard deviation for the 3 zones (standard deviation = sqrt(sigma)) 
	sigma1 = (Sum2[dir][0] - NbPixel*M1*M1) * InvNbPixel;
	sigma2 = (Sum2[dir][1] - NbPixel*M2*M2) * InvNbPixel;
	sigma3 = (Sum2[dir][2] - NbPixel*M3*M3) * InvNbPixel; 
     	
        // Calculation of the cross correlation coefficient
        
        double d1 = 0.;
        double d2 = 0.;
        double d3 = 0.;
        
        // rho12
        if ( M2 != 0. )
          d1 = NbPixel*sigma1*pow(M1/M2,2);
        d2 = NbPixel*sigma2;
        if ( M2 != 0. )
          d3 = NbPixel*NbPixel*pow(((M1/M2)-1.),2);
        
        if ( ( d3 != 0. ) )
          rho12_theta[dir] = static_cast<double>( 1. / ( 1. + ( 2.*NbPixel*(d1+d2)/d3 ) ) );
        else
          rho12_theta[dir] = 0.;
	 
	// rho13 
	if ( M3 != 0. )
	  d1 = NbPixel*sigma1*pow(M1/M3,2);
        d2 = NbPixel*sigma3;
        if ( M3 != 0. )
          d3 = NbPixel*NbPixel*pow(((M1/M3)-1.),2);
      
        if ( ( d3 != 0. ) )
          rho13_theta[dir] = static_cast<double>( 1. / ( 1. + ( 2.*NbPixel*(d1+d2)/d3 ) ) );
        else
          rho13_theta[dir] = 0.;
        
         
	// Determination of the minimum intensity of detection between R12 et R13
	rho_theta[dir] = static_cast<double>( MIN( rho12_theta[dir], rho13_theta[dir] ) );
	
	rho = static_cast<double>( MAX ( rho, rho_theta[dir] ) );
                
        // Calculation of the directions
        Direction += Theta[dir] * rho_theta[dir];        
        Sum_rho_theta += rho_theta[dir];
	        		
        } // end of the loop on the directions
      
      // Determination of the direction of the contour
      if ( Sum_rho_theta != 0. )	
        Direction = Direction / Sum_rho_theta;	

      // Assignment of this value to the output pixel
      it.Set( static_cast<OutputPixelType>(rho) );  

      // Assignment of this value to the "outputdir" pixel
      itdir.Set( static_cast<OutputPixelType>(Direction) ); 
      
      ++bit;
      ++it;
      ++itdir;
      progress.CompletedPixel();  
      
      }
      
    }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput, class InterpolatorType>
void 
LineCorrelationDetector<TInputImage, TOutput, InterpolatorType>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Length: " << m_LengthLine << std::endl;
  os << indent << "Width: " << m_WidthLine << std::endl;
}


} // end namespace otb


#endif
