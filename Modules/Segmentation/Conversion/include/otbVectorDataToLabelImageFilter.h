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
#ifndef __otbVectorDataToLabelImageFilter_h
#define __otbVectorDataToLabelImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageRegionIterator.h"
#include "otbMacro.h"

#include "otbVectorData.h"

#include "gdal.h"
#include "ogr_api.h"
#include "otbOGRVersionProxy.h"

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
  * \sa GetSpacing()
  */
  virtual void SetOutputSpacing(const OutputSpacingType& spacing);
  virtual void SetOutputSpacing(const double spacing[2]);
  virtual void SetOutputSpacing(const float spacing[2]);

  /** Set/Get Output Projection Ref */
  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);

  itkSetStringMacro(BurnAttribute);
  itkGetStringMacro(BurnAttribute);

  /** Useful to set the output parameters from an existing image*/
  void SetOutputParametersFromImage(const ImageBaseType * image);

protected:
  virtual void GenerateData();

  VectorDataToLabelImageFilter();
  virtual ~VectorDataToLabelImageFilter()
  {
    // Destroy the geometries stored
    for (unsigned int idx = 0; idx < m_SrcDataSetGeometries.size(); ++idx)
      {
      OGR_G_DestroyGeometry(m_SrcDataSetGeometries[idx]);
      }

    if (m_OGRDataSourcePointer != NULL)
      {
      ogr::version_proxy::Close(m_OGRDataSourcePointer);
      }
  }

  virtual void GenerateOutputInformation();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  VectorDataToLabelImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  ogr::version_proxy::GDALDatasetType * m_OGRDataSourcePointer;

  // Vector Of OGRGeometyH
  std::vector< OGRGeometryH >   m_SrcDataSetGeometries;

  std::vector<double>           m_BurnValues;
  std::vector<double>           m_FullBurnValues;
  std::vector<int>              m_BandsToBurn;

  // Field used to extract the burn value
  std::string                   m_BurnAttribute;

  // Default burn value
  double                        m_DefaultBurnValue;

  // Output params
  std::string                   m_OutputProjectionRef;
  OutputSpacingType             m_OutputSpacing;
  OutputOriginType              m_OutputOrigin;
  OutputSizeType                m_OutputSize;
  OutputIndexType               m_OutputStartIndex;
}; // end of class VectorDataToLabelImageFilter

} // end of namespace otb


#ifndef  OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToLabelImageFilter.txx"
#endif

#endif
