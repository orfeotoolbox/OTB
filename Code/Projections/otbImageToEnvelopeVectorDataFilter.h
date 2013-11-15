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
#ifndef __otbImageToEnvelopeVectorDataFilter_h
#define __otbImageToEnvelopeVectorDataFilter_h

#include "otbVectorDataSource.h"
#include "otbGenericRSTransform.h"

namespace otb
{

/** \class ImageToEnvelopeVectorDataFilter
  * \brief Build a vector data containing the polygon of the image envelope
  *
  * This filter uses the GenericRSTransform to project the four corners of the image into ground position.
  * If the sampling rate is not null, the image edges are also projected (using one point every
  * "m_SamplingRate" pixels).
  * In case of raw image geometry, a DEM directory or average elevation can be set for better accuracy.
  *
  * This filter supports user-specified output projection. If no projection is defined, the standard WGS84
  * projection will be used.
  *
  * \ingroup VectorDataFilter
  * \ingroup Projection
  *
  */
template <class TInputImage, class TOutputVectorData>
class ITK_EXPORT ImageToEnvelopeVectorDataFilter :
  public otb::VectorDataSource<TOutputVectorData>
{

public:
  /** Standard class typedefs. */
  typedef ImageToEnvelopeVectorDataFilter          Self;
  typedef otb::VectorDataSource<TOutputVectorData> Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  typedef TInputImage                              InputImageType;
  typedef TOutputVectorData                        OutputVectorDataType;
  typedef typename TInputImage::ConstPointer       InputImagePointer;
  typedef typename TOutputVectorData::Pointer      OutputVectorDataPointer;

  /** Some typedefs. */
  typedef otb::GenericRSTransform<double, 2, 2>    InternalTransformType;
  typedef typename InternalTransformType::Pointer  InternalTransformPointerType;
  typedef typename OutputVectorDataType
                   ::DataNodeType                  OutputDataNodeType;
  typedef typename OutputVectorDataType
                   ::DataNodePointerType           OutputDataNodePointerType;
  typedef typename OutputVectorDataType
                   ::DataTreePointerType           OutputDataTreePointerType;
  typedef typename OutputVectorDataType
                   ::DataTreeType::TreeNodeType    OutputInternalTreeNodeType;
  typedef typename OutputDataNodeType::PolygonType PolygonType;

  /** Set input image */
  void SetInput(const InputImageType *input);

  /** Get input image */
  const InputImageType * GetInput();

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToEnvelopeVectorDataFilter, VectorDataSource);

  /** Set/Get output projection (default is WGS84)  */
  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);

  itkSetMacro(SamplingRate, unsigned int);
  itkGetMacro(SamplingRate, unsigned int);

protected:
  ImageToEnvelopeVectorDataFilter();
  virtual ~ImageToEnvelopeVectorDataFilter() {}

  void GenerateOutputInformation(void);

  void GenerateInputRequestedRegion();

  void GenerateData(void);

  void InstantiateTransform();

private:
  ImageToEnvelopeVectorDataFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  InternalTransformPointerType m_Transform;
  std::string                  m_OutputProjectionRef;
  unsigned int                 m_SamplingRate;      // Sampling rate for edges (in pixels)
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToEnvelopeVectorDataFilter.txx"
#endif

#endif
