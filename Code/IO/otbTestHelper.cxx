/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbTestHelper.h"
#include "otbMacro.h"
#include <iostream>
#include <fstream>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbDifferenceImageFilter.h"
#include "otbPrintableImageFilter.h"

#define ITK_TEST_DIMENSION_MAX 6

namespace otb
{


  int TestHelper::RegressionTestAsciiFile(const char * testAsciiFileName, const char * baselineAsciiFileName, int reportErrors, const double epsilon, std::vector<std::string> ignoredLines) const
  {
    std::ifstream fluxfiletest(testAsciiFileName);
    std::ifstream fluxfileref(baselineAsciiFileName);
    enum TypeEtat { ETAT_NUM, ETAT_CHAR };

    std::string diffAsciiFileName(testAsciiFileName);
    diffAsciiFileName += ".diff.txt";
    std::ofstream fluxfilediff;
    if ( reportErrors )
    {
      fluxfilediff.open(diffAsciiFileName.c_str());
    }

    std::string strfiletest;
    std::string strfileref;

    int nbdiff(0);
    int numLine(1);
    if (!fluxfiletest)
    {
      itkGenericExceptionMacro(<<"Impossible to open the test ASCII file <"<<testAsciiFileName<<">.");
    }
    if (!fluxfileref)
    {
      itkGenericExceptionMacro(<< "Impossible to open the baseline ASCII file <"<<baselineAsciiFileName<<">.");
    }

    TypeEtat etatPrec(ETAT_NUM), etatCour(ETAT_NUM);

    std::vector<std::string> listStrDiffLineFileRef;
    std::vector<std::string> listStrDiffLineFileTest;



    while ( std::getline(fluxfileref,strfileref)!=0  )
    {
      otb::StringStream buffstreamRef, buffstreamTest;
      buffstreamRef << strfileref;

    //check if we've reach end of test file
      if (std::getline(fluxfiletest,strfiletest) == 0)
      {
        std::string strRef = "";

        buffstreamRef >> strRef;
        fluxfilediff << "Line missing in test file: " << numLine
            << " : " << strRef
            << std::endl;
        nbdiff++;
      }
      else
      {

        bool foundexpr = false;
        if (ignoredLines.size()>0)
        {

          std::vector<std::string>::iterator itIgnoredLines = ignoredLines.begin();

          for (;(itIgnoredLines != ignoredLines.end()); ++itIgnoredLines)
          {
            std::string ignoredLinesAscii = (*itIgnoredLines);
            std::string::size_type loc = strfileref.find(ignoredLinesAscii);
            if ( loc != std::string::npos )
            {
              foundexpr = true;
            }

          }

        }

        if ( foundexpr == false )
        {
          buffstreamTest << strfiletest;
          int nblinediff(0);

          while (buffstreamRef.peek() != EOF)
          {
            std::string strRef = "";
            std::string strTest = "";

            std::string strNumRef = "";
            std::string strCharRef = "";
            std::string strNumTest = "";
            std::string strCharTest = "";

            buffstreamRef >> strRef;
            buffstreamTest >> strTest;

            bool chgt= false;
            std::string charTmpRef = "";
            std::string charTmpTest = "";
            unsigned int i=0;
            if (!isHexaPointerAddress(strRef))
            {
            //Analyse if strRef contains scientific value (ex: "-142.124e-012")
              if (isScientificNumeric(strRef))
              {
                if (!isScientificNumeric(strTest))
                {
                  if ( reportErrors )
                  {
                    fluxfilediff << "Diff at line " << numLine << " : compare numeric value with no numeric value ("<<strRef
                        << strRef << " != " << strTest <<")"<< std::endl;
                    nblinediff++;
                  }
                  nbdiff++;

                }
                else if ( (strRef != strTest) && (vcl_abs(atof(strRef.c_str())-atof(strTest.c_str())) > epsilon) )
                {
                  if ( reportErrors )
                  {
                    fluxfilediff << "Diff at line " << numLine << " : vcl_abs ( ("
                        << strRef << ") - (" << strTest
                        << ") ) > " << epsilon << std::endl;
                    nblinediff++;
                  }
                  nbdiff++;
                }
              }
              else
              {
                while (i < strRef.size())
                {
                  charTmpRef=strRef[i];

                  if (i<strTest.size())
                  {
                    charTmpTest=strTest[i];
                  }

                  if (isNumeric(charTmpRef))
                    etatCour = ETAT_NUM;
                  else
                    etatCour = ETAT_CHAR;

                // "reference" state initialisation.
                  if (i==0)
                    etatPrec=etatCour;

                // Case where there's a number after characteres.
                  if ((etatCour==ETAT_NUM)&&(etatPrec==ETAT_CHAR))
                  {
                    if ( strCharRef != strCharTest )
                    {
                      if ( reportErrors )
                      {
                        fluxfilediff << "Diff at line " << numLine
                            << " : " << strCharRef
                            << " != " << strCharTest << std::endl;
                        nblinediff++;
                      }
                      nbdiff++;
                    }

                    strCharRef="";
                    strCharTest="";
                    strNumRef=charTmpRef;
                    strNumTest=charTmpTest;
                    chgt=true;
                  }
                // Case where there's a character after numbers.
                  else if ((etatCour==ETAT_CHAR)&&(etatPrec==ETAT_NUM))
                  {

                    if ( (strNumRef != strNumTest) && (vcl_abs(atof(strNumRef.c_str())-atof(strNumTest.c_str())) > epsilon) )
                    {
                      if ( reportErrors )
                      {
                        fluxfilediff << "Diff at line " << numLine << " : vcl_abs ( ("
                            << strNumRef << ") - (" << strNumTest
                            << ") ) > " << epsilon << std::endl;
                        nblinediff++;
                      }
                      nbdiff++;
                    }

                    strNumRef="";
                    strNumTest="";
                    strCharRef=charTmpRef;
                    strCharTest=charTmpTest;
                    chgt=true;
                  }
                  else if (etatCour==etatPrec)
                  {
                    if (etatCour==ETAT_CHAR)
                    {
                      strCharRef+=charTmpRef;
                      strCharTest+=charTmpTest;
                    }
                    else
                    {
                      strNumRef+=charTmpRef;
                      strNumTest+=charTmpTest;
                    }
                  }

                  etatPrec = etatCour;
                  i++;
                }

              // Simpliest case : string characters or numeric value between 2 separators
                if (!chgt)
                {
                  if (isNumeric(strRef))
                  {

                    if ( ( strRef != strTest) && (vcl_abs(atof(strRef.c_str())-atof(strTest.c_str())) > epsilon))
                    {
                      if ( reportErrors )
                      {
                        fluxfilediff << "Diff at line " << numLine << " : vcl_abs( ("
                            << strRef << ") - (" << strTest
                            << ") ) > " << epsilon << std::endl;
                        nblinediff++;
                      }
                      nbdiff++;
                    }
                  }
                  else
                  {
                    if ( strRef != strTest )
                    {
                      if ( reportErrors )
                      {
                        fluxfilediff << "Diff at line " << numLine
                            << " : " << strRef
                            << " != " << strTest << std::endl;
                        nblinediff++;
                      }
                      nbdiff++;
                    }
                  }
                }
              } // else
            } // if(!isHexaPointerAddress(strRef))
            else
            {
              if ( reportErrors )
              {
                fluxfilediff<<"Pointer address found at line "<<numLine
                    <<" : "<<strRef
                    <<" -> comparison skipped."<<std::endl;
              }
            }
          }

          numLine++;
        //Store alls differences lines
          if ( nblinediff!=0 && reportErrors)
          {
            listStrDiffLineFileRef.push_back(strfileref);
            listStrDiffLineFileTest.push_back(strfiletest);
          }

        }
      }

    }


    while (std::getline(fluxfiletest,strfiletest) != 0)
    {
      int nblinediff(0);
      otb::StringStream buffstreamTest;
      std::string strTest = "";
      buffstreamTest << strfiletest;
      buffstreamTest >> strTest;
      fluxfilediff << "Additional line in test file: " << numLine
          << " : " << strTest
          << std::endl;
      nblinediff++;
      nbdiff++;
      if ( nblinediff!=0 && reportErrors)
      {
        listStrDiffLineFileRef.push_back(strfileref);
        listStrDiffLineFileTest.push_back(strfiletest);
      }
    }


    fluxfiletest.close();
    fluxfileref.close();
    if ( reportErrors )
    {
      fluxfilediff.close();
    }

    if ( nbdiff!=0 && reportErrors)
    {
      std::cout << "<DartMeasurement name=\"ASCIIFileError\" type=\"numeric/int\">";
      std::cout << nbdiff;
      std::cout <<  "</DartMeasurement>" << std::endl;
      std::cout << "================================================================"<<std::endl;
      std::cout << "baseline ASCII File : "<<baselineAsciiFileName << std::endl;
      std::cout << "Test ASCII File     : "<<testAsciiFileName << std::endl;
      std::cout << "Diff ASCII File     : "<<diffAsciiFileName << std::endl;
      std::cout << "Tolerance value     : "<<epsilon << std::endl;

      std::cout << "Nb lines differents : "<<listStrDiffLineFileRef.size() << std::endl;
      for ( unsigned int i = 0; i  < listStrDiffLineFileRef.size(); i++)
      {
        std::cout << "   -------------------------------"<<std::endl;
        std::cout << "   Base << "<<listStrDiffLineFileRef[i]<<std::endl;
        std::cout << "   Test >> "<<listStrDiffLineFileTest[i]<<std::endl;
      }
    }
    return (nbdiff != 0) ? 1 : 0;
  }

  int TestHelper::RegressionTestBinaryFile(const char * testBinaryFileName, const char * baselineBinaryFileName, int reportErrors) const
  {
    int nbdiff(0);
    std::ifstream fluxfiletest(testBinaryFileName,std::ifstream::binary);
    std::ifstream fluxfileref(baselineBinaryFileName,std::ifstream::binary);
    if (fluxfiletest.fail())
    {
      itkGenericExceptionMacro(<<"Impossible to open the test binary file <" << testBinaryFileName << ">.");
    }
    if (!fluxfileref)
    {
      itkGenericExceptionMacro(<< "Impossible to open the baseline binary file <" << baselineBinaryFileName << ">.");
    }
    while ( fluxfiletest.good() && fluxfileref.good() )
    {
      if ( fluxfiletest.get() != fluxfileref.get() )
      {
        nbdiff++;
      }
    }
    fluxfiletest.close();
    fluxfileref.close();

    if ( nbdiff!=0 && reportErrors)
    {
      std::cout << "<DartMeasurement name=\"BINARYFileError\" type=\"numeric/int\">";
      std::cout << nbdiff;
      std::cout <<  "</DartMeasurement>" << std::endl;
    }
    return (nbdiff != 0) ? 1 : 0;
  }


  int TestHelper::RegressionTestImage (int cpt, const char *testImageFilename, const char *baselineImageFilename, int reportErrors, const double toleranceDiffPixelImage) const
  {
  // Use the factory mechanism to read the test and baseline files and convert them to double

    typedef otb::VectorImage<double,2> ImageType;
    typedef otb::VectorImage<unsigned char,2> OutputType;
    typedef otb::VectorImage<unsigned char,2> DiffOutputType;
    typedef otb::ImageFileReader<ImageType> ReaderType;

  // Read the baseline file
    ReaderType::Pointer baselineReader = ReaderType::New();
    baselineReader->SetFileName(baselineImageFilename);
    try
    {
      baselineReader->UpdateLargestPossibleRegion();
    }
    catch (itk::ExceptionObject& e)
    {
      itkGenericExceptionMacro(<< "Exception detected while reading "
          << baselineImageFilename << " : "  << e.GetDescription());
      return 1000;
    }

  // Read the file generated by the test
    ReaderType::Pointer testReader = ReaderType::New();
    testReader->SetFileName(testImageFilename);
    try
    {
      testReader->UpdateLargestPossibleRegion();
    }
    catch (itk::ExceptionObject& e)
    {
      itkGenericExceptionMacro(<< "Exception detected while reading "
          << testImageFilename << " : "  << e.GetDescription() );
      return 1000;
    }

  // The sizes of the baseline and test image must match
    ImageType::SizeType baselineSize;
    baselineSize = baselineReader->GetOutput()->GetLargestPossibleRegion().GetSize();
    ImageType::SizeType testSize;
    testSize = testReader->GetOutput()->GetLargestPossibleRegion().GetSize();

    if (baselineSize != testSize)
    {
      std::cerr << "The size of the baseline image and Test image do not match!" << std::endl;
      std::cerr << "baseline image: " << baselineImageFilename
          << " has size " << baselineSize << std::endl;
      std::cerr << "Test image:     " << testImageFilename
          << " has size " << testSize << std::endl;
      return 1;
    }


  // Now compare the two images
    typedef otb::DifferenceImageFilter<ImageType,ImageType> DiffType;
    DiffType::Pointer diff = DiffType::New();
    diff->SetValidInput(baselineReader->GetOutput());
    diff->SetTestInput(testReader->GetOutput());
    diff->SetDifferenceThreshold(toleranceDiffPixelImage);
    diff->UpdateLargestPossibleRegion();

    ImageType::PixelType status = diff->GetTotalDifference();
    unsigned long numberOfPixelsWithDifferences = diff->GetNumberOfPixelsWithDifferences();

  //Write only one this message
    if (reportErrors == 0)
    {
      otbGenericMsgDebugMacro(<< "RegressionTestImage DifferenceThreshold: "<<toleranceDiffPixelImage);
      otbGenericMsgDebugMacro(<< "Status diff->GetTotalDifference:         "
          << status <<" for "<<numberOfPixelsWithDifferences<<" pixel(s)." );
    }
  // if there are discrepencies, create an diff image
    if (status.GetSquaredNorm()>0 && reportErrors)
    {
      typedef otb::PrintableImageFilter<ImageType> RescaleType;
      typedef RescaleType::OutputImageType OutputType;
      /*     typedef itk::ExtractImageFilter<OutputType,DiffOutputType> ExtractType; */
      typedef otb::ImageFileWriter<RescaleType::OutputImageType> WriterType;

      RescaleType::Pointer rescale = RescaleType::New();

      WriterType::Pointer writer = WriterType::New();
      writer->SetInput(rescale->GetOutput());

//    std::cout << "<DartMeasurement name=\"ImageError\" type=\"numeric/double\">";
      std::cout << "<DartMeasurement name=\"ImageError "<<cpt<<"\" type=\"numeric/double\">";
      std::cout << status;
      std::cout <<  "</DartMeasurement>" << std::endl;

      std::cout << "<DartMeasurement name=\"NumberOfPixelsWithDifferences "<<cpt<<"\" type=\"numeric/integer\">";
      std::cout << numberOfPixelsWithDifferences;
      std::cout <<  "</DartMeasurement>" << std::endl;

      std::cout << "<DartMeasurement name=\"ToleranceDiffPixelImage\" type=\"numeric/double\">";
      std::cout << toleranceDiffPixelImage;
      std::cout <<  "</DartMeasurement>" << std::endl;

        ::itk::OStringStream diffName;
        diffName << testImageFilename << ".diff.png";
        try
        {
          rescale->SetInput(diff->GetOutput());

          for (unsigned int i = 1;i<=min(diff->GetOutput()->GetNumberOfComponentsPerPixel(),3U);++i)
          {
            rescale->SetChannel(i);
          }

          rescale->Update();
        }
        catch (...)
        {
          itkGenericExceptionMacro(<< "Error during rescale of " << diffName.str() );
        }
        writer->SetFileName(diffName.str().c_str());
        try
        {
          writer->Update();
        }
        catch (...)
        {
          itkGenericExceptionMacro(<< "Error during write of " << diffName.str() );
        }

//    std::cout << "<DartMeasurementFile name=\"DifferenceImage\" type=\"image/png\">";
        std::cout << "<DartMeasurementFile name=\"DifferenceImage "<<cpt<<"\" type=\"image/png\">";
        std::cout << diffName.str();
        std::cout << "</DartMeasurementFile>" << std::endl;

        ::itk::OStringStream baseName;
        baseName << testImageFilename << ".base.png";
        try
        {
          rescale->SetInput(baselineReader->GetOutput());
          rescale->Update();
        }
        catch (...)
        {
          itkGenericExceptionMacro(<<"Error during rescale of " << baseName.str() );
        }
        try
        {
          writer->SetFileName(baseName.str().c_str());
          writer->Update();
        }
        catch (...)
        {
          itkGenericExceptionMacro(<<"Error during write of " << baseName.str() );
        }

//    std::cout << "<DartMeasurementFile name=\"baselineImage\" type=\"image/png\">";
        std::cout << "<DartMeasurementFile name=\"baselineImage "<<cpt<<"\" type=\"image/png\">";
        std::cout << baseName.str();
        std::cout << "</DartMeasurementFile>" << std::endl;

        ::itk::OStringStream testName;
        testName << testImageFilename << ".test.png";
        try
        {
          rescale->SetInput(testReader->GetOutput());
          rescale->Update();
        }
        catch (...)
        {
          itkGenericExceptionMacro(<< "Error during rescale of " << testName.str());
        }
        try
        {
          writer->SetFileName(testName.str().c_str());
          writer->Update();
        }
        catch (...)
        {
          itkGenericExceptionMacro(<<"Error during write of " << testName.str() );
        }

//    std::cout << "<DartMeasurementFile name=\"TestImage\" type=\"image/png\">";
        std::cout << "<DartMeasurementFile name=\"TestImage "<<cpt<<"\" type=\"image/png\">";
        std::cout << testName.str();
        std::cout << "</DartMeasurementFile>" << std::endl;


    }
    return (status.GetSquaredNorm()> 0) ? 1 : 0;
  }

  int TestHelper::RegressionTestMetaData (const char *testImageFilename, const char *baselineImageFilename, int reportErrors, const double toleranceDiffPixelImage) const
  {
  // Use the factory mechanism to read the test and baseline files and convert them to double
    typedef otb::Image<double,ITK_TEST_DIMENSION_MAX> ImageType;
    typedef otb::ImageFileReader<ImageType> ReaderType;

  // Read the baseline file
    ReaderType::Pointer baselineReader = ReaderType::New();
    baselineReader->SetFileName(baselineImageFilename);
    try
    {
      baselineReader->GenerateOutputInformation();
    }
    catch (itk::ExceptionObject& e)
    {
      itkGenericExceptionMacro(<< "Exception detected while reading "
          << baselineImageFilename << " : "  << e.GetDescription());
    }

  // Read the baseline file
    ReaderType::Pointer testReader = ReaderType::New();
    testReader->SetFileName(testImageFilename);
    try
    {
      testReader->GenerateOutputInformation();
    }
    catch (itk::ExceptionObject& e)
    {
      itkGenericExceptionMacro(<< "Exception detected while reading "
          << baselineImageFilename << " : "  << e.GetDescription());
    }

    unsigned int errcount = 0;
  // The sizes of the baseline and test image must match
    ImageType::SizeType baselineSize;
    baselineSize = baselineReader->GetOutput()->GetLargestPossibleRegion().GetSize();
    ImageType::SizeType testSize;
    testSize = testReader->GetOutput()->GetLargestPossibleRegion().GetSize();

    if (baselineSize != testSize)
    {
      std::cerr << "The size of the baseline image and Test image do not match!" << std::endl;
      std::cerr << "baseline image: " << baselineImageFilename
          << " has size " << baselineSize << std::endl;
      std::cerr << "Test image:     " << testImageFilename
          << " has size " << testSize << std::endl;
      errcount++;
    }
    ImageType::Pointer blImPtr = baselineReader->GetOutput();
    ImageType::Pointer testImPtr = testReader->GetOutput();

  // test orgin
    if (blImPtr->GetOrigin()!=testImPtr->GetOrigin())
    {
      std::cerr << "The origin of the baseline image and Test image do not match!" << std::endl;
      std::cerr << "baseline image: " << baselineImageFilename
          << " has origin " << blImPtr->GetOrigin() << std::endl;
      std::cerr << "Test image:     " << testImageFilename
          << " has origin "<< testImPtr->GetOrigin() << std::endl;
      errcount++;
    }

  // test spacing
    if (blImPtr->GetSpacing()!=testImPtr->GetSpacing())
    {
      std::cerr << "The spacing of the baseline image and Test image do not match!" << std::endl;
      std::cerr << "baseline image: " << baselineImageFilename
          << " has spacing " << blImPtr->GetSpacing() << std::endl;
      std::cerr << "Test image:     " << testImageFilename
          << " has spacing "<< testImPtr->GetSpacing() << std::endl;
      errcount++;
    }

  // test projection reference
    if (blImPtr->GetProjectionRef().compare(testImPtr->GetProjectionRef())!=0)
    {
      std::cerr << "The projection reference of the baseline image and Test image do not match!" << std::endl;
      std::cerr << "baseline image: " << baselineImageFilename
          << " has projection reference " << blImPtr->GetProjectionRef() << std::endl;
      std::cerr << "Test image:     " << testImageFilename
          << " has projection reference " << testImPtr->GetProjectionRef() << std::endl;
      errcount++;
    }

  // test Geographic transform
    if (blImPtr->GetGeoTransform()!=testImPtr->GetGeoTransform())
    {
      std::cerr << "The geographic transform of the baseline image and Test image do not match!" << std::endl;
      std::cerr << "baseline image: " << baselineImageFilename
          << " has geographic transform " << VectorToString(blImPtr->GetGeoTransform()) << std::endl;
      std::cerr << "Test image:     " << testImageFilename
          << " has geographic transform " << VectorToString(testImPtr->GetGeoTransform()) << std::endl;
      errcount++;
    }

// test upper left corner
    if (blImPtr->GetUpperLeftCorner()!=testImPtr->GetUpperLeftCorner())
    {
      std::cerr << "The upper left corner of the baseline image and Test image do not match!" << std::endl;
      std::cerr << "baseline image: " << baselineImageFilename
          << " has upper left corner " << VectorToString(blImPtr->GetUpperLeftCorner()) << std::endl;
      std::cerr << "Test image:     " << testImageFilename
          << " has upper left corner " <<VectorToString( testImPtr->GetUpperLeftCorner()) << std::endl;
      errcount++;
    }

// test upper right corner
    if (blImPtr->GetUpperRightCorner()!=testImPtr->GetUpperRightCorner())
    {
      std::cerr << "The upper right corner of the baseline image and Test image do not match!" << std::endl;
      std::cerr << "baseline image: " << baselineImageFilename
          << " has upper right corner " <<VectorToString( blImPtr->GetUpperRightCorner()) << std::endl;
      std::cerr << "Test image:     " << testImageFilename
          << " has upper right corner " << VectorToString(testImPtr->GetUpperRightCorner()) << std::endl;
      errcount++;
    }

// test lower left corner
    if (blImPtr->GetLowerLeftCorner()!=testImPtr->GetLowerLeftCorner())
    {
      std::cerr << "The lower left corner  of the baseline image and Test image do not match!" << std::endl;
      std::cerr << "baseline image: " << baselineImageFilename
          << " has lower left corner " << VectorToString(blImPtr->GetLowerLeftCorner()) << std::endl;
      std::cerr << "Test image:     " << testImageFilename
          << " has lower left corner " << VectorToString(testImPtr->GetLowerLeftCorner()) << std::endl;
      errcount++;
    }

  // test lower right corner
    if (blImPtr->GetLowerRightCorner()!=testImPtr->GetLowerRightCorner())
    {
      std::cerr << "The lower right of the baseline image and Test image do not match!" << std::endl;
      std::cerr << "baseline image: " << baselineImageFilename
          << " has lower right corner " << VectorToString(blImPtr->GetLowerRightCorner()) << std::endl;
      std::cerr << "Test image:     " << testImageFilename
          << " has lower right corner " << VectorToString(testImPtr->GetLowerRightCorner()) << std::endl;
      errcount++;
    }

  // test gcp projection
    if (blImPtr->GetGCPProjection().compare(testImPtr->GetGCPProjection())!=0)
    {
      std::cerr << "The gcp projection of the baseline image and Test image do not match!" << std::endl;
      std::cerr << "baseline image: " << baselineImageFilename
          << " has gcp projection " << blImPtr->GetGCPProjection() << std::endl;
      std::cerr << "Test image:     " << testImageFilename
          << " has gcp projection " << testImPtr->GetGCPProjection() << std::endl;
      errcount++;
    }


  // test gcp count
    if (blImPtr->GetGCPCount()!=testImPtr->GetGCPCount())
    {
      std::cerr << "The gcp count of the baseline image and Test image do not match!" << std::endl;
      std::cerr << "baseline image: " << baselineImageFilename
          << " has gcp count " << blImPtr->GetGCPCount() << std::endl;
      std::cerr << "Test image:     " << testImageFilename
          << " has gcp count " << testImPtr->GetGCPCount() << std::endl;
      errcount++;
    }
    else
    {
      for (unsigned int i=0;i<blImPtr->GetGCPCount();++i)
      {
        if ((blImPtr->GetGCPId(i).compare(testImPtr->GetGCPId(i))!=0)
             ||(blImPtr->GetGCPInfo(i).compare(testImPtr->GetGCPInfo(i))!=0)
             ||(blImPtr->GetGCPRow(i)!=testImPtr->GetGCPRow(i))
             ||(blImPtr->GetGCPCol(i)!=testImPtr->GetGCPCol(i))
             ||(blImPtr->GetGCPX(i)!=testImPtr->GetGCPX(i))
             ||(blImPtr->GetGCPY(i)!=testImPtr->GetGCPY(i))
             ||(blImPtr->GetGCPZ(i)!=testImPtr->GetGCPZ(i)))
        {
          std::cerr << "The GCP number "<<i<<" of the baseline image and Test image do not match!" << std::endl;
          std::cerr << "baseline image: " << baselineImageFilename
              << " has gcp number "<<i<<" ("
              <<"id: "<<blImPtr->GetGCPId(i)<<", "
              <<"info: "<<blImPtr->GetGCPInfo(i)<<", "
              <<"row: "<<blImPtr->GetGCPRow(i)<<", "
              <<"col: "<<blImPtr->GetGCPCol(i)<<", "
              <<"X: "<<blImPtr->GetGCPX(i)<<", "
              <<"Y: "<<blImPtr->GetGCPY(i)<<", "
              <<"Z: "<<blImPtr->GetGCPZ(i)<<")"<<std::endl;
          std::cerr << "Test image:     " << testImageFilename
              << " has gcp  number "<<i<<" ("
              <<"id: "<<testImPtr->GetGCPId(i)<<", "
              <<"info: "<<testImPtr->GetGCPInfo(i)<<", "
              <<"row: "<<testImPtr->GetGCPRow(i)<<", "
              <<"col: "<<testImPtr->GetGCPCol(i)<<", "
              <<"X: "<<testImPtr->GetGCPX(i)<<", "
              <<"Y: "<<testImPtr->GetGCPY(i)<<", "
              <<"Z: "<<testImPtr->GetGCPZ(i)<<")"<<std::endl;
          errcount++;
        }
      }
    }
    if (errcount>0)
    {
      std::cout << "<DartMeasurement name=\"MetadataError\" type=\"numeric/int\">";
      std::cout << errcount;
      std::cout <<  "</DartMeasurement>" << std::endl;
    }
    return errcount;

  }


  //
// Generate all of the possible baselines
// The possible baselines are generated fromn the baselineFilename using the following algorithm:
// 1) strip the suffix
// 2) append a digit _x
// 3) append the original suffix.
// It the file exists, increment x and continue
  //
  std::map<std::string,int>
      TestHelper::RegressionTestbaselines (char *baselineFilename) const
  {
    std::map<std::string,int> baselines;
    baselines[std::string(baselineFilename)] = 0;

    std::string originalBaseline(baselineFilename);

    int x = 0;
    std::string::size_type suffixPos = originalBaseline.rfind(".");
    std::string suffix;
    if (suffixPos != std::string::npos)
    {
      suffix = originalBaseline.substr(suffixPos,originalBaseline.length());
      originalBaseline.erase(suffixPos,originalBaseline.length());
    }
    while (++x)
    {
        ::itk::OStringStream filename;
        filename << originalBaseline << "." << x << suffix;
        std::ifstream filestream(filename.str().c_str());
        if (!filestream)
        {
          break;
        }
        baselines[filename.str()] = 0;
        filestream.close();
    }
    return baselines;
  }



// Regression Testing Code

  bool TestHelper::isNumber(int i) const
  {
    return ((i>47)&&(i<58));
  }

  bool TestHelper::isHexaNumber(int i) const
  {
    return (((i>47)&&(i<58))||((i>96)&&(i<103)));
  }


  bool TestHelper::isPoint(int i) const
  {
    return (i==46);
  }

  bool TestHelper::isMinusSign(int i) const
  {
    return (i==45);
  }

  bool TestHelper::isNumeric(std::string str) const
  {
    int nbOfPoints = 0;
    int nbOfNumbers = 0;
    int number;
    unsigned int i = 0;
    bool result = true;

    while ((i<str.size())&&(result==true))
    {
      number = str[i];

      if (isPoint(number))
        nbOfPoints++;
      if (isNumber(number))
        nbOfNumbers++;
      if ((!isNumber(number)&&!isPoint(number)&&!isMinusSign(number))
            ||(isMinusSign(number)&&(i!=0)))
        result = false;

      i++;
    }
    if ((str.size()==0)||(nbOfPoints > 1)/*||(nbOfNumbers==0)*/)
      result = false;

    return result;
  }

  bool TestHelper::isScientificNumeric(std::string str) const
  {

    int number(0);
    number = str[0];
    bool pointDetected(false);
    bool eDetected(false);
    bool signDetected(false);

  // Analyse first character (+, -, 0...9)
    unsigned int cpt(0);
    if ( (str[0] != '+') && (str[0] != '-') && (!isNumber(number)) ) return false;
    if ( (str[0] == '+') || (str[0] == '-') )
    {
      cpt++;
      signDetected = true;
    }

    while ( cpt < str.size() )
    {
      if ( str[cpt] == '.' )
      {
      // Exit false if two points or if e already detected
        if ( ( pointDetected == true ) || ( eDetected == true ) ) return false;
        pointDetected = true;
      }
      else if ( ( str[cpt] == 'e' )||( str[cpt] == 'E' ) )
      {
      // Exit false if two e
        if ( eDetected == true ) return false;
        eDetected = true;
      }
      else if ( ( str[cpt] == '-' )||( str[cpt] == '+' ) )
      {
      // Exit false if already sign with no E detected
        if ( ( signDetected == true ) && ( eDetected == false ) )return false;
        signDetected = true;
      }
      else
      {
        number = str[cpt];
        if (!isNumber(number)) return false;
      }
      cpt++;
    }

    return true;
  }

  bool TestHelper::isHexaPointerAddress(std::string str) const
  {
    unsigned int size(0);
    bool result(false);
    unsigned int start(0);
  //If (0xadresss)
    if (  (str[0]==40) &&
           (str[str.size()-1]==41) &&
           (str[1]==48) &&
           (str[2]==120) &&
           (str.size()==11))
    {
      result = true;
      start = 3;
      size = str.size() - 1;
    }
  //If 0xadresss
    else if (
             (str[0]==48) &&
             (str[1]==120) &&
             (str.size()==9))
    {
      result = true;
      start = 2;
      size = str.size();
    }
  // if 01adress
    else if (
             (str[0]==48) &&
             (str[1]==49) &&
             (str.size()==8))
    {
      result = true;
      start = 2;
      size = str.size();
    }

    unsigned int i(start);
    while (result&&(i<size))
    {
      result=result&&isHexaNumber(str[i]);
      ++i;
    }
    return result;
  }

  std::string TestHelper::VectorToString(otb::MetaDataKey::VectorType vector) const
  {
    otb::StringStream oss;
    oss.str("");
    otb::MetaDataKey::VectorType::iterator it = vector.begin();
    oss<<"[";
    while (it!=vector.end())
    {
      oss<<(*it);
      ++it;
      if (it==vector.end())
      {
        oss<<"]";
        break;
      }
      else
      {
        oss<<", ";
      }
    }
    return oss.str();
  }


}

