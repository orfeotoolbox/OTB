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

#ifndef PersistentMaskFilter_h
#define PersistentMaskFilter_h

#include "otbPersistentImageToOGRLayerFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbImage.h"





// ----------------------------
#include <itkImageRegionConstIteratorWithIndex.h>
//-----------


namespace otb
{
/**
 * \class PersistentMaskFilter
 * 
 * \brief Persistent filter to extract sample values from an image
 * 
 * \ingroup OTBSampling
 */
template<class TInputImage>
class ITK_EXPORT PersistentMaskFilter :
  public PersistentImageToOGRLayerFilter<TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentMaskFilter            Self;
  typedef PersistentImageToOGRLayerFilter<TInputImage>     Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef TInputImage                                     InputImageType;
  typedef typename InputImageType::Pointer                InputImagePointer;
  typedef typename InputImageType::RegionType             RegionType;
  typedef typename InputImageType::PointType              PointType;
  typedef typename InputImageType::IndexType              IndexType;
  typedef typename InputImageType::SizeType               SizeType;
  typedef typename InputImageType::PixelType              PixelType;
  typedef typename InputImageType::InternalPixelType      InternalPixelType;
  
  typedef otb::LabelImageToOGRDataSourceFilter<InputImageType>          LabelImageToOGRDataSourceFilterType;
  typedef typename Superclass::OGRDataSourceType                        OGRDataSourceType;
  typedef typename Superclass::OGRDataSourcePointerType                 OGRDataSourcePointerType;
  typedef typename Superclass::OGRLayerType                             OGRLayerType;
  typedef typename Superclass::OGRFeatureType                           OGRFeatureType;

  typedef itk::DataObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentMaskFilter, PersistentSamplingFilterBase);

  void Synthetize(void) ITK_OVERRIDE{}

  /** Reset method called before starting the streaming*/
  void Reset(void) ITK_OVERRIDE;
  
  /** Get the sample names */
  const std::vector<std::string> & GetSampleFieldNames();
  
  itkSetStringMacro(FieldName);
  itkGetStringMacro(FieldName);

  /**
   * Set the value of 8-connected neighborhood option used in \c LabelImageToOGRDataSourceFilter
   */
  itkSetMacro(Use8Connected, bool);
  /**
   * Get the value of 8-connected neighborhood option used in \c LabelImageToOGRDataSourceFilter
   */
  itkGetMacro(Use8Connected, bool);
/*
  itkSetMacro(Labels, std::vector<int>);
  itkGetMacro(Labels, std::vector<int>);
*/

  void SetLabels( const std::vector<int> & labels)
  {
    m_Labels = labels;
  }

  const std::vector<int> & GetLabels() const
  {
    return m_Labels;
  }
protected:
  /** Constructor */
  PersistentMaskFilter();
  /** Destructor */
  ~PersistentMaskFilter() ITK_OVERRIDE {}
  
  //void GenerateData() ITK_OVERRIDE;

  void GenerateOutputInformation() ITK_OVERRIDE;

  void GenerateInputRequestedRegion() ITK_OVERRIDE;

private:
  PersistentMaskFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  
  OGRDataSourcePointerType ProcessTile() ITK_OVERRIDE;
  int m_TileNumber;
  std::string m_FieldName;
  std::vector<int> m_Labels;
  bool m_Use8Connected;
};


//--------------------------------------------------------------------------------------------------------------

template <class TImageType>
class ITK_EXPORT MaskFilter :
public PersistentFilterStreamingDecorator<PersistentMaskFilter<TImageType>>
{

public:
  /** Standard Self typedef */
  typedef MaskFilter                                                                    Self;
  typedef PersistentFilterStreamingDecorator<PersistentMaskFilter<TImageType>>           Superclass;
  typedef itk::SmartPointer<Self>                                                       Pointer;
  typedef itk::SmartPointer<const Self>                                                 ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(MaskFilter, PersistentFilterStreamingDecorator);

  typedef TImageType                               InputImageType;
  //typedef typename PersistentMaskFilter<TImageType>::LabelPixelType                           LabelPixelType;
  //typedef typename PersistentMaskFilter<TImageType>::LabelImageType                           LabelImageType;
  typedef typename PersistentMaskFilter<TImageType>::OGRDataSourcePointerType                 OGRDataSourcePointerType;
  typedef typename PersistentMaskFilter<TImageType>::OGRLayerType                             OGRLayerType;

  typedef typename InputImageType::SizeType                                   SizeType;
 /** Set the input image. */
 // using Superclass::SetInput;
  void SetInput(InputImageType * input)
  {
    this->GetFilter()->SetInput(input);
  }
  const InputImageType * GetInput()
  {
    return this->GetFilter()->GetInput();
  }

  /** Set the \c ogr::Layer in which the layer LayerName will be created. */
  void SetOGRLayer( const OGRLayerType & ogrLayer )
  {
    this->GetFilter()->SetOGRLayer(ogrLayer);
  }

  void SetFieldName( const std::string & fieldName )
  {
    this->GetFilter()->SetFieldName(fieldName);
  }

  const std::string & GetFieldName() const
  {
    return this->GetFilter()->GetFieldName();
  }
 
  void SetLabels( const std::vector<int> & labels)
  {
    this->GetFilter()->SetLabels(labels);
  }

  const std::vector<int> & GetLabels() const
  {
    return this->GetFilter()->GetLabels();
  }
  /** Retrieve the actual streamsize used */
  SizeType GetStreamSize()
  {
    return this->GetFilter()->GetStreamSize();
  }

  void Initialize()
  {
     this->GetFilter()->Initialize();
  }
  /**
   * Set the value of 8-connected neighborhood option used in \c LabelImageToOGRDataSourceFilter
   */
  void SetUse8Connected(bool flag)
  {
     this->GetFilter()->SetUse8Connected(flag);
  }

  bool GetUse8Connected()
  {
     return this->GetFilter()->GetUse8Connected();
  }
protected:
  /** Constructor */
  MaskFilter() {}
  /** Destructor */
  ~MaskFilter() ITK_OVERRIDE {}

private:
  MaskFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};




} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "PersistentMaskFilter.txx"
#endif

#endif
