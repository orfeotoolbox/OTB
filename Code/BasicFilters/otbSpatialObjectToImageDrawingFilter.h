/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSpatialObjectToImageDrawingFilter_h
#define __otbSpatialObjectToImageDrawingFilter_h

#include "itkImageSource.h"
#include "itkSpatialObject.h"

namespace otb
{
  
/** \class SpatialObjectToImageDrawingFilter
 * \brief 
 *
 */
template <class TInputSpatialObject, class TOutputImage>
class ITK_EXPORT SpatialObjectToImageDrawingFilter : public itk::ImageSource<TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef SpatialObjectToImageDrawingFilter  Self;
  typedef itk::ImageSource<TOutputImage>     Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(SpatialObjectToImageDrawingFilter,itk::ImageSource);

  /** Template parameters typedef */
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::ValueType   ValueType;
  typedef typename OutputImageType::SpacingType SpacingType;
  typedef typename OutputImageType::SizeType    SizeType;
  typedef typename OutputImageType::PointType   PointType;

  typedef TInputSpatialObject InputSpatialObjectType;
  typedef typename InputSpatialObjectType::Pointer        InputSpatialObjectPointer;
  typedef typename InputSpatialObjectType::ConstPointer   InputSpatialObjectConstPointer;
  typedef typename TInputSpatialObject::ChildrenListType  ChildrenListType;

  /** Spatial objects related typedefs */
  typedef itk::SpatialObject<InputSpatialObjectType::ObjectDimension> SpatialObjectType;
  typedef typename SpatialObjectType::ChildrenListType* ChildrenType;
  typedef typename SpatialObjectType::ChildrenListType::iterator IteratorType;
  
  /** Superclass typedefs. */
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;


  /** ImageDimension constants */
  itkStaticConstMacro(ObjectDimension, unsigned int,
                      InputSpatialObjectType::ObjectDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Set/Get the image input of this process object.  */
  virtual void SetInput( const InputSpatialObjectType *object);
  virtual void SetInput( unsigned int, const InputSpatialObjectType * object);
  const InputSpatialObjectType * GetInput(void);
  const InputSpatialObjectType * GetInput(unsigned int idx);

  /** Spacing (size of a pixel) of the output image. The
   * spacing is the geometric distance between image samples.
   * It is stored internally as double, but may be set from
   * float. \sa GetSpacing() */
  itkGetConstReferenceMacro(Spacing,SpacingType);
  virtual void SetSpacing( const SpacingType& spacing);
  virtual void SetSpacing( const double* spacing);
  virtual void SetSpacing( const float* spacing);

  /** Set/Get the value for pixels inside the spatial object. 
  * By default, this filter will return an image
  * that contains values from the spatial object specified as input. 
  * If this "inside" value is changed to a non-null value,
  * the output produced by this filter will be a mask with inside/outside values 
  * specified by the user. */
  itkSetMacro(InsideValue, ValueType);
  itkGetMacro(InsideValue, ValueType);

  /** Set/Get the value for pixels outside the spatial object.
  * By default, this filter will return an image
  * that contains values from the spatial object specified as input. 
  * If this "outside" value is changed to a non-null value,
  * the output produced by this filter will be a mask with inside/outside values
  * specified by the user. */
  itkSetMacro(OutsideValue, ValueType);
  itkGetMacro(OutsideValue, ValueType);

  /** The origin of the output image. The origin is the geometric
   * coordinates of the index (0,0,...,0).  It is stored internally
   * as double but may be set from float.
   * \sa GetOrigin() */
  virtual void SetOrigin( const PointType& origin);
  virtual void SetOrigin( const double* origin);
  virtual void SetOrigin( const float* origin);
  itkGetConstReferenceMacro(Origin,PointType);
 

  /** The spatial object being transformed can be part of a hierarchy.
   * How deep in the hierarchy should we descend in generating the
   * image?  A ChildrenDepth of 0 means to only include the object
   * itself. */
  itkSetMacro(ChildrenDepth, unsigned int);
  itkGetMacro(ChildrenDepth, unsigned int);

  /** Set/Get Size */
  virtual void SetSize(const SizeType& size);
  itkGetMacro(Size,SizeType);

  /** If UseObjectValue is set to true, then the filter uses
   *  the ValueAt() function instead of IsInside() */
  itkSetMacro(UseObjectValue,bool);
  itkGetMacro(UseObjectValue,bool);
  
protected:
  /** Constructor */
  SpatialObjectToImageDrawingFilter();
  /** Destructor */
  ~SpatialObjectToImageDrawingFilter();
  /** Main computation method */
  virtual void GenerateData();
  /** Printself method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Dummy GenerateOuptutInformation */
  virtual void GenerateOutputInformation(void){}
  private:
  SpatialObjectToImageDrawingFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  SizeType     m_Size;
  SpacingType  m_Spacing;
  PointType    m_Origin;
  unsigned int m_ChildrenDepth;
  ValueType    m_InsideValue;
  ValueType    m_OutsideValue;
  bool         m_UseObjectValue;
  bool         m_OriginSpecified;
  bool         m_SpacingSpecified;
  bool         m_SizeSpecified;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpatialObjectToImageDrawingFilter.txx"
#endif

#endif
