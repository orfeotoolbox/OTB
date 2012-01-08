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
#ifndef __otbSparseWvltToAngleMapperListFilter_txx
#define __otbSparseWvltToAngleMapperListFilter_txx
#include "otbSparseWvltToAngleMapperListFilter.h"

#include <vnl/vnl_math.h>

#include "itkProgressReporter.h"

namespace otb {

template < class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages >
SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::SparseWvltToAngleMapperListFilter ()
{
  this->SetNumberOfRequiredInputs(NumberOfInputImages);

  // Generate the output sample list
  typename OutputSampleListObjectType::Pointer outputPtr =
    static_cast< OutputSampleListObjectType * >(this->MakeOutput(0).GetPointer());
  this->ProcessObject::SetNthOutput(0, outputPtr.GetPointer());

  m_ThresholdValue = static_cast<ValueType>( 10. );
}

template < class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages >
void
SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::SetInput ( unsigned int i, const InputImageListType * img )
{
  this->itk::ProcessObject::SetNthInput(i,
    const_cast< InputImageListType * >( img ) );
}

template < class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages >
const TInputImageList *
SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::GetInput ( unsigned int i ) const
{
  if ( i < this->GetNumberOfInputs() )
  {
    return 0;
  }

  return static_cast<const InputImageListType * >
    (this->itk::ProcessObject::GetInput(i) );
}

template < class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages >
typename SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::DataObjectPointer
SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::MakeOutput(unsigned int itkNotUsed(idx))
{
  typename OutputSampleListObjectType::Pointer outputPtr = OutputSampleListObjectType::New();
  OutputSampleListPointer outputSampleList = OutputSampleListType::New();
  outputPtr->Set(outputSampleList);
  return static_cast<DataObjectPointer>(outputPtr);
}

template < class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages >
typename SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::OutputSampleListType *
SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::GetOutputSampleList()
{
  typename OutputSampleListObjectType::Pointer dataObjectPointer
    = static_cast<OutputSampleListObjectType * > (this->ProcessObject::GetOutput(0) );
  return const_cast<OutputSampleListType *>(dataObjectPointer->Get());
}

template < class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages >
typename SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::OutputSampleListObjectType *
SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::GetOutput()
{
  return static_cast<OutputSampleListObjectType * >
    (this->ProcessObject::GetOutput(0) );
}

template < class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages >
void
SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Threshold : " << m_ThresholdValue << "\n";
}

template < class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages >
void
SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::GenerateData ()
{
  InputImageListConstIteratorVectorType it ( NumberOfInputImages );
  for ( unsigned int i = 0; i < NumberOfInputImages; ++i )
    it[i] = this->GetInput(i)->Begin();

  OutputSampleListType * outputList = this->GetOutputSampleList();

  // Set-up progress reporting
  itk::ProgressReporter progress(this, 0, this->GetInput(0)->Size());

  bool iteratorsNotAtEnd = true;
  while ( iteratorsNotAtEnd )
  {
    ImageConstIteratorVectorType imgIt ( NumberOfInputImages );
    for ( unsigned int i = 0; i < NumberOfInputImages; ++i )
    {
      imgIt[i] = ImageConstIteratorType( it[i].Get(), it[i].Get()->GetLargestPossibleRegion() );
      imgIt[i].GoToBegin();
    }

    bool localIteratorsNotAtEnd = true;
    while ( localIteratorsNotAtEnd )
    {
      if ( IsToGenerate( imgIt ) )
      {
        outputList->PushBack( GenerateData( imgIt ) );
      }

      for ( unsigned int i = 0; i < NumberOfInputImages; ++i )
      {
        ++(imgIt[i]);
        if ( imgIt[i].IsAtEnd() )
          localIteratorsNotAtEnd = false;
      }
    }

    for  ( unsigned int i = 0; i < NumberOfInputImages; ++i )
    {
      ++(it[i]);
      if ( it[i] != this->GetInput(i)->End() )
        iteratorsNotAtEnd = false;
    }
    progress.CompletedPixel();
  }
}

template < class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages >
bool
SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::IsToGenerate ( const ImageConstIteratorVectorType & it ) const
{
  for ( unsigned int i = 0; i < NumberOfInputImages; ++i )
  {
    if ( it[i].Get() < m_ThresholdValue )
      return false;
  }

  return true;
}

template < class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages >
typename SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::OutputMeasurementVectorType
SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::GenerateData ( const ImageConstIteratorVectorType & it ) const
{
  return FromEuclideanToSphericalSpace( it );
}

template < class TInputImageList, class TOutputSampleList, unsigned int VNbInputImages >
typename SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::OutputMeasurementVectorType
SparseWvltToAngleMapperListFilter< TInputImageList, TOutputSampleList, VNbInputImages >
::FromEuclideanToSphericalSpace 
( const ImageConstIteratorVectorType & it ) const
{
  // First, get the modulus of the vector
  double modulus = 0;
  for ( unsigned int i = 0; i < NumberOfInputImages; ++i )
  {
    modulus += vcl_pow( static_cast<double>( it[i].Get() ), 2. ); 
  }
  // The modulus cannot be nul since it is over the threshold...
  modulus = vcl_sqrt( modulus );

  // FIXME test if NaN nor infinite

  OutputMeasurementVectorType angle;
  angle.Fill( static_cast< OutputValueType >( 0. ) );
  for ( unsigned int k = 0; k < angle.Size()-1; ++k )
  {
    double phase = vcl_acos( it[k].Get() / modulus );
    angle[k] = phase;
    modulus *= vcl_sin( phase );

    // FIXME test also if not finite
    if ( modulus < 1e-5 )
    {
      while ( ++k < angle.Size() )
        angle[k] = 0.;
      return angle;
    }
  }

  if ( it[ NumberOfInputImages-1 ].Get() >= 0 )
  {
    angle[ angle.Size()-1 ] = vcl_acos( it[ NumberOfInputImages-2 ].Get() / modulus );
  }
  else
  {
    angle[ angle.Size()-1 ] = - vcl_acos( it[ NumberOfInputImages-2 ].Get() / modulus );
  }

  return angle;
}

} // end of namespace otb

#endif

