
#ifndef cbTrainPCA_txx
#define cbTrainPCA_txx

#include "otbTrainDimensionalityReductionApplicationBase.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void
TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::InitPCAParams()
{


  AddChoice("algorithm.pca", "Shark PCA");
  SetParameterDescription("algorithm.pca",
                          "This group of parameters allows setting Shark PCA parameters. "
                          );
  
  
   //Output Dimension
  AddParameter(ParameterType_Int, "algorithm.pca.dim",
               "Dimension of the output of the pca transformation");
  SetParameterInt("algorithm.pca.dim",10, false);
  SetParameterDescription(
    "algorithm.pca.dim",
    "Dimension of the output of the pca transformation.");
  
 
}

template <class TInputValue, class TOutputValue>
void TrainDimensionalityReductionApplicationBase<TInputValue,TOutputValue>
::TrainPCA(typename ListSampleType::Pointer trainingListSample,std::string modelPath)
{
		typename PCAModelType::Pointer dimredTrainer = PCAModelType::New();
		dimredTrainer->SetDimension(GetParameterInt("algorithm.pca.dim"));
		dimredTrainer->SetInputListSample(trainingListSample);
		dimredTrainer->SetWriteEigenvectors(true);
		dimredTrainer->Train();
		dimredTrainer->Save(modelPath);
}

} //end namespace wrapper
} //end namespace otb

#endif
