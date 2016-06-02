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
#ifndef __otbPersistentSamplingFilterBase_h
#define __otbPersistentSamplingFilterBase_h

#include "otbPersistentImageFilter.h"
#include "otbOGRDataSourceWrapper.h"

namespace otb
{
/** \class PersistentSamplingFilterBase
 *  \brief Base class for persistent filter doing sampling tasks
 *
 *  \note This class contains pure virtual method, and can not be instantiated.
 *
 * \sa PersistentOGRDataToClassStatisticsFilter
 * \sa PersistentOGRDataToSamplePositionFilter
 *
 * \ingroup OTBSampling
 */
template <class TInputImage, class TMaskImage>
class ITK_EXPORT PersistentSamplingFilterBase
  : public otb::PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard typedefs */
  typedef PersistentSamplingFilterBase                       Self;
  typedef PersistentImageFilter<TInputImage, TInputImage>   Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Creation through object factory macro */
  itkTypeMacro(PersistentSamplingFilterBase, PersistentImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage  InputImageType;
  typedef TMaskImage   MaskImageType;

  typedef typename TInputImage::RegionType RegionType;

  typedef ogr::DataSource::Pointer OGRDataPointer;

  /** Set the input OGRDataSource that contains sampling areas for each class*/
  void SetOGRData(const ogr::DataSource* vector);

  /** Get the input OGRDataSource with sampling regions*/
  const ogr::DataSource* GetOGRData();

  /** Set an input mask (optional) */
  void SetMask(const TMaskImage* mask);

  /** Get the input mask (may be null) */
  const TMaskImage* GetMask();

  /** Set the OGR layer creation options */
  void SetOGRLayerCreationOptions(const std::vector<std::string> & options);

  /** Set/Get macro for the field name containing class names
   * in the input vectors.*/
  itkSetMacro(FieldName, std::string);
  itkGetMacro(FieldName, std::string);

  /** Set/Get macro for the layer index containing the sampling areas */
  itkSetMacro(LayerIndex, int);
  itkGetMacro(LayerIndex, int);

  /**
   * Reset the persistent data of the filter.
   */
  virtual void Reset(void) = 0;
  /**
   * Synthesize the persistent data of the filter.
   */
  virtual void Synthetize(void) = 0;

protected:
  /** Constructor */
  PersistentSamplingFilterBase();
  /** Destructor */
  virtual ~PersistentSamplingFilterBase() {}

  virtual void BeforeThreadedGenerateData(void);

  virtual void AfterThreadedGenerateData(void);

  virtual void ThreadedGenerateData(const RegionType&, itk::ThreadIdType threadid);

  void ExploreGeometry(ogr::Feature& feature,
                       OGRGeometry* geom,
                       RegionType& region,
                       itk::ThreadIdType& threadid);

  virtual void ProcessLine(ogr::Feature& feature,
                           OGRLineString* line,
                           RegionType& region,
                           itk::ThreadIdType& threadid);

  virtual void ProcessPolygon(ogr::Feature& feature,
                              OGRPolygon* polygon,
                              RegionType& region,
                              itk::ThreadIdType& threadid);

  virtual void ProcessSample(ogr::Feature& feature,
                             typename TInputImage::IndexType& imgIndex,
                             typename TInputImage::PointType& imgPoint,
                             itk::ThreadIdType& threadid);

  //bool IsSampleInsidePolygon();

  bool IsSampleOnLine(OGRLineString* line,
                      typename TInputImage::PointType& position,
                      typename TInputImage::SpacingType& absSpacing,
                      OGRPolygon& tmpPolygon);

  /** Get the region bounding a set of features */
  RegionType FeatureBoundingRegion(const TInputImage* image, otb::ogr::Layer::const_iterator& featIt) const;

  virtual void PrepareInputVectors();

  virtual void PrepareOutputVectors();

  /** In-memory containers storing input geometries for each thread*/
  std::vector<OGRDataPointer> m_InMemoryInputs;

  /** In-memory containers storing position during iteration loop*/
  std::vector<std::vector<OGRDataPointer> > m_InMemoryOutputs;


private:
  PersistentSamplingFilterBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Field name containing the class name*/
  std::string m_FieldName;

  /** Layer to use in the input vector file, default to 0 */
  int m_LayerIndex;

  /** (internal) name of the layer at position 'm_LayerIndex' */
  std::string m_LayerName;

  /** Creation option for output layers */
  std::vector<std::string> m_OGRLayerCreationOptions;

};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPersistentSamplingFilterBase.txx"
#endif

#endif
