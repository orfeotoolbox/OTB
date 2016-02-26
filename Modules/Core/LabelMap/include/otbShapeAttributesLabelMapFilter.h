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

#include "otbLabelMapFeaturesFunctorImageFilter.h"
#include "otbImage.h"
#include "otbPolygon.h"
#include "otbLabelObjectToPolygonFunctor.h"
#include "otbFlusserPathFunction.h"
#include "otbSimplifyPathFunctor.h"


namespace otb
{

namespace Functor
{
/** \class ShapeAttributesLabelObjectFunctor
*   \brief Functor to compute shape attributes of one LabelObject.
*
*   \sa ShapeAttributesLabelMapFilter
 *
 * \ingroup OTBLabelMap
*/
template <class TLabelObject, class TLabelImage>
class ShapeAttributesLabelObjectFunctor
{
public:
  /** Self typedef */
  typedef ShapeAttributesLabelObjectFunctor Self;

  /** LabelObject typedef */
  typedef TLabelObject LabelObjectType;

  /** Const iterator over LabelObject lines */
  typedef typename LabelObjectType::ConstLineIterator  ConstLineIteratorType;
  /** Labeled image type */
  typedef TLabelImage LabelImageType;

  /** Flusser moments typedef */
  typedef typename TLabelObject::PolygonType PolygonType;
  typedef Functor::LabelObjectToPolygonFunctor
  <LabelObjectType, PolygonType>           PolygonFunctorType;
  typedef FlusserPathFunction<PolygonType> FlusserPathFunctionType;
  typedef SimplifyPathFunctor<PolygonType,
                              PolygonType> SimplifyPolygonFunctorType;

  typedef unsigned int DimensionType;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TLabelObject::ImageDimension);
  typedef itk::ImageRegion< TLabelObject::ImageDimension > RegionType;
  typedef itk::Offset< TLabelObject::ImageDimension > OffsetType;
  /** Constructor */
  ShapeAttributesLabelObjectFunctor();

  /** Destructor */
  virtual ~ShapeAttributesLabelObjectFunctor() {}

  /** The comparators */
  bool operator !=(const Self& self);
  bool operator ==(const Self& self);

  /** Set the compute perimeter flag */
  void SetComputePerimeter(bool flag);

  /** Get the compute perimeter flag */
  bool GetComputePerimeter() const;

  /** Set the compute perimeter flag */
  void SetComputeFlusser(bool flag);

  /** Get the compute perimeter flag */
  bool GetComputeFlusser() const;

  /** Set the polygonalisation flag */
  void SetComputePolygon(bool flag);

  /** Get the polygonalisation flag */
  bool GetComputePolygon() const;

  /** Set the compute feret diameter flag */
  void SetComputeFeretDiameter(bool flag);

  /** Get the compute feret diameter flag */
  bool GetComputeFeretDiameter() const;

  /** Set the compute reduced attributes set flag */
  void SetReducedAttributeSet(bool flag);

  /** Get the compute reduced attributes set flag */
  bool GetReducedAttributeSet() const;

  /** Set the label image (used only to compute
   *  the Feret diameter */
  void SetLabelImage(const TLabelImage * image);

  /** Get the label image */
  const TLabelImage * GetLabelImage() const;

  /** This is the functor implementation
   *  Calling the functor on a label object
   *  will update its shape attributes */
  inline void operator ()(LabelObjectType * lo);

private:
  /** Convenience internal method */
  static long factorial(long n);

  /** Convenience internal method */
  static long doubleFactorial(long n);

  /** Convenience internal method  */
  static double gammaN2p1(long n);

  /** Convenience internal method  */
  static double hyperSphereVolume(double radius);

  /** Convenience internal method  */
  static double hyperSpherePerimeter(double radius);

  /** Convenience internal method  */
  static double hyperSphereRadiusFromVolume(double volume);

  double ComputePerimeter(LabelObjectType *labelObject, const RegionType & region);

  typedef itk::Offset<2>                                                          Offset2Type;
  typedef itk::Offset<3>                                                          Offset3Type;
  typedef itk::Vector<double, 2>                                                  Spacing2Type;
  typedef itk::Vector<double, 3>                                                  Spacing3Type;
  typedef std::map<Offset2Type, itk::SizeValueType, Offset2Type::LexicographicCompare> MapIntercept2Type;
  typedef std::map<Offset3Type, itk::SizeValueType, Offset3Type::LexicographicCompare> MapIntercept3Type;

  template<class TMapIntercept, class TSpacing> double PerimeterFromInterceptCount( TMapIntercept & intercepts, const TSpacing & spacing );

#if ! defined(ITK_DO_NOT_USE_PERIMETER_SPECIALIZATION)
  double PerimeterFromInterceptCount( MapIntercept2Type & intercepts, const Spacing2Type spacing );
  double PerimeterFromInterceptCount( MapIntercept3Type & intercepts, const Spacing3Type spacing );
#endif

  /** Do we compute the feret diameter ? */
  bool m_ComputeFeretDiameter;

  /** Do we compute the perimeter ? */
  bool m_ComputePerimeter;

  /** Do we compute flusser moments ? */
  bool m_ComputeFlusser;

  /** Do we polygonise ? */
  bool m_ComputePolygon;

  /** Compute only a reduced attribute set */
  bool m_ReducedAttributeSet;

  /** The label image is used to compute the feret diameter */
  typename LabelImageType::ConstPointer m_LabelImage;
};

} // End namespace Functor

/** \class ShapeAttributesLabelMapFilter
 * \brief This class is a fork of the itk::ShapeLabelMapFilter working with AttributesMapLabelObject.
 *
 * The keys of the attributes inserted into the attributes map follow
 * the following rule: "SHAPE:NameOfAttributes".
 *
 * With respect to the original itk filter, this filter also compute
 * the Flusser moments for each LabelObject.
 *
 * The flag ReducedAttributesSet allows computing only non-redundant
 * statistics and statistics usefull for classification (not spatially
 * dependent statitistics). Reduced set contains only Flusser moments,
 * physical size, elongation, Feret diameter (if activated),
 * perimeter (if activated) and roundness (if activated).
 *
 * For the sake of speed optimization, the original label image must
 * be given to the filter. If not, the filter will convert the
 * LabelObjectMap back to a LabelImage structure.
 *
 * \sa itk::ShapeLabelMapFilter
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 *
 * \ingroup OTBLabelMap
 */
template<class TImage,
    class TLabelImage = Image<typename TImage::PixelType,
        TImage::ImageDimension> >
class ITK_EXPORT ShapeAttributesLabelMapFilter
  : public otb::LabelMapFeaturesFunctorImageFilter
  <TImage,
      typename Functor::ShapeAttributesLabelObjectFunctor
      <typename TImage::LabelObjectType, TLabelImage> >
{
public:
  /** Template parameters typedefs */
  typedef TImage                              ImageType;
  typedef typename ImageType::LabelObjectType LabelObjectType;
  typedef typename ImageType::RegionType      InputImageRegionType;
  typedef TLabelImage                         LabelImageType;
  typedef Functor::ShapeAttributesLabelObjectFunctor
  <LabelObjectType, LabelImageType>                      FunctorType;

  /** Standard class typedefs. */
  typedef ShapeAttributesLabelMapFilter Self;
  typedef LabelMapFeaturesFunctorImageFilter
  <ImageType, FunctorType>                               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename ImageType::Pointer         ImagePointer;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ShapeAttributesLabelMapFilter, LabelMapFeaturesFunctorImageFilter);

  /**
   * Set/Get whether the maximum Feret diameter should be computed or not. The
   * defaut value is false, because of the high computation time required.
   */
  void SetComputeFeretDiameter(bool flag);
  bool GetComputeFeretDiameter() const;
  itkBooleanMacro(ComputeFeretDiameter);

  /**
   * Set/Get whether the perimeter should be computed or not. The defaut value
   * is false, because of the high computation time required.
   */
  void SetComputePerimeter(bool flag);
  bool GetComputePerimeter() const;
  itkBooleanMacro(ComputePerimeter);

  /**
   * Set/Get whether the  polygonalisation process should be computed or not. The default value
   * is true, to assure backward compatibility.
   */
  void SetComputePolygon(bool flag);
  bool GetComputePolygon() const;
  itkBooleanMacro(ComputePolygon);

  /**
    * Set/Get whether the Flussrer moments should be computed or not. The default value
    * is true, to assure backward compatibility.
    */
   void SetComputeFlusser(bool flag);
   bool GetComputeFlusser() const;
   itkBooleanMacro(ComputeFlusser);


  /** Set/get the ReducedAttributesSet flag */
  void SetReducedAttributeSet(bool flag);
  bool GetReducedAttributeSet() const;
  itkBooleanMacro(ReducedAttributeSet);

  /** Set/Get the label image (this is used only to compute the Feret
   *  diameter) */
  void SetLabelImage(const TLabelImage *);
  const TLabelImage * GetLabelImage() const;

protected:
  /** Constructor */
  ShapeAttributesLabelMapFilter(){}

  /** Destructor */
  virtual ~ShapeAttributesLabelMapFilter(){}

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void AllocateOutputs();

  /** Things to to before threaded data generation */
  virtual void BeforeThreadedGenerateData();

  void GenerateInputRequestedRegion();

  void EnlargeOutputRequestedRegion(itk::DataObject *){};

private:
  ShapeAttributesLabelMapFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbShapeAttributesLabelMapFilter.txx"
#endif

#endif
