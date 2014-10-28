#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkShapeLabelObject.h"
#include "itkLabelImageToStatisticsLabelMapFilter.h"
#include "itkSimpleFilterWatcher.h"

bool eq( double a, double b, double precision=0.00001 )
  {
  return std::abs( ( a - b ) / a ) < precision;
  }


int ijObjectLabelAttributeValuesTest(int argc, char * argv[])
{
  const int dim = 3;
  typedef unsigned char PixelType;
  typedef otb::Image< PixelType, dim >    ImageType;

  if( argc != 3)
    {
    std::cerr << "usage: " << argv[0] << " input1 input2" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  typedef otb::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName( argv[1] );

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );

  typedef itk::LabelImageToStatisticsLabelMapFilter< ImageType, ImageType > ConverterType;
  ConverterType::Pointer converter = ConverterType::New();
  converter->SetInput( reader1->GetOutput() );
  converter->SetFeatureImage( reader2->GetOutput() );
  converter->SetComputeFeretDiameter( true );
  converter->SetComputePerimeter( true );
  itk::SimpleFilterWatcher watcher(converter, "filter");

  converter->Update();

  typedef ConverterType::OutputImageType LabelMapType;
  LabelMapType::Pointer labelMap = converter->GetOutput();

  typedef LabelMapType::LabelObjectType LabelObjectType;
  const LabelObjectType * ball = labelMap->GetLabelObject( 1 );

  assert( eq(3.0, ball->GetCentroid()[0]) );
  assert( eq(3.0, ball->GetCentroid()[1]) );
  assert( eq(6.0, ball->GetCentroid()[2]) );

  assert( eq(120.95, ball->GetPhysicalSize()) );
  assert( 60475 == ball->GetSize() );
  assert( eq(1.01639344262, ball->GetRegionElongation()) );
  assert( eq(0.52426940382, ball->GetSizeRegionRatio()) );
  assert( 141 == ball->GetSizeOnBorder() );
  assert( eq(2.82, ball->GetPhysicalSizeOnBorder()) );
  assert( eq(116.327, ball->GetPerimeter()) );
  assert( eq(6.1903150162168652, ball->GetFeretDiameter()) );
  assert( eq(118.274/116.327, ball->GetRoundness()) );

  assert( eq(1.85995, ball->GetBinaryPrincipalMoments()[0]) );
  assert( eq(1.85995, ball->GetBinaryPrincipalMoments()[1]) );
  assert( eq(1.92809, ball->GetBinaryPrincipalMoments()[2]) );

  assert( eq(1.01815, ball->GetBinaryElongation()) );
  assert( eq(3.06789, ball->GetEquivalentRadius()) );
  assert( eq(118.274, ball->GetEquivalentPerimeter()) );

  assert( eq(6.09908, ball->GetEquivalentEllipsoidSize()[0]) );
  assert( eq(6.09908, ball->GetEquivalentEllipsoidSize()[1]) );
  assert( eq(6.20981, ball->GetEquivalentEllipsoidSize()[2]) );

  assert( 0 == ball->GetRegion().GetIndex()[0] );
  assert( 0 == ball->GetRegion().GetIndex()[1] );
  assert( 15 == ball->GetRegion().GetIndex()[2] );

  assert( 61 == ball->GetRegion().GetSize()[0] );
  assert( 61 == ball->GetRegion().GetSize()[1] );
  assert( 31 == ball->GetRegion().GetSize()[2] );

//   assert( eq(0.577199, ball->GetBinaryPrincipalAxes()[0][0]) );
//   assert( eq(0.816603, ball->GetBinaryPrincipalAxes()[0][1]) );
//   assert( eq(3.52768e-12, ball->GetBinaryPrincipalAxes()[0][2]) );
//
//   assert( eq(-0.816603, ball->GetBinaryPrincipalAxes()[1][0]) );
//   assert( eq(0.577199, ball->GetBinaryPrincipalAxes()[1][1]) );
//   assert( eq(6.73715e-14, ball->GetBinaryPrincipalAxes()[1][2]) );
//
//   assert( eq(-1.98116e-12, ball->GetBinaryPrincipalAxes()[2][0]) );
//   assert( eq(-2.9196e-12, ball->GetBinaryPrincipalAxes()[2][1]) );
//   assert( eq(1.0, ball->GetBinaryPrincipalAxes()[2][2]) );


  const LabelObjectType * ellipsoid = labelMap->GetLabelObject( 2 );

  assert( eq(3.0, ellipsoid->GetCentroid()[0]) );
  assert( eq(3.0, ellipsoid->GetCentroid()[1]) );
  assert( eq(22.0, ellipsoid->GetCentroid()[2]) );

  assert( eq(238.018, ellipsoid->GetPhysicalSize()) );
  assert( 119009 == ellipsoid->GetSize() );
  assert( eq(2.0, ellipsoid->GetRegionElongation()) );
  assert( eq(0.524313, ellipsoid->GetSizeRegionRatio()) );
  assert( 388 == ellipsoid->GetSizeOnBorder() );
  assert( eq(6.79, ellipsoid->GetPhysicalSizeOnBorder()) );
//  assert( eq(, ellipsoid->GetPerimeter()) );
//  assert( eq(, ellipsoid->GetFeretDiameter()) );
//  assert( eq(, ellipsoid->GetRoundness()) );

  assert( eq(1.8622, ellipsoid->GetBinaryPrincipalMoments()[0]) );
  assert( eq(1.8622, ellipsoid->GetBinaryPrincipalMoments()[1]) );
  assert( eq(7.4488, ellipsoid->GetBinaryPrincipalMoments()[2]) );

  assert( eq(2.0, ellipsoid->GetBinaryElongation()) );
  assert( eq(3.8445, ellipsoid->GetEquivalentRadius()) );
  assert( eq(185.734, ellipsoid->GetEquivalentPerimeter()) );

  assert( eq(6.10277, ellipsoid->GetEquivalentEllipsoidSize()[0]) );
  assert( eq(6.10277, ellipsoid->GetEquivalentEllipsoidSize()[1]) );
  assert( eq(12.2055, ellipsoid->GetEquivalentEllipsoidSize()[2]) );

  assert( 0 == ellipsoid->GetRegion().GetIndex()[0] );
  assert( 0 == ellipsoid->GetRegion().GetIndex()[1] );
  assert( 80 == ellipsoid->GetRegion().GetIndex()[2] );

  assert( 61 == ellipsoid->GetRegion().GetSize()[0] );
  assert( 61 == ellipsoid->GetRegion().GetSize()[1] );
  assert( 61 == ellipsoid->GetRegion().GetSize()[2] );

//   assert( eq(0.821007, ellipsoid->GetBinaryPrincipalAxes()[0][0]) );
//   assert( eq(-0.570918, ellipsoid->GetBinaryPrincipalAxes()[0][1]) );
//   assert( eq(5.49736e-14, ellipsoid->GetBinaryPrincipalAxes()[0][2]) );
//
//   assert( eq(-0.570918, ellipsoid->GetBinaryPrincipalAxes()[1][0]) );
//   assert( eq(-0.821007, ellipsoid->GetBinaryPrincipalAxes()[1][1]) );
//   assert( eq(-2.5511e-13, ellipsoid->GetBinaryPrincipalAxes()[1][2]) );
//
//   assert( eq(1.9078e-13, ellipsoid->GetBinaryPrincipalAxes()[2][0]) );
//   assert( eq(1.78062e-13, ellipsoid->GetBinaryPrincipalAxes()[2][1]) );
//   assert( eq(-1.0, ellipsoid->GetBinaryPrincipalAxes()[2][2]) );



  const LabelObjectType * disk = labelMap->GetLabelObject( 3 );

  assert( eq(3.0, disk->GetCentroid()[0]) );
  assert( eq(11.0, disk->GetCentroid()[1]) );
  assert( eq(22.0, disk->GetCentroid()[2]) );

  assert( eq(5.866, disk->GetPhysicalSize()) );
  assert( 2933 == disk->GetSize() );
  assert( eq(122.0, disk->GetRegionElongation()) );
  assert( eq(0.788229, disk->GetSizeRegionRatio()) );
  assert( 22 == disk->GetSizeOnBorder() );
  assert( eq(0.33, disk->GetPhysicalSizeOnBorder()) );
//  assert( eq(, disk->GetPerimeter()) );
//  assert( eq(, disk->GetFeretDiameter()) );
//  assert( eq(, disk->GetRoundness()) );

  assert( eq(-7.78731e-28, disk->GetBinaryPrincipalMoments()[0]) );
  assert( eq(2.33397, disk->GetBinaryPrincipalMoments()[1]) );
  assert( eq(9.33589, disk->GetBinaryPrincipalMoments()[2]) );

//  assert( NaN == disk->GetBinaryElongation() );
  assert( eq(1.1188, disk->GetEquivalentRadius()) );
  assert( eq(15.7294, disk->GetEquivalentPerimeter()) );

//   assert( eq(1.5874, disk->GetEquivalentEllipsoidSize()[0]) );
//   assert( eq(1.5874, disk->GetEquivalentEllipsoidSize()[1]) );
//   assert( eq(3.1748, disk->GetEquivalentEllipsoidSize()[2]) );

  assert( 30 == disk->GetRegion().GetIndex()[0] );
  assert( 80 == disk->GetRegion().GetIndex()[1] );
  assert( 80 == disk->GetRegion().GetIndex()[2] );

  assert( 1 == disk->GetRegion().GetSize()[0] );
  assert( 61 == disk->GetRegion().GetSize()[1] );
  assert( 61 == disk->GetRegion().GetSize()[2] );

  assert( eq(1.0, disk->GetBinaryPrincipalAxes()[0][0]) );
  assert( eq(-2.01391e-17, disk->GetBinaryPrincipalAxes()[0][1]) );
  assert( eq(-9.13305e-15, disk->GetBinaryPrincipalAxes()[0][2]) );

  assert( eq(-2.01391e-17, disk->GetBinaryPrincipalAxes()[1][0]) );
  assert( eq(-1.0, disk->GetBinaryPrincipalAxes()[1][1]) );
  assert( eq(4.05913e-14, disk->GetBinaryPrincipalAxes()[1][2]) );

  assert( eq(-9.13305e-15, disk->GetBinaryPrincipalAxes()[2][0]) );
  assert( eq(-4.05913e-14, disk->GetBinaryPrincipalAxes()[2][1]) );
  assert( eq(-1.0, disk->GetBinaryPrincipalAxes()[2][2]) );


//  assert( eq(, ball->Get()) );
//
//   assert( eq(, ball->Get()[0]) );
//   assert( eq(, ball->Get()[1]) );
//   assert( eq(, ball->Get()[2]) );

//  std::cout << ball->GetBinaryPrincipalAxes() << std::endl;

  // avoid warning when compiling in release
  ball = 0;
  ellipsoid = 0;
  disk = 0;

  return 0;
}

