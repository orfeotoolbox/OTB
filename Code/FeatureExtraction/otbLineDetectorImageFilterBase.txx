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
#ifndef __otbLineDetectorImageFilterBase_txx
#define __otbLineDetectorImageFilterBase_txx

#include "otbLineDetectorImageFilterBase.h"

#include "itkDataObject.h"
#include "itkExceptionObject.h"
#include "itkConstantPadImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkProgressReporter.h"
#include <math.h>
#include "otbMacro.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class InterpolatorType >
LineDetectorImageFilterBase<TInputImage, TOutputImage, InterpolatorType>::LineDetectorImageFilterBase()
{
  m_Radius.Fill(1);
  m_LengthLine = 1;
  m_WidthLine = 0;
  m_Threshold = 0;
  m_NumberOfDirections = 4;
  m_FaceList.Fill(0);
  m_OutputDirection = OutputImageType::New();
  // m_PadLowerBound[0]=0;
//   m_PadUpperBound[0]=0;
//   m_PadLowerBound[1]=0;
//   m_PadUpperBound[1]=0;
}

template <class TInputImage, class TOutputImage, class InterpolatorType>
void LineDetectorImageFilterBase<TInputImage, TOutputImage, InterpolatorType>::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
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
  otbMsgDevMacro(<<"GenerateInputRequestedRegion(): Original input requested region: "<<inputRequestedRegion.GetIndex()<<" "<<inputRequestedRegion.GetSize());


  // Define the size of the region by the radius
  m_Radius[1] = static_cast<unsigned int>(3*(2*m_WidthLine+1) + 2); 
  m_Radius[0] = 2*m_LengthLine+1 ;

  // Define the size of the facelist by taking into account the rotation of the region
  m_FaceList[0] = static_cast<unsigned int>( sqrt( static_cast<double>((m_Radius[0]*m_Radius[0]) + (m_Radius[1]*m_Radius[1]) ) ) + 1 );
  m_FaceList[1] = m_FaceList[0];
  
  otbMsgDevMacro( << "Radius   : "<<m_Radius[0]<<" "<<m_Radius[1]);
  otbMsgDevMacro( << "-> FaceList : "<<m_FaceList[0]<<" "<<m_FaceList[1]);

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( m_FaceList );
  otbMsgDevMacro(<<"GenerateInputRequestedRegion(): padded input requested region: "<<inputRequestedRegion.GetIndex()<<" "<<inputRequestedRegion.GetSize());
 
  // typename TInputImage::IndexType minPad;
//   typename TInputImage::IndexType maxPad;
//   typename TInputImage::IndexType min;
//   typename TInputImage::IndexType max;
  
//   minPad = inputRequestedRegion.GetIndex();
//   min = inputPtr->GetLargestPossibleRegion().GetIndex();
  
//   maxPad[0]=inputRequestedRegion.GetIndex()[0]+inputRequestedRegion.GetSize()[0];  
//   maxPad[1]=inputRequestedRegion.GetIndex()[1]+inputRequestedRegion.GetSize()[1];
//   max[0]=inputPtr->GetLargestPossibleRegion().GetIndex()[0]+inputPtr->GetLargestPossibleRegion().GetSize()[0];  
//   max[1]=inputPtr->GetLargestPossibleRegion().GetIndex()[1]+inputPtr->GetLargestPossibleRegion().GetSize()[1];

//   m_PadLowerBound[0]=( minPad[0]<min[0] ? min[0]-minPad[0]+10 : 0);
//   m_PadLowerBound[1]=( minPad[1]<min[1] ? min[1]-minPad[1]+10 : 0);
//   m_PadUpperBound[0]=( maxPad[0]>max[0] ? maxPad[0]-max[0]+10 : 0);
//   m_PadUpperBound[1]=( maxPad[1]>max[1] ? maxPad[1]-max[1]+10 : 0);

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
      otbMsgDevMacro(<<"GenerateInputRequestedRegion(): cropped input requested region: "<<inputRequestedRegion.GetIndex()<<" "<<inputRequestedRegion.GetSize());
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

/*
 * Set up state of filter before multi-threading.
 * InterpolatorType::SetInputImage is not thread-safe and hence
 * has to be set up before ThreadedGenerateData
 */
template <class TInputImage, class TOutputImage, class InterpolatorType>
void 
LineDetectorImageFilterBase< TInputImage, TOutputImage, InterpolatorType>
::BeforeThreadedGenerateData()
{

  typename OutputImageType::RegionType  region;    
  typename OutputImageType::Pointer     output = this->GetOutput();
  output->FillBuffer(0);


  region.SetSize(output->GetLargestPossibleRegion().GetSize());
  region.SetIndex(output->GetLargestPossibleRegion().GetIndex());
  m_OutputDirection->SetRegions( region );
  m_OutputDirection->SetOrigin(output->GetOrigin());
  m_OutputDirection->SetSpacing(output->GetSpacing());
  m_OutputDirection->Allocate();
  m_OutputDirection->FillBuffer(0);




}

template <class TInputImage, class TOutputImage, class InterpolatorType>
const typename LineDetectorImageFilterBase< TInputImage, TOutputImage, InterpolatorType>::OutputImageType *
LineDetectorImageFilterBase< TInputImage, TOutputImage, InterpolatorType>
::GetOutputDirection()
{
  this->Update();
  return 	static_cast< const OutputImageType *> (m_OutputDirection);
}

template< class TInputImage, class TOutputImage, class InterpolatorType>
void LineDetectorImageFilterBase< TInputImage, TOutputImage, InterpolatorType>
::ThreadedGenerateData(	
			const 	OutputImageRegionType& 		outputRegionForThread,
                       	int 	threadId
		       )
{
//   typename InputImageType::ConstPointer inputPtr  = this->GetInput();
//   typedef itk::ConstantPadImageFilter<InputImageType,InputImageType> PadFilterType;
//   typename PadFilterType::Pointer pad = PadFilterType::New();
//   pad->SetInput(inputPtr);
//   pad->SetConstant(static_cast<InputPixelType>(0));
//   pad->SetPadLowerBound(m_PadLowerBound);
//   pad->SetPadUpperBound(m_PadUpperBound);
//   pad->Update();
//   typename InputImageType::ConstPointer paddedInput  = pad->GetOutput();
  typename InputImageType::ConstPointer input  = this->GetInput();

 //  otbMsgDevMacro(<<"Padded input: "<<input->GetLargestPossibleRegion().GetIndex()
// 		 <<" "<<input->GetLargestPossibleRegion().GetSize());


  InterpolatorPointer interpolator2 = InterpolatorType::New();
  interpolator2->SetInputImage(input);
 
  
  itk::ZeroFluxNeumannBoundaryCondition<InputImageType> 	nbc;
  itk::ConstNeighborhoodIterator<InputImageType> 		bit,cit;
  typename itk::ConstNeighborhoodIterator<InputImageType>::OffsetType	off;
  itk::ImageRegionIterator<OutputImageType> 			it;
  itk::ImageRegionIterator<OutputImageType> 			itdir;
  typedef  itk::ImageRegionIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionConstIterator<InputImageType> ConstInputIteratorType;

  // Allocate output
  typename OutputImageType::Pointer     output = this->GetOutput();  
  typename OutputImageType::Pointer     outputDir = m_OutputDirection;
  

   
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
  const int NB_DIR = this->GetNumberOfDirections();
  // Number of zone	  
  const int NB_ZONE = 3;  
  // Definition of the 4 directions
  //double Theta[NB_DIR];
  //ROMAIN
  double* Theta = new double[NB_DIR];

  // La rotation nulle correspond a un contour horizontal -> 0 !!
  for(int i=0; i<NB_DIR; i++)
    {
    Theta[i] = (M_PI*(i/double(NB_DIR)));
/*    if(Theta[i]>M_PI)
      Theta[i] = Theta[i]-M_PI;
    if((i/double(NB_DIR))==0.5)
      Theta[i]=0.;*/
    }

  // Number of the zone 
  unsigned int zone;
  
  
  // Intensity of the linear feature
  double R;
  
  // Direction of detection
  double Direction = 0.; 

  // Pixel location in the input image
  int X, Y;
  
  // Pixel location after rotation in the system axis of the region  
  double xout, yout;
  
  // location of the central pixel in the input image
  int Xc, Yc;

  // location of the central pixel between zone 1 and 2 and between zone 1 and 3
  int Yc12, Yc13;
  
  //---------------------------------------------------------------------------
  otbMsgDevMacro( << "Theta    : "<<Theta[0]<<" "<<Theta[1]<<" "<<Theta[2]<<" "<<Theta[3]);

 
  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit=faceList.begin(); fit != faceList.end(); ++fit)
    { 


       

       // otbMsgDevMacro(<<"Region: "<<(*fit).GetIndex()<<" "<<(*fit).GetSize());



	
    bit = itk::ConstNeighborhoodIterator<InputImageType>(m_Radius, input, *fit);
    cit = itk::ConstNeighborhoodIterator<InputImageType>(m_FaceList, input,*fit);
    unsigned int neighborhoodSize = bit.Size();
//     typename InputImageType::IndexType min, max;

    // min[0]=inputPtr->GetLargestPossibleRegion().GetIndex()[0];  
//     min[1]=inputPtr->GetLargestPossibleRegion().GetIndex()[1];
//     max[0]=inputPtr->GetLargestPossibleRegion().GetIndex()[0]+inputPtr->GetLargestPossibleRegion().GetSize()[0];  
//     max[1]=inputPtr->GetLargestPossibleRegion().GetIndex()[1]+inputPtr->GetLargestPossibleRegion().GetSize()[1];
    
    it = itk::ImageRegionIterator<OutputImageType>(output, *fit);
    itdir = itk::ImageRegionIterator<OutputImageType>(outputDir, *fit);
    
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();
    cit.OverrideBoundaryCondition(&nbc);
    cit.GoToBegin();

  otbMsgDevMacro( << " ------------------- FaceList --------------------------");


    while ( (!bit.IsAtEnd())&&(!cit.IsAtEnd()) )
      {
	
	
      // Location of the central pixel of the region
      off.Fill(0);
      bitIndex = bit.GetIndex(off);
      
      typename InputImageType::RegionType tempRegion;
      typename InputImageType::SizeType tempSize;
      tempSize[0] = 2*m_FaceList[0]+1;
      tempSize[1] = 2*m_FaceList[1]+1;
      tempRegion.SetSize(tempSize);
      typename itk::ConstNeighborhoodIterator<InputImageType>::OffsetType tempIndex;
      tempIndex[0]=off[0]-m_FaceList[0];
      tempIndex[1]=off[1]-m_FaceList[1];
      tempRegion.SetIndex(cit.GetIndex(tempIndex));
      
      // otbMsgDevMacro(<<"TempRegion: index: "<<cit.GetIndex(tempIndex)<<" size: "<<tempSize);

      typename InputImageType::Pointer tempImage = InputImageType::New();
      tempImage->SetRegions(tempRegion);
      tempImage->Allocate();
      
      for(int p = 0; p<=2*m_FaceList[0];p++)
	{
	  for(int q = 0; q<=2*m_FaceList[1];q++)
	    {
	      typename itk::ConstNeighborhoodIterator<InputImageType>::OffsetType  index;
	      index[0]=p-m_FaceList[0];
	      index[1]=q-m_FaceList[1];
	      if(cit.GetPixel(index)!=input->GetPixel(cit.GetIndex(index))&&(*fit).IsInside(cit.GetIndex(index)))
  		{
  	      otbMsgDebugMacro(<<"Iterator Index: "<<index
  			       <<" Iterator image value: "<<cit.GetIndex(index)<<"-> "<<static_cast<unsigned int>(cit.GetPixel(index))
  	      <<" Original image value: "<<cit.GetIndex(index)<<" -> "<<static_cast<unsigned int>(input->GetPixel(cit.GetIndex(index))));
 	    }
	      tempImage->SetPixel(cit.GetIndex(index),cit.GetPixel(index));	      
		
	    }
	}
      
      InterpolatorPointer interpolator = InterpolatorType::New();
      interpolator->SetInputImage(tempImage);
	
      Xc = bitIndex[0];
      Yc = bitIndex[1];


            
      // Location of the central pixel between zone 1 and zone 2
      Yc12 = Yc - m_WidthLine - 1;
      
      // Location of the central pixel between zone 1 and zone 3
      Yc13 = Yc + m_WidthLine + 1;


      // Contains for the 4 directions the the pixels belonging to each zone
	  //std::vector<double> PixelValues[NB_DIR][NB_ZONE];
	  // ROMAIN
        std::vector<double>** PixelValues = NULL;
        PixelValues = new std::vector<double>*[NB_DIR];
	for (int i=0; i<NB_DIR; i++)
        {
		PixelValues[i] = NULL;
                PixelValues[i] = new std::vector<double>[NB_ZONE];
        }
	//otbMsgDevMacro( << "\tCentre Xc/Yc="<<Xc<<" "<<Yc<<" Yc12/Yc13="<<Yc12<<" "<<Yc13);          

      // Loop on the region 
      for (int i = 0; i < m_Radius[0]; i++)
	for (int j = 0; j < m_Radius[1]; j++)
        {

	off[0]=i-m_Radius[0]/2;
	off[1]=j-m_Radius[1]/2;
	
        bitIndex = bit.GetIndex(off);
        X = bitIndex[0];
        Y = bitIndex[1];
	


	// We determine in the horizontal direction with which zone the pixel belongs. 
         
        if ( Y < Yc12 )
      	  zone = 1;
        else if ( ( Yc12 < Y ) && ( Y < Yc13 ) )
          zone = 0;
        else if ( Y > Yc13 )
          zone = 2;
        else
          continue;

                 
        //otbMsgDevMacro( << "\t\tPoint traite (i,j)=("<<i<<","<<j<<") -> X,Y="<<X<<","<<Y<<"  zone="<<zone);
        // Loop on the directions
        for (unsigned int dir=0; dir<NB_DIR; dir++ )
          {      
	  //ROTATION( (X-Xc), (Y-Yc), Theta[dir], xout, yout);

	  xout = (X-Xc)*cos(Theta[dir]) - (Y-Yc)*sin(Theta[dir]);
	  yout = (X-Xc)*sin(Theta[dir]) + (Y-Yc)*cos(Theta[dir]);

            
	  Index[0] = static_cast<CoordRepType>(xout + Xc);
	  Index[1] = static_cast<CoordRepType>(yout + Yc);
	  
	 //  if(  Index[0]<min[0]
// 	     ||Index[1]<min[1]
// 	     ||Index[0]>max[0]
// 	     ||Index[1]>max[1])
// 	    {

// 	      PixelValues[dir][zone].push_back(static_cast<double>(0));
// 	    }
// 	  else
// 	    {
 	      PixelValues[dir][zone].push_back(static_cast<double>(interpolator->EvaluateAtContinuousIndex( Index )));
// 	    }

	      if(interpolator->EvaluateAtContinuousIndex( Index )!=interpolator2->EvaluateAtContinuousIndex(Index)
	      &&(*fit).IsInside(Index))
 		 otbMsgDevMacro(<<"Index: "<<Index<<" specific interpolator: "<<interpolator->EvaluateAtContinuousIndex( Index )
  			       <<" global interpolator: "<<interpolator2->EvaluateAtContinuousIndex( Index ));



  	  // if(Index[0]<input->GetLargestPossibleRegion().GetIndex()[0])
  	    // otbMsgDevMacro( << "\t\t\tInterpole : direction "<<dir<<" out(x,y)=("<<xout<<","<<yout<<") Index : "<<Index[0]<<" "<<Index[1]
//   			    <<" Interpolated value: "<<PixelValues[dir][zone].back());
	  }     

        } // end of the loop on the pixels of the region 
          
      R = 0.;
      Direction = 0.;
           
      // Loop on the 4 directions


	for (unsigned int dir=0; dir<NB_DIR; dir++ )
	  {
	  

	  double Rtemp = this->ComputeMeasure(&PixelValues[dir][0], &PixelValues[dir][1], &PixelValues[dir][2]);
	  
	  if( Rtemp > R)
	    {
	    R = Rtemp;
	    Direction = Theta[dir];
	    }
	  
	  } // end of the loop on the directions

        //otbMsgDevMacro( << "\t\tR,Direction : "<<R<<","<<Direction);	  
	if( R >= this->GetThreshold() )
	  {
      
	  // Assignment of this value to the output pixel
	  it.Set( static_cast<OutputPixelType>(R) );
	  
	  // Assignment of this value to the "outputdir" pixel
	  itdir.Set( static_cast<OutputPixelType>(Direction) );  
	  }
	else
	  {
	  
	  it.Set( itk::NumericTraits<OutputPixelType>::Zero );
	  
	  itdir.Set( static_cast<OutputPixelType>(0) );  
	  }
	
	++bit;
	++cit;
	++it;
	++itdir;
	progress.CompletedPixel();  

	// ROMAIN
	for (int i=0; i<NB_DIR; i++)
        {
		delete[] PixelValues[i];
                PixelValues[i] = NULL;
        }
	delete[] PixelValues;
        PixelValues = NULL;
    }
    
    }
	delete[] Theta;
}

template <class TInputImage, class TOutput, class InterpolatorType>
double LineDetectorImageFilterBase<TInputImage, TOutput, InterpolatorType>::ComputeMeasure(std::vector<double>* m1, std::vector<double>* m2, std::vector<double>* m3)
{
  return 0;
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput, class InterpolatorType>
void 
LineDetectorImageFilterBase<TInputImage, TOutput, InterpolatorType>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Length: " << m_LengthLine << std::endl;
  os << indent << "Width: " << m_WidthLine << std::endl;
  
}


} // end namespace otb


#endif
