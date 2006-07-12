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
#ifndef __otbLineRatioDetectorImageFilter_txx
#define __otbLineRatioDetectorImageFilter_txx

#include "otbLineRatioDetectorImageFilter.h"
#include "otbMacro.h"

#include "itkDataObject.h"
#include "itkExceptionObject.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkProgressReporter.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class InterpolatorType >
LineRatioDetectorImageFilter<TInputImage, TOutputImage, InterpolatorType>::LineRatioDetectorImageFilter()
{
  this->m_Radius.Fill(1);
  this->m_LengthLine = 1;
  this->m_WidthLine = 0;
  this->m_FaceList.Fill(0);
}

template <class TInputImage, class TOutputImage, class InterpolatorType>
double LineRatioDetectorImageFilter<TInputImage, TOutputImage, InterpolatorType>::ComputeMeasure(std::vector<double>* m1, std::vector<double>* m2, std::vector<double>* m3)
{
  
  double M1 = 0.0;
  double M2 = 0.0;
  double M3 = 0.0;

  std::vector<double>::iterator m1It = m1->begin();
  std::vector<double>::iterator m1End = m1->end();

  std::vector<double>::iterator m2It = m2->begin();
  std::vector<double>::iterator m2End = m2->end();

  std::vector<double>::iterator m3It = m3->begin();
  std::vector<double>::iterator m3End = m3->end();

  while( m1It!=m1End && m2It!=m2End && m3It!=m3End )
    {

    M1 += (*m1It);
    ++m1It;

    M2 += (*m2It);
    ++m2It;

    M3 += (*m3It);
    ++m3It;

    }

  M1 /= m1->size();
  M2 /= m2->size();
  M3 /= m3->size();

  double R12 = 0.0;
  double R13 = 0.0;
  
  if (( M1 != 0. ) && (M2 != 0. )) 
    R12 = static_cast<double>( 1 - MIN( (M1/M2), (M2/M1) ) );
  else if (( M1 != 0. ) || (M2 != 0. ))
    R12 = 1.0;
  else R12 = 0.;
    
  
  if (( M1 != 0. ) && (M3 != 0. )) 
    R13 = static_cast<double>( 1 - MIN( (M1/M3), (M3/M1) ) );
  else if (( M1 != 0. ) || (M3 != 0. ))
    R13 = 1.0;
  else R13 = 0.;

  
  // Determination of the minimum intensity of detection between R12 et R13
  return static_cast<double>( MIN( R12, R13 ) );

}




/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput, class InterpolatorType>
void 
LineRatioDetectorImageFilter<TInputImage, TOutput, InterpolatorType>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Length: " << this->m_LengthLine << std::endl;
  os << indent << "Width: " << this->m_WidthLine << std::endl;
  
}


} // end namespace otb


#endif
