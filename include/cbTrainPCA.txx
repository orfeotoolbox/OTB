
#ifndef cbTrainPCA_txx
#define cbTrainPCA_txx

#include "cbLearningApplicationBaseDR.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void
cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::InitPCAParams()
{


  AddChoice("model.pca", "Shark PCA");
  SetParameterDescription("model.pca",
                          "This group of parameters allows setting Shark PCA parameters. "
                          );
  
  
   //Output Dimension
  AddParameter(ParameterType_Int, "model.pca.dim",
               "Dimension of the output of the pca transformation");
  SetParameterInt("model.pca.dim",10, false);
  SetParameterDescription(
    "model.pca.dim",
    "Dimension of the output of the pca transformation.");
  
 
}

template <class TInputValue, class TOutputValue>
void cbLearningApplicationBaseDR<TInputValue,TOutputValue>
::TrainPCA(typename ListSampleType::Pointer trainingListSample,std::string modelPath)
{
		typename PCAModelType::Pointer dimredTrainer = PCAModelType::New();
		dimredTrainer->SetDimension(GetParameterInt("model.pca.dim"));
		dimredTrainer->SetInputListSample(trainingListSample);
		dimredTrainer->Train();
		dimredTrainer->Save(modelPath);
}

} //end namespace wrapper
} //end namespace otb

#endif
