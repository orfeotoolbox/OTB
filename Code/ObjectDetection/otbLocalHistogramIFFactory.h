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
#ifndef __otbLocalHistogramIFFactory_h
#define __otbLocalHistogramIFFactory_h

#include "itkLightObject.h"

#include "otbMetaImageFunction.h"
#include "itkDataObject.h"
#include "itkVariableLengthVector.h"
#include "otbImage.h"
#include "otbImageFunctionAdaptor.h"
#include "otbLocalHistogramImageFunction.h"


namespace otb
{
/** \class LocalHistogramIFFactory
 *  \brief add a LocalHistogram image function to a
 *  MetaImageFunction
 *
 *  This class aims at adding an adapted
 *  LocalHistogramImageFunction to an existing MetaImageFunction
 *  through the method Create.
 *
 */

template <class TImageType, class TCoordRep = double, class TPrecision = double>
class ITK_EXPORT LocalHistogramIFFactory :
    public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef LocalHistogramIFFactory                                         Self;
  typedef itk::LightObject                                                Superclass;
  typedef itk::SmartPointer<Self>                                         Pointer;
  typedef itk::SmartPointer<const Self>                                   ConstPointer;

  // New macro
  itkNewMacro(Self);

  // RTTI typeinfo
  itkTypeMacro(LocalHistogramIFFactory, itk::LightObject);

  // Input and output typedef
  typedef TImageType              InputImageType;
  typedef TCoordRep               CoordRepType;
  typedef TPrecision              PrecisionType;

  // Other typedef
  typedef typename MetaImageFunction<TPrecision>::Pointer  MetaImageFunctionPointerType;
  typedef typename std::vector<itk::DataObject::Pointer>   DataObjectContainerType;
  typedef typename std::vector<PrecisionType>              ParamContainerType;
  typedef LocalHistogramImageFunction<InputImageType, CoordRepType>
                                                      LocalHistogramIF;
  typedef ImageFunctionAdaptor<LocalHistogramIF, TPrecision>
                                                      AdaptedLocalHistogramIF;

  void Create(InputImageType * image,
              ParamContainerType param,
              MetaImageFunctionPointerType metaIF,
              DataObjectContainerType * container);

protected:
  LocalHistogramIFFactory(){}
  ~LocalHistogramIFFactory(){}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  LocalHistogramIFFactory(const Self& ); //purposely not implemented
  void operator=(const Self& ); //purposely not implemented

};

} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLocalHistogramIFFactory.txx"
#endif

#endif
