/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbShapeAttributesLabelMapFilter_h
#define __otbShapeAttributesLabelMapFilter_h

#include "itkLabelPerimeterEstimationCalculator.h"
#include "otbLabelObjectToPolygonFunctor.h"
#include "itkInPlaceLabelMapFilter.h"
#include "itkMatrix.h"
#include "itkVector.h"
#include "otbImage.h"
#include "otbPolygon.h"
#include "otbFlusserPathFunction.h"

//#include "itkObjectFactory.h"
  
namespace otb
{

/** \class ShapeAttributesLabelMapFilter
 * \brief This class is a fork of the itk::ShapeLabelMapFilter working with AttributesMapLabelObject.
 *
 * The keys of the attributes inserted into the attributes map follow
 * the following rule: "SHAPE:NameOfAttributes".
 *
 * With respect to the original itk filter, this filter also compute
 * the Flusser moments for each LabelObject.
 *
 * The flag ReducedAttributesSet allows to compute only non-redundant
 * statistics and statistics usefull for classification (not spatially
 * dependent statitistics). Reduced set contains only Flusser moments,
 * physical size, elongation, Feret diameter (if activated),
 * perimeter (if activated) and roundness (if activated).
 *
 * \sa itk::ShapeLabelMapFilter
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage, class TLabelImage=Image< typename TImage::PixelType, TImage::ImageDimension > >
class ITK_EXPORT ShapeAttributesLabelMapFilter :
    public itk::InPlaceLabelMapFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef ShapeAttributesLabelMapFilter                             Self;
  typedef itk::InPlaceLabelMapFilter<TImage>                        Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,TImage::ImageDimension);
  
  /** Standard New method. */
  itkNewMacro(Self);
  
  /** Runtime information support. */
  itkTypeMacro(ShapeAttributesLabelMapFilter,InPlaceLabelMapFilter);

  /** Some convenient typedefs. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer                               ImagePointer;
  typedef typename ImageType::ConstPointer                          ImageConstPointer;
  typedef typename ImageType::PixelType                             PixelType;
  typedef typename ImageType::IndexType                             IndexType;
  typedef typename ImageType::SizeType                              SizeType;
  typedef typename ImageType::RegionType                            RegionType;
  typedef typename ImageType::PointType                             PointType;
  typedef typename ImageType::LabelObjectType                       LabelObjectType;
  typedef typename itk::Matrix<double,
          TImage::ImageDimension,TImage::ImageDimension>            MatrixType;
  typedef typename itk::Vector<double,TImage::ImageDimension>       VectorType;
  typedef TLabelImage LabelImageType;
  typedef typename LabelImageType::Pointer                          LabelImagePointer;
  typedef typename LabelImageType::ConstPointer                     LabelImageConstPointer;
  typedef typename LabelImageType::PixelType                        LabelPixelType;
  typedef itk::LabelPerimeterEstimationCalculator< LabelImageType > PerimeterCalculatorType;
  
  /// Flusser moments typedef
  typedef Polygon<double>                                           PolygonType;
  typedef Functor::LabelObjectToPolygonFunctor<LabelObjectType,
                                          PolygonType>         PolygonFunctorType;
  typedef FlusserPathFunction<PolygonType>                          FlusserPathFunctionType;
  
  /**
   * Set/Get whether the maximum Feret diameter should be computed or not. The
   * defaut value is false, because of the high computation time required.
   */
  itkSetMacro(ComputeFeretDiameter, bool);
  itkGetConstReferenceMacro(ComputeFeretDiameter, bool);
  itkBooleanMacro(ComputeFeretDiameter);

  /**
   * Set/Get whether the perimeter should be computed or not. The defaut value
   * is false, because of the high computation time required.
   */
  itkSetMacro(ComputePerimeter, bool);
  itkGetConstReferenceMacro(ComputePerimeter, bool);
  itkBooleanMacro(ComputePerimeter);

  /** Set/get the ReducedAttributesSet flag */
  itkSetMacro(ReducedAttributeSet,bool);
  itkGetMacro(ReducedAttributeSet,bool);
  itkBooleanMacro(ReducedAttributeSet);

  /** Set the label image */
  void SetLabelImage( const TLabelImage *input )
     {
     m_LabelImage = input;
     }

protected:
  /** Constructor */
  ShapeAttributesLabelMapFilter();
  
  /** Destructor */
  ~ShapeAttributesLabelMapFilter() {};
  
  /** Threaded GenerateData */
  virtual void ThreadedProcessLabelObject( LabelObjectType * labelObject );
  virtual void BeforeThreadedGenerateData();
  virtual void AfterThreadedGenerateData();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ShapeAttributesLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Convenience internal method */
  static long factorial( long n );
  
  /** Convenience internal method */
  static long doubleFactorial( long n );

  /** Convenience internal method  */
  static double gammaN2p1( long n );
  
  /** Convenience internal method  */
  static double hyperSphereVolume( double radius );
  
  /** Convenience internal method  */
  static double hyperSpherePerimeter( double radius );
  
  /** Convenience internal method  */
  static double hyperSphereRadiusFromVolume( double volume );

  /** Do we compute the feret diameter ? */
  bool m_ComputeFeretDiameter;

  /** Du we compute the perimeter ? */
  bool m_ComputePerimeter;

  /** The label image */
  LabelImageConstPointer m_LabelImage;

  /** The perimeter calculator */
  typename PerimeterCalculatorType::Pointer m_PerimeterCalculator;

 /// Use only a reduced attribute set
  bool m_ReducedAttributeSet;

} ; // end of class

} // end namespace itk
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbShapeAttributesLabelMapFilter.txx"
#endif

#endif


