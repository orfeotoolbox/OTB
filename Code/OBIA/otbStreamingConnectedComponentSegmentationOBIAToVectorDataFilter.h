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
#ifndef __otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter_h
#define __otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter_h

#include "otbPersistentImageToVectorDataFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "otbConnectedComponentMuParserFunctor.h"
#include "itkConnectedComponentFunctorImageFilter.h"
#include "otbMaskMuParserFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "otbLabelImageToLabelMapWithAdjacencyFilter.h"
#include "otbBandsStatisticsAttributesLabelMapFilter.h"
#include "otbShapeAttributesLabelMapFilter.h"
#include "otbLabelObjectOpeningMuParserFilter.h"
#include "otbLabelMapToVectorDataFilter.h"

namespace otb {

/** \class PersistentConnectedComponentSegmentationOBIAToVectorDataFilter
*   \brief [internal] Helper class to perform connected component segmentation on an input image,
*   apply OBIA filtering and output a VectorData
*
*  The whole chain is described in :
*  http://wiki.orfeo-toolbox.org/index.php/Connected_component_segmentation_module
*
*  This class wraps a whole processing chain based on connected component segmentation,
*  OBIA filtering and conversion to VectorData.
*  An optional mask can be applied to segment only the pixels inside the mask.
*
*  Parameters of the chain are :
*  - MaskExpression : mathematical expression to apply on the input image to make a mask
*  - ConnectedComponentExpression : mathematical expression which connects two pixels
*  - MinimumObjectSize : minimum object size kept after segmentation
*  - OBIAExpression : mathematical expression for OBIA filtering
*
* \ingroup Streamed
*/
template <class TVImage, class TLabelImage, class TMaskImage, class TOutputVectorData>
class PersistentConnectedComponentSegmentationOBIAToVectorDataFilter : public otb::PersistentImageToVectorDataFilter<TVImage, TOutputVectorData>
{
public:
  /** Standard Self typedef */
  typedef PersistentConnectedComponentSegmentationOBIAToVectorDataFilter      Self;
  typedef PersistentImageToVectorDataFilter<TVImage, TOutputVectorData >       Superclass;
  typedef itk::SmartPointer<Self>                                             Pointer;
  typedef itk::SmartPointer<const Self>                                       ConstPointer;

  typedef TVImage           VectorImageType;
  typedef TLabelImage       LabelImageType;
  typedef TMaskImage        MaskImageType;
  typedef TOutputVectorData VectorDataType;

  typedef typename VectorImageType::Pointer VectorImagePointerType;
  typedef typename VectorDataType::Pointer  VectorDataPointerType;

  typedef typename VectorImageType::PixelType VectorImagePixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentConnectedComponentSegmentationOBIAToVectorDataFilter, PersistentImageToVectorDataFilter);

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TVImage::ImageDimension);

  // Mask generation
  typedef Functor::ConnectedComponentMuParserFunctor<VectorImagePixelType> FunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<
      VectorImageType,
      LabelImageType,
      FunctorType,
      MaskImageType > ConnectedComponentFilterType;

  // mask typedef
  typedef otb::MaskMuParserFilter<VectorImageType, MaskImageType> MaskMuParserFilterType;

  // Labelization
  typedef itk::RelabelComponentImageFilter<LabelImageType, LabelImageType> RelabelComponentFilterType;
  typedef otb::AttributesMapLabelObject<unsigned int, InputImageDimension, double>   AttributesMapLabelObjectType;

  typedef otb::LabelMapWithAdjacency<AttributesMapLabelObjectType> AttributesLabelMapType;
  typedef otb::LabelImageToLabelMapWithAdjacencyFilter<LabelImageType, AttributesLabelMapType>
      LabelImageToLabelMapFilterType;

  typedef otb::BandsStatisticsAttributesLabelMapFilter<AttributesLabelMapType, VectorImageType>
      RadiometricLabelMapFilterType;
  typedef otb::ShapeAttributesLabelMapFilter<AttributesLabelMapType> ShapeLabelMapFilterType;

  typedef otb::LabelObjectOpeningMuParserFilter<AttributesLabelMapType>            LabelObjectOpeningFilterType;
  typedef otb::LabelMapToVectorDataFilter<AttributesLabelMapType, VectorDataType>  LabelMapToVectorDataFilterType;

  typedef typename RelabelComponentFilterType::ObjectSizeType ObjectSizeType;


  /* Set the mathematical expression used for the mask */
  itkSetStringMacro(MaskExpression);

  /* Get the mathematical expression used for the mask */
  itkGetStringMacro(MaskExpression);

  /* Set the mathematical expression used during connected component segmentation */
  itkSetStringMacro(ConnectedComponentExpression);

  /* Get the mathematical expression used during connected component segmentation */
  itkGetStringMacro(ConnectedComponentExpression);

  /* Set the minimum object size */
  itkSetMacro(MinimumObjectSize, ObjectSizeType);

  /* Get the minimum object size */
  itkGetMacro(MinimumObjectSize, ObjectSizeType);

  /* Set the mathematical expression for filtering labelobjects */
  itkSetStringMacro(OBIAExpression);

  /* Get the mathematical expression for filtering labelobjects */
  itkGetStringMacro(OBIAExpression);

  /* Set shape reduced set attributes flag for object attributes computing  */
  itkSetMacro(ShapeReducedSetOfAttributes, bool);

  /* Get shape reduced set attributes flag for object attributes computing  */
  itkGetMacro(ShapeReducedSetOfAttributes, bool);

  /* Set stat reduced set attributes flag for object attributes computing  */
  itkSetMacro(StatsReducedSetOfAttributes, bool);

  /* Get stat reduced set attributes flag for object attributes computing  */
  itkGetMacro(StatsReducedSetOfAttributes, bool);

  /* Set compute polygon flag for object attributes computing  */
  itkSetMacro(ComputePolygon, bool);

  /* Get compute polygon flag for object attributes computing  */
  itkGetMacro(ComputePolygon, bool);

  /* Set compute Flusser flag for object attributes computing  */
  itkSetMacro(ComputeFlusser, bool);

  /* Get compute Flusser flag for object attributes computing  */
  itkGetMacro(ComputeFlusser, bool);

  /* Set compute perimeter flag for object attributes computing  */
  itkSetMacro(ComputePerimeter, bool);

  /* Get compute perimeter flag for object attributes computing  */
  itkGetMacro(ComputePerimeter, bool);

  /* Set compute feret diameter flag for object attributes computing  */
  itkSetMacro(ComputeFeretDiameter, bool);

  /* Get compute FeretdDiameter flag for object attributes computing  */
  itkGetMacro(ComputeFeretDiameter, bool);


protected:
  PersistentConnectedComponentSegmentationOBIAToVectorDataFilter();

  virtual ~PersistentConnectedComponentSegmentationOBIAToVectorDataFilter();

  void GenerateInputRequestedRegion();
private:

  ObjectSizeType m_MinimumObjectSize;
  std::string    m_MaskExpression;
  std::string    m_ConnectedComponentExpression;
  std::string    m_OBIAExpression;

  // attributes
  bool m_ShapeReducedSetOfAttributes;
  bool m_StatsReducedSetOfAttributes;
  bool m_ComputeFlusser;
  bool m_ComputePolygon;
  bool m_ComputeFeretDiameter;
  bool m_ComputePerimeter;

  virtual VectorDataPointerType ProcessTile();
};

/** \class StreamingConnectedComponentSegmentationOBIAToVectorDataFilter
*   \brief Performs connected component segmentation on an input image,
*   apply OBIA filtering and output a VectorData, with streaming capabilities
*
*  This class wraps a whole processing chain based on connected component segmentation,
*  OBIA filtering and conversion to VectorData.
*  An optional mask can be applied to segment only the pixels inside the mask.
*
*  The whole chain is described in :
*  http://wiki.orfeo-toolbox.org/index.php/Connected_component_segmentation_module
*
*  The parameters of the processing chain must be applied on the internal filter,
*  which can be accessed with GetFilter. See PersistentConnectedComponentSegmentationOBIAToVectorDataFilter
*
*  Parameters of the chain are :
*  - MaskExpression : mathematical expression to apply on the input image to make a mask
*  - ConnectedComponentExpression : mathematical expression which connects two pixels
*  - MinimumObjectSize : minimum object size kept after segmentation
*  - OBIAExpression : mathematical expression for OBIA filtering
*
* \sa PersistentConnectedComponentSegmentationOBIAToVectorDataFilter
* \sa PersistentFilterStreamingDecorator
*
* \ingroup Streamed
*/
template <class TVImage, class TLabelImage, class TMaskImage, class TOutputVectorData>
class StreamingConnectedComponentSegmentationOBIAToVectorDataFilter
{
public:

  // define the PersistentStreamingLineSegmentDetector template
  typedef PersistentConnectedComponentSegmentationOBIAToVectorDataFilter<
        TVImage, TLabelImage, TMaskImage, TOutputVectorData>
    PersistentConnectedComponentSegmentationOBIAToVectorDataFilterType;

  typedef typename PersistentConnectedComponentSegmentationOBIAToVectorDataFilterType::VectorImageType VectorImageType;
  typedef typename PersistentConnectedComponentSegmentationOBIAToVectorDataFilterType::LabelImageType  LabelImageType;
  typedef typename PersistentConnectedComponentSegmentationOBIAToVectorDataFilterType::MaskImageType   MaskImageType;
  typedef typename PersistentConnectedComponentSegmentationOBIAToVectorDataFilterType::VectorDataType  VectorDataType;

  // typedef for streaming capable filter
  typedef PersistentFilterStreamingDecorator<PersistentConnectedComponentSegmentationOBIAToVectorDataFilterType>
    FilterType;
};

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.txx"
#endif

#endif
