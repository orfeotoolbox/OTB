/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImageList_h
#define otbImageList_h

#include "otbObjectList.h"

namespace otb
{
/** \class ImageList
 *  \brief This class represent a list of images.
 *
 * It is derived from the otbObjectList class, which allows manipulating an ITK/OTB
 * object list with the appropriate formalism (iterators, accessors).
 * \sa ObjectList
 *
 * \ingroup OTBObjectList
 */

template <class TImage>
class ITK_EXPORT ImageList : public ObjectList<TImage>
{
public:
  /** Standard typedefs */
  typedef ImageList                     Self;
  typedef ObjectList<TImage>            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkTypeMacro(ImageList, ObjectList);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Template parameter typedefs */
  typedef TImage                                    ImageType;
  typedef typename Superclass::ObjectPointerType    ImagePointerType;
  typedef typename Superclass::Iterator             Iterator;
  typedef typename Superclass::ConstIterator        ConstIterator;
  typedef typename Superclass::ReverseIterator      ReverseIterator;
  typedef typename Superclass::ReverseConstIterator ReverseConstIterator;

  /**
   * Update images in the list.
   */
  void UpdateOutputInformation(void) override;
  void PropagateRequestedRegion(void) throw(itk::InvalidRequestedRegionError) override;
  void UpdateOutputData(void) override;

  void SetRequestedRegion(const itk::DataObject* source) override;

protected:
  /** Constructor */
  ImageList(){};
  /** Destructor */
  ~ImageList() override
  {
  }
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  ImageList(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageList.hxx"
#endif

#endif
