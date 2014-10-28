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
#ifndef __otbHaralickTexturesIFFactory_h
#define __otbHaralickTexturesIFFactory_h

#include "itkLightObject.h"

#include "otbMetaImageFunction.h"
#include "itkDataObject.h"
#include "otbImage.h"
#include "otbImageFunctionAdaptor.h"
#include "otbHaralickTexturesImageFunction.h"


namespace otb
{
/** \class HaralickTexturesFFactory
 *  \brief add a HaralickTextures image function to a
 *  MetaImageFunction
 *
 *  This class aims at adding an adapted
 *  HaralickTexturesImageFunction to an existing
 *  MetaImageFunction through the method Create.
 *
 */

template <class TImageType, class TCoordRep = double, class TPrecision = double>
class ITK_EXPORT HaralickTexturesIFFactory :
    public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef HaralickTexturesIFFactory                                       Self;
  typedef itk::LightObject                                                Superclass;
  typedef itk::SmartPointer<Self>                                         Pointer;
  typedef itk::SmartPointer<const Self>                                   ConstPointer;

  // New macro
  itkNewMacro(Self);

  // RTTI typeinfo
  itkTypeMacro(haralickTexturesIFFactory, itk::LightObject);

  // Input and output typedef
  typedef TImageType              InputImageType;
  typedef TCoordRep               CoordRepType;
  typedef TPrecision              PrecisionType;

  // Other typedef
  typedef typename MetaImageFunction<TPrecision>::Pointer  MetaImageFunctionPointerType;
  typedef typename std::vector<itk::DataObject::Pointer>   DataObjectContainerType;
  typedef typename std::vector<PrecisionType>              ParamContainerType;
  typedef HaralickTexturesImageFunction<InputImageType, CoordRepType>
                                                           HaralickTexturesIF;
  typedef typename HaralickTexturesIF::OffsetType          OffsetType;
  typedef ImageFunctionAdaptor<HaralickTexturesIF, TPrecision>
                                                           AdaptedHaralickTexturesIF;

  void Create(InputImageType * image,
              ParamContainerType param,
              MetaImageFunctionPointerType metaIF,
              DataObjectContainerType * container);

protected:
  HaralickTexturesIFFactory(){}
  ~HaralickTexturesIFFactory(){}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  HaralickTexturesIFFactory(const Self& ); //purposely not implemented
  void operator=(const Self& ); //purposely not implemented

};

} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHaralickTexturesIFFactory.txx"
#endif

#endif
