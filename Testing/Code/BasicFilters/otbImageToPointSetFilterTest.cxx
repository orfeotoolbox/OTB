/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   23 janvier 2006
  Role      :  ImageToPointSetFilter class testing
  $Id$ 

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImageToPointSetFilter.h"
#include "itkBinaryMaskToNarrowBandPointSetFilter.h"
#include "itkImage.h"

int otbImageToPointSetFilterTest(int , char *[] )
{


  const unsigned int Dimension = 2;

  typedef unsigned char  BinaryMaskPixelType;

  typedef itk::Image< BinaryMaskPixelType, 
                      Dimension  >           BinaryMaskImageType;


  // 
  //  Initialize an image with a white square in a black background
  //
  BinaryMaskImageType::Pointer binaryMask = BinaryMaskImageType::New();

  BinaryMaskImageType::SizeType     size;
  BinaryMaskImageType::IndexType    index;
  BinaryMaskImageType::RegionType   region;

  size[0] = 100;
  size[1] = 100;

  index[0] = 0;
  index[1] = 0;

  region.SetIndex( index );
  region.SetSize(  size );
  
  binaryMask->SetRegions( region );
  binaryMask->Allocate();

  binaryMask->FillBuffer( 0 );

  size[0] = 60;
  size[1] = 60;

  index[0] = 20;
  index[1] = 20;

  region.SetIndex( index );
  region.SetSize(  size );


  
  itk::ImageRegionIterator< BinaryMaskImageType > it( binaryMask, region );

  it.GoToBegin();
  while( !it.IsAtEnd() )
    {
    it.Set( 255 );
    ++it;
    }
  
#if 0


  //
  //  Set up the filter
  //
  typedef itk::PointSet< float, Dimension >    MeshType;

  typedef itk::BinaryMaskToNarrowBandPointSetFilter< 
                                BinaryMaskImageType,
                                MeshType
                                            >  GeneratorType;




  GeneratorType::Pointer narrowBandGenerator = GeneratorType::New();

  narrowBandGenerator->SetInput( binaryMask );



  try
    {
    narrowBandGenerator->Update();
    } 
  catch( itk::ExceptionObject & exp )
    {
    std::cerr << "Exception thrown during the excecution of the generator " << std::endl;
    std::cerr << exp << std::endl;
    return 1;
    }



  //
  //  Checking the output 
  //
  typedef PointSetType::PointType               PointType;

  typedef PointSetType::PointsContainer     PointsContainer;
  typedef PointsContainer::Pointer          PointsContainerPointer;
  typedef PointsContainer::ConstIterator    PointsIterator;

  typedef PointSetType::PointDataContainer  PointDataContainer;
  typedef PointDataContainer::Pointer       PointDataContainerPointer;
  typedef PointDataContainer::ConstIterator PointDataIterator;

//  PointSetType::Pointer                     pointSet  = narrowBandGenerator->GetOutput();
  PointSetType::Pointer                     pointSet;

  PointsContainerPointer      points    = pointSet->GetPoints();
  PointDataContainerPointer   pointData = pointSet->GetPointData();

  PointsIterator point     = points->Begin();
  PointsIterator lastPoint = points->End();

  PointDataIterator  data     = pointData->Begin();
  PointDataIterator  lastData = pointData->End();

  while( point != lastPoint  && data != lastData )
    {
      
    const PointType & p = point.Value();  
      
    binaryMask->TransformPhysicalPointToIndex( p, index );

    if( ( !binaryMask->GetPixel( index ) && data.Value() > 0 ) ||
        (  binaryMask->GetPixel( index ) && data.Value() < 0 )    )
      {
      std::cerr << "Pixel " << index << " shouldn't be in the narrow band" << std::endl;
      return EXIT_FAILURE;
      }

    ++point;
    ++data;
    }
  
#endif  


  return EXIT_SUCCESS;
}



