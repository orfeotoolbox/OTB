/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CNES - J.Inglada Rodriguez
  Language  :   C++
  Date      :   07 mars 2006
  Version   :   
  Role      :   Classe qui met en application l'opération générique 
  		voisinage-sage de deux images (Join Histogram)
  $Id: $

=========================================================================*/
#ifndef _otbBinaryFunctorNeighborhoodJoinHistogramImageFilter_txx
#define _otbBinaryFunctorNeighborhoodJoinHistogramImageFilter_txx

#include "otbBinaryFunctorNeighborhoodJoinHistogramImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage1, class TInputImage2, 
          class TOutputImage, class TFunction  >
BinaryFunctorNeighborhoodJoinHistogramImageFilter<TInputImage1,TInputImage2,TOutputImage,TFunction>
::BinaryFunctorNeighborhoodJoinHistogramImageFilter()
{
  this->SetNumberOfRequiredInputs( 2 );
  this->InPlaceOff();
  m_Radius = 3;
  m_HistogramSize.Fill(256);
  m_UsePaddingValue = false;
   m_UpperBoundIncreaseFactor = 0.001;
}


/**
 * Connect one of the operands for neighborhood-wise operation
 */
template <class TInputImage1, class TInputImage2, 
          class TOutputImage, class TFunction  >
void
BinaryFunctorNeighborhoodJoinHistogramImageFilter<TInputImage1,TInputImage2,TOutputImage,TFunction>
::SetInput1( const TInputImage1 * image1 ) 
{
  // Process object is not const-correct so the const casting is required.
  SetNthInput(0, const_cast<TInputImage1 *>( image1 ));
}


/**
 * Connect one of the operands for neighborhood-wise operation
 */
template <class TInputImage1, class TInputImage2, 
          class TOutputImage, class TFunction  >
void
BinaryFunctorNeighborhoodJoinHistogramImageFilter<TInputImage1,TInputImage2,TOutputImage,TFunction>
::SetInput2( const TInputImage2 * image2 ) 
{
  // Process object is not const-correct so the const casting is required.
  SetNthInput(1, const_cast<TInputImage2 *>( image2 ));
}

/**
 * Initialize the histogram
 */
template <class TInputImage1, class TInputImage2, 
          class TOutputImage, class TFunction  >
void
BinaryFunctorNeighborhoodJoinHistogramImageFilter<TInputImage1,TInputImage2,TOutputImage,TFunction>
::Initialize( ) 
{
  // Calculate min and max image values in input1 image.
  Input1ImagePointer pInput1Image
    = dynamic_cast<const TInputImage1*>(itk::ProcessObject::GetInput(0));

  Input1ImagePixelType minInput1, maxInput1;
  itk::ImageRegionConstIterator<Input1ImageType> fiIt(pInput1Image,
                                                pInput1Image->GetBufferedRegion());
  fiIt.GoToBegin();
  minInput1 = fiIt.Value();
  maxInput1 = fiIt.Value();
  ++fiIt;
  while (!fiIt.IsAtEnd())
    {
    Input1ImagePixelType value = fiIt.Value();
      
    if (value < minInput1)
      {
      minInput1 = value;
      }
    else if (value > maxInput1)
      {
      maxInput1 = value;
      }

    ++fiIt;
    }
    
  // Calculate min and max image values in input2 image.
    Input2ImagePointer pInput2Image
    = dynamic_cast<const TInputImage2*>(itk::ProcessObject::GetInput(1));
  Input2ImagePixelType minInput2, maxInput2;
  itk::ImageRegionConstIterator<Input2ImageType> miIt(pInput2Image,
                                                 pInput2Image->GetBufferedRegion());
  miIt.GoToBegin();
  minInput2 = miIt.Value();
  maxInput2 = miIt.Value();
  ++miIt;
  while (!miIt.IsAtEnd())
    {
    Input2ImagePixelType value = miIt.Value();

    if (value < minInput2)
      {
      minInput2 = value;
      }
    else if (value > maxInput2)
      {
      maxInput2 = value;
      }
    ++miIt;
    }

  // Initialize the upper and lower bounds of the histogram.
  m_LowerBound[0] = minInput1;
  m_LowerBound[1] = minInput2;
  m_UpperBound[0] =
    maxInput1 + (maxInput1 - minInput1 ) * m_UpperBoundIncreaseFactor;
  m_UpperBound[1] =
    maxInput2 + (maxInput2 - minInput2 ) * m_UpperBoundIncreaseFactor;



  typedef itk::ImageRegionConstIteratorWithIndex<Input1ImageType>    Input1IteratorType;
    typedef itk::ImageRegionConstIteratorWithIndex<Input2ImageType>    Input2IteratorType;

  typename Input1ImageType::RegionType input1Region;
  typename Input1ImageType::RegionType input2Region;

  input1Region = pInput1Image->GetRequestedRegion();
  Input1IteratorType ti1(pInput1Image, input1Region);

  input2Region = pInput2Image->GetRequestedRegion();
  Input2IteratorType ti2(pInput2Image, input2Region);

  m_Histogram = HistogramType::New();
    
  m_Histogram->Initialize(m_HistogramSize, m_LowerBound, m_UpperBound);
    
  ti1.GoToBegin();
  ti2.GoToBegin();
  while (!ti1.IsAtEnd() && !ti2.IsAtEnd())
    {

          
        typename HistogramType::MeasurementVectorType sample;
        sample[0] = ti1.Get();
        sample[1] = ti2.Get();
	if(sample[0]!=itk::NumericTraits<Input1ImagePixelType>::Zero &&
	   sample[1]!=itk::NumericTraits<Input2ImagePixelType>::Zero)
	  m_Histogram->IncreaseFrequency(sample, 1);
      
    ++ti1;
    ++ti2;
    }

}



/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TInputImage1, class TInputImage2, class TOutputImage, class TFunction  >
void
BinaryFunctorNeighborhoodJoinHistogramImageFilter<TInputImage1, TInputImage2, TOutputImage, TFunction>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        int threadId)
{

  this->Initialize();
  //m_Functor->SetHistogram(m_Histogram);
  unsigned int i;
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage1> nbc1;
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage2> nbc2;

// We use dynamic_cast since inputs are stored as DataObjects.  The
  // ImageToJoinHistogramImageFilter::GetInput(int) always returns a pointer to a
  // TInputImage1 so it cannot be used for the second input.
  Input1ImagePointer inputPtr1
    = dynamic_cast<const TInputImage1*>(itk::ProcessObject::GetInput(0));
  Input2ImagePointer inputPtr2
    = dynamic_cast<const TInputImage2*>(itk::ProcessObject::GetInput(1));
  OutputImagePointer outputPtr = this->GetOutput(0);
  
  
  RadiusType1 r1;
  r1.Fill(m_Radius);
  NeighborhoodIteratorType1 neighInputIt1;

  RadiusType2 r2;
  r2.Fill(m_Radius);
  NeighborhoodIteratorType2 neighInputIt2;
    
  itk::ImageRegionIterator<TOutputImage> outputIt;


  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage1>::FaceListType faceList1;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage1> bC1;
  faceList1 = bC1(inputPtr1, outputRegionForThread, r1);

  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage2>::FaceListType faceList2;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage2> bC2;
  faceList2 = bC2(inputPtr2, outputRegionForThread, r2);

  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage1>::FaceListType::iterator fit1;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage2>::FaceListType::iterator fit2;

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit1=faceList1.begin(), fit2=faceList2.begin(); fit1 != faceList1.end(), fit2 != faceList2.end(); ++fit1, ++fit2)
    { 
    neighInputIt1 = itk::ConstNeighborhoodIterator<TInputImage1>(r1, inputPtr1, *fit1);
    neighInputIt2 = itk::ConstNeighborhoodIterator<TInputImage2>(r1, inputPtr2, *fit2);
    outputIt = itk::ImageRegionIterator<TOutputImage>(outputPtr, outputRegionForThread);
      
    outputIt = itk::ImageRegionIterator<TOutputImage>(outputPtr, *fit1);
    neighInputIt1.OverrideBoundaryCondition(&nbc1);
    neighInputIt1.GoToBegin();
    neighInputIt2.OverrideBoundaryCondition(&nbc2);
    neighInputIt2.GoToBegin();

    while ( ! outputIt.IsAtEnd() )
      {

      outputIt.Set( m_Functor( neighInputIt1, neighInputIt2, m_Histogram ) );

      ++neighInputIt1;
      ++neighInputIt2;
      ++outputIt;
      progress.CompletedPixel();
      }
    }


  

}

} // end namespace otb

#endif
