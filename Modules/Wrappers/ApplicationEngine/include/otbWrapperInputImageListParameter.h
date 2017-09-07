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

#ifndef otbWrapperInputImageListParameter_h
#define otbWrapperInputImageListParameter_h

#include "otbWrapperParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperStringListInterface.h"

namespace otb
{
namespace Wrapper
{
/** \class InputImageListParameter
 *  \brief This class represents a list of InputImage parameter
 *
 * \ingroup OTBApplicationEngine
 */

class OTBApplicationEngine_EXPORT InputImageListParameter :
    public Parameter,
    public StringListInterface
{
public:
  /** Standard class typedef */
  typedef InputImageListParameter Self;
  typedef Parameter Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef std::vector< InputImageParameter::Pointer > InputImageParameterVectorType;

  typedef itk::ImageBase< 2 > ImageBaseType;

  /** Defining ::New() static method */
  itkNewMacro( Self );

  /** RTTI support */
  itkTypeMacro( InputImageListParameter, Parameter );

  /** Set image form a list of filename */
  void SetListFromFileName( const StringVector & filenames ) ITK_OVERRIDE;

  /** */
  void InsertNullElement( std::size_t = -1 ) ITK_OVERRIDE;

  /** Add an image from a filename */
  void AddFromFileName( const std::string & filename ) ITK_OVERRIDE;

  /** */
  void Insert( const std::string &, std::size_t = -1 ) ITK_OVERRIDE;

  /** Set one specific stored image filename. */
  void SetNthFileName( std::size_t, const std::string & ) ITK_OVERRIDE;

  /** Get the stored image filename list */
  StringVector GetFileNameList() const ITK_OVERRIDE;

 /** Get one specific stored image filename. */
  const std::string & GetNthFileName( std::size_t i ) const ITK_OVERRIDE;

  /** Get one list of the stored image. WARNING : if the parameter list changes,
   *  the returned image list may become obsolete. You should call
   *  GetImageList() again to make sure your image list is up-to-date. */
  FloatVectorImageListType* GetImageList() const;

  /** Get one specific stored image. */
  FloatVectorImageType * GetNthImage( unsigned int i ) const;

  /** Set one specific image. */
  void SetNthImage( unsigned int i, ImageBaseType * img );

  /** */
  const std::string & GetToolTip( std::size_t ) const ITK_OVERRIDE;

  /** Set the list of image. */
  void SetImageList( FloatVectorImageListType * imList );

  /** Add an image to the list. */
  void AddImage( ImageBaseType * image );

  /** */
  bool HasValue() const ITK_OVERRIDE;

  /** */
  using StringListInterface::Erase;
  void Erase( std::size_t start, std::size_t count ) ITK_OVERRIDE;

  /** Clear all the list. */
  void ClearValue() ITK_OVERRIDE;

  /** Retrieve number of elements */
  std::size_t Size() const ITK_OVERRIDE;

  /** */
  bool IsActive( size_t ) const ITK_OVERRIDE;

  /** */
  void Swap( std::size_t, std::size_t ) ITK_OVERRIDE;

  /** */
  Role GetDirection( std::size_t ) const ITK_OVERRIDE;

  /** */
  Role GetDirection() const ITK_OVERRIDE;

  /** */
  const std::string & GetFilenameFilter( std::size_t ) const ITK_OVERRIDE;

  /** */
  const std::string & GetFilenameFilter() const ITK_OVERRIDE;

protected:
  /** Constructor */
  InputImageListParameter();

  /** Destructor */
  ~InputImageListParameter() ITK_OVERRIDE;

//
// Private attributes.
private:
  InputImageListParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

  InputImageParameterVectorType m_InputImageParameterVector;
  FloatVectorImageListType::Pointer m_ImageList;


}; // End class InputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
