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
#ifndef _otbMatrixTransposeMatrixImageFilter_txx
#define _otbMatrixTransposeMatrixImageFilter_txx
#include "otbMatrixTransposeMatrixImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"


namespace otb {

template<class TInputImage, class TInputImage2>
MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::MatrixTransposeMatrixImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around pixel types
  

  typename ImageType::Pointer output1 = static_cast<ImageType*>(this->MakeOutput(0).GetPointer());
  this->itk::ProcessObject::SetNthOutput(0, output1.GetPointer());
  typename MatrixObjectType::Pointer output2 = static_cast<MatrixObjectType*>(this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(1, output2.GetPointer());
  
  // false means no pad added
  m_UsePadFirstInput = false;
  m_UsePadSecondInput = false;
  // Streaming initialization
  m_BufferMemorySize = 0;
  m_BufferNumberOfLinesDivisions = 0;
  m_NumberOfStreamDivisions = 0;
  // default to AUTOMATIC_NUMBER_OF_DIVISIONS
  m_StreamingMode = SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS;
  // create default region splitter
  m_RegionSplitter = itk::ImageRegionSplitter<InputImageDimension>::New();

  // Number of component initialisation
  m_NumberOfComponents1 = 0;
  m_NumberOfComponents2 = 0;
}


template<class TInputImage, class TInputImage2>
itk::DataObject::Pointer
MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::MakeOutput(unsigned int output)
{
  switch (output)
    {
   case 0:
     return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
      return static_cast<itk::DataObject*>(MatrixObjectType::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }
    
}



template<class TInputImage, class TInputImage2>
typename MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>::MatrixObjectType*
MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::GetResultOutput()
{
  return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TInputImage2>
const typename MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>::MatrixObjectType*
MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::GetResultOutput() const
{
  return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(1));
}


template<class TInputImage, class TInputImage2>
void
MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
  if ( this->GetInput() )
    {
      InputImagePointer image = const_cast< typename Superclass::InputImageType * >( this->GetFirstInput() );
      InputImagePointer image2 = const_cast< typename Superclass::InputImageType * >( this->GetSecondInput() );

      IndexType index = image->GetLargestPossibleRegion().GetIndex();
      SizeType size;
      size.Fill(0);
      RegionType region;
      region.SetSize(size);
      region.SetIndex(index);
      image->SetRequestedRegion(region);

      IndexType index2 = image2->GetLargestPossibleRegion().GetIndex();
      SizeType size2;
      size2.Fill(0);
      RegionType region2;
      region2.SetSize(size2);
      region2.SetIndex(index2);
      image2->SetRequestedRegion(region2);

    }
}

template<class TInputImage, class TInputImage2>
void
MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if ( this->GetInput() )
    {
      this->GetOutput()->CopyInformation(this->GetFirstInput());
      this->GetOutput()->SetLargestPossibleRegion(this->GetFirstInput()->GetLargestPossibleRegion());
    }
}

template<class TInputImage, class TInputImage2>
void
MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::AllocateOutputs()
{
  
  // Pass the input through as the output
  InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template<class TInputImage, class TInputImage2>
void
MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::BeforeThreadedGenerateData()
{ 
  if(this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()==0)
    {
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
    }

  if ( this->GetFirstInput()->GetLargestPossibleRegion().GetSize() !=  this->GetSecondInput()->GetLargestPossibleRegion().GetSize() )
    {
      itkExceptionMacro( <<" Can't multiply the transposed matrix of a "
			 << this->GetFirstInput()->GetLargestPossibleRegion().GetSize() 
			 << " and a "
			 << this->GetSecondInput()->GetLargestPossibleRegion().GetSize()
			 << " matrix " );
    }

  m_NumberOfComponents1 = this->GetFirstInput()->GetNumberOfComponentsPerPixel();
  m_NumberOfComponents2 = this->GetSecondInput()->GetNumberOfComponentsPerPixel();
  unsigned int numberOfThreads = this->GetNumberOfThreads();
  
  if ( m_UsePadFirstInput == true )
    {
      m_NumberOfComponents1++;
    }
  if ( m_UsePadSecondInput == true )
    {
      m_NumberOfComponents2++;
    }

  MatrixType tempMatrix, initMatrix;
  tempMatrix.SetSize(m_NumberOfComponents1, m_NumberOfComponents2);
  tempMatrix.Fill(itk::NumericTraits<RealType>::Zero);
  m_ThreadSum = ArrayMatrixType(numberOfThreads, tempMatrix);

  initMatrix.SetSize(m_NumberOfComponents2, m_NumberOfComponents2);
  initMatrix.Fill(itk::NumericTraits<RealType>::Zero);
  this->GetResultOutput()->Set( initMatrix );  
}




template<class TInputImage, class TInputImage2>
void
MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::AfterThreadedGenerateData()
{
  unsigned int numberOfThreads = this->GetNumberOfThreads();
  MatrixType resultMatrix;
  resultMatrix.SetSize(m_NumberOfComponents1, m_NumberOfComponents2);
  resultMatrix.Fill(itk::NumericTraits<RealType>::Zero);
  

  for( unsigned int thread = 0; thread < numberOfThreads; thread++)
    {
      /** TODO
       * A modifier en utilisant l'opérateur + de la méthode. Pour le moment probleme avec exceptionmacro (pas de GetClassName...)
       * resultMatrix += m_ThreadSum[thread];
       **/
      for (unsigned int i=0; i<resultMatrix.Rows(); i++)
	{
	  for (unsigned int j=0; j<resultMatrix.Cols(); j++)
	    {
	      resultMatrix(i, j) += m_ThreadSum[thread](i, j);
	    }
	}
      /********END TODO ******/
    }
  this->GetResultOutput()->Set( resultMatrix );
}

template<class TInputImage, class TInputImage2>
void
MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId) 
{
  /**
   * Grab the input
   */

  InputImagePointer input1Ptr = const_cast< TInputImage * >( this->GetFirstInput() );
  InputImagePointer input2Ptr = const_cast< TInputImage2 * >( this->GetSecondInput() );
  
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Here we will divide the input region into pieces to process one afther the other.
  unsigned int numDivisions = static_cast<unsigned int>(StreamingTraitsType
    ::CalculateNumberOfStreamDivisions(this->GetFirstInput(),
				       outputRegionForThread,
				       m_StreamingMode,
				       m_NumberOfStreamDivisions,
				       m_BufferMemorySize,
				       m_BufferNumberOfLinesDivisions));
  

  otbMsgDebugMacro(<<"ThreadedGeneraeData() - thread "<<threadId<<" - Thread region: "<<outputRegionForThread);
  otbMsgDebugMacro(<<"ThreadedGenerateData() - thread "<<threadId<<" - Streaming configuration: "<<m_StreamingMode<<" "<<m_NumberOfStreamDivisions<<" "<<m_BufferMemorySize<<" "<<m_BufferNumberOfLinesDivisions);
  otbMsgDebugMacro(<<"ThreadedGenerateData() - thread "<<threadId <<" - nb of divisions from StreamingTraits: "<<numDivisions);
 

  unsigned int numDivisionsFromSplitter = m_RegionSplitter->GetNumberOfSplits(outputRegionForThread, numDivisions);


  if (numDivisionsFromSplitter < numDivisions)
    {
      numDivisions = numDivisionsFromSplitter;
    }
  otbMsgDebugMacro(<<"ThreadedGenerateData() - thread "<<threadId <<" - nb of divisions: "<<numDivisions);

  /*
  unsigned int numberOfComponents1 = this->GetFirstInput()->GetNumberOfComponentsPerPixel();
  unsigned int numberOfComponents2 = this->GetSecondInput()->GetNumberOfComponentsPerPixel();

  MatrixType result;
  result.SetSize(numberOfComponents1, numberOfComponents2);
  result.Fill(itk::NumericTraits<RealType>::Zero);
  */

  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  unsigned int piece;
  for (piece = 0;
       piece < numDivisions && !this->GetAbortGenerateData();
       piece++)
    {
      RegionType streamRegion = m_RegionSplitter->GetSplit(piece,numDivisions,outputRegionForThread);

      
      input1Ptr->SetRequestedRegion(streamRegion);
      input2Ptr->SetRequestedRegion(streamRegion);
      otbMsgDebugMacro(<<"ThreadedGenerateData() - thread "<<threadId <<" - streaming region: "<<streamRegion);
      input1Ptr->PropagateRequestedRegion();
      input1Ptr->UpdateOutputData();
      input2Ptr->PropagateRequestedRegion();
      input2Ptr->UpdateOutputData(); 
      
      itk::ImageRegionConstIteratorWithIndex<TInputImage> it1 (input1Ptr, streamRegion); 
      itk::ImageRegionConstIteratorWithIndex<TInputImage> it2 (input2Ptr, streamRegion);
      it1.GoToBegin();
      it2.GoToBegin();
 
      // loop the second image and get one pixel a time
      while (!it1.IsAtEnd())
	{
	  //IndexType index1 = it1.GetIndex();
	  PixelType vectorValue1 = it1.Get();
	  //IndexType2 index2 = it2.GetIndex();
	  PixelType2 vectorValue2 = it2.Get();
	  
	  // Add a first component to vectorValue2 and vectorValue1 filled with ones.
	  if (m_UsePadFirstInput == true)
	    {
	      PixelType vectortemp1(vectorValue1.Size()+1);
	      vectortemp1[0] = 1;
	       for (unsigned int n=0; n<vectorValue1.Size(); n++)
		 {
		   vectortemp1[n+1] = vectorValue1[n];

		 }
	       vectorValue1.SetSize(vectortemp1.Size());
	       vectorValue1 = vectortemp1;
	    }

	  if (m_UsePadSecondInput == true)
	    { 
	       PixelType2 vectortemp2(vectorValue2.Size()+1);
	       vectortemp2[0] = 1;
	       for (unsigned int m=0; m<vectorValue2.Size(); m++)
		 {
		   vectortemp2[m+1] = vectorValue2[m];

		 }
	       vectorValue2.SetSize(vectortemp2.Size());
	       vectorValue2 = vectortemp2;
	    }

	  for (unsigned int i=0; i<vectorValue1.Size(); i++)
	    {
	      for (unsigned int j=0; j<vectorValue2.Size(); j++)
		{
		  m_ThreadSum[threadId](i, j) += vectorValue1[i]*static_cast<InternalPixelType>(vectorValue2[j]);
		}
	      
	    }
	  ++it1;
	  ++it2;
	  progress.CompletedPixel();
	}
    }
  //otbMsgDebugMacro(<<"Leaving ThreadedGenerateData()");
}

template<class TInputImage, class TInputImage2>
void 
MatrixTransposeMatrixImageFilter<TInputImage, TInputImage2>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "Result: "  <<this->GetResultOutput()->Get()<< std::endl;
}


}// end namespace otb
#endif
