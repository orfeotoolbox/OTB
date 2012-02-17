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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"


namespace otb
{
namespace Wrapper
{

class HorizontalPixelWiseBlockMatching : public Application
{
public:
  /** Standard class typedefs. */
  typedef HorizontalPixelWiseBlockMatching              Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(HorizontalPixelWiseBlockMatching, otb::Application);

  /** Filters typedef */

private:
  void DoInit()
  {
    SetName("HorizontalPixelWiseBlockMatching");
    SetDescription("Performs block-matching to estimate pixel-wise horizontal disparities between two images");

    SetDocName("Horizontal Pixel-wise Block-Matching");
    SetDocLongDescription("This application allows to erforms block-matching to estimate pixel-wise horizontal disparities between two images. This is useful in the case of stereo images in epipolar geometry, when displacements related to elevation only occur in the horizontal direction. The application allows to choose the block-matching method to use. It also allows to input a mask (related to the left input image) of pixels for which the disparity should be investigated. Additionnaly, two criterions can be optionnaly use to disable disparity investigation for some pixel: a no-data value, and a threshold on the local variance. This allows to speed-up computation by avoiding to investigate disparities that will not be reliable anyway. For efficiency reasons, if the optimal metric values image is desired, it will be concatenated to the output image (which will then have two bands, the first being the disparity, and the second the metric values). One can split these images afterward.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbStereoRectificationGridGenerator");

    AddDocTag(Tags::Stereo);

    AddParameter(ParameterType_Group,"io","Input and output data");
    SetParameterDescription("io","This group of parameters allows to set the input and output images.");
    
    AddParameter(ParameterType_InputImage,"io.inleft","Left input image");
    SetParameterDescription("io.inleft","The left input image (reference)");

    AddParameter(ParameterType_InputImage,"io.inright","Right input image");
    SetParameterDescription("io.inright","The right input (secondary)");

    AddParameter(ParameterType_OutputImage, "io.out", "The output disparity map");
    SetParameterDescription("io.out","An image containing the estimated disparity as well as the metric values if the option is used");

    AddParameter(ParameterType_Empty,"io.outmetric","Output optimal metric values as well");
    SetParameterDescription("io.outmetric","If used, the output image will have a second component with metric optimal values");

    AddParameter(ParameterType_Group,"mask","Image masking parameters");
    SetParameterDescription("mask","This group of parameters allows to determine the masking parameters to prevent disparities estimation for some pixels of the left image");
    
    AddParameter(ParameterType_InputImage,"mask.in","Discard pixels from mask image");
    SetParameterDescription("mask.in","This parameter allows to provide a custom mask");
    MandatoryOff("mask.in");

    AddParameter(ParameterType_Float,"mask.nodata","Discard pixels with no-data value");
    SetParameterDescription("mask.in","This parameter allows to discard pixels whose value is equal to the user-defined no-data value.");
    MandatoryOff("mask.nodata");
    
    AddParameter(ParameterType_Float,"mask.variancet","Discard pixels with low local variance");
    SetParameterDescription("mask.variancet","This parameter allows to discard pixels whose local variance is too small (the size of the neighborhood is given by the radius parameter)");
    MandatoryOff("mask.variancet");

    AddParameter(ParameterType_Group,"bm","Block matching parameters");
    SetParameterDescription("bm","This group of parameters allow to tune the block-matching behaviour");
    
    AddParameter(ParameterType_Choice,   "bm.metric", "Block-matching metric");
    AddChoice("bm.metric.ssd","Sum of Squared Distances");
    SetParameterDescription("bm.metric.ssd","Sum of squared distances between pixels value in the metric window");

    AddChoice("bm.metric.ncc","Normalized Cross-Correlation");
    SetParameterDescription("bm.metric.ncc","Normalized Cross-Correlation between the left and right windows");
    
    AddParameter(ParameterType_Int,"bm.radius","Radius of blocks");
    SetParameterDescription("bm.radius","The radius (in pixels) of blocks in Block-Matching");
    SetDefaultParameterInt("bm.radius",3);

    AddParameter(ParameterType_Int,"bm.mind","Minimum disparity");
    SetParameterDescription("bm.mind","Minimum disparity to explore (can be negative)");

    AddParameter(ParameterType_Int,"bm.maxd","Maximum disparity");
    SetParameterDescription("bm.maxd","Maximum disparity to explore (can be negative)");

    
    AddRAMParameter();
 
    // Doc example parameter settings
  }

  void DoUpdateParameters()
  {
    // Nothing to do
  }

  void DoExecute()
  {
    
  }  
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::HorizontalPixelWiseBlockMatching)
