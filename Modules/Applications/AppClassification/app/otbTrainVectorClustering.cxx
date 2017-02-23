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

namespace otb
{
namespace Wrapper
{

class TrainVectorClustering : public TrainVectorBase
{
public:
  typedef TrainVectorClustering Self;
  typedef TrainVectorBase Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro( Self )

  itkTypeMacro( Self, Superclass )

  typedef Superclass::SampleType SampleType;
  typedef Superclass::ListSampleType ListSampleType;
  typedef Superclass::TargetListSampleType TargetListSampleType;

protected :
  TrainVectorClustering() : TrainVectorBase()
  {
    m_ClassifierCategory = Unsupervised;
  }

private:
  void DoTrainInit()
  {
    SetName( "TrainVectorClustering" );
    SetDescription( "Train a classifier based on labeled or unlabeled geometries and a list of features to consider." );

    SetDocName( "Train Vector Clustering" );
    SetDocLongDescription( "This application trains a classifier based on "
                                   "labeled or unlabeled geometries and a list of features to consider for classification." );
    SetDocLimitations( " " );
    SetDocAuthors( "OTB Team" );
    SetDocSeeAlso( " " );

    // Doc example parameter settings
    SetDocExampleParameterValue( "io.vd", "vectorData.shp" );
    SetDocExampleParameterValue( "io.out", "kmeansModel.txt" );
    SetDocExampleParameterValue( "feat", "perimeter width area" );

  }

  void DoTrainUpdateParameters()
  {
    // Nothing to do here
  }

  void DoBeforeTrainExecute()
  {
    // Nothing to do here
  }

  void DoAfterTrainExecute()
  {
    // Nothing to do here
  }



};
}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::TrainVectorClustering )
