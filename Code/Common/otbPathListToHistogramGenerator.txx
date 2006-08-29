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

#ifndef _otbPathListToHistogramGenerator_txx
#define _itkPathListToHistogramGenerator_txx

#include "otbPathListToHistogramGenerator.h"


namespace otb { 


template< class TPath,class TFunction >
PathListToHistogramGenerator< TPath, TFunction >
::PathListToHistogramGenerator() 
{
  m_HistogramGenerator = GeneratorType::New();
}



template< class TPath,class TFunction >
void
PathListToHistogramGenerator< TPath, TFunction >
::SetInput( PathListPointer path ) 
{
  m_PathList = path ;
}


template< class TPath,class TFunction >
const typename PathListToHistogramGenerator< TPath, TFunction >::HistogramType *
PathListToHistogramGenerator< TPath, TFunction >
::GetOutput() const
{
  return m_HistogramGenerator->GetOutput();
}



template< class TPath,class TFunction >
void
PathListToHistogramGenerator< TPath, TFunction >
::Compute() 
{
	
    PathPointer vertexList;
    
    ListSamplePointer  ListSample = ListSampleType::New();
    int nbElementPathList = m_PathList->size();
 
    ListSampleVectorType ResultFunction;
    
    typename TFunction::Pointer function = TFunction::New();
   
    for(int noPathList = 0 ; noPathList < nbElementPathList; noPathList++ )
    {
       vertexList = (*m_PathList)[noPathList]; 
       function->SetInputPath( vertexList );
       
       ResultFunction[0] = static_cast<MeasurementType>( function->Evaluate() );
       ListSample->PushBack(ResultFunction);

    }

    m_HistogramGenerator->SetListSample( ListSample ) ;
    m_HistogramGenerator->Update();
}



template< class TPath,class TFunction >
void
PathListToHistogramGenerator< TPath, TFunction >
::SetNumberOfBins( const SizeType & size ) 
{
  m_HistogramGenerator->SetNumberOfBins( size );
}



template< class TPath,class TFunction >
void
PathListToHistogramGenerator< TPath, TFunction >
::SetMarginalScale( double marginalScale )
{
  m_HistogramGenerator->SetMarginalScale( marginalScale );
}


template< class TPath,class TFunction >
void
PathListToHistogramGenerator< TPath, TFunction >
::SetHistogramMin(const MeasurementVectorType & histogramMin)
{
  m_HistogramGenerator->SetHistogramMin(histogramMin);
}


template< class TPath,class TFunction >
void
PathListToHistogramGenerator< TPath, TFunction >
::SetHistogramMax(const MeasurementVectorType & histogramMax)
{
  m_HistogramGenerator->SetHistogramMax(histogramMax);
}


template< class TPath,class TFunction >
void
PathListToHistogramGenerator< TPath, TFunction >
::SetAutoMinMax(bool autoMinMax)
{
  m_HistogramGenerator->SetAutoMinMax(autoMinMax);
}


template< class TPath,class TFunction >
void
PathListToHistogramGenerator< TPath, TFunction >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << "PathList = " << m_PathList << std::endl;
  os << "HistogramGenerator = " << m_HistogramGenerator << std::endl;
}



} // end of namespace otb

#endif


