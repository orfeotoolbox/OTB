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
#include "otbTrainVectorBase.h"

// Validation
#include "otbConfusionMatrixCalculator.h"

namespace otb
{
namespace Wrapper
{

class TrainVectorClassifier : public TrainVectorBase
{
public:
  typedef TrainVectorClassifier Self;
  typedef TrainVectorBase Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro( Self )

  itkTypeMacro( Self, Superclass )

  typedef Superclass::SampleType SampleType;
  typedef Superclass::ListSampleType ListSampleType;
  typedef Superclass::TargetListSampleType TargetListSampleType;

private:
  void DoTrainInit()
  {
    // Nothing to do here
  }

  void DoTrainUpdateParameters()
  {
    // Nothing to do here
  }

  void DoTrainExecute()
  {
    // Nothing to do here
  }



};
}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::TrainVectorClustering )
