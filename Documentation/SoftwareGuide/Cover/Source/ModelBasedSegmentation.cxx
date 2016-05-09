/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: ModelBasedSegmentation.cxx,v $
  Language:  C++
  Date:      $Date: 2005/05/24 14:27:47 $
  Version:   $Revision: 1.8 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "itkEllipseSpatialObject.h"
#include "itkImageToSpatialObjectRegistrationMethod.h"
#include "itkImageToSpatialObjectMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkTranslationTransform.h"
#include "itkOnePlusOneEvolutionaryOptimizer.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkNormalVariateGenerator.h" 
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"



template < class TOptimizer >
class IterationCallback : public itk::Command 
{

public:
  typedef IterationCallback   Self;
  typedef itk::Command  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  itkTypeMacro( IterationCallback, Superclass );
  itkNewMacro( Self );

  /** Type defining the optimizer */
  typedef    TOptimizer     OptimizerType;

  /** Set Optimizer */
  void SetOptimizer( OptimizerType * optimizer )
  { 
    m_Optimizer = optimizer;
    m_Optimizer->AddObserver( itk::IterationEvent(), this );
  }


  /** Execute method will print data at each iteration */
  void Execute(itk::Object *caller, const itk::EventObject & event)
  {
    Execute( (const itk::Object *)caller, event);
  }

  void Execute(const itk::Object *, const itk::EventObject & event)
  {
    if( typeid( event ) == typeid( itk::StartEvent ) )
      {
      std::cout << std::endl << "Position              Value";
      std::cout << std::endl << std::endl;
      }    
    else if( typeid( event ) == typeid( itk::IterationEvent ) )
      {
      std::cout << m_Optimizer->GetCurrentIteration() << "   ";
      std::cout << m_Optimizer->GetValue() << "   ";
      std::cout << m_Optimizer->GetCurrentPosition() << std::endl;
      }
    else if( typeid( event ) == typeid( itk::EndEvent ) )
      {
      std::cout << std::endl << std::endl;
      std::cout << "After " << m_Optimizer->GetCurrentIteration();
      std::cout << "  iterations " << std::endl;
      std::cout << "Solution is    = " << m_Optimizer->GetCurrentPosition();
      std::cout << std::endl;
      }

  }


protected:
  IterationCallback() {};
  itk::WeakPointer<OptimizerType>   m_Optimizer;
 
};






template <typename TFixedImage, typename TMovingSpatialObject>
class SimpleImageToSpatialObjectMetric : 
    public itk::ImageToSpatialObjectMetric<TFixedImage,TMovingSpatialObject>
{
public:

  /** Standard class typedefs. */
  typedef SimpleImageToSpatialObjectMetric  Self;
  typedef itk::ImageToSpatialObjectMetric<TFixedImage,TMovingSpatialObject>  
                                                                     Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TFixedImage::ImageDimension);

  typedef itk::Point<double,ImageDimension>   PointType;
  typedef std::list<PointType> PointListType;
  typedef TMovingSpatialObject MovingSpatialObjectType;
  typedef typename Superclass::ParametersType ParametersType;
  typedef typename Superclass::DerivativeType DerivativeType;
  typedef typename Superclass::MeasureType    MeasureType;
  typedef typename TFixedImage::IndexType     IndexType;
  typedef typename TFixedImage::RegionType    RegionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(SimpleImageToSpatialObjectMetric, ImageToSpatialObjectMetric);

  enum { SpaceDimension = 3 };

  /** Connect the MovingSpatialObject */
  void SetMovingSpatialObject( const MovingSpatialObjectType * object)
  {
    if(!m_FixedImage)
    {
      std::cout << "Please set the image before the moving spatial object" << std::endl;
      return;
    }
    m_MovingSpatialObject = object;
    m_PointList.clear();
    typedef itk::ImageRegionConstIteratorWithIndex<TFixedImage> myIteratorType;

    RegionType region;
    if( m_FixedImageRegionSetByUser )
      {
      region = m_FixedImageRegion;
      }
    else
      {
      region = m_FixedImage->GetBufferedRegion();
      }

    myIteratorType it(m_FixedImage,region);

    PointType point;

    while(!it.IsAtEnd())
      {
      m_FixedImage->TransformIndexToPhysicalPoint( it.GetIndex(), point );
      if(m_MovingSpatialObject->IsInside(point,99999))
        { 
        m_PointList.push_back(point);
        }    
      ++it;
      }

    std::cout << "Number of points in the metric = " << static_cast<unsigned long>( m_PointList.size() ) << std::endl;
  }

  unsigned int GetNumberOfParameters(void) const  {return SpaceDimension;};

  /** Get the Derivatives of the Match Measure */
  void GetDerivative( const ParametersType &,
                                    DerivativeType & ) const
  {
    return;
  }


  void SetFixedImageRegion( const RegionType & region )
    {
    m_FixedImageRegionSetByUser = true;
    m_FixedImageRegion = region;
    }

  /** Get the Value for SingleValue Optimizers */

  MeasureType    GetValue( const ParametersType & parameters ) const
  {   
    double value;
    m_Transform->SetParameters( parameters );
    
    typename PointListType::const_iterator it = m_PointList.begin();
    
    typename TFixedImage::RegionType region = m_FixedImage->GetBufferedRegion();

    IndexType index;

    value = 0;
    while(it != m_PointList.end())
      {
      PointType transformedPoint = m_Transform->TransformPoint(*it);
      m_FixedImage->TransformPhysicalPointToIndex(transformedPoint,index);
      if( region.IsInside( index ) )
        {
        value += m_FixedImage->GetPixel(index);
        }
      it++;
      }
//    std::cout << "GetValue( " << parameters << " )  = " << value << std::endl; 
    return value;
  }

  /** Get Value and Derivatives for MultipleValuedOptimizers */
  void GetValueAndDerivative( const ParametersType & parameters,
       MeasureType & Value, DerivativeType  & Derivative ) const
  {
    Value = this->GetValue(parameters);
    this->GetDerivative(parameters,Derivative);
  }

protected:
  SimpleImageToSpatialObjectMetric()
    {
    m_FixedImageRegionSetByUser = false;  
    }
  ~SimpleImageToSpatialObjectMetric() {}

private:

  PointListType    m_PointList;

  RegionType       m_FixedImageRegion;
  bool             m_FixedImageRegionSetByUser;

};






int main( int argc, char *argv[] )
{
  if( argc != 2 )
  {
    std::cerr << "Usage: " << argv[0] << "InputImageFilename" << std::endl;
  }


  const unsigned int Dimension = 3;

  typedef itk::EllipseSpatialObject< Dimension >   EllipseType;


  typedef itk::Image< float, Dimension >      ImageType;

  EllipseType::Pointer ellipse = EllipseType::New();


  EllipseType::ArrayType axis;
  axis[0] =  6;
  axis[1] =  3;
  axis[2] =  6;


  ellipse->SetRadius( axis );


  typedef itk::ImageToSpatialObjectRegistrationMethod<
                                      ImageType,
                                      EllipseType  >  RegistrationType;

  RegistrationType::Pointer registration = RegistrationType::New();


  typedef SimpleImageToSpatialObjectMetric<  ImageType,
                                             EllipseType   > MetricType;

  MetricType::Pointer metric = MetricType::New();


  typedef itk::LinearInterpolateImageFunction< 
                                         ImageType,
                                         double     >  InterpolatorType;

  InterpolatorType::Pointer interpolator = InterpolatorType::New();


  typedef itk::OnePlusOneEvolutionaryOptimizer  OptimizerType;

  OptimizerType::Pointer optimizer  = OptimizerType::New();


  typedef itk::TranslationTransform< double, Dimension > TransformType; 
  TransformType::Pointer transform = TransformType::New();


  itk::Statistics::NormalVariateGenerator::Pointer generator 
                      = itk::Statistics::NormalVariateGenerator::New();


  generator->Initialize(12345);

  optimizer->SetNormalVariateGenerator( generator );
  optimizer->Initialize( 5 );
  optimizer->SetMaximumIteration( 1000 );

 
  TransformType::ParametersType parametersScale(Dimension);

  parametersScale.Fill( 2.0 );

  optimizer->SetScales( parametersScale );


  typedef IterationCallback< OptimizerType >   IterationCallbackType;

  IterationCallbackType::Pointer callback = IterationCallbackType::New();

  callback->SetOptimizer( optimizer );

  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer  reader  = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();


  // Place the ellipse close to the optimal position
  typedef ImageType::IndexType IndexType;
  IndexType initialIndexPosition;

  initialIndexPosition[0] = 26; 
  initialIndexPosition[1] = 18;
  initialIndexPosition[2] = 30;

  TransformType::InputPointType initialPhysicalPosition;

  reader->GetOutput()->TransformIndexToPhysicalPoint( 
                                  initialIndexPosition, 
                                  initialPhysicalPosition );
  
  EllipseType::TransformType::OffsetType offset;
  for(unsigned int i=0; i<Dimension; i++)
    {
    offset[i] = initialPhysicalPosition[i];
    }

  ellipse->GetObjectToParentTransform()->SetOffset(offset);
  ellipse->ComputeObjectToWorldTransform();


  typedef ImageType::RegionType    RegionType;
  typedef ImageType::IndexType     IndexType;
  typedef ImageType::SizeType      SizeType;

  RegionType fixedRegion;

  IndexType  fixedRegionStart;
  SizeType   fixedRegionSize;

  fixedRegionStart[0] = 20;
  fixedRegionStart[1] = 15;
  fixedRegionStart[2] = 20;

  fixedRegionSize[0] = 20;
  fixedRegionSize[1] = 10;
  fixedRegionSize[2] = 25;
  
  fixedRegion.SetSize(  fixedRegionSize  );
  fixedRegion.SetIndex( fixedRegionStart );

  metric->SetFixedImageRegion( fixedRegion );
  
  
  registration->SetFixedImage( reader->GetOutput() );
  registration->SetMovingSpatialObject( ellipse );
  registration->SetTransform( transform );
  registration->SetInterpolator( interpolator );
  registration->SetOptimizer( optimizer );
  registration->SetMetric( metric );

  TransformType::ParametersType initialParameters = transform->GetParameters();

  initialParameters.Fill( 0.0 );
 

  registration->SetInitialTransformParameters(initialParameters);

  std::cout << "Initial Parameters  : " << initialParameters << std::endl;

  optimizer->MaximizeOn();


  try {
    registration->StartRegistration();
    }
  catch( itk::ExceptionObject & exp ) {
    std::cerr << "Exception caught ! " << std::endl;
    std::cerr << exp << std::endl;
    }


  RegistrationType::ParametersType finalParameters 
                         = registration->GetLastTransformParameters();

  std::cout << "Final Solution is : " << finalParameters << std::endl;

  return 0;

}



