/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbVectorDataToLabelImageFilter_h
#define otbVectorDataToLabelImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageRegionIterator.h"
#include "otbMacro.h"

#include "otbVectorData.h"

#include "gdal.h"
#include "ogr_api.h"
#include "otbOGRVersionProxy.h"
#include <string>

namespace otb {

/** \class VectorDataToLabelImageFilter
 *  \brief Burn geometries from the specified VectorData into raster
 *
 *  This class handles burning several input VectorDatas into the
 *  output raster.  The burn values are extracted from a field set by
 *  the user.If no burning field is set, the "FID" is used by default.
 *
 *  Setting the output raster information can be done in two ways by:
 *    - Setting the Origin/Size/Spacing of the output image
 *    - Using an existing image as support via SetOutputParametersFromImage(ImageBase)
 *
 *  OGRRegisterAll() method must have been called before applying filter.
 *
 *
 * \ingroup OTBConversion
 */
template <class TVectorData, class TOutputImage  >
class  ITK_EXPORT VectorDataToLabelImageFilter :
    public itk::ImageSource<TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef VectorDataToLabelImageFilter                           Self;
  typedef itk::ImageSource<TOutputImage>     Superclass;
  typedef itk::SmartPointer< Self >                           Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataToLabelImageFilter, itk::ImageSource);

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
  typedef TVectorData                            VectorDataType;
  typedef typename VectorDataType::DataTreeType  DataTreeType;
  typedef typename DataTreeType::TreeNodeType    InternalTreeNodeType;
  typedef typename DataTreeType::Pointer         DataTreePointerType;
  typedef typename DataTreeType::ConstPointer    DataTreeConstPointerType;

  typedef itk::ImageBase<OutputImageType::ImageDimension>      ImageBaseType;

  /** Get Nth input VectorData */
  const VectorDataType*  GetInput(unsigned int idx);

  /** Method for adding VectorData to rasterize  */
  virtual void AddVectorData(const VectorDataType* vd);

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
  * \sa GetSignedSpacing()
  */
  virtual void SetOutputSpacing(const OutputSpacingType& spacing);
  virtual void SetOutputSpacing(const double spacing[2]);
  virtual void SetOutputSpacing(const float spacing[2]);

  /** Set/Get Output Projection Ref */
  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);

  itkSetStringMacro(BurnAttribute);
  itkGetStringMacro(BurnAttribute);

  /** Set the background value */
  itkSetMacro(BackgroundValue, OutputImageInternalPixelType);
  itkGetMacro(BackgroundValue, OutputImageInternalPixelType);

  /** Set the default burn value */
  itkSetMacro(DefaultBurnValue, OutputImageInternalPixelType);
  itkGetMacro(DefaultBurnValue, OutputImageInternalPixelType);

  /** Set/Get the AllTouchedMode flag */
  itkSetMacro(AllTouchedMode,bool);
  itkGetConstReferenceMacro(AllTouchedMode,bool);
  itkBooleanMacro(AllTouchedMode);

  /** Useful to set the output parameters from an existing image*/
  void SetOutputParametersFromImage(const ImageBaseType * image);

protected:
  void GenerateData() override;

  VectorDataToLabelImageFilter();
  ~VectorDataToLabelImageFilter() override
  {
    // Destroy the geometries stored
    for (unsigned int idx = 0; idx < m_SrcDataSetGeometries.size(); ++idx)
      {
      OGR_G_DestroyGeometry(m_SrcDataSetGeometries[idx]);
      }

    if (m_OGRDataSourcePointer != nullptr)
      {
      ogr::version_proxy::Close(m_OGRDataSourcePointer);
      }
  }

  void GenerateOutputInformation() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  VectorDataToLabelImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  GDALDataset * m_OGRDataSourcePointer;

  // Vector Of OGRGeometyH
  std::vector< OGRGeometryH >   m_SrcDataSetGeometries;

  std::vector<double>           m_BurnValues;
  std::vector<double>           m_FullBurnValues;
  std::vector<int>              m_BandsToBurn;

  // Field used to extract the burn value
  std::string                   m_BurnAttribute;

  // Default burn value
  double                        m_DefaultBurnValue;

  // Background value
  OutputImageInternalPixelType  m_BackgroundValue;

  // All touched mode
  bool                          m_AllTouchedMode;

  // Output params
  std::string                   m_OutputProjectionRef;
  OutputSpacingType             m_OutputSpacing;
  OutputOriginType              m_OutputOrigin;
  OutputSizeType                m_OutputSize;
  OutputIndexType               m_OutputStartIndex;
}; // end of class VectorDataToLabelImageFilter

} // end of namespace otb


#ifndef  OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToLabelImageFilter.hxx"
#endif

#endif
