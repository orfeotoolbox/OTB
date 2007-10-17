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
#ifndef _otbStreamingStatisticsVectorImageFilter_txx
#define _otbStreamingStatisticsVectorImageFilter_txx
#include "otbStreamingStatisticsVectorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"


namespace otb {

template<class TInputImage>
StreamingStatisticsVectorImageFilter<TInputImage>
::StreamingStatisticsVectorImageFilter()
{
	
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around pixel types
  
  for (int i=1; i < 3; ++i)
    {
    typename PixelObjectType::Pointer output = static_cast<PixelObjectType*>(this->MakeOutput(i).GetPointer());
    this->itk::ProcessObject::SetNthOutput(i, output.GetPointer());
    }
  
  // allocate the data objects for the outputs which are
  // just decorators around real types
  
  for (int i=3; i < 5; ++i)
    {
    typename RealPixelObjectType::Pointer output = static_cast<RealPixelObjectType*>(this->MakeOutput(i).GetPointer());
    this->itk::ProcessObject::SetNthOutput(i, output.GetPointer());
    }
 
  // allocate the data objects for the outputs which are
  // just decorators around matrix types
  typename MatrixObjectType::Pointer output = static_cast<MatrixObjectType*>(this->MakeOutput(5).GetPointer());
  this->itk::ProcessObject::SetNthOutput(5, output.GetPointer());

  // Streaming initialization
  m_BufferMemorySize = 0;
  m_BufferNumberOfLinesDivisions = 0;
  m_NumberOfStreamDivisions = 0;
  // default to AUTOMATIC_NUMBER_OF_DIVISIONS
  m_StreamingMode = SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS;
  
  // create default region splitter
  m_RegionSplitter = itk::ImageRegionSplitter<InputImageDimension>::New();
  
}


template<class TInputImage>
itk::DataObject::Pointer
StreamingStatisticsVectorImageFilter<TInputImage>
::MakeOutput(unsigned int output)
{
  switch (output)
    {
   case 0:
     return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
    case 2:
      return static_cast<itk::DataObject*>(PixelObjectType::New().GetPointer());
      break;
    case 3:
    case 4:
      return static_cast<itk::DataObject*>(RealPixelObjectType::New().GetPointer());
      break;
    case 5:
      return static_cast<itk::DataObject*>(MatrixObjectType::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }
    
}


template<class TInputImage>
typename StreamingStatisticsVectorImageFilter<TInputImage>::PixelObjectType*
StreamingStatisticsVectorImageFilter<TInputImage>
::GetMinimumOutput()
{
  return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
const typename StreamingStatisticsVectorImageFilter<TInputImage>::PixelObjectType*
StreamingStatisticsVectorImageFilter<TInputImage>
::GetMinimumOutput() const
{
  return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
}


template<class TInputImage>
typename StreamingStatisticsVectorImageFilter<TInputImage>::PixelObjectType*
StreamingStatisticsVectorImageFilter<TInputImage>
::GetMaximumOutput()
{
  return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
const typename StreamingStatisticsVectorImageFilter<TInputImage>::PixelObjectType*
StreamingStatisticsVectorImageFilter<TInputImage>
::GetMaximumOutput() const
{
  return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
}


template<class TInputImage>
typename StreamingStatisticsVectorImageFilter<TInputImage>::RealPixelObjectType*
StreamingStatisticsVectorImageFilter<TInputImage>
::GetMeanOutput()
{
  return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage>
const typename StreamingStatisticsVectorImageFilter<TInputImage>::RealPixelObjectType*
StreamingStatisticsVectorImageFilter<TInputImage>
::GetMeanOutput() const
{
  return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
}

template<class TInputImage>
typename StreamingStatisticsVectorImageFilter<TInputImage>::RealPixelObjectType*
StreamingStatisticsVectorImageFilter<TInputImage>
::GetSumOutput()
{
  return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(4));
}

template<class TInputImage>
const typename StreamingStatisticsVectorImageFilter<TInputImage>::RealPixelObjectType*
StreamingStatisticsVectorImageFilter<TInputImage>
::GetSumOutput() const
{
  return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(4));
}

template<class TInputImage>
typename StreamingStatisticsVectorImageFilter<TInputImage>::MatrixObjectType*
StreamingStatisticsVectorImageFilter<TInputImage>
::GetCovarianceOutput()
{
  return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(5));
}

template<class TInputImage>
const typename StreamingStatisticsVectorImageFilter<TInputImage>::MatrixObjectType*
StreamingStatisticsVectorImageFilter<TInputImage>
::GetCovarianceOutput() const
{
  return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(5));
}


template<class TInputImage>
void
StreamingStatisticsVectorImageFilter<TInputImage>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
  if ( this->GetInput() )
    {
      InputImagePointer image = const_cast< typename Superclass::InputImageType * >( this->GetInput() );
      IndexType index = image->GetLargestPossibleRegion().GetIndex();
      SizeType size;
      size.Fill(0);
      RegionType region;
      region.SetSize(size);
      region.SetIndex(index);
      image->SetRequestedRegion(region);
    }
}

template<class TInputImage>
void
StreamingStatisticsVectorImageFilter<TInputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if ( this->GetInput() )
    {
      this->GetOutput()->CopyInformation(this->GetInput());
      this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());
    }
}

template<class TInputImage>
void
StreamingStatisticsVectorImageFilter<TInputImage>
::AllocateOutputs()
{
  
  // Pass the input through as the output
  InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template<class TInputImage>
void
StreamingStatisticsVectorImageFilter<TInputImage>
::BeforeThreadedGenerateData()
{ 
  if(this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()==0)
    {
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
    }

  unsigned int numberOfThreads = this->GetNumberOfThreads();
  unsigned int numberOfComponent = this->GetInput()->GetNumberOfComponentsPerPixel();
  // Variable Initialisation
  PixelType tempPixel;
  tempPixel.SetSize(numberOfComponent);
  tempPixel.Fill(itk::NumericTraits<InternalPixelType>::Zero);
  this->GetMaximumOutput()->Set( tempPixel );

  tempPixel.Fill(itk::NumericTraits<InternalPixelType>::max());
  this->GetMinimumOutput()->Set( tempPixel );

  RealPixelType tempRealPixel;
  tempRealPixel.SetSize(numberOfComponent);
  tempRealPixel.Fill(itk::NumericTraits<RealType>::max());
  this->GetMeanOutput()->Set( tempRealPixel );

  tempRealPixel.Fill(itk::NumericTraits<RealType>::Zero);
  this->GetMeanOutput()->Set( tempRealPixel );

  MatrixType tempMatrix;
  tempMatrix.SetSize(numberOfComponent, numberOfComponent);
  tempMatrix.Fill(itk::NumericTraits<RealType>::Zero);


  // Initialize the tempories
  m_Count.SetSize(numberOfThreads);
  m_Count.Fill( itk::NumericTraits<long>::Zero );

  PixelType tempTemporiesPixel;
  tempTemporiesPixel.SetSize(numberOfComponent);
  tempTemporiesPixel.Fill(itk::NumericTraits<InternalPixelType>::max());
  m_ThreadMin = ArrayPixelType(numberOfThreads, tempTemporiesPixel);

  tempTemporiesPixel.Fill(itk::NumericTraits<InternalPixelType>::Zero);
  m_ThreadMax = ArrayPixelType(numberOfThreads, tempTemporiesPixel);

  RealPixelType tempTemporiesRealPixel;
  tempTemporiesRealPixel.SetSize(numberOfComponent);
  tempTemporiesRealPixel.Fill(itk::NumericTraits<RealType>::Zero);
  m_ThreadSum = ArrayRealPixelType(numberOfThreads, tempTemporiesRealPixel);

  m_XX = ArrayMatrixType(numberOfThreads, tempMatrix);

  otbMsgDevMacro(<<"BeforeThreadedGenerateData() - output requested region: "<<this->GetOutput()->GetRequestedRegion());
  otbMsgDevMacro(<<"Leaving BeforeThreadedGenerateData() - nb threads: "<<numberOfThreads);
}

template<class TInputImage>
void
StreamingStatisticsVectorImageFilter<TInputImage>
::AfterThreadedGenerateData()
{
  //otbMsgDebugMacro(<<"Entering AfterThreadedGenerateData()");
  int i;
  long count;
  
  int numberOfThreads = this->GetNumberOfThreads();
  unsigned int numberOfComponent = this->GetInput()->GetNumberOfComponentsPerPixel();
  
  PixelType minimumVector;
  minimumVector.SetSize( numberOfComponent );
  minimumVector.Fill(itk::NumericTraits<InternalPixelType>::max());

  PixelType maximumVector;
  maximumVector.SetSize( numberOfComponent );
  maximumVector.Fill(itk::NumericTraits<InternalPixelType>::NonpositiveMin());

  RealPixelType sumVector;
  sumVector.SetSize( numberOfComponent );
  sumVector.Fill(itk::NumericTraits<InternalPixelType>::Zero);

  RealPixelType meanVector;
  meanVector.SetSize( numberOfComponent );
  MatrixType crossedMatrix;
  crossedMatrix.SetSize(numberOfComponent,numberOfComponent);
  crossedMatrix.Fill(itk::NumericTraits<RealType>::Zero);
  count = 0;
   
  // Find the min/max over all threads and accumulate count, sum and
  // sum of squares 
  for( i = 0; i < numberOfThreads; i++)
    {
      count += m_Count[i];
      /** TODO
       *  To modify using + method operator. If we use it now -> exceptionmacro (no GetClassName...)
       * crossedMatrix += m_XX[i];
       **/
      if( (m_XX[i].Rows() != crossedMatrix.Rows()) || (m_XX[i].Cols() != crossedMatrix.Cols()))
	{ 
	  itkExceptionMacro( << "Matrix with size (" << m_XX[i].Rows() << "," << 
			     m_XX[i].Cols() << ") cannot be subtracted from matrix with size (" <<
			     crossedMatrix.Rows() << "," << crossedMatrix.Cols() );
	}
      
      for( unsigned int r=0; r<m_XX[i].Rows(); r++) 
	{
	  for( unsigned int c=0; c<m_XX[i].Cols(); c++ ) 
	    {
	     crossedMatrix(r,c) += m_XX[i](r,c);
	    }
	}
      //**** END TODO ****
  
      for (unsigned int j=0; j<numberOfComponent; j++)
	{
	  sumVector[j] += m_ThreadSum[i][j];
	  if (m_ThreadMin[i][j] < minimumVector[j])
	    {
	      minimumVector[j] = m_ThreadMin[i][j];
	    }
	  if (m_ThreadMax[i][j] > maximumVector[j])
	    {
	      maximumVector[j] = m_ThreadMax[i][j];
	    }
	}
    }
  for (unsigned int j=0; j<numberOfComponent; j++)
    {
      // compute statistics
      meanVector[j] = sumVector[j] / static_cast<RealType>(count);
    }
  
  // Compute Matrix Covariance
  MatrixType pixelSumMatrix;
  pixelSumMatrix.SetSize(static_cast<unsigned int>(numberOfComponent), 1);
  pixelSumMatrix.Fill(itk::NumericTraits<RealType>::Zero);
  for( unsigned int j = 0; j < numberOfComponent; j++)
    {
      pixelSumMatrix(j, 0) = sumVector[j];
    }

  MatrixType covMatrix, covMatrixTemp, tempTranspose;
  covMatrix.SetSize(static_cast<unsigned int>(numberOfComponent), static_cast<unsigned int>(numberOfComponent));
  covMatrixTemp.SetSize(static_cast<unsigned int>(numberOfComponent), static_cast<unsigned int>(numberOfComponent));
  tempTranspose.SetSize(1, static_cast<unsigned int>(numberOfComponent));
  
  covMatrix = crossedMatrix/static_cast<RealType>(count); 
  pixelSumMatrix/=static_cast<RealType>(count);
  tempTranspose = pixelSumMatrix.GetTranspose();
  covMatrixTemp = pixelSumMatrix*tempTranspose;
  /** TODO
    * A modifier en utilisant l'opérateur + de la méthode. Pour le moment probleme avec exceptionmacro (pas de GetClassName...)
    *covMatrix -= covMatrixTemp;
  **/
  if( (covMatrix.Rows() != covMatrixTemp.Rows()) || (covMatrix.Cols() != covMatrixTemp.Cols()))
    { 
      itkExceptionMacro( << "Matrix with size (" << covMatrix.Rows() << "," << 
			 covMatrix.Cols() << ") cannot be subtracted from matrix with size (" <<
			 covMatrixTemp.Rows() << "," << covMatrixTemp.Cols() );
    }
  
  for( unsigned int r=0; r<covMatrix.Rows(); r++) 
    {
      for( unsigned int c=0; c<covMatrix.Cols(); c++ ) 
	{
	  covMatrix(r,c) -= covMatrixTemp(r,c);
	}
    }
  //**** END TODO ****/

  // Set the outputs
  this->GetMinimumOutput()->Set( minimumVector );
  this->GetMaximumOutput()->Set( maximumVector );
  this->GetMeanOutput()->Set( meanVector );
  this->GetSumOutput()->Set( sumVector);
  this->GetCovarianceOutput()->Set( covMatrix );

  //otbMsgDebugMacro(<<"Leaving AfterThreadedGenerateData()");
}

template<class TInputImage>
void
StreamingStatisticsVectorImageFilter<TInputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId) 
{
  //otbMsgDebugMacro(<<"Entering ThreadedGenerateData()");
  /**
   * Grab the input
   */

  InputImagePointer inputPtr = const_cast< TInputImage * >( this->GetInput(0) );

  
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Here we will divide the input region into pieces to process one afther the other.
  unsigned int numDivisions = static_cast<unsigned int>(StreamingTraitsType
    ::CalculateNumberOfStreamDivisions(this->GetInput(),
				       outputRegionForThread,
				       m_StreamingMode,
				       m_NumberOfStreamDivisions,
				       m_BufferMemorySize,
				       m_BufferNumberOfLinesDivisions)); // calcul en combien fau diviser idealement l'image

  otbMsgDebugMacro(<<"ThreadedGeneraeData() - thread "<<threadId <<" - Thread region: "<<outputRegionForThread);
  otbMsgDebugMacro(<<"ThreadedGenerateData() - thread "<<threadId<<" - Streaming configuration: "<<m_StreamingMode<<" "<<m_NumberOfStreamDivisions<<" "<<m_BufferMemorySize<<" "<<m_BufferNumberOfLinesDivisions);
  otbMsgDebugMacro(<<"ThreadedGenerateData() - thread "<<threadId <<" - nb of divisions from StreamingTraits: "<<numDivisions);

  unsigned int numDivisionsFromSplitter = m_RegionSplitter->GetNumberOfSplits(outputRegionForThread, numDivisions);
  if (numDivisionsFromSplitter < numDivisions)
    {
      numDivisions = numDivisionsFromSplitter;
    }
  otbMsgDebugMacro(<<"ThreadedGenerateData() - thread "<<threadId <<" - nb of divisions: "<<numDivisions);


  MatrixType pixelVector, pixelTransposeVector, pixelSumVector, tempMatrix;

  pixelVector.SetSize(this->GetInput()->GetNumberOfComponentsPerPixel(), 1);
  pixelVector.Fill(itk::NumericTraits<RealType>::Zero);
  pixelTransposeVector.SetSize(1, this->GetInput()->GetNumberOfComponentsPerPixel());
  pixelTransposeVector.Fill(itk::NumericTraits<RealType>::Zero);
  pixelSumVector.SetSize(this->GetInput()->GetNumberOfComponentsPerPixel(), this->GetInput()->GetNumberOfComponentsPerPixel());
  pixelSumVector.Fill(itk::NumericTraits<RealType>::Zero);
  tempMatrix.SetSize(this->GetInput()->GetNumberOfComponentsPerPixel(), this->GetInput()->GetNumberOfComponentsPerPixel());

  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  unsigned int piece;
  for (piece = 0;
       piece < numDivisions && !this->GetAbortGenerateData();
       piece++)
    {
      //otbMsgDebugMacro(<<"ThreadedGenerateData() - processing piece: "<<piece<<"/"<<numDivisions);
      RegionType streamRegion = m_RegionSplitter->GetSplit(piece,numDivisions,outputRegionForThread);
      //otbMsgDebugMacro(<<"ThreadedGenerateData() - piece region: "<<streamRegion);
      
      inputPtr->SetRequestedRegion(streamRegion);
      otbMsgDebugMacro(<<"ThreadedGenerateData() - thread "<<threadId <<" - streaming region: "<<streamRegion);
      inputPtr->PropagateRequestedRegion();
      inputPtr->UpdateOutputData();
      
  
      
      itk::ImageRegionConstIteratorWithIndex<TInputImage> it (inputPtr, streamRegion);   
      it.GoToBegin();
      // do the work
      while (!it.IsAtEnd())
	{

	  IndexType index = it.GetIndex();
	  PixelType vectorValue = it.Get();
	  for (unsigned int j=0; j<vectorValue.GetSize(); j++)
	    {
	      InternalPixelType value = vectorValue[j];

	      RealType realValue = static_cast<RealType>( value );
	      if (value < m_ThreadMin[threadId][j])
		{
		  m_ThreadMin[threadId][j] = value;
		}
	      if (value > m_ThreadMax[threadId][j])
		{
		  m_ThreadMax[threadId][j] = value;
		}
	      m_ThreadSum[threadId][j] += realValue;
	      pixelVector(j, 0) = realValue;
	    }

	  ++it;
	  progress.CompletedPixel();
	  pixelTransposeVector = pixelVector.GetTranspose();

	  /** TODO
	   * A modifier en utilisant l'opérateur + de la méthode. Pour le moment probleme avec exceptionmacro (pas de GetClassName...)
	   * m_XX[threadId]+=pixelVector*pixelTransposeVector;
	   **/
 	  tempMatrix = pixelVector*pixelTransposeVector;
	  if( (m_XX[threadId].Rows() != tempMatrix.Rows()) || (m_XX[threadId].Cols() != tempMatrix.Cols()))
	    { 
	      itkExceptionMacro( << "Matrix with size (" << m_XX[threadId].Rows() << "," << 
				 m_XX[threadId].Cols() << ") cannot be subtracted from matrix with size (" <<
				 tempMatrix.Rows() << "," << tempMatrix.Cols() );
	    }

	  for( unsigned int r=0; r<m_XX[threadId].Rows(); r++) 
	    {
	      for( unsigned int c=0; c<m_XX[threadId].Cols(); c++ ) 
		{
		  m_XX[threadId](r,c) += tempMatrix(r,c);
		}
	    }
	  //**** END TODO ****
	  m_Count[threadId]++;
	}
    }

  //otbMsgDebugMacro(<<"Leaving ThreadedGenerateData()");
}

template <class TImage>
void 
StreamingStatisticsVectorImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "Minimum: "  <<this->GetMinimumOutput()->Get()<< std::endl;
  os << indent << "Maximum: "<<  this->GetMaximumOutput()->Get() << std::endl;
  os << indent << "Sum: "      << this->GetSumOutput()->Get() << std::endl;
  os << indent << "Mean: "     << this->GetMeanOutput()->Get() << std::endl;
  os << indent << "Covariance: " << this->GetCovarianceOutput()->Get() << std::endl;
  
}


}// end namespace otb
#endif
