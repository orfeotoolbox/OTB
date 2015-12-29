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

#include "otbObjectList.h"

#include "otbBandMathXImageFilter.h"

namespace otb
{

namespace Wrapper
{

class BandMathX : public Application
{

public:
  /** Standard class typedefs. */
  typedef BandMathX                      Self;
  typedef Application                    Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(BandMathX, otb::Application);


  typedef otb::BandMathXImageFilter<FloatVectorImageType>                             BandMathImageFilterType;


private:
  void DoInit()
  {
    SetName("BandMathX");
    SetDescription("This application performs mathematical operations on multiband images.\n"
      "Mathematical formula interpretation is done via muParserX library : http://articles.beltoforion.de/article.php?a=muparserx");

    SetDocName("Band Math X");
    SetDocLongDescription("The goal of this documentation is to give the user some hints about the syntax used in this application.\n"
      "The syntax is mainly constrained by the muparserx library, which can be considered as the core of the application.\n"
      "\n\n"
      "- Fundamentals:\n\n"
      "The default prefix name for variables related to the ith input is 'im(i+1)'(note the indexing from 1 to N, for N inputs). \n"
      "The following list summaries the available variables for input #0 (and so on for every input): \n"
      "\n"
      "im1                                -->   a pixel from first input, made of n components (n bands)\n"
      "im1bj                             -->   jth component of a pixel from first input (first band is indexed by 1)\n"
      "im1bjNkxp                     -->   a neighbourhood ('N') of pixels of the jth component from first input, of size kxp\n"
      "im1PhyX and im1PhyY  -->   spacing of first input in X and Y directions (horizontal and vertical)\n"
      "im1bjMean im1bjMin im1bjMax im1bjSum im1bjVar  -->   mean, min, max, sum, variance of jth band from first input (global statistics)\n"
      "\nMoreover, we also have the following generic variables:\n"
      "idxX and idxY        -->   indices of the current pixel\n"
      "\n"
      "Always keep in mind that this application only addresses mathematically well-defined formulas.\n"
      "For instance, it is not possible to add vectors of different dimensions (this implies the addition of a row vector with a column vector),\n"
      "or add a scalar to a vector or a matrix, or divide two vectors, and so on...\n"
      "Thus, it is important to remember that a pixel of n components is always represented as a row vector.\n"
      "\n"
      "Example :\n\n"
      "                   im1 + im2       (1)\n"
      "\nrepresents the addition of pixels from first and second inputs. This expression is consistent only if\n"
      "both inputs have the same number of bands.\n"
      "Note that it is also possible to use the following expressions to obtain the same result:\n"
      "\n"
      "                   im1b1 + im2b1 \n"
      "                   im1b2 + im2b2       (2)\n"
      "                         ...."
      "\n\nNevertheless, the first expression is by far much pleaseant. We call this new functionnality the 'batch mode'\n"
      "(performing the same operation in a band-to-band fashion).\n"

      "\n\n"
      "- Operations involving neighborhoods of pixels:\n\n"
      "Another new fonctionnality is the possibility to perform operations that involve neighborhoods of pixels.\n"
      "Variable related to such neighborhoods are always defined following the pattern imIbJNKxP, where: \n"
      "- I is an number identifying the image input (remember, input #0 = im1, and so on)\n"
      "- J is an number identifying the band (remember, first band is indexed by 1)\n"
      "- KxP are two numbers that represent the size of the neighborhood (first one is related to the horizontal direction)\n"
      "All neighborhood are centred, thus K and P must be odd numbers.\n"
      "Many operators come with this new functionnality: dotpr, mean var median min max...\n"
      "For instance, if im1 represents the pixel of 3 bands image:\n\n"
      "               im1 - mean(im1b1N5x5,im1b2N5x5,im1b3N5x5)       (3)\n"
      "\ncould represent a high pass filter (Note that by implying three neighborhoods, the operator mean returns a row vector of three components.\n"
      "It is a typical behaviour for many operators of this application).\n"

      "\n\n"
      "- Operators:\n\n"
      "In addition to the previous operators, other operators are available:\n"
      "- existing operators/functions from muParserX, that were not originally defined for vectors and\n"
      "matrices (for instance cos, sin, ...). These new operators/ functions keep the original names to which we added the prefix 'v' for vector (vcos, vsin, ...).\n"
      "- mult, div and pow operators, that perform element-wise multiplication, division or exponentiation of vector/matrices (for instance im1 div im2)\n"
      "- mlt, dv and pw operators, that perform multiplication, division or exponentiation of vector/matrices by a scalar (for instance im1 dv 2.0)\n"
      "- bands, which is a very usefull operator. It allows selecting specific bands from an image, and/or to rearrange them in a new vector;\n"
      "for instance bands(im1,{1,2,1,1}) produces a vector of 4 components made of band 1, band 2, band 1 and band 1 values from the first input.\n"
      "Note that curly brackets must be used in order to select the desired band indices.\n"
      "... and so on.\n"

      "\n\n"
      "- Application itself:\n\n"
      "The application takes the following parameters :\n"
      "- Setting the list of inputs can be done with the 'il' parameter.\n"
      "- Setting expressions can be done with the 'exp' parameter (see also limitations section below).\n"
      "- Setting constants can be done with the 'incontext' parameter. User must provide a txt file with a specific syntax: #type name value\n"
      "An example of such a file is given below:\n\n"
      "#F expo 1.1\n"
      "#M kernel1 { 0.1 , 0.2 , 0.3; 0.4 , 0.5 , 0.6; 0.7 , 0.8 , 0.9; 1 , 1.1 , 1.2; 1.3 , 1.4 , 1.5 }\n"
      "\nAs we can see,  #I/#F allows the definition of an integer/float constant, whereas #M allows the definition of a vector/matrix.\n"
      "In the latter case, elements of a row must be separated by commas, and rows must be separated by semicolons.\n"
      "It is also possible to define expressions within the same txt file, with the pattern #E expr. For instance (two expressions; see also limitations section below):\n\n"
      "#E $dotpr(kernel1,im1b1N3x5); im2b1^expo$\n"
      "\n- The 'outcontext' parameter allows saving user's constants and expressions (context).\n"
      "- Setting the output image can be done with the 'out' parameter (multi-outputs is not implemented yet).\n"
      "\n\n"
      "Finally, we strongly recommend that the reader takes a look at the cookbook, where additional information can be found (http://www.orfeo-toolbox.org/packages/OTBCookBook.pdf).\n"

);

    SetDocLimitations("The application is currently unable to produce one output image per expression, contrary to otbBandMathXImageFilter.\n"
                      "Separating expressions by semi-colons (; ) will concatenate their results into a unique multiband output image. ");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    AddDocTag("Util");

    AddParameter(ParameterType_InputImageList,  "il",   "Input image list");
    SetParameterDescription("il", "Image list to perform computation on.");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Output image.");

    AddRAMParameter();

    AddParameter(ParameterType_String, "exp", "Expressions");
    SetParameterDescription("exp",
                            "Mathematical expression to apply.");
    MandatoryOff("exp");

    AddParameter(ParameterType_InputFilename, "incontext", "Import context");
    SetParameterDescription("incontext",
                            "A txt file containing user's constants and expressions.");
    MandatoryOff("incontext");

    AddParameter(ParameterType_OutputFilename, "outcontext", "Export context");
    SetParameterDescription("outcontext",
                            "A txt file where to save user's constants and expressions.");
    MandatoryOff("outcontext");

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "verySmallFSATSW_r.tif verySmallFSATSW_nir.tif verySmallFSATSW.tif");
    SetDocExampleParameterValue("out", "apTvUtBandMathOutput.tif");
    SetDocExampleParameterValue("exp", "\"cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3, im3b4)\"");
  }

  void DoUpdateParameters()
  {

  }

  void DoExecute()
  {
    // Get the input image list
    FloatVectorImageListType::Pointer inList = GetParameterImageList("il");

    // checking the input images list validity
    const unsigned int nbImages = inList->Size();

    if (nbImages == 0)
    {
     itkExceptionMacro("No input Image set...; please set at least one input image");
    }

    if ( (!IsParameterEnabled("exp")) && (!IsParameterEnabled("incontext")) )
    {
     itkExceptionMacro("No expression set...; please set and enable at least one one expression");
    }

    m_Filter               = BandMathImageFilterType::New();
    m_Filter->SetManyExpressions(false);

    for (unsigned int i = 0; i < nbImages; i++)
      {
        FloatVectorImageType::Pointer currentImage = inList->GetNthElement(i);
        currentImage->UpdateOutputInformation();

        otbAppLogINFO( << "Image #" << i + 1 << " has "
                       << currentImage->GetNumberOfComponentsPerPixel()
                       << " components" << std::endl );

        m_Filter->SetNthInput(i,currentImage);

      }


    if ( IsParameterEnabled("exp") )
    {
      std::string string = GetParameterString("exp");
      otbAppLogINFO( << string << std::endl );
      m_Filter->SetExpression(string);
    }

    if ( IsParameterEnabled("incontext") )
      m_Filter->ImportContext(GetParameterString("incontext"));

    if ( IsParameterEnabled("outcontext") )
      m_Filter->ExportContext(GetParameterString("outcontext"));

    // Set the output image
    SetParameterOutputImage("out", m_Filter->GetOutput());


  }

  BandMathImageFilterType::Pointer  m_Filter;
};

} // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::BandMathX)
