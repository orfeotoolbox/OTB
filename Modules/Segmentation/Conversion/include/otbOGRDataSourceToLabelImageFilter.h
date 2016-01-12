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
#ifndef __otbOGRDataSourceToLabelImageFilter_h
#define __otbOGRDataSourceToLabelImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbMacro.h"


#include "gdal.h"
#include "gdal_alg.h"
#include "otbOGRDataSourceWrapper.h"

namespace otb {

/** \class OGRDataSourceToLabelImageFilter
 *  \brief Burn geometries from the specified VectorData into raster
 *
 *  This class handles burning several input \c OGRDataSource into the
 *  output raster.  It has two different modes:
 *  - either the burn values are extracted from a field set by
 *    the user (default field is DN). This is the default behaviour,
 *  - or the burn values are made of foreground (when inside
 *    geometries) and background (outside geometries) values, which can
 *    be set by the user. One can set this behaviour by disable the
 *    BurnAttributeMode using the BurnAttributeModeOff() method.
 *
 *  Please note that the background value is also used in
 *  BurnAttributeModeOn() to fill areas where there are no geometries.
 *
 *  Setting the output raster information can be done in two ways by:
 *    - Setting the Origin/Size/Spacing of the output image
 *    - Using an existing image as support via SetOutputParametersFromImage(ImageBase)
 *
 *
 * \ingroup OTBConversion
 */
template < class TOutputImage >
class  ITK_EXPORT OGRDataSourceToLabelImageFilter :
    public itk::ImageSource<TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef OGRDataSourceToLabelImageFilter       Self;
  typedef itk::ImageSource<TOutputImage>        Superclass;
  typedef itk::SmartPointer< Self >             Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(OGRDataSourceToLabelImageFilter, itk::ImageSource);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TOutputImage                                OutputImageType;
  typedef typename OutputImageType::Pointer           OutputImagePointer;
  typedef typename OutputImageType::SizeType          OutputSizeType;
  typedef typename OutputImageType::IndexType         OutputIndexType;
  typedef typename OutputImageType::SpacingType       OutputSpacingType;
  typedef typename OutputImageType::PointType         OutputOriginType;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;
  typedef typename OutputImageType::PixelType         OutputImagePixelType;
  typedef typename OutputImageType::InternalPixelType OutputImageInternalPixelType;

  /** VectorData typedefs*/
  typedef ogr::DataSource                            OGRDataSourceType;
  typedef typename OGRDataSourceType::Pointer        OGRDataSourcePointerType;
  typedef ogr::Layer                                 OGRLayerType;

  typedef itk::ImageBase<OutputImageType::ImageDimension>      ImageBaseType;

  /** Get Nth input \c OGRDataSource */
  const OGRDataSourceType*  GetInput(unsigned int idx);

  /** Method for adding a \c OGRDataSource to rasterize  */
  virtual void AddOGRDataSource(const OGRDataSourceType* ds);

  /** Set the size of the output image. */
  itkSetMacro(OutputSize, OutputSizeType);

  /** Get the size of the output image. */
  itkGetConstReferenceMacro(OutputSize, OutputSizeType);

  /** Set the origin of the output image.
   * \sa GetOrigin()
   */
  itkSetMacro(OutputOrigin, OutputOriginType);
  virtual void SetOutputOrigin(const double origin[2]);
  virtual void SetOutputOrigin(const float origin[2]);

  itkGetConstReferenceMacro(OutputOrigin, OutputOriginType);

  /** Set the spacing (size of a pixel) of the output image.
  * \sa GetSpacing()
  */
  virtual void SetOutputSpacing(const OutputSpacingType& spacing);
  virtual void SetOutputSpacing(const double spacing[2]);
  virtual void SetOutputSpacing(const float spacing[2]);

  /** Set/Get Output Projection Ref */
  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);

  /** Set the attribute field on the \c OGRFeature to be used as burn value in the output image*/
  itkSetStringMacro(BurnAttribute);
  itkGetStringMacro(BurnAttribute);

  /** Set/Get the background value */
  itkSetMacro(BackgroundValue,OutputImageInternalPixelType);
  itkGetConstReferenceMacro(BackgroundValue,OutputImageInternalPixelType);

  /** Set/Get the foreground value */
  itkSetMacro(ForegroundValue,OutputImageInternalPixelType);
  itkGetConstReferenceMacro(ForegroundValue,OutputImageInternalPixelType);

  /** Set/Get the BurnAttributeMode flag */
  itkSetMacro(BurnAttributeMode,bool);
  itkGetConstReferenceMacro(BurnAttributeMode,bool);
  itkBooleanMacro(BurnAttributeMode);

  /** Useful to set the output parameters from an existing image*/
  void SetOutputParametersFromImage(const ImageBaseType * image);

protected:
  virtual void GenerateData();

  OGRDataSourceToLabelImageFilter();
  virtual ~OGRDataSourceToLabelImageFilter() {}

  virtual void GenerateOutputInformation();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  OGRDataSourceToLabelImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::vector< OGRLayerH >    m_SrcDataSetLayers;
  std::vector<int>            m_BandsToBurn;

  // Field used to extract the burn value
  std::string                   m_BurnAttribute;

  // Output params
  std::string                   m_OutputProjectionRef;
  OutputSpacingType             m_OutputSpacing;
  OutputOriginType              m_OutputOrigin;
  OutputSizeType                m_OutputSize;
  OutputIndexType               m_OutputStartIndex;
  OutputImageInternalPixelType  m_BackgroundValue;
  OutputImageInternalPixelType  m_ForegroundValue;
  bool                          m_BurnAttributeMode;
}; // end of class VectorDataToLabelImageFilter

} // end of namespace otb


#ifndef  OTB_MANUAL_INSTANTIATION
#include "otbOGRDataSourceToLabelImageFilter.txx"
#endif

#endif
