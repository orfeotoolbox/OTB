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
#ifndef __otbFourierMellinDescriptorsIFFactory_h
#define __otbFourierMellinDescriptorsIFFactory_h

#include "itkLightObject.h"

#include "otbMetaImageFunction.h"
#include "itkDataObject.h"
#include "otbImage.h"
#include "otbImageFunctionAdaptor.h"
#include "otbFourierMellinDescriptorsImageFunction.h"


namespace otb
{
/** \class FourierMellinDescriptorsFFactory
 *  \brief add a FourierMellinDescriptors image function to a
 *  MetaImageFunction
 *
 *  This class aims at adding an adapted
 *  FourierMellinDescriptorsImageFunction to an existing
 *  MetaImageFunction through the method Create.
 *
 */

template <class TImageType, class TCoordRep = double, class TPrecision = double>
class ITK_EXPORT FourierMellinDescriptorsIFFactory :
    public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef FourierMellinDescriptorsIFFactory                               Self;
  typedef itk::LightObject                                                Superclass;
  typedef itk::SmartPointer<Self>                                         Pointer;
  typedef itk::SmartPointer<const Self>                                   ConstPointer;

  // New macro
  itkNewMacro(Self);

  // RTTI typeinfo
  itkTypeMacro(FourierMellinDescriptorsIFFactory, itk::LightObject);

  // Input and output typedef
  typedef TImageType              InputImageType;
  typedef TCoordRep               CoordRepType;
  typedef TPrecision              PrecisionType;

  // Other typedef
  typedef typename MetaImageFunction<TPrecision>::Pointer  MetaImageFunctionPointerType;
  typedef typename std::vector<itk::DataObject::Pointer>   DataObjectContainerType;
  typedef typename std::vector<PrecisionType>         ParamContainerType;
  typedef FourierMellinDescriptorsImageFunction<InputImageType, CoordRepType>
                                                      FourierMellinDescriptorsIF;
  typedef ImageFunctionAdaptor<FourierMellinDescriptorsIF, TPrecision>
                                                      AdaptedFourierMellinDescriptorsIF;

  void Create(InputImageType * image,
              ParamContainerType param,
              MetaImageFunctionPointerType metaIF,
              DataObjectContainerType * container);

protected:
  FourierMellinDescriptorsIFFactory(){}
  ~FourierMellinDescriptorsIFFactory(){}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  FourierMellinDescriptorsIFFactory(const Self& ); //purposely not implemented
  void operator=(const Self& ); //purposely not implemented

};

} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFourierMellinDescriptorsIFFactory.txx"
#endif

#endif
