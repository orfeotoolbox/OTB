
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbSpot5TransformBase.h"
#include "otbSpot5ForwardTransform.h"
#include "otbSpot5InverseTransform.h"
#include "otbGenericRSTransform.h"
#include "otbGeographicalDistance.h"
#include "itkPoint.h"
#include "itkEuclideanDistanceMetric.h"
#include <string>
#include <vector>
#include <iomanip>


#include "otbSpot5SensorModel.h"
#include "otbSpot5Metadata.h"


using Point2DType = itk::Point<double, 2>;
using Point3DType = itk::Point<double, 3>;
using Points2DContainerType = std::vector<Point2DType>;
using Points3DContainerType = std::vector<Point3DType>;
using ForwardTransformType = otb::Spot5ForwardTransform<double, 2, 3>;
using InverseTransformType = otb::Spot5InverseTransform<double, 3, 2>;
// using SensorModelType = otb::Spot5SensorModel<double, 3, 2>;

using GenericRSTransformType = otb::GenericRSTransform<double, 3, 3>;
using DistanceType = itk::Statistics::EuclideanDistanceMetric<Point2DType>;
using GeographicalDistanceType = otb::GeographicalDistance<Point3DType>;


int otbSpot5TransformTest(int itkNotUsed(argc), char* argv[])
{
  bool success = true;
  Point2DType imagePoint;
  Point3DType geo3dPoint;

  // Inputs
  std::string Spot5File(argv[1]);

  Point2DType imPt; 
  otb::Ephemeris imRay;




  using ImageType = otb::Image<double, 2>;
  using ImageFileReaderType = otb::ImageFileReader<ImageType>;
  auto reader = ImageFileReaderType::New();
  reader->SetFileName(Spot5File);
  reader->UpdateOutputInformation();
  auto imd = reader->GetOutput()->GetImageMetadata();

  otb::Spot5SensorModel SensorModel(imd);
  //SensorModel.ImagingRay(imPt, imRay);

  // Setting the transforms
  auto ForwardTransform = ForwardTransformType::New();
  ForwardTransform->SetMetadata(imd);

  if (!ForwardTransform->IsValidSensorModel())
  {
    std::cout << "Model not set!" << std::endl;
    return EXIT_FAILURE;
  }

  auto InverseTransform = InverseTransformType::New();
  InverseTransform->SetMetadata(imd);
  if (!InverseTransform->IsValidSensorModel())
  {
    std::cout << "Model not set!" << std::endl;
    return EXIT_FAILURE;
  }


  imagePoint[0] = 100;
  imagePoint[1] = 100;

  // Point3DType intersectPt;

  // NearestIntersection(ray, heightAboveEllipsoid, intersectPt);


  std::cout << "Testing geopoint: " << imagePoint << "\n\n";
  auto geoPoint = ForwardTransform->TransformPoint(imagePoint);
  std::cout << "Testing ForwardSensorModel: " << imagePoint << " -> " << geoPoint << "\n";
  
  geo3dPoint[0] = -122.332683260625586285641475115;
  geo3dPoint[1] = 38.6647750923265505207382375374;
  geo3dPoint[2] = 0;

  auto newImagePoint = InverseTransform->TransformPoint(geoPoint);

  std::cout << "Testing InverseSensorModel: " << geo3dPoint << " -> " << newImagePoint << "\n";




}


    // /* SPOT5 TEST */
    // typedef otb::Spot5ForwardTransform<double, 2, 2> ForwardSensorModelType;
    // ForwardSensorModelType::Pointer         forwardSensorModel = ForwardSensorModelType::New();
    // forwardSensorModel->SetMetadata(imd);
    // if (forwardSensorModel->IsValidSensorModel() == false)
    // {
    //   std::cerr << "Invalid Model pointer m_Model == NULL!\n The metadata is invalid!" << std::endl;
    //   //return EXIT_FAILURE;
    // }
    // /* FIN TEST SPOT5*/