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
#ifndef otbLabelMapSVMClassifier_txx
#define otbLabelMapSVMClassifier_txx

#include "otbLabelMapSVMClassifier.h"


namespace otb {

template <class TInputImage>
LabelMapSVMClassifier<TInputImage>
::LabelMapSVMClassifier()
{
  // Force to single-threaded (SVMModel is not thread-safe)
  // This way, we benefit of the LabelMapFilter design and only need
  // to implement ThreadedProcessLabelObject
  this->SetNumberOfThreads(1);
}

template<class TInputImage>
void
LabelMapSVMClassifier<TInputImage>
::ReleaseInputs( )
{
  // by pass itk::InPlaceLabelMapFilter::ReleaseInputs() implementation,
  // which caused memory issue when ran inplace
  this->itk::LabelMapFilter<TInputImage, TInputImage>::ReleaseInputs();
}

template<class TInputImage>
void
LabelMapSVMClassifier<TInputImage>
::ThreadedProcessLabelObject( LabelObjectType * labelObject )
{
  ClassLabelType classLabel = m_Model->EvaluateLabel(m_MeasurementFunctor(labelObject));
  labelObject->SetClassLabel(classLabel);
}

}// end namespace otb
#endif
