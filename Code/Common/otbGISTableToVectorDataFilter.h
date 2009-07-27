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
#ifndef __otbGISTableToVectorDataFilter_h
#define __otbGISTableToVectorDataFilter_h

//#include "itkImageToImageFilter.h"
//#include "itkAttributeLabelObject.h"
#include "otbVectorDataSource.h"
#include "otbGISTable.h"
//include "otbVectorData.h"

namespace otb {

/** \class GISTableToVectorDataFilter
 * \brief Convert a LabelMap to a VectorData
 *
   * GISTableToVectorDataFilter converts a GIS Table (PostGIS...) to a
 * VectorData.
 *
 * \author Manuel GRIZONNET. CNES, France.
 *
 * \sa LabelMapToBinaryImageFilter, LabelMapMaskImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TGISTable, class TVectorData >
class ITK_EXPORT GISTableToVectorDataFilter : 
    public VectorDataSource< TVectorData >
{
public:
  /** Standard class typedefs. */
  typedef GISTableToVectorDataFilter Self;
  typedef VectorDataSource< TVectorData >
  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TGISTable InputGISTableType;
  typedef TVectorData OutputVectorDataType;
  //typedef typename InputImageType::Pointer         InputImagePointer;
  //typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  //typedef typename InputImageType::RegionType      InputImageRegionType;
  //typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename OutputVectorDataType::Pointer        OutputVectorDataPointer;
  typedef typename OutputVectorDataType::ConstPointer   OutputVectorDataConstPointer;
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
  virtual void SetInput( const InputGISTableType *input);
  virtual void SetInput( unsigned int idx, const InputGISTableType *input);
  const InputGISTableType * GetInput(void);
  const InputGISTableType * GetInput(unsigned int idx);
  
protected:
  GISTableToVectorDataFilter();
  ~GISTableToVectorDataFilter() {};
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
  GISTableToVectorDataFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  //OutputImagePixelType m_BackgroundValue;

} ; // end of class

} // end namespace otb
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "otbGISTableToVectorDataFilter.txx"
#endif

#endif


