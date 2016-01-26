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
#ifndef __otbStandardMetaImageFunctionBuilder_h
#define __otbStandardMetaImageFunctionBuilder_h


#include "itkVariableLengthVector.h"

#include "otbMultiChannelIFFactory.h"

#include "otbLocalHistogramIFFactory.h"
#include "otbFlusserMomentsIFFactory.h"
#include "otbRadiometricMomentsIFFactory.h"
#include "otbFourierMellinDescriptorsIFFactory.h"
#include "otbHaralickTexturesIFFactory.h"


namespace otb
{
/** \class StandardMetaImageFunctionBuilder
 *  \brief Build a Standard MetaImageFunction
 *
 *  The StandardMetaImageFunctionBuilder class allows calling multiple
 *  ImageFunctions:
 *    - otbRadiometricMomentsImageFunction
 *    - otbFlusserMomentsImageFunction
 *    - otbFourierMellinDescriptorsImageFunction
 *    - otbLocalHistogramImageFunction
 *  On Multiple Images (and VectorImages Processed as ImageList).
 *
 *  Each ImageFunction can be parametrized. The only thing to do to
 *  complete the MetaImageFunction is to AddImages.
 *
 *
 *
 *
 * \ingroup OTBObjectDetection
 */

template <class TPrecision = double, class TCoordRep = double>
class ITK_EXPORT StandardMetaImageFunctionBuilder :
    public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef StandardMetaImageFunctionBuilder                                Self;
  typedef itk::LightObject                                                Superclass;
  typedef itk::SmartPointer<Self>                                         Pointer;
  typedef itk::SmartPointer<const Self>                                   ConstPointer;

  // New macro
  itkNewMacro(Self);

  // RTTI typeinfo
  itkTypeMacro(StandardMetaImageFunctionBuilder, itk::LightObject);

  // Input and output typedef
  typedef TPrecision                                                ValueType;
  typedef TCoordRep                                                 CoordRepType;

  // Other typedef
  typedef MetaImageFunction<TPrecision, CoordRepType>               MetaImageFunctionType;
  typedef typename MetaImageFunctionType::Pointer                   MetaImageFunctionPointerType;
  typedef typename std::vector<itk::DataObject::Pointer>            DataObjectContainerType;
  typedef typename std::vector<ValueType>                           ParamContainerType;

  //Accessors
  unsigned int GetNeighborhoodRadius();
  void SetNeighborhoodRadius(unsigned int radius);

  ParamContainerType GetFlusserMomentsIFParameters();
  void SetFlusserMomentsIFParameters(ParamContainerType Param);

  ParamContainerType GetLocalHistogramIFParameters();
  void SetLocalHistogramIFParameters(ParamContainerType Param);

  ParamContainerType GetRadiometricMomentsIFParameters();
  void SetRadiometricMomentsIFParameters(ParamContainerType Param);

  ParamContainerType GetFourierMellinDescriptorsIFParameters();
  void SetFourierMellinDescriptorsIFParameters(ParamContainerType Param);

  ParamContainerType GetHaralickTexturesIFParameters();
  void SetHaralickTexturesIFParameters(ParamContainerType Param);

  MetaImageFunctionPointerType GetMetaImageFunction();
  DataObjectContainerType GetDataObjectContainer();

  // Adding Image
  template <class TInputImage>
  void AddImage(TInputImage * image)
  {
    typedef TInputImage                                   InputImageType;
    typedef typename InputImageType::InternalPixelType    InternalPixelType;
    typedef Image<InternalPixelType, 2>                   ImageType;

    //Mono-Channel Factories
    typedef LocalHistogramIFFactory<ImageType,
      CoordRepType, TPrecision>               LocalHistogramIFFactoryType;
    typedef FlusserMomentsIFFactory<ImageType,
      CoordRepType, TPrecision>               FlusserMomentsIFFactoryType;
    typedef RadiometricMomentsIFFactory<ImageType,
      CoordRepType, TPrecision>               RadiometricMomentsIFFactoryType;
    typedef FourierMellinDescriptorsIFFactory<ImageType,
      CoordRepType, TPrecision>               FourierMellinDescriptorsIFFactoryType;
    typedef HaralickTexturesIFFactory<ImageType,
      CoordRepType, TPrecision>               HaralickTexturesIFFactoryType;

    //Multi-Channel Factories
    typedef MultiChannelIFFactory<LocalHistogramIFFactoryType, InputImageType>
                                                            MCLocalHistogramIFFactoryType;
    typedef MultiChannelIFFactory<FlusserMomentsIFFactoryType, InputImageType>
                                                            MCFlusserMomentsIFFactoryType;
    typedef MultiChannelIFFactory<RadiometricMomentsIFFactoryType, InputImageType>
                                                            MCRadiometricMomentsIFFactoryType;
    typedef MultiChannelIFFactory<FourierMellinDescriptorsIFFactoryType, InputImageType>
                                                            MCFourierMellinDescriptorsIFFactoryType;
    typedef MultiChannelIFFactory<HaralickTexturesIFFactoryType, InputImageType>
                                                            MCHaralickTexturesIFFactoryType;

    MCLocalHistogramIFFactoryType MCLocalHistogramIFFactory;
    MCFlusserMomentsIFFactoryType MCFlusserMomentsIFFactory;
    MCRadiometricMomentsIFFactoryType MCRadiometricMomentsIFFactory;
    MCFourierMellinDescriptorsIFFactoryType MCFourierMellinDescriptorsIFFactory;
    MCHaralickTexturesIFFactoryType MCHaralickTexturesIFFactory;


    MCRadiometricMomentsIFFactory.Create(image,
                                         this->GetRadiometricMomentsIFParameters(),
                                         m_MetaImageFunction,
                                         &m_DataObjectContainer);
    MCFlusserMomentsIFFactory.Create(image,
                                     this->GetFlusserMomentsIFParameters(),
                                     m_MetaImageFunction,
                                     &m_DataObjectContainer);

    MCFourierMellinDescriptorsIFFactory.Create(image,
                                               this->GetFourierMellinDescriptorsIFParameters(),
                                               m_MetaImageFunction,
                                               &m_DataObjectContainer);

    //MCLocalHistogramIFFactory.Create(image,
    //                                 m_LocalHistogramParam,
    //                                 m_MetaImageFunction,
    //                                 &m_DataObjectContainer);

    //MCHaralickTexturesIFFactory.Create(image,
    //                                   m_HaralickTexturesParam,
    //                                   m_MetaImageFunction,
    //                                   &m_DataObjectContainer);
  }

protected:
  StandardMetaImageFunctionBuilder();
  ~StandardMetaImageFunctionBuilder();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  StandardMetaImageFunctionBuilder(const Self& ); //purposely not implemented
  void operator=(const Self& ); //purposely not implemented

  MetaImageFunctionPointerType    m_MetaImageFunction;
  DataObjectContainerType         m_DataObjectContainer;
  unsigned int                    m_NeighborhoodRadius;
  ParamContainerType              m_FlusserMomentsParam;              //[NeighborhoodRadius]
  ParamContainerType              m_RadiometricMomentsParam;          //[NeighborhoodRadius]
  ParamContainerType              m_LocalHistogramParam;              //[NeighborhoodRadius; nbBins; minHistogram; maxHistogram]
  ParamContainerType              m_FourierMellinDescriptorsParam;    //[NeighborhoodRadius; PMax; QMax]
  ParamContainerType              m_HaralickTexturesParam;            //[NeighborhoodRadius; ImageMin; ImageMax; NbBinPerAxis; Offset]

};

} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStandardMetaImageFunctionBuilder.txx"
#endif

#endif
