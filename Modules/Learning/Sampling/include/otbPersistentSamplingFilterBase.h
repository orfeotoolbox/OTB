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
#ifndef otbPersistentSamplingFilterBase_h
#define otbPersistentSamplingFilterBase_h

#include "otbPersistentImageFilter.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbImage.h"

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
template <class TInputImage, class TMaskImage = otb::Image<unsigned char,2> >
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

  /** Get the OGR layer creation options */
  const std::vector<std::string>& GetOGRLayerCreationOptions();

  /** Set/Get macro for the field name containing class names
   * in the input vectors.*/
  itkSetMacro(FieldName, std::string);
  itkGetMacro(FieldName, std::string);

  /** Get macro for the field index (deduced from the field name) */
  itkGetMacro(FieldIndex, int);

  /** Set/Get macro for the layer index containing the sampling areas */
  itkSetMacro(LayerIndex, int);
  itkGetMacro(LayerIndex, int);

  /** Set/Get macro for the layer name */
  itkSetMacro(OutLayerName, std::string);
  itkGetMacro(OutLayerName, std::string);

protected:
  /** Constructor */
  PersistentSamplingFilterBase();
  /** Destructor */
  virtual ~PersistentSamplingFilterBase() {}

  /** Use the same output information as input image, check the field index
   *  and the mask footprint */
  virtual void GenerateOutputInformation();

  /** Use an empty region to input image (pixel values not needed) and set
   *  the requested region for the mask */
  virtual void GenerateInputRequestedRegion();

  /** Generate data should thread over */
  virtual void GenerateData(void);

  /** Allocate in-memory layers for input and outputs */
  virtual void AllocateOutputs(void);

  /** Start of main processing loop */
  virtual void ThreadedGenerateVectorData(const ogr::Layer& layerForThread, itk::ThreadIdType threadid);

  /** Process a geometry, recursive method when the geometry is a collection */
  void ExploreGeometry(const ogr::Feature& feature,
                       OGRGeometry* geom,
                       RegionType& region,
                       itk::ThreadIdType& threadid);

  /** Process a line string : use pixels that cross the line */
  virtual void ProcessLine(const ogr::Feature& feature,
                           OGRLineString* line,
                           RegionType& region,
                           itk::ThreadIdType& threadid);

  /** Process a polygon : use pixels inside the polygon */
  virtual void ProcessPolygon(const ogr::Feature& feature,
                              OGRPolygon* polygon,
                              RegionType& region,
                              itk::ThreadIdType& threadid);

  /** Generic method called for each matching pixel position (NOT IMPLEMENTED)*/
  virtual void ProcessSample(const ogr::Feature& feature,
                             typename TInputImage::IndexType& imgIndex,
                             typename TInputImage::PointType& imgPoint,
                             itk::ThreadIdType& threadid);

  /** Generic method called once before processing each feature */
  virtual void PrepareFeature(const ogr::Feature& feature,
                              itk::ThreadIdType& threadid);

  /** Common function to test if a point is inside a polygon */
  bool IsSampleInsidePolygon(OGRPolygon* poly,
                             OGRPoint* tmpPoint);

  /** Common function to test if a pixel crosses the line */
  bool IsSampleOnLine(OGRLineString* line,
                      typename TInputImage::PointType& position,
                      typename TInputImage::SpacingType& absSpacing,
                      OGRPolygon& tmpPolygon);

  /** Get the region bounding a set of features */
  RegionType FeatureBoundingRegion(const TInputImage* image, otb::ogr::Layer::const_iterator& featIt) const;

  /** Method to split the input OGRDataSource between several containers
   *  for each thread. Default is to put the same number of features for
   *  each thread.*/
  virtual void DispatchInputVectors(void);

  /** Gather the content of in-memory output layer into the filter outputs */
  virtual void GatherOutputVectors(void);

  /** Utility method to add new fields on an output layer */
  virtual void InitializeOutputDataSource(ogr::DataSource* inputDS, ogr::DataSource* outputDS);

  typedef struct {
    std::string Name;
    OGRFieldType Type;
    int Width;
    int Precision;
    } SimpleFieldDefn;

  /** Clear current additional fields */
  void ClearAdditionalFields();

  /** Create a new additional field */
  void CreateAdditionalField(std::string name,
                             OGRFieldType type,
                             int width=0,
                             int precision=0);

  /** Get a reference over the additional fields */
  const std::vector<SimpleFieldDefn>& GetAdditionalFields();

  /** Callback function to launch VectorThreadedGenerateData in each thread */
  static ITK_THREAD_RETURN_TYPE VectorThreaderCallback(void *arg);

  /** basically the same struct as itk::ImageSource::ThreadStruct */
  struct VectorThreadStruct
    {
      Pointer Filter;
    };

  /** Give access to in-memory input layers */
  ogr::Layer GetInMemoryInput(unsigned int threadId);

  /** Give access to in-memory output layers */
  ogr::Layer GetInMemoryOutput(unsigned int threadId, unsigned int index=0);

private:
  PersistentSamplingFilterBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Field name containing the class name*/
  std::string m_FieldName;

  /** Field index corresponding to the field name m_FieldName */
  int m_FieldIndex;

  /** Layer to use in the input vector file, default to 0 */
  int m_LayerIndex;

  /** name of the output layers */
  std::string m_OutLayerName;

  /** Creation option for output layers */
  std::vector<std::string> m_OGRLayerCreationOptions;

  /** Additional field definitions to add in output data sources */
  std::vector<SimpleFieldDefn> m_AdditionalFields;

  /** In-memory containers storing input geometries for each thread*/
  std::vector<OGRDataPointer> m_InMemoryInputs;

  /** In-memory containers storing position during iteration loop*/
  std::vector<std::vector<OGRDataPointer> > m_InMemoryOutputs;

};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPersistentSamplingFilterBase.txx"
#endif

#endif
