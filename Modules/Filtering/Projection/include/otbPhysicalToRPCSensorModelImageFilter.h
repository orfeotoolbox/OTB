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

#ifndef otbPhysicalToRPCSensorModelImageFilter_h
#define otbPhysicalToRPCSensorModelImageFilter_h

#include "otbGCPsToRPCSensorModelImageFilter.h"
#include "otbGenericRSTransform.h"


namespace otb {

/** \class PhysicalToRPCSensorModelImageFilter
 * \brief This filter estimates a RPC sensor models from a physical model.
 *
 * This filters estimates an RPC sensor model from a list of points
 * homologous point in the physical coordinate system and the WGS84
 * CS.
 *
 * The number of points used to estimate the rpc sensor model is
 * defined by the GridSize, note that at least 16 points are needed
 * to estimate a proper RPC sensor model, there are no warning when
 * the model estimated is inaccurate.
 *
 * The UseImageGCPs flag allows importing GCPs from the image
 * metadata, if any.(TODO)
 *
 * GCPs can be passed to the filter using one of the AddGCP method
 * implementation.(TODO)
 *
 * Depending on the value of the DEMDirectory, an elevation fetched
 * from the SRT directory is used.(TODO)
 *
 * This filter does not modify the image buffer, but only the
 * metadata. Therefore, it provides in-place support, which is
 * enabled by default. Call InPlaceOff() to change the default
 * behavior.
 *
 * \ingroup Projections
 *
 * \ingroup OTBProjection
 */
template <class TImage>
class ITK_EXPORT PhysicalToRPCSensorModelImageFilter :
  public itk::CastImageFilter<TImage, TImage>
{
public:
  /** Standard class typedefs. */
  typedef PhysicalToRPCSensorModelImageFilter Self;
  typedef itk::CastImageFilter<TImage, TImage> Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PhysicalToRPCSensorModelImageFilter, CastImageFilter);

  /** template parameters typedef */
  typedef TImage                          ImageType;
  typedef typename ImageType::PointType   PointType;
  typedef typename ImageType::SizeType    SizeType;

  /** GCPsToSensor model filter */
  typedef GCPsToRPCSensorModelImageFilter<ImageType>          GCPsToSensorModelType;
  typedef typename GCPsToSensorModelType::Pointer             GCPsToSensorModelPointerType;
  typedef typename GCPsToSensorModelType::Point2DType         Point2DType;
  typedef typename GCPsToSensorModelType::Point3DType         Point3DType;
  typedef typename GCPsToSensorModelType::GCPType             GCPType;
  typedef typename GCPsToSensorModelType::GCPsContainerType   GCPsContainerType;
  typedef typename GCPsToSensorModelType::ErrorsContainerType ErrorsContainerType;

  /** Remote Sensing Transform */
  typedef GenericRSTransform<>                                RSTransformType;
  typedef typename RSTransformType::Pointer                   RSTransformPointerType;

  /** Set/Get the grid size*/
  itkSetMacro(GridSize, SizeType);
  itkGetConstReferenceMacro(GridSize, SizeType);

  /** Set Grid size with an unsigned int parmeter*/
  void SetGridSize(unsigned int inSize)
  {
    SizeType iSize;
    iSize.Fill(inSize);

    if(m_GridSize != iSize)
      {
      m_GridSize = iSize;
      this->Modified();
      }
  }

  /** Reimplement the method Modified() */
  void Modified() const override;

protected:
  /** Constructor */
  PhysicalToRPCSensorModelImageFilter();
  /** Destructor */
  ~PhysicalToRPCSensorModelImageFilter() override;

  /** The PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Generate the Output image information*/
  void GenerateOutputInformation() override;

private:
  PhysicalToRPCSensorModelImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** The rpc model estimator */
  GCPsToSensorModelPointerType       m_GCPsToSensorModelFilter;

  SizeType                           m_GridSize;
  mutable bool                       m_OutputInformationGenerated;

};
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPhysicalToRPCSensorModelImageFilter.hxx"
#endif

#endif
