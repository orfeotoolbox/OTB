/*=========================================================================

  Program :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J. Inglada
  Language  :   C++
  Date      :   26 April 2006
  Version   :   
  Role      :   SVM Membership Function
  $Id$

=========================================================================*/
#ifndef __otbSVMMembershipFunction_txx
#define __otbSVMMembershipFunction_txx

#include "otbSVMMembershipFunction.h"

namespace otb{ 

template < class TVector >
SVMMembershipFunction< TVector >
::SVMMembershipFunction():
  m_NumberOfSamples(0),
  m_NumberOfClasses(0)//,
//   m_PreFactor(0),
//   m_Epsilon( 1e-100 ),
//   m_DoubleMax( 1e+20 )
{

  m_Model = SVMModelType::New();
  this->m_MeasurementVectorSize = 0;
}

template< class TVector >
void 
SVMMembershipFunction< TVector >
::SetMeasurementVectorSize( const MeasurementVectorSizeType s )
{
  if( s == this->m_MeasurementVectorSize )
    {
    return;
    }
  
  if( this->m_MeasurementVectorSize != 0 )
    {  
    itkWarningMacro( << "Destructively resizing paramters of the DistanceToCentroidMembershipFunction." );
    }
  this->m_MeasurementVectorSize = s;
  
  this->Modified();
}


template < class TVector >
double 
SVMMembershipFunction< TVector >
::Evaluate(const MeasurementVectorType &measurement) const
{ 
//FIXME : Implement!!
  double temp;
//   m_TempVec.set_size( 1, this->m_MeasurementVectorSize);
//   m_TempMat.set_size( 1, this->m_MeasurementVectorSize);

//   // Compute |y - mean |   
//   for ( unsigned int i = 0; i < this->m_MeasurementVectorSize; i++ )
//     {
//     m_TempVec[0][i] = measurement[i] - m_Mean[i];
//     }

//   // Compute |y - mean | * inverse(cov) 
//   m_TempMat= m_TempVec * m_InverseCovariance;

//   // Compute |y - mean | * inverse(cov) * |y - mean|^T 
//   //tmp = (m_TmpMat * (m_TmpVec.transpose()))[0][0];
//   temp = dot_product( vnl_vector_ref<double>( m_TempMat.size(), m_TempMat.begin()),
//                       vnl_vector_ref<double>( m_TempVec.size(), m_TempVec.begin()));
//   // should be this, but to remain comatible with the old vnl for
//   // some time, the above is used
// //  temp = dot_product( m_TempMat.as_ref(), m_TempVec.as_ref() ); 
  
  return temp ;
}
  
template < class TVector >
void  
SVMMembershipFunction< TVector >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  unsigned int i ;
  Superclass::PrintSelf(os,indent);

//   if ( this->m_MeasurementVectorSize &&
//        m_Mean.size() == this->m_MeasurementVectorSize )
//     {
//     os << indent << "Mean: [" ;
//     for (i=0 ; (i + 1) < this->m_MeasurementVectorSize; i++)
//       {
//       os << m_Mean[i] << ", ";
//       }
//     os << m_Mean[i] << "]" << std::endl;
//     }
//   else
//     {
//     os << indent << "Mean: not set or size does not match" << std::endl ;
//     }

//   os << indent << "Number of Samples: " << m_NumberOfSamples << std::endl;
//   os << indent << "Covariance:        " << std::endl;
//   os << m_Covariance << std::endl;
//   os << indent << "Inverse covariance:        " << std::endl;
//   os << m_InverseCovariance << std::endl;
}

} // end of namespace otb



#endif
