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
#ifndef __otbVectorDataToSpecificDescriptionFilterBase_h
#define __otbVectorDataToSpecificDescriptionFilterBase_h

#include "otbVectorDataToVectorDataFilter.h"

namespace otb
{
/** \class VectorDataToSpecificDescriptionFilterBase
  * \brief Base class for VectorDataToSpecificDescriptionFilters.
  *
  * VectorDataToSpecificDescriptionFilterBase is a base class, it is
  * supposed to be used through a inheriting class.
  * Input and output are both VectorDatas. Each data node of the
  * output VectorData contains its score regarding to the associated
  * descriptors.
  *
  *
  * \ingroup VectorDataFilter
  * \sa VectorDataToRoadDescriptionFilter
  * \sa VectorDataToBuildingDescriptionFilter
 */

template <class TVectorData>
class ITK_EXPORT VectorDataToSpecificDescriptionFilterBase :
  public VectorDataToVectorDataFilter<TVectorData, TVectorData>
{
public:
  /** Standard class typedefs. */
  typedef VectorDataToSpecificDescriptionFilterBase    Self;
  typedef VectorDataToVectorDataFilter<TVectorData,
    TVectorData>                                       Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataToSpecificDescriptionFilterBase, VectorDataToVectorDataFilter);

  /** Some typedefs. */
  typedef TVectorData                             VectorDataType;
  typedef typename VectorDataType::Pointer        VectorDataPointer;
  typedef typename VectorDataType::DataNodeType   DataNodeType;

  typedef itk::DataObject::Pointer DataObjectPointer;
  typedef itk::DataObject          DataObject;

  /** Support accessors. */
  virtual void AddSupport(const itk::DataObject * support, unsigned int idx);
  const itk::DataObject * GetSupport(unsigned int idx);

protected:
  /** Constructor */
  VectorDataToSpecificDescriptionFilterBase();
  /** Destructor */
  virtual ~VectorDataToSpecificDescriptionFilterBase() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  VectorDataToSpecificDescriptionFilterBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToSpecificDescriptionFilterBase.txx"
#endif

#endif
