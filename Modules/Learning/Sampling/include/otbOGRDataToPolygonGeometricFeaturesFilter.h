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

#ifndef otbOGRDataToPolygonGeometricFeaturesFilter_h
#define otbOGRDataToPolygonGeometricFeaturesFilter_h

#include "otbOGRDataSourceWrapper.h"
#include "itkProcessObject.h"

namespace otb
{
/**
 * \class OGRDataToPolygonGeometricFeaturesFilter
 * 
 * \brief Computes Geometric features (perimter and size) on ogr polygons.
 * 
 * This filter takes an OGR Data Source as input and computes size and perimeter of each polygon in it.
 * Tbese features are then written as attributes of the polygon in the output Data Source.
 * The name of the attributes can be specified using the setters 'SetSizeField' and 
 * 'SetPerimeterField'.
 * 
 * This filter is able to work in place, updating the output Data-Source.
 * 
 * \ingroup OTBSampling
 * 
 */

class ITK_EXPORT OGRDataToPolygonGeometricFeaturesFilter
  : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef OGRDataToPolygonGeometricFeaturesFilter  Self;
  typedef itk::ProcessObject                Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** OGR related typedefs. */
  typedef ogr::DataSource                            OGRDataSourceType;
  typedef typename OGRDataSourceType::Pointer        OGRDataPointer;
  typedef ogr::Layer                                 OGRLayerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information. */
  itkTypeMacro(OGRDataToPolygonGeometricFeaturesFilter, itk::ProcessObject);
  
  
  /** Set/Get the input OGRDataSource of this process object.  */
  using Superclass::SetInput;
  virtual void SetInput(const OGRDataSourceType* ds);
  const OGRDataSourceType*  GetInput(unsigned int idx = 0);

  /**
   * Get the output \c ogr::DataSource which is a "memory" datasource.
   */
  const OGRDataSourceType * GetOutput();
  virtual void SetOutput(const OGRDataSourceType* ds);
  
  /** Set/Get macro for the layer name */
  itkSetMacro(OutLayerName, std::string);
  itkGetMacro(OutLayerName, std::string);

  /** Set/Get macro for the size field name*/
  itkSetMacro(SizeField, std::string);
  itkGetMacro(SizeField, std::string);

  /** Set/Get macro for the perimeter field name */
  itkSetMacro(PerimeterField, std::string);
  itkGetMacro(PerimeterField, std::string);


protected:
  /** Constructor */
  OGRDataToPolygonGeometricFeaturesFilter();
  /** Destructor */
  ~OGRDataToPolygonGeometricFeaturesFilter() override;
  /** Main computation method */
  void GenerateData() override;

  /** Utility method to add new fields on an output layer */
  virtual void InitializeOutputDataSource(ogr::DataSource* inputDS, ogr::DataSource* outputDS);

  /** Multi-threading implementation */

  virtual void BeforeThreadedGenerateVectorData();
  
  /** Method to split the input OGRDataSource between several containers
   *  for each thread. Default is to put the same number of features for
   *  each thread.*/
  virtual void DispatchInputVectors(void);

  /** Allocate in-memory layers for input and outputs */
  void AllocateOutputs(void);

  
  /** Gather the content of in-memory output layer into the filter outputs */
  virtual void GatherOutputVectors(void);
  
  /** Fill output vectors for a particular output */
  virtual void FillOneOutput( ogr::DataSource* outDS, bool update);

  virtual void AfterThreadedGenerateVectorData();

  /** Start of main processing loop */
  virtual void ThreadedGenerateVectorData(const ogr::Layer& layerForThread, itk::ThreadIdType threadid);

  /** Static function used as a "callback" by the MultiThreader.  The threading
   * library will call this routine for each thread, which will delegate the
   * control to ThreadedGenerateData(). */
  static ITK_THREAD_RETURN_TYPE VectorThreaderCallback(void *arg);

  /** Internal structure used for passing image data into the threading library */
  struct VectorThreadStruct
  {
    Pointer Filter;
  };

  /** End Multi-threading implementation */
  
  typedef struct {
    std::string Name;
    OGRFieldType Type;
    int Width;
    int Precision;
    } SimpleFieldDefn;
  
  /** Create a new additional field */
  void CreateAdditionalField(std::string name,
                             OGRFieldType type,
                             int width=0,
                             int precision=0);

  /** Get a reference over the additional fields */
  const std::vector<SimpleFieldDefn>& GetAdditionalFields();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  
  /** Give access to in-memory input layers */
  ogr::Layer GetInMemoryInput(unsigned int threadId);

  /** Give access to in-memory output layers */
  ogr::Layer GetInMemoryOutput(unsigned int threadId);
  
private:
  OGRDataToPolygonGeometricFeaturesFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
  
  /** name of the output layers */
  std::string m_OutLayerName;
  
  /** In-memory containers storing input geometries for each thread*/
  std::vector<OGRDataPointer> m_InMemoryInputs;

  /** In-memory containers storing position during iteration loop*/
  std::vector<OGRDataPointer> m_InMemoryOutputs;
  
  /** Creation option for output layers */
  std::vector<std::string> m_OGRLayerCreationOptions;

  /** Additional field definitions to add in output data sources */
  std::vector<SimpleFieldDefn> m_AdditionalFields;

  /** Size field name*/
  std::string m_SizeField;
  
  /** Perimeter field name */
  std::string m_PerimeterField;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRDataToPolygonGeometricFeaturesFilter.hxx"
#endif

#endif
