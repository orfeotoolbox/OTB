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


//  Software Guide : BeginCommandLineArgs
//
// INPUTS:  {LAIverySmallFSATSW.tif, Simu_label_LAI.png, Simu_mask_LAI_1.png, label-params-SO-2006-Level-2.txt, acqui-params.txt, rep6S.dat}
// OUTPUTS: {siTvLAIAndPROSAILToSensorResponse.tif}
// 5
//  Software Guide : EndCommandLineArgs
//
// Software Guide : BeginLatex
//
// The following code is an example of Sensor spectral response image generated using image of labeled objects image, objects properties (vegetation classes are handled using PROSAIL model, non-vegetation classes
// are characterized using \href{http://speclib.jpl.nasa.gov/}{Aster database} characteristics provided by a text file), acquisition parameters, sensor characteristics, and LAI (Leaf Area Index) image.
//
// Sensor RSR is modeled by 6S (Second Simulation of a Satellite Signal in the Solar Spectrum) model \cite{Vermote1997}. Detailed information about 6S can be found \href{http://6s.ltdri.org/6S_code2_thiner_stuff/6S_Manual_Part_1.pdf}{here}.
//
// Let's look at the minimal code required to use this algorithm. First, the
// following headers must be included.
//
// Software Guide : EndLatex

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileReader.h"
#include "otbStandardWriterWatcher.h"
#include "itkImageRegionConstIterator.h"
#include "otbMultiChannelExtractROI.h"
#include "itkOrImageFilter.h"

// Software Guide : BeginCodeSnippet
#include "otbProspectModel.h"
#include "otbSailModel.h"
#include "otbLeafParameters.h"
#include "otbSatelliteRSR.h"
#include "otbReduceSpectralResponse.h"
#include "otbImageSimulationMethod.h"
#include "otbSpatialisationFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "otbSpectralResponse.h"
#include "itkTernaryFunctorImageFilter.h"
#include "otbVegetationIndicesFunctor.h"
#include "otbRAndNIRIndexImageFilter.h"
#include "otbVectorDataToLabelMapWithAttributesFilter.h"
// Software Guide : EndCodeSnippet

namespace otb
{

// Software Guide : BeginLatex
//
// \code{ImageUniqueValuesCalculator} class is defined here. Method \code{GetUniqueValues()} returns an array with all values contained in an image.
// This class is implemented and used to test if all labels in labeled image are present in label parameter file.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
template < class TImage >
class ITK_EXPORT ImageUniqueValuesCalculator : public itk::Object
{
public:
  typedef ImageUniqueValuesCalculator<TImage>   Self;
  typedef itk::Object                           Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(ImageUniqueValuesCalculator, itk::Object);

  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  typedef typename TImage::PixelType PixelType;

  typedef TImage ImageType;

  typedef std::vector<PixelType>           ArrayType;

  typedef typename ImageType::Pointer      ImagePointer;
  typedef typename ImageType::ConstPointer ImageConstPointer;

  virtual void SetImage( const ImageType * image )
    {
    if ( m_Image != image )
      {
      m_Image = image;
      this->Modified();
      }
    }


  ArrayType GetUniqueValues() const
  {
    typedef typename ImageType::IndexType IndexType;

    if( !m_Image )
      {
       itkExceptionMacro(<<"GetUniqueValues(): Null input image pointer.");
      }

    itk::ImageRegionConstIterator< ImageType > it( m_Image,
                                              m_Image->GetRequestedRegion() );

    ArrayType uniqueValues;

    uniqueValues.push_back(it.Get());
    ++it;
    while( !it.IsAtEnd() )
      {
      if( std::find(uniqueValues.begin(),
                    uniqueValues.end(), it.Get()) == uniqueValues.end())
        {
        uniqueValues.push_back(it.Get());
        }
      ++it;
      }

    return uniqueValues;
  }


protected:
  ImageUniqueValuesCalculator()
    {
    m_Image = NULL;
    }
  virtual ~ImageUniqueValuesCalculator()
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "Image: " << m_Image.GetPointer() << std::endl;
  }

private:
  ImageUniqueValuesCalculator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  ImageConstPointer         m_Image;

};  // class ImageUniqueValuesCalculator
// EndCodeSnippet


namespace Functor
{

// Software Guide : BeginLatex
//
// \code{ProsailSimulatorFunctor} functor is defined here.
//
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
template<class TLAI, class TLabel, class TMask, class TOutput,
  class TLabelSpectra, class TLabelParameter,
    class TAcquistionParameter, class TSatRSR>
class ProsailSimulatorFunctor
// Software Guide : EndCodeSnippet
{
public:
  /** Standard class typedefs */

  // Software Guide : BeginLatex
  //
  // \code{ProsailSimulatorFunctor} functor is defined here.
  //
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef TLAI LAIPixelType;
  typedef TLabel LabelPixelType;
  typedef TMask MaskPixelType;
  typedef TOutput OutputPixelType;
  typedef TLabelSpectra LabelSpectraType;
  typedef TLabelParameter LabelParameterType;
  typedef TAcquistionParameter AcquistionParameterType;
  typedef TSatRSR SatRSRType;
  typedef typename SatRSRType::Pointer SatRSRPointerType;
  typedef typename otb::ProspectModel ProspectType;
  typedef typename otb::SailModel SailType;

  typedef double PrecisionType;
  typedef std::pair<PrecisionType, PrecisionType> PairType;
  typedef typename std::vector<PairType> VectorPairType;
  typedef otb::SpectralResponse<PrecisionType, PrecisionType> ResponseType;
  typedef ResponseType::Pointer ResponsePointerType;
  typedef otb::ReduceSpectralResponse
      <ResponseType, SatRSRType> ReduceResponseType;
  typedef typename ReduceResponseType::Pointer
      ReduceResponseTypePointerType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // In this example spectra are generated form $400$ to $2400nm$. the number of simulated band is set by \code{SimNbBands} value.
  //
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  static const unsigned int SimNbBands = 2000;
  // Software Guide : EndCodeSnippet
  /** Constructor */
  ProsailSimulatorFunctor()
  {
    m_SatRSR = SatRSRType::New();
    m_InvertedMask = false;
  }

  /** Destructor */
  ~ProsailSimulatorFunctor()
  {
  }
;

  void SetInvertedMask(bool ivm)
  {
    m_InvertedMask = ivm;
  }

  /** Implementation of the () operator*/
  inline OutputPixelType operator ()(const LAIPixelType &lai, const LabelPixelType& label, const MaskPixelType& mask)
  {

    // Software Guide : BeginLatex
    //
    // mask value is read to know if the pixel have to be calculated, it is set to 0 otherwise.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    OutputPixelType pix;
    pix.SetSize(m_SatRSR->GetNbBands());
    if ((!mask && !m_InvertedMask) || (mask && m_InvertedMask))
      {
      for (unsigned int i = 0; i < m_SatRSR->GetNbBands(); i++)
        pix[i] = static_cast<typename OutputPixelType::ValueType> (0);
      return pix;
      }
    // Software Guide : EndCodeSnippet


    // Software Guide : BeginLatex
    //
    // Object reflectance \code{hxSpectrum} is calculated. If object label correspond to vegetation label then Prosail code is used, aster database is used otherwise.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    VectorPairType hxSpectrum;

    for (unsigned int i = 0; i < SimNbBands; i++)
      {
      PairType resp;
      resp.first = static_cast<PrecisionType> ((400.0 + i) / 1000);
      hxSpectrum.push_back(resp);
      }

    // either the spectrum has to be simulated by Prospect+Sail
    if (m_LabelParameters.find(label) != m_LabelParameters.end())
      {
      ProspectType::Pointer prospect = ProspectType::New();
      prospect->SetInput(m_LabelParameters[label]);

      SailType::Pointer sail = SailType::New();

      sail->SetLAI(lai);
      sail->SetAngl(m_AcquisitionParameters[std::string("Angl")]);
      sail->SetPSoil(m_AcquisitionParameters[std::string("PSoil")]);
      sail->SetSkyl(m_AcquisitionParameters[std::string("Skyl")]);
      sail->SetHSpot(m_AcquisitionParameters[std::string("HSpot")]);
      sail->SetTTS(m_AcquisitionParameters[std::string("TTS")]);
      sail->SetTTO(m_AcquisitionParameters[std::string("TTO")]);
      sail->SetPSI(m_AcquisitionParameters[std::string("PSI")]);
      sail->SetReflectance(prospect->GetReflectance());
      sail->SetTransmittance(prospect->GetTransmittance());
      sail->Update();

      for (unsigned int i = 0; i < SimNbBands; i++)
        {
        hxSpectrum[i].second = static_cast<typename OutputPixelType::ValueType>
          (sail->GetHemisphericalReflectance()->GetResponse()[i].second);
        }

      }
    // or the spectra has been set from outside the functor (ex. bare soil, etc.)
    else
      if (m_LabelSpectra.find(label) != m_LabelSpectra.end())
        {
        for (unsigned int i = 0; i < SimNbBands; i++)
          hxSpectrum[i].second =
              static_cast<typename OutputPixelType::ValueType>
                (m_LabelSpectra[label][i]);
        }
      // or the class does not exist
      else
        {
        for (unsigned int i = 0; i < SimNbBands; i++)
          hxSpectrum[i].second =
              static_cast<typename OutputPixelType::ValueType> (0);
        }
    // Software Guide : EndCodeSnippet
    // Software Guide : BeginLatex
    //
    // Spectral response \code{aResponse} is set using \code{hxSpectrum}.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    ResponseType::Pointer aResponse = ResponseType::New();
    aResponse->SetResponse(hxSpectrum);
    // Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    //
    // Satellite RSR is initialized and set with \code{aResponse}. Reflectance
    // mode is used in this case to take into account solar irradiance into
    // spectral response reduction.
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    ReduceResponseTypePointerType reduceResponse = ReduceResponseType::New();
    reduceResponse->SetInputSatRSR(m_SatRSR);
    reduceResponse->SetInputSpectralResponse(aResponse);
    
    reduceResponse->SetReflectanceMode(true);

    reduceResponse->CalculateResponse();
    VectorPairType reducedResponse =
        reduceResponse->GetReduceResponse()->GetResponse();
    // Software Guide : EndCodeSnippet


    // Software Guide : BeginLatex
    //
    // \code{pix} value is returned for desired Satellite bands
    //
    // Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    for (unsigned int i = 0; i < m_SatRSR->GetNbBands(); i++)
      pix[i] =
          static_cast<typename OutputPixelType::ValueType>
            (reducedResponse[i].second);
    return pix;
    // Software Guide : EndCodeSnippet

  }

  bool operator !=(const ProsailSimulatorFunctor& other) const
  {
    return (m_AcquisitionParameters != other.m_AcquisitionParameters || m_LabelParameters != other.m_LabelParameters);
  }

  bool operator ==(const ProsailSimulatorFunctor& other) const
  {
    return (m_AcquisitionParameters == other.m_AcquisitionParameters && m_LabelParameters == other.m_LabelParameters);
  }

  /** The spectra associated with labels are supposed to have the same
   * sampling as a Prosail simulation, that is, 2000 bands starting at
   * 400nm up to 2400 nm*/
  inline void SetLabelSpectra(const TLabelSpectra& lSpectra)
  {
    m_LabelSpectra = lSpectra;
  }

  inline void SetLabelParameters(const TLabelParameter& lParameters)
  {
    m_LabelParameters = lParameters;
  }

  inline void SetAcquisitionParameters(const TAcquistionParameter& aParameters)
  {
    m_AcquisitionParameters = aParameters;
  }

  inline void SetRSR(const SatRSRPointerType rsr)
  {
    m_SatRSR = rsr;
  }

  inline SatRSRPointerType GetRSR() const
  {
    return m_SatRSR;
  }

protected:

  /** Spectra associated to labels*/
  LabelSpectraType m_LabelSpectra;
  /** Prospect+sail parameters to labels*/
  LabelParameterType m_LabelParameters;
  /** Prospect+sail acquisition parameters*/
  AcquistionParameterType m_AcquisitionParameters;
  /** Satellite Relative spectral response*/
  SatRSRPointerType m_SatRSR;

  /** Simulate pixels with mask != 0 ==> m_InvertedMask = false; */
  bool m_InvertedMask;

};

} // namespace Functor

// Software Guide : BeginLatex
//
// \code{TernaryFunctorImageFilterWithNBands} class is defined here.
// This class inherits form \doxygen{itk::TernaryFunctorImageFilter} with additionnal nuber of band parameters.
// It's implementation is done to process Label, LAI, and mask image with Simulation functor.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
template <class TInputImage1, class TInputImage2, class TInputImage3,
    class TOutputImage, class TFunctor>
class ITK_EXPORT TernaryFunctorImageFilterWithNBands :
  public itk::TernaryFunctorImageFilter< TInputImage1, TInputImage2,
  TInputImage3, TOutputImage, TFunctor >
{
public:
  typedef TernaryFunctorImageFilterWithNBands Self;
  typedef itk::TernaryFunctorImageFilter< TInputImage1, TInputImage2,
      TInputImage3, TOutputImage, TFunctor > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Macro defining the type*/
  itkTypeMacro(TernaryFunctorImageFilterWithNBands, SuperClass);

  /** Accessors for the number of bands*/
  itkSetMacro(NumberOfOutputBands, unsigned int);
  itkGetConstMacro(NumberOfOutputBands, unsigned int);

protected:
  TernaryFunctorImageFilterWithNBands() {}
  virtual ~TernaryFunctorImageFilterWithNBands() {}

  void GenerateOutputInformation()
  {
    Superclass::GenerateOutputInformation();
    this->GetOutput()->SetNumberOfComponentsPerPixel( m_NumberOfOutputBands );
  }
private:
  TernaryFunctorImageFilterWithNBands(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  unsigned int m_NumberOfOutputBands;


};
// EndCodeSnippet
}

int main(int argc, char *argv[])
{
  char *cmifname = NULL;
  if (argc != 10)
    {
    if (argc == 11) // cloud mask filename optional parameter
      {
      cmifname = argv[10];
      }
    else
      {
      std::cerr << "Wrong Parameters " << std::endl;
      return EXIT_FAILURE;
      }
    }
  char * laiifname = argv[1];
  char * outfname = argv[2];
  char * lifname = argv[3];
  char * mifname = argv[4];
  char * lpfname = argv[5];
  char * apfname = argv[6];
  char * rsfname = argv[7];
  unsigned int nbBands = static_cast<unsigned int> (atoi(argv[8]));
  char * rootPath = argv[9];

  // Read the label parameter file. It is assumed to have the form
  // label 1 Cab Car CBrown Cw Cm N   --> for vegetation classes
  // label 0 /path/to/spectra/file    --> for other classes


  // Software Guide : BeginLatex
  //
  // input images typedef are presented below. This example uses \code{double} LAI image, \code{binary} mask and
  //  cloud mask, and \code{integer} label image
  //
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef double LAIPixelType;
  typedef unsigned short LabelType;
  typedef unsigned short MaskPixelType;
  typedef float OutputPixelType;
  // Image typedef
  typedef otb::Image<LAIPixelType, 2> LAIImageType;
  typedef otb::Image<LabelType, 2> LabelImageType;
  typedef otb::Image<MaskPixelType, 2> MaskImageType;
  typedef otb::VectorImage<OutputPixelType, 2> SimulatedImageType;
  // Software Guide : EndCodeSnippet

  // reader typedef
  typedef otb::ImageFileReader<LAIImageType> LAIReaderType;
  typedef otb::ImageFileReader<LabelImageType> LabelReaderType;
  typedef otb::ImageFileReader<MaskImageType> MaskReaderType;

  // Software Guide : BeginCodeSnippet
  // Software Guide : BeginLatex
  //
  // Leaf parameters typedef is defined.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::LeafParameters LeafParametersType;
  typedef LeafParametersType::Pointer LeafParametersPointerType;
  typedef std::map<LabelType, LeafParametersPointerType> LabelParameterMapType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Sensor spectral response typedef is defined
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef double PrecisionType;
  typedef std::vector<PrecisionType> SpectraType;
  typedef std::map<LabelType, SpectraType> SpectraParameterType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Acquisition response typedef is defined
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef std::map<std::string, double> AcquistionParsType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Satellite typedef is defined
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::SatelliteRSR<PrecisionType, PrecisionType> SatRSRType;
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // Filter type is the specific \code{TernaryFunctorImageFilterWithNBands} defined below with specific functor.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Functor::ProsailSimulatorFunctor
      <LAIPixelType, LabelType, MaskPixelType, SimulatedImageType::PixelType,
      SpectraParameterType, LabelParameterMapType, AcquistionParsType, SatRSRType>
        SimuFunctorType;
  typedef otb::TernaryFunctorImageFilterWithNBands
      <LAIImageType, LabelImageType, MaskImageType, SimulatedImageType,
        SimuFunctorType> SimulatorType;
  // Software Guide : EndCodeSnippet

  // Read the acquisition parameter file which is like
  // Angl val
  // PSoil val
  // Skyl val
  // HSpot val
  // TTS val
  // TTO val
  // PSI val

  // Software Guide : BeginLatex
  //
  // Acquisition parameters are loaded using text file. A detailled definition of acquisition parameters can
  // be found in class \doxygen{SailModel}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  AcquistionParsType acquistionPars;
  acquistionPars[std::string("Angl")] = 0.0;
  acquistionPars[std::string("PSoil")] = 0.0;
  acquistionPars[std::string("Skyl")] = 0.0;
  acquistionPars[std::string("HSpot")] = 0.0;
  acquistionPars[std::string("TTS")] = 0.0;
  acquistionPars[std::string("TTO")] = 0.0;
  acquistionPars[std::string("PSI")] = 0.0;

  std::ifstream acquistionParsFile;

  try
    {
    acquistionParsFile.open(apfname);
    }
  catch (...)
    {
    std::cerr << "Could not open file " << apfname << std::endl;
    return EXIT_FAILURE;
    }

  //unsigned int acPar = 0;
  while (acquistionParsFile.good())
    {
    std::string line;
    std::getline(acquistionParsFile, line);
    std::stringstream ss(line);
    std::string parName;
    ss >> parName;
    double parValue;
    ss >> parValue;
    acquistionPars[parName] = parValue;

    }

  acquistionParsFile.close();
  //Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // Label parameters are loaded using text file.
  // Two type of object characteristic can be found. If label corresponds to vegetation class,
  // then leaf parameters are loaded.
  // A detailled definition of leaf parameters can be found in class \doxygen{otb}{LeafParameters} class.
  // Otherwise object reflectance is generated from $400$ to $2400nm$ using \href{http://speclib.jpl.nasa.gov/}{Aster database}.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  LabelParameterMapType labelParameters;
  std::ifstream labelParsFile;

  SpectraParameterType spectraParameters;
  try
    {
    labelParsFile.open(lpfname, std::ifstream::in);
    }
  catch (...)
    {
    std::cerr << "Could not open file " << lpfname << std::endl;
    return EXIT_FAILURE;
    }

  while (labelParsFile.good())
    {
    char fileLine[256];
    labelParsFile.getline(fileLine, 256);

    if (fileLine[0] != '#')
      {
      std::stringstream ss(fileLine);
      unsigned short label;
      ss >> label;
      unsigned short paramsOrSpectra;
      ss >> paramsOrSpectra;
      if (paramsOrSpectra == 1)
        {
        double Cab;
        ss >> Cab;
        double Car;
        ss >> Car;
        double CBrown;
        ss >> CBrown;
        double Cw;
        ss >> Cw;
        double Cm;
        ss >> Cm;
        double N;
        ss >> N;

        LeafParametersType::Pointer leafParams = LeafParametersType::New();

        leafParams->SetCab(Cab);
        leafParams->SetCar(Car);
        leafParams->SetCBrown(CBrown);
        leafParams->SetCw(Cw);
        leafParams->SetCm(Cm);
        leafParams->SetN(N);

        labelParameters[label] = leafParams;
        }
      else
        {
        std::string spectraFilename = rootPath;
        ss >> spectraFilename;
        spectraFilename = rootPath + spectraFilename;
        typedef otb::SpectralResponse<PrecisionType, PrecisionType> ResponseType;
        ResponseType::Pointer resp = ResponseType::New();

        // Coefficient 100 since Aster database is given in % reflectance
        resp->Load(spectraFilename, 100.0);

        SpectraType spec;
        for (unsigned int i = 0; i < SimuFunctorType::SimNbBands; i++)
          //Prosail starts at 400 and lambda in Aster DB is in micrometers
          spec.push_back
            (static_cast<PrecisionType> ((*resp)((i + 400.0) / 1000.0)));

        spectraParameters[label] = spec;
        }
      }
    }

  labelParsFile.close();
  //Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // LAI image is read.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  LAIReaderType::Pointer laiReader = LAIReaderType::New();
  laiReader->SetFileName(laiifname);
  laiReader->Update();
  LAIImageType::Pointer laiImage = laiReader->GetOutput();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Label image is then read. Label image is processed using \code{ImageUniqueValuesCalculator} in order to check if all the labels are present in the labelParameters file.
  // Software Guide : EndLatex


  // Software Guide : BeginCodeSnippet
  LabelReaderType::Pointer labelReader = LabelReaderType::New();
  labelReader->SetFileName(lifname);
  labelReader->Update();

  LabelImageType::Pointer labelImage = labelReader->GetOutput();

  typedef otb::ImageUniqueValuesCalculator<LabelImageType> UniqueCalculatorType;

  UniqueCalculatorType::Pointer uniqueCalculator = UniqueCalculatorType::New();

  uniqueCalculator->SetImage(labelImage);

  UniqueCalculatorType::ArrayType uniqueVals =
      uniqueCalculator->GetUniqueValues();
  std::cout << "Labels are " << std::endl;
  UniqueCalculatorType::ArrayType::const_iterator uvIt = uniqueVals.begin();

  while (uvIt != uniqueVals.end())
    {
    std::cout << (*uvIt) << ", ";
    ++uvIt;
    }

  std::cout << std::endl;

  uvIt = uniqueVals.begin();

  while (uvIt != uniqueVals.end())
    {
    if (labelParameters.find(static_cast<LabelType>
        (*uvIt)) == labelParameters.end() &&
        spectraParameters.find(static_cast<LabelType> (*uvIt)) ==
            spectraParameters.end() &&
        static_cast<LabelType> (*uvIt) != 0)
      {
      std::cout << "label " << (*uvIt) << " not found in " <<
          lpfname << std::endl;
      return EXIT_FAILURE;
      }
    ++uvIt;
    }
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // Mask image is read. If cloud mask is filename is given, a new mask image is generated with masks concatenation.
  // Software Guide : EndLatex


  // Software Guide : BeginCodeSnippet
  MaskReaderType::Pointer miReader = MaskReaderType::New();
  miReader->SetFileName(mifname);
  miReader->UpdateOutputInformation();
  MaskImageType::Pointer maskImage = miReader->GetOutput();

  if (cmifname != NULL)
    {

    MaskReaderType::Pointer cmiReader = MaskReaderType::New();
    cmiReader->SetFileName(cmifname);
    cmiReader->UpdateOutputInformation();

    typedef itk::OrImageFilter
        <MaskImageType, MaskImageType, MaskImageType> OrType;
    OrType::Pointer orfilter = OrType::New();

    orfilter->SetInput1(miReader->GetOutput());
    orfilter->SetInput2(cmiReader->GetOutput());

    orfilter->Update();
    maskImage = orfilter->GetOutput();

    }
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // A test is done. All images must have the same size.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  if (laiImage->GetLargestPossibleRegion().GetSize()[0] !=
      labelImage->GetLargestPossibleRegion().GetSize()[0] ||
      laiImage->GetLargestPossibleRegion().GetSize()[1] !=
          labelImage->GetLargestPossibleRegion().GetSize()[1] ||
      laiImage->GetLargestPossibleRegion().GetSize()[0] !=
          maskImage->GetLargestPossibleRegion().GetSize()[0] ||
      laiImage->GetLargestPossibleRegion().GetSize()[1] !=
          maskImage->GetLargestPossibleRegion().GetSize()[1])
    {
    std::cerr << "Image of labels, mask and LAI image must have the same size"
        << std::endl;
    return EXIT_FAILURE;
    }
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // Satellite RSR (Reduced Spectral Response) is defined using filename and band number given by command line arguments.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  SatRSRType::Pointer satRSR = SatRSRType::New();
  satRSR->SetNbBands(nbBands);
  satRSR->SetSortBands(false);
  satRSR->Load(rsfname);

  for (unsigned int i = 0; i < nbBands; ++i)
    std::cout << i << " " << (satRSR->GetRSR())[i]->GetInterval().first << " "
        << (satRSR->GetRSR())[i]->GetInterval().second << std::endl;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // At this step all initialization have been done. The next step is to implement and initialize simulation functor \code{ProsailSimulatorFunctor}.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  SimuFunctorType simuFunctor;
  simuFunctor.SetLabelParameters(labelParameters);
  simuFunctor.SetLabelSpectra(spectraParameters);
  simuFunctor.SetAcquisitionParameters(acquistionPars);
  simuFunctor.SetRSR(satRSR);
  simuFunctor.SetInvertedMask(true);
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // Inputs and Functor are plugged to simulator filter.
  //
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  SimulatorType::Pointer simulator = SimulatorType::New();
  simulator->SetInput1(laiImage);
  simulator->SetInput2(labelImage);
  simulator->SetInput3(maskImage);
  simulator->SetFunctor(simuFunctor);
  simulator->SetNumberOfOutputBands(nbBands);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //  The invocation of the \code{Update()} method triggers the
  //  execution of the pipeline.
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  simulator->Update();
  // Software Guide : EndCodeSnippet


  // Write output image to disk

  typedef otb::ImageFileWriter<SimulatedImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(outfname);
  writer->SetInput(simulator->GetOutput());
  // writer->SetBufferNumberOfLinesDivisions(nbStreams);

  otb::StandardWriterWatcher watcher(writer, simulator, "Simulation");

  writer->Update();

  return EXIT_SUCCESS;

}


