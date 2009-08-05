/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMapToAttributeImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbLabelMapToVectorDataFilter_h
#define __otbLabelMapToVectorDataFilter_h

//#include "itkImageToImageFilter.h"
//#include "itkAttributeLabelObject.h"
#include "otbVectorDataSource.h"
#include "itkLabelMap.h"
#include "otbPolygon.h"
#include "otbLabelObjectToPolygonFunctor.h"

//#include "itkMinimumMaximumImageCalculator.h"

namespace otb {

/** \class LabelMapToVectorDataFilter
   * \brief This class vectorizes a LabelObject to a VectorData.
 *
   * LabelMapToVectorDataFilter converts a LabelMap to a
 * VectorData where all the pixels get the attribute value of the label object they belong.
   * It uses the class otbLabelObjectToPolygonFunctor wich follows a finite states machine described in 
   *
   * "An algorithm for the rapid computation of boundaries of run-length
   * encoded regions", Francis K. H. Queck, in Pattern Recognition 33
   * (2000), p 1637-1649.  
 * \author Manuel GRIZONNET. CNES, France.
 *
 * \sa LabelMapToBinaryImageFilter, LabelMapMaskImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TLabelMap, class TVectorData >
class ITK_EXPORT LabelMapToVectorDataFilter : 
    public VectorDataSource< TVectorData >
{
public:
  /** Standard class typedefs. */
  typedef LabelMapToVectorDataFilter Self;
  typedef VectorDataSource< TVectorData >
  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TLabelMap InputLabelMapType;
  typedef TVectorData OutputVectorDataType;
  typedef typename OutputVectorDataType::Pointer        OutputVectorDataPointer;
  typedef typename OutputVectorDataType::ConstPointer   OutputVectorDataConstPointer;
  
  typedef typename InputLabelMapType::LabelObjectType   LabelObjectType;
  typedef otb::Polygon<double>                                   PolygonType;
      
  typedef otb::Functor::LabelObjectToPolygonFunctor<LabelObjectType,PolygonType> FunctorType;
  
  typedef typename OutputVectorDataType::DataNodeType  DataNodeType;
  typedef typename DataNodeType::Pointer         DataNodePointerType;
  
  //typedef itk::MinimumMaximumImageCalculator<LabeledImageType>           MinMaxCalculatorType;
  //typedef typename OutputVectorDataType::RegionType     OutputVectorDataRegionType;
  //typedef typename OutputVectorDataType::PixelType      OutputVectorDataPixelType;
  //typedef typename OutputVectorDataType::IndexType      IndexType;
  
  
  /** ImageDimension constants */
  //itkStaticConstMacro(InputImageDimension, unsigned int,
  //                    TInputImage::ImageDimension);
  //itkStaticConstMacro(OutputImageDimension, unsigned int,
  //                    TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  //itkTypeMacro(LabelMapToAttributeImageFilter, 
  //           ImageToImageFilter);

  /**
   * Set/Get the value used as "background" in the output image, if the input
   * LabelMap use a background.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  //itkSetMacro(BackgroundValue, OutputImagePixelType);
  //itkGetConstMacro(BackgroundValue, OutputImagePixelType);

  /** Set/Get the LabelMap input of this process object.  */
  virtual void SetInput( const InputLabelMapType *input);
  virtual void SetInput( unsigned int idx, const InputLabelMapType *input);
  const InputLabelMapType * GetInput(void);
  const InputLabelMapType * GetInput(unsigned int idx);
  
protected:
  LabelMapToVectorDataFilter();
  ~LabelMapToVectorDataFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** LabelMapToAttributeImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** LabelMapToAttributeImageFilter will produce the entire output. */
  //void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  

private:
  LabelMapToVectorDataFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  //OutputImagePixelType m_BackgroundValue;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "otbLabelMapToVectorDataFilter.txx"
#endif

#endif


