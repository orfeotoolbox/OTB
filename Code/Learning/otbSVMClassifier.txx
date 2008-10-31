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
#ifndef __otbSVMClassifier_txx
#define __otbSVMClassifier_txx

#include "otbSVMClassifier.h"
#include "otbMacro.h"

namespace otb{ 

template< class TSample, class TLabel >
SVMClassifier< TSample, TLabel >
::SVMClassifier()
{
  m_Output = OutputType::New() ;
  m_Model = SVMModelType::New();
}

template< class TSample, class TLabel >
void
SVMClassifier< TSample, TLabel >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

template< class TSample, class TLabel >
void
SVMClassifier< TSample, TLabel >
::Update()
{
  this->GenerateData();
}



template< class TSample, class TLabel >
void
SVMClassifier< TSample, TLabel >
::GenerateData()
{

  m_Output->SetSample(this->GetSample()) ;
  otbMsgDevMacro(  << "m_Output " << m_Output );

  m_Output->Resize( this->GetSample()->Size() ) ;

  
  otbMsgDevMacro(  << "Resize to " << this->GetSample()->Size() );
  otbMsgDevMacro(  << "Resize to " << m_Output->GetSample()->Size() );
  
  unsigned int numberOfClasses = this->GetNumberOfClasses() ;
  otbMsgDevMacro(  << "NbClass " << numberOfClasses );
 

  m_Output->SetNumberOfClasses(numberOfClasses) ;
  otbMsgDevMacro(  << "Do Classif "  );
  this->DoClassification();
  otbMsgDevMacro(  << "End of classif" );  

}

template< class TSample, class TLabel >
typename SVMClassifier< TSample, TLabel >::OutputType*
SVMClassifier< TSample, TLabel >
::GetOutput() 
{
  return m_Output ;
}

template< class TSample, class TLabel >
void
SVMClassifier< TSample, TLabel >
::SetOutput( OutputType * output ) 
{
  m_Output = output;
}


template< class TSample, class TLabel >
void
SVMClassifier< TSample, TLabel >
::DoClassification()
{
  // implement progress reporting
  this->UpdateProgress(0.0);

  float nbSamples = static_cast<float>(this->GetSample()->Size());
  if(nbSamples == 0)
    {
      nbSamples = 1;
    }
  float nbProcessedSamples = 0;

  typename TSample::ConstIterator iter = this->GetSample()->Begin() ;
  typename TSample::ConstIterator end  = this->GetSample()->End() ;

  typename OutputType::ConstIterator iterO = m_Output->Begin() ;
  typename OutputType::ConstIterator endO  = m_Output->End() ;


  int numberOfComponentsPerSample  = iter.GetMeasurementVector().Size();
 

  // Set up the SVM funtor
  SVMFunctorType svmFunctor;
  svmFunctor.SetModel(m_Model->GetModel());
  svmFunctor.SetNumberOfComponentsPerSample(numberOfComponentsPerSample);
  svmFunctor.AllocateProblem();

  otbMsgDevMacro(  << "Starting iterations " );
  while (iter != end && iterO != endO)
    {
      MeasurementVectorType measurements = iter.GetMeasurementVector() ;
      ClassLabelType classLabel = static_cast<unsigned int>(svmFunctor(measurements));
      m_Output->AddInstance(classLabel, iterO.GetInstanceIdentifier()) ;
      this->UpdateProgress(nbProcessedSamples/nbSamples);
      ++iter;
      ++iterO;
    }
}

} // end of namespace otb

#endif
