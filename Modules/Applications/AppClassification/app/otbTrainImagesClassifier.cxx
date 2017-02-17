#include "otbTrainImagesBase.h"

namespace otb
{
namespace Wrapper
{

class TrainImagesClassifier : public TrainImagesBase<true>
{
public:
  typedef TrainImagesClassifier         Self;
  typedef TrainImagesBase<true>         Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro( Self )
  itkTypeMacro( Self, Superclass )
};

}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::TrainImagesClassifier )