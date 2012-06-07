/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbPersistentImageToOGRDataFilter_h
#define __otbPersistentImageToOGRDataFilter_h

#include "otbPersistentImageFilter.h"

#include "otbLabelImageToOGRDataSourceFilter.h"
#include "otbOGRDataSourceWrapper.h"

#include "itkMacro.h"
#include <string>

namespace otb
{

/** \class PersistentImageToOGRDataFilter
 *  \brief Perform vectorization in a persistent way.
 *
 *  This filter is a generic filter. It is the base class of the Large scale segmentation framework.
 *  The \c Initialize() method is used to create the new layer (LayerName) in the input \c OGRDataSource,
 *  and the field (FieldName) in the layer. This field will contain the label of the polygon in the input image.
 *  The \c ProcessTile() method is pure virtual and is implemented in sub class
 *  (@see \c PersistentStreamingLabelImageToOGRDataFilter). It returns a "memory" DataSource.
 *  This filter only copy each feature of the layer in the "memory" DataSource into the LayerName layer in the
 *  input \c OGRDataSource set by \c SetOGRDataSource() method.
 *
 * \sa PersistentImageFilter
 *
 */
template<class TImage>
class ITK_EXPORT PersistentImageToOGRDataFilter :
  public PersistentImageFilter<TImage, TImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentImageToOGRDataFilter                  Self;
  typedef PersistentImageFilter<TImage, TImage>           Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(PersistentImageToOGRDataFilter, PersistentImageFilter);

  typedef TImage                                     InputImageType;
  typedef typename InputImageType::Pointer           InputImagePointer;
  typedef typename InputImageType::RegionType        RegionType;
  typedef typename InputImageType::SizeType          SizeType;
  typedef typename InputImageType::IndexType         IndexType;
  typedef typename InputImageType::PixelType         PixelType;
  typedef typename InputImageType::InternalPixelType InternalPixelType;

  typedef ogr::DataSource                            OGRDataSourceType;
  typedef typename OGRDataSourceType::Pointer        OGRDataSourcePointerType;
  typedef ogr::Layer                                 OGRLayerType;
  typedef ogr::Feature                               OGRFeatureType;


  void AllocateOutputs();
  virtual void Reset(void);
  virtual void Synthetize(void);
  
  /** This method creates the output layer in the OGRDataSource set by the user.
   * The name of the layer is set by \c SetLayerName .
   * \note This methode must be called before the call of Update .
   */
  virtual void Initialize(void);
  
  /** Set/Get the name of the output layer to write in the \c ogr::DataSource. */
  itkSetStringMacro(LayerName);
  itkGetStringMacro(LayerName);
  
  /** Set the Field Name in which labels will be written. (default is "DN")
   * A field FieldName is created in the output layer LayerName. The Field type is Integer.
   */
  itkSetMacro(FieldName, std::string);
  
  /**
   * Return the Field name in which labels have been written.
   */
  itkGetMacro(FieldName, std::string);
  
  /** Get the size of the tile used for streaming.
   * This is useful if you use for example the \c OGRFusionTileFilter
   * for fusioning streaming tiles.
   */
  itkGetMacro(StreamSize, SizeType);
  
  /** Set the \c ogr::DataSource in which the layer LayerName will be created. */
  void SetOGRDataSource( OGRDataSourcePointerType ogrDS );
  /** Get the \c ogr::DataSource output. */
  OGRDataSourceType * GetOGRDataSource( void );

  /** Add one option for OGR layer creation */
  void AddOGRLayerCreationOption(const std::string& option);

  /** Clear all OGR layer creation options */
  void ClearOGRLayerCreationOptions();
  
  /** Set the OGR layer creation options */
  void SetOGRLayerCreationOptions(const std::vector<std::string> & options);

protected:
  PersistentImageToOGRDataFilter();
  virtual ~PersistentImageToOGRDataFilter();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateData();


private:
  PersistentImageToOGRDataFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  virtual OGRDataSourcePointerType ProcessTile() = 0;
  
  std::string m_FieldName;
  std::string m_LayerName;
  OGRwkbGeometryType m_GeometryType;
  SizeType m_StreamSize;
  std::vector<std::string> m_OGRLayerCreationOptions;

}; // end of class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPersistentImageToOGRDataFilter.txx"
#endif

#endif
