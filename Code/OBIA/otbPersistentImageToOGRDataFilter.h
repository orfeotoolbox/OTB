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


namespace otb
{

/** \class PersistentImageToOGRDataFilter
 *  \brief Perform vectorization in a persistent way.
 *
 *  This filter is a generic PersistentImageFilter, which encapsulate any filter
 *  which produces OGR data from an input Image.
 *
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
  
  virtual void Initialize(void);
  
  /** Set/Get the name of the output layer to write in the input ogrDataSource. */
  itkSetStringMacro(LayerName);
  itkGetStringMacro(LayerName);
  
  /** Set/Get the Field Name of the ogr file in which labels will be written. (default is "DN")*/
  itkSetMacro(FieldName, std::string);
  itkGetMacro(FieldName, std::string);
  
  /** Get the size of the tile used for streaming. */
  itkGetMacro(StreamSize, SizeType);
  
  /** Set/Get the input OGRDataSource */
  void SetOGRDataSource( OGRDataSourcePointerType ogrDS );
  OGRDataSourceType * GetOGRDataSource( void );


protected:
  PersistentImageToOGRDataFilter();
  virtual ~PersistentImageToOGRDataFilter();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateData();
  
  void GenerateInputRequestedRegion();


private:
  PersistentImageToOGRDataFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  virtual OGRDataSourcePointerType ProcessTile() = 0;
  
  std::string m_FieldName;
  std::string m_LayerName;
  OGRwkbGeometryType m_GeometryType;
  SizeType m_StreamSize;
  //OGRDataSourcePointerType m_DataSource;

}; // end of class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPersistentImageToOGRDataFilter.txx"
#endif

#endif
