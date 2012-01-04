/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


    This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE,  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbViewerModel_h
#define __otbViewerModel_h

#include "otbMVCModel.h"
#include "otbListenerBase.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkRGBAPixel.h"
#include "otbImageFileReader.h"
#include "otbObjectList.h"

/** NewVisu */
#include "otbImageLayer.h"
#include "otbImageLayerGenerator.h"
#include "otbImageLayerRenderingModel.h"

#include "otbWidgetResizingActionHandler.h"
#include "otbChangeScaledExtractRegionActionHandler.h"
#include "otbChangeExtractRegionActionHandler.h"
#include "otbChangeScaleActionHandler.h"
#include "otbArrowKeyMoveActionHandler.h"

#include "otbAmplitudeFunctor.h"
#include "otbPhaseFunctor.h"

#include "otbPixelDescriptionModel.h"
#include "otbPixelDescriptionView.h"
#include "otbPixelDescriptionActionHandler.h"
#include "otbCurves2DWidget.h"


#include "otbImageView.h"
#include "otbImageWidgetController.h"

namespace otb
{
/** \class ViewerModel
 *
 *
 *
 */

class ITK_EXPORT ViewerModel
      : public MVCModel<ListenerBase>, public itk::Object
{

public:
  /** Standard class typedefs */
  typedef ViewerModel       Self;
  typedef MVCModel<ListenerBase>        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard type macro */
  itkTypeMacro(ViewerModel, MVCModel);

  /** Images typedefs */
  typedef double PixelType;

  /**  Image Type*/
  typedef VectorImage<PixelType , 2>    ImageType;
  typedef itk::RGBAPixel<unsigned char> RGBPixelType;
  typedef Image<RGBPixelType, 2>        ViewerImageType;
  typedef ImageType::Pointer            ImagePointerType;

  /** typedef support for layers */
  typedef ImageLayer<ImageType, ViewerImageType> LayerType;
  typedef LayerType::Pointer                      LayerPointerType;

  typedef ImageLayerGenerator<LayerType>                                          LayerGeneratorType;
  typedef LayerGeneratorType::Pointer                                             LayerGeneratorPointerType;
  typedef LayerGeneratorType::RenderingFunctionType                               RenderingFunctionType;
  typedef Function::StandardRenderingFunction<ImageType::PixelType, RGBPixelType> StandardRenderingFunctionType;

  typedef Function::StandardRenderingFunction<ImageType::PixelType, RGBPixelType,
    Function::AmplitudeFunctor<ImageType::PixelType> >                        AmplitudeRenderingFunction;
  typedef Function::StandardRenderingFunction<ImageType::PixelType, RGBPixelType,
    Function::PhaseFunctor<ImageType::PixelType> >                            PhaseRenderingFunction;

  /** typedef support for reader*/
  typedef ImageFileReader<ImageType> ReaderType;
  typedef ReaderType::Pointer        ReaderPointerType;

  /** Typedef support for rendering image*/
  typedef ImageLayerRenderingModel<ViewerImageType>  VisuModelType;
  typedef VisuModelType::Pointer                     VisuModelPointerType;

  /** NewVisu */
  typedef ImageView<VisuModelType>      VisuViewType;
  typedef VisuViewType::OffsetType      OffsetType;
  typedef VisuViewType::Pointer         VisuViewPointerType;

  typedef ImageWidgetController         WidgetControllerType;
  typedef WidgetControllerType::Pointer WidgetControllerPointerType;

  typedef WidgetResizingActionHandler<VisuModelType, VisuViewType>               ResizingHandlerType;
  typedef ResizingHandlerType::Pointer                                           ResizingHandlerPointerType;
  typedef ChangeScaledExtractRegionActionHandler<VisuModelType, VisuViewType>    ChangeScaledRegionHandlerType;
  typedef ChangeExtractRegionActionHandler<VisuModelType, VisuViewType>          ChangeRegionHandlerType;
  typedef ChangeScaleActionHandler<VisuModelType, VisuViewType>                  ChangeScaleHandlerType;
  typedef ArrowKeyMoveActionHandler<VisuModelType, VisuViewType>                 ArrowKeyMoveActionHandlerType;

  typedef PixelDescriptionModel<ViewerImageType>                                 PixelDescriptionModelType;
  typedef PixelDescriptionModelType::Pointer                                     PixelDescriptionModelPointerType;
  typedef PixelDescriptionView<PixelDescriptionModelType>                        PixelDescriptionViewType;
  typedef PixelDescriptionActionHandler<PixelDescriptionModelType, VisuViewType> PixelDescriptionActionHandlerType;

  typedef Curves2DWidget                                                         CurvesWidgetType;

  /**
   * Struct embedded in the model
   */
  struct _ObjectsTracked
  {
    ReaderPointerType                 pReader;
    ImagePointerType                  pQuicklook;
    LayerPointerType                  pLayer;
    VisuModelPointerType              pRendering;
    WidgetControllerPointerType       pWidgetController;
    VisuViewPointerType               pVisuView;
    RenderingFunctionType::Pointer    pRenderFunction;
    PixelDescriptionViewType::Pointer pPixelView;
    PixelDescriptionModelPointerType  pPixelModel;
    CurvesWidgetType::Pointer         pCurveWidget;
    std::string                       pFileName;
  };

  typedef struct _ObjectsTracked ObjectsTracked;

  /**
   * List of objectTracked, we cannot use ObjectList
   * for struct cause don't implenement Register method
   */
  typedef std::vector<ObjectsTracked> ObjectTrackedList;

  /** Get the unique instanc1e of the model */
  static Pointer GetInstance();

  virtual unsigned int OpenImage(std::string strfilename, const unsigned int id=0);
  virtual unsigned int OpenImageList(std::string strfilename);
  virtual void CloseImage(unsigned int selectedItem);
  virtual void UpdateRGBChannelOrder(int redChoice , int greenChoice, int BlueChoice, unsigned int selectedItem);
  virtual void UpdateGrayScaleChannelOrder(int choice, unsigned int selectedItem);
  virtual void UpdateAmplitudeChannelOrder(int realChoice , int imChoice, unsigned int selectedItem );
  virtual void UpdatePhaseChannelOrder(int realChoice , int imChoice, unsigned int selectedItem );
  virtual void Link(unsigned int leftChoice, unsigned int rightChoice, OffsetType offset);
  virtual void InitializeImageViewController(unsigned int selectedItem);

  /** Method needed to Get the list of componenets stored*/
   const ObjectTrackedList & GetObjectList() const
    {
      return m_ObjectTrackedList;
    }


   /** Boolean Flags */
   itkGetMacro(HasImageOpened, bool);
   itkGetMacro(HasChangedChannelOrder, bool);

 // Test a filename and returns true if the file is JPEG2000
  bool IsJPEG2000File(const std::string & filepath);
  // If the file is JPEG2000, the function will find the available
  // resolutions.
  std::vector<unsigned int> GetJPEG2000Resolution(const std::string & filepath);
  // If the file is JPEG2000, the function will find the available
  // resolutions and informations
  void GetJPEG2000ResolutionAndInformations(const std::string & filepath,
                                            std::vector<unsigned int>& res,
                                            std::vector<std::string> & desc);

protected:
  /** This is protected for the singleton. Use GetInstance() instead. */
  itkNewMacro(Self);

  /** Constructor */
  ViewerModel();

  /** Destructor */
  virtual ~ViewerModel();

  /** Built Visu & Controller*/
  virtual VisuViewPointerType BuiltVisu(VisuModelPointerType pRendering);
  virtual WidgetControllerPointerType BuiltController(VisuModelPointerType modelRenderingLayer,
                                                      VisuViewPointerType visuView ,
                                                      PixelDescriptionModelType::Pointer pixelModel);

private:
  ViewerModel(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Notify a given listener of changes */
  virtual void NotifyListener(ListenerBase * listener);

  /** The instance singleton */
  static Pointer Instance;

  /** Boolean flags*/
  bool   m_HasImageOpened;
  bool   m_HasChangedChannelOrder;

  /** The manipuleted list*/
  ObjectTrackedList                          m_ObjectTrackedList;
};


}
#endif
