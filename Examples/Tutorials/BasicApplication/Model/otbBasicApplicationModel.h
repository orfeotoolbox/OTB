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

#ifndef __otbBasicApplicationModel_h
#define __otbBasicApplicationModel_h

#include "otbMVCModel.h"
#include "otbListenerBase.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"

//Vis
#include "otbImageLayerRenderingModel.h"
#include "otbImageLayerGenerator.h"
#include "otbImageLayer.h"

namespace otb {

/** \class BasicApplicationModel
 *
 *
 *
 */
class ITK_EXPORT BasicApplicationModel
  : public MVCModel<ListenerBase>, public itk::Object
{

public:
  /** Standard class typedefs */
  typedef BasicApplicationModel         Self;
  typedef MVCModel<ListenerBase>        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard type macro */
  itkTypeMacro(BasicApplicationModel, Object);

  typedef      double                           PixelType;
  typedef      VectorImage<PixelType, 2>        VectorImageType;
  typedef      ImageFileReader<VectorImageType> VectorReaderType;

  /** Visualization model */
  typedef itk::RGBPixel<unsigned char>
  RGBPixelType;
  typedef      Image<RGBPixelType,
                     2>
  RGBImageType;
  typedef      ImageLayer<VectorImageType,
                          RGBImageType>
  LayerType;
  typedef      ImageLayerGenerator<LayerType>
  LayerGeneratorType;
  typedef      ImageLayerRenderingModel<RGBImageType>
  VisualizationModelType;
  typedef Function::UniformAlphaBlendingFunction<LayerGeneratorType::
                                                 ImageLayerType::
                                                 OutputPixelType>
  BlendingFunctionType;

  /** Get the unique instanc1e of the model */
  static Pointer GetInstance();

  /** Get the visualization models */
  itkGetObjectMacro(VisualizationModel, VisualizationModelType);

  /** Open an image */
  void OpenImage(const char * filename);

protected:
  /** This is protected for the singleton. Use GetInstance() instead. */
  itkNewMacro(Self);
  /** Constructor */
  BasicApplicationModel();
  /** Destructor */
  ~BasicApplicationModel();

private:
  BasicApplicationModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Notify a given listener of changes */
  virtual void Notify(ListenerBase * listener);

  /** Singleton instance */
  static Pointer Instance;

  /** Visualization model */
  VisualizationModelType::Pointer m_VisualizationModel;

  /** Vector reader */
  VectorReaderType::Pointer m_Reader;
};

} //end namespace otb
#endif
