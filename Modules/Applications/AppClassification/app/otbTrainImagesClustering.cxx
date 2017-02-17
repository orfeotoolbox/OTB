#include "otbTrainImagesBase.h"

namespace otb
{
namespace Wrapper
{

class TrainImagesClustering : public TrainImagesBase<false>
{
public:
  typedef TrainImagesClustering         Self;
  typedef TrainImagesBase<false>        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro( Self )
  itkTypeMacro( Self, Superclass )
};

}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::TrainImagesClustering )