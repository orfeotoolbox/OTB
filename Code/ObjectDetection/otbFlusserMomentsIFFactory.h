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
#ifndef __otbFlusserMomentsIFFactory_h
#define __otbFlusserMomentsIFFactory_h

#include "itkLightObject.h"

#include "otbMetaImageFunction.h"
#include "itkDataObject.h"
#include "otbImage.h"
#include "otbImageFunctionAdaptor.h"
#include "otbFlusserMomentsImageFunction.h"


namespace otb
{
/** \class FlusserMomentsFFactory
 *  \brief add a FlusserMoments image function to a
 *  MetaImageFunction
 *
 *  This class aims at adding an adapted
 *  FlusserMomentsImageFunction to an existing
 *  MetaImageFunction through the method Create.
 *
 */

template <class TImageType, class TCoordRep = double, class TPrecision = double>
class ITK_EXPORT FlusserMomentsIFFactory :
    public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef FlusserMomentsIFFactory                                         Self;
  typedef itk::LightObject                                                Superclass;
  typedef itk::SmartPointer<Self>                                         Pointer;
  typedef itk::SmartPointer<const Self>                                   ConstPointer;

  // New macro
  itkNewMacro(Self);

  // RTTI typeinfo
  itkTypeMacro(FlusserMomentsIFFactory, itk::LightObject);

  // Input and output typedef
  typedef TImageType              InputImageType;
  typedef TCoordRep               CoordRepType;
  typedef TPrecision              PrecisionType;

  // Other typedef
  typedef typename MetaImageFunction<TPrecision>::Pointer  MetaImageFunctionPointerType;
  typedef typename std::vector<itk::DataObject::Pointer>   DataObjectContainerType;
  typedef typename std::vector<PrecisionType>         ParamContainerType;
  typedef FlusserMomentsImageFunction<InputImageType, CoordRepType>
                                                      FlusserMomentsIF;
  typedef ImageFunctionAdaptor<FlusserMomentsIF, TPrecision>
                                                      AdaptedFlusserMomentsIF;

  void Create(InputImageType * image,
              ParamContainerType param,
              MetaImageFunctionPointerType metaIF,
              DataObjectContainerType * container);

protected:
  FlusserMomentsIFFactory(){}
  ~FlusserMomentsIFFactory(){}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  FlusserMomentsIFFactory(const Self& ); //purposely not implemented
  void operator=(const Self& ); //purposely not implemented

};

} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFlusserMomentsIFFactory.txx"
#endif

#endif
