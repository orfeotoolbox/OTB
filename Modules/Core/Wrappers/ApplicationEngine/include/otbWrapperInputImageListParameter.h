/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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


#include "otbWrapperInputImageParameter.h"
#include "otbWrapperParameterList.h"


namespace otb
{


namespace Wrapper
{

/** \class InputImageListParameter
 *  \brief This class represents a list of InputImage parameter
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT InputImageListParameter : public ParameterList<InputImageParameter>
{
public:
  /** Standard class typedef */
  typedef InputImageListParameter            Self;
  typedef ParameterList<InputImageParameter> Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputImageListParameter, ParameterList);

  /** Get one list of the stored image. WARNING : if the parameter list changes,
   *  the returned image list may become obsolete. You should call
   *  GetImageList() again to make sure your image list is up-to-date. */
  const FloatVectorImageListType* GetImageList() const;
  FloatVectorImageListType*       GetImageList();

  /** Get one specific stored image. */
  const FloatVectorImageType* GetNthImage(std::size_t) const;
  FloatVectorImageType*       GetNthImage(std::size_t);

  /** Set one specific image. */
  void SetNthImage(std::size_t, ImageBaseType*);

  /** Set the list of image. */
  void SetImageList(FloatVectorImageListType*);

  /** Add an image to the list. */
  void AddImage(ImageBaseType*);

  /** Clear all the list. */
  void ClearValue() override;

  /** */
  using StringListInterface::GetDirection;
  Role GetDirection() const override;

  /** */
  using StringListInterface::GetFilenameFilter;
  const std::string& GetFilenameFilter() const override;

  ParameterType GetType() const override
  {
    return ParameterType_InputImageList;
  }

protected:
  /** Constructor */
  InputImageListParameter();

  /** Destructor */
  ~InputImageListParameter() override;

  //
  // Private methods.
private:
  InputImageListParameter(const Parameter&) = delete;
  void operator=(const Parameter&) = delete;

  InputImageParameter::Pointer FromImage(ImageBaseType*);

  InputImageParameter::Pointer& FromImage(InputImageParameter::Pointer&, ImageBaseType*);

  //
  // Private attributes
private:
  FloatVectorImageListType::Pointer m_ImageList;


}; // End class InputImage Parameter

} // End namespace Wrapper

} // End namespace otb

#endif
