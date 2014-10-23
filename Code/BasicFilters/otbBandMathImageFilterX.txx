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
#ifndef __otbBandMathImageFilterX_txx
#define __otbBandMathImageFilterX_txx
#include "otbBandMathImageFilterX.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"


#include <iostream>
#include <string>

namespace otb
{

/** Constructor */
template <class TImage>
BandMathImageFilterX<TImage>
::BandMathImageFilterX()
{
  //This number will be incremented each time an image
  //is added over the one minimumrequired
  this->SetNumberOfRequiredInputs( 1 );
  this->InPlaceOff();

  m_UnderflowCount = 0;
  m_OverflowCount = 0;
  m_ThreadUnderflow.SetSize(1);
  m_ThreadOverflow.SetSize(1);


  //idxX and idxY
  adhocStruct ahcX;
  ahcX.name = "idxX";
  ahcX.type = 0;
  m_VAllowedVarName.push_back(ahcX);

  adhocStruct ahcY;
  ahcY.name = "idxY";
  ahcY.type = 1;
  m_VAllowedVarName.push_back(ahcY);

  //this->SetNumberOfThreads(1);

}

/** Destructor */
template <class TImage>
BandMathImageFilterX<TImage>
::~BandMathImageFilterX()
{
}


template <class TImage>
void BandMathImageFilterX<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Expression: "      << m_Expression[0]                  << std::endl;
  os << indent << "Computed values follow:"                            << std::endl;
  os << indent << "UnderflowCount: "  << m_UnderflowCount              << std::endl;
  os << indent << "OverflowCount: "   << m_OverflowCount               << std::endl;
  os << indent << "itk::NumericTraits<typename PixelType::ValueType>::NonpositiveMin()  :  "
               << itk::NumericTraits<typename PixelType::ValueType>::NonpositiveMin()      << std::endl;
  os << indent << "itk::NumericTraits<typename PixelType::ValueType>::max()  :             "
         << itk::NumericTraits<typename PixelType::ValueType>::max()                 << std::endl;
}

template <class TImage>
void BandMathImageFilterX<TImage>
::SetNthInput(unsigned int idx, const ImageType * image)
{

  std::stringstream sstm;
  sstm << "im" << (idx+1);
  this->SetNthInput(idx, image, sstm.str());
}

template <class TImage>
void BandMathImageFilterX<TImage>
::SetNthInput(unsigned int idx, const ImageType * image, const std::string& varName)
{
  this->SetInput(idx, const_cast<TImage *>( image ));

  //imiPhyX and imiPhyY
  std::stringstream sstmPhyX;
  adhocStruct ahcPhyX;
  sstmPhyX << varName << "PhyX";
  ahcPhyX.name = sstmPhyX.str();
  ahcPhyX.type = 2;
  ahcPhyX.info[0] = idx; // Input image #ID
  m_VAllowedVarName.push_back(ahcPhyX);

  std::stringstream sstmPhyY;
  adhocStruct ahcPhyY;
  sstmPhyY << varName << "PhyY";
  ahcPhyY.name = sstmPhyY.str();
  ahcPhyY.type = 3;
  ahcPhyY.info[0] = idx; // Input image #ID
  m_VAllowedVarName.push_back(ahcPhyY);

  //imi
  std::stringstream sstm;
  adhocStruct ahc;
  sstm << varName;
  ahc.name = sstm.str();
  ahc.type = 4;
  ahc.info[0] = idx; // Input image #ID
  m_VAllowedVarName.push_back(ahc);

  //imibj
  for (int j=0; j<image->GetNumberOfComponentsPerPixel(); j++)
  {
    std::stringstream sstm;
    adhocStruct ahc;
    sstm << varName << "b" << (j+1);
    ahc.name = sstm.str();
    ahc.type = 5;
    ahc.info[0] = idx; // Input image #ID
    ahc.info[1] = j; // Band #ID
    m_VAllowedVarName.push_back(ahc);
  }

  //imibjNkxp
  int size=0;
  for (int j=0; j<image->GetNumberOfComponentsPerPixel(); j++)
    for(int k=1; k<=size; k++)
      for(int p=1; p<=size; p++)
      {
        std::stringstream sstm;
        adhocStruct ahc;
        sstm << varName << "b" << (j+1) << "N" << k << "x" << p;
        ahc.name = sstm.str();
        ahc.type = 6;
        ahc.info[0] = idx; // Input image #ID
        ahc.info[1] = j; // Band #ID
        ahc.info[2] = k;
        ahc.info[3] = p;
        m_VAllowedVarName.push_back(ahc);
      }

}

template <typename TImage>
TImage * BandMathImageFilterX<TImage>
::GetNthInput(unsigned int idx)
{
  return const_cast<TImage *>(this->GetInput(idx));
}

template< typename TImage >
void BandMathImageFilterX<TImage>
::SetExpression(const std::string& expression) 
{
  m_Expression.push_back(expression); 

  if (m_Expression.size()>1)
    this->SetNthOutput( (int) (m_Expression.size()) -1, ( TImage::New() ).GetPointer() );

  this->Modified();
}

template< typename TImage >
std::string BandMathImageFilterX<TImage>
::GetExpression(int IDExpression) const
{
  return m_Expression.at(IDExpression); 
}

template< typename TImage >
std::vector<std::string>& BandMathImageFilterX<TImage>
::GetVarNames() const
{
  std::vector<std::string> res;
  for(int y=0; y<m_VVarName.size(); y++)
    res.push_back(m_VVarName[y].name);

  return res;

}


template< typename TImage >
void BandMathImageFilterX<TImage>
::AddVariable(adhocStruct &ahc)
{
    bool found=false;
    for(int i=0; i<m_VVarName.size(); ++i)
      if (m_VVarName[i].name == ahc.name)
        found=true;

    if (!found)
      m_VVarName.push_back(ahc);
}


template< typename TImage >
void BandMathImageFilterX<TImage>
::PrepareParsers()
{

  // Generate variables names
  m_VVarName.clear();
  m_VNotAllowedVarName.clear();

  this->SetNumberOfRequiredOutputs((int) m_Expression.size());

  for(int IDExpression=0; IDExpression<m_Expression.size(); ++IDExpression)
  {
      ParserType::Pointer dummyParser = ParserType::New();
      dummyParser->SetExpr(this->GetExpression(IDExpression));

      mup::var_maptype vmap = dummyParser->GetExprVar();
      for (mup::var_maptype::iterator item = vmap.begin(); item!=vmap.end(); ++item)
      {
        bool OK=false; int i=0;
        while( ( !OK ) && (i<m_VAllowedVarName.size()) )
        {
          if (item->first == m_VAllowedVarName[i].name)
            OK=true;
          else
            i++;
        }
        
        if (OK) {AddVariable(m_VAllowedVarName[i]);} 
        else {
                adhocStruct ahc;
                ahc.name = item->first;  
                m_VNotAllowedVarName.push_back(ahc);
              }
      }

  }


  if (m_VNotAllowedVarName.size()>0)
  {
    std::stringstream sstm;
    sstm << "Following variables not allowed : ";
    for(int i=0; i<m_VNotAllowedVarName.size(); ++i)
      sstm << m_VNotAllowedVarName[i].name << " ";
    sstm << std::endl;
    itkExceptionMacro(<< sstm.str());
  }

  
  // Register variables for each parser (important : one parser per thread)
  m_VParser.clear();
  unsigned int nbThreads = this->GetNumberOfThreads();
  typename std::vector<ParserType::Pointer>::iterator        itParser;
  m_VParser.resize(nbThreads);
  for(itParser = m_VParser.begin(); itParser < m_VParser.end(); itParser++)
    {
      *itParser = ParserType::New();
    }

  m_NbVar = m_VVarName.size();

  m_AImage.resize(nbThreads);

  double initValue = 1.0;
  for(int i = 0; i < nbThreads; ++i)
  {
    m_AImage[i].resize(m_NbVar);

    for(int j=0; j < m_NbVar; ++j)
      {
        m_AImage[i][j].name = m_VVarName[j].name;
        m_AImage[i][j].type  = m_VVarName[j].type;
        for (int t=0;t<5;++t) 
          m_AImage[i][j].info[t]=m_VVarName[j].info[t];

       
        if ( (m_AImage[i][j].type == 0 ) || (m_AImage[i][j].type == 1) ) // indices (idxX & idxY)
        {
            m_AImage[i][j].value = ValueType(initValue);
        }

        if (m_AImage[i][j].type == 2) //imiPhyX 
        {
          SpacingType spacing = this->GetNthInput(m_AImage[i][j].info[0])->GetSpacing();
          m_AImage[i][j].value = ValueType(static_cast<double>(spacing[0]));
        }

        if (m_AImage[i][j].type == 3) //imiPhyY
        {
          SpacingType spacing = this->GetNthInput(m_AImage[i][j].info[0])->GetSpacing();
          m_AImage[i][j].value = ValueType(static_cast<double>(spacing[1]));
        }

        if (m_AImage[i][j].type == 4 ) // vector
        {
            unsigned int nbBands = this->GetNthInput(m_AImage[i][j].info[0])->GetNumberOfComponentsPerPixel();
            m_AImage[i][j].value = ValueType(1,nbBands,initValue);
        }

        if (m_AImage[i][j].type == 5 ) // pixel
        {
            m_AImage[i][j].value = ValueType(initValue);
        }

        if (m_AImage[i][j].type == 6 ) // neighborhood
        {
            //TODO
        }

        m_VParser.at(i)->DefineVar(m_AImage[i][j].name, &(m_AImage[i][j].value)); 

        initValue += 0.01;
        if (initValue>10.0)
          initValue=1.0;
      }
  }

}


template< typename TImage >
void BandMathImageFilterX< TImage >
::OutputsDimensions()
{
  m_outputsDimensions.clear();

  for(int i=0; i<m_Expression.size(); ++i)
  {
    m_VParser.at(0)->SetExpr(m_Expression[i]);
    ValueType value = m_VParser.at(0)->Eval();

    switch (value.GetType())
    {   //ValueType
        case 'i':
        m_outputsDimensions.push_back(1);
        break;

        case 'f':
        m_outputsDimensions.push_back(1);
        break;

        case 'c':
        itkExceptionMacro(<< "Complex numbers not supported." << std::endl);
        break;

        case 'm':
        mup::matrix_type vect = value.GetArray();
        m_outputsDimensions.push_back(vect.GetCols());
        break;

    }
  }

}



template< typename TImage >
void BandMathImageFilterX< TImage >
::AllocateOutputs()
{
  typedef itk::ImageBase< TImage::ImageDimension > ImageBaseType;
  typename ImageBaseType::Pointer outputPtr;

  PrepareParsers(); // addition
  OutputsDimensions(); // addition

  // Allocate the output memory
  int i=0;
  for ( itk::OutputDataObjectIterator it(this); !it.IsAtEnd(); it++ )
    {
      // Check whether the output is an image of the appropriate
      // dimension (use ProcessObject's version of the GetInput()
      // method since it returns the input as a pointer to a
      // DataObject as opposed to the subclass version which
      // static_casts the input to an TInputImage).
      outputPtr = dynamic_cast< ImageBaseType * >( it.GetOutput() );

        if ( outputPtr )
        {
          outputPtr->SetBufferedRegion( outputPtr->GetRequestedRegion() );
          outputPtr->SetNumberOfComponentsPerPixel(m_outputsDimensions[i]);
          outputPtr->Allocate();
        }

      i++;
    }
}

template< typename TImage >
void BandMathImageFilterX<TImage>
::BeforeThreadedGenerateData()
{
  // Some useful variable
  unsigned int nbThreads = this->GetNumberOfThreads();
  unsigned int nbInputImages = this->GetNumberOfInputs();

  // Check if input image dimensions matches
  unsigned int inputSize[2];
  inputSize[0] = this->GetNthInput(0)->GetLargestPossibleRegion().GetSize(0);
  inputSize[1] = this->GetNthInput(0)->GetLargestPossibleRegion().GetSize(1);

  for(unsigned int p = 1; p < nbInputImages; p++)
    {
    if((inputSize[0] != this->GetNthInput(p)->GetLargestPossibleRegion().GetSize(0))
       || (inputSize[1] != this->GetNthInput(p)->GetLargestPossibleRegion().GetSize(1)))
      {
      itkExceptionMacro(<< "Input images must have the same dimensions." << std::endl
                        << "band #1 is [" << inputSize[0] << ";" << inputSize[1] << "]" << std::endl
                        << "band #" << p+1 << " is ["
                        << this->GetNthInput(p)->GetLargestPossibleRegion().GetSize(0) << ";"
                        << this->GetNthInput(p)->GetLargestPossibleRegion().GetSize(1) << "]");
      }
    }

  // Allocate and initialize the thread temporaries
  m_ThreadUnderflow.SetSize(nbThreads);
  m_ThreadUnderflow.Fill(0);
  m_ThreadOverflow.SetSize(nbThreads);
  m_ThreadOverflow.Fill(0);

}


template< typename TImage >
void BandMathImageFilterX<TImage>
::AfterThreadedGenerateData()
{
  unsigned int nbThreads = this->GetNumberOfThreads();
  unsigned int i;

  m_UnderflowCount = 0;
  m_OverflowCount = 0;

  // Accumulate counts for each thread
  for(i = 0; i < nbThreads; ++i)
    {
    m_UnderflowCount += m_ThreadUnderflow[i];
    m_OverflowCount += m_ThreadOverflow[i];
    }

  if((m_UnderflowCount != 0) || (m_OverflowCount!=0)) //TODO
  {
    std::stringstream sstm;
    sstm << std::endl
        << "The Following Parsed Expression  :  ";
    for(int t=0; t<m_Expression.size(); ++t)
        sstm << this->GetExpression(t) << std::endl;
    sstm << "Generated " << m_UnderflowCount << " Underflow(s) "
        << "And " << m_OverflowCount        << " Overflow(s) "   << std::endl
        << "The Parsed Expression, The Inputs And The Output "
        << "Type May Be Incompatible !";

    otbWarningMacro(<< sstm.str());
  }
}

template< typename TImage >
void BandMathImageFilterX<TImage>
::ThreadedGenerateData(const ImageRegionType& outputRegionForThread,
           itk::ThreadIdType threadId)
{

  ValueType value;
  unsigned int nbInputImages = this->GetNumberOfInputs();

  // Iterators
  typedef itk::ImageRegionConstIterator<TImage> ImageRegionConstIteratorType;
  std::vector< ImageRegionConstIteratorType > Vit;
  Vit.resize(nbInputImages);
  for(int j=0; j < nbInputImages; ++j)
    Vit[j] = ImageRegionConstIteratorType (this->GetNthInput(j), outputRegionForThread);
    

  std::vector< ImageRegionConstIteratorType > VoutIt;
  VoutIt.resize(m_Expression.size());
  for(int j=0; j < VoutIt.size(); ++j)
    VoutIt[j] = ImageRegionConstIteratorType (this->GetOutput(j), outputRegionForThread);
    

  // Support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Pixel affectation 
  for(int j=0; j < nbInputImages; ++j) {Vit[j].GoToBegin();}
  for(int j=0; j < m_Expression.size(); ++j) {VoutIt[j].GoToBegin();}
  //outIt.GoToBegin();

  while(!Vit.at(0).IsAtEnd()) // For each pixel
  {

    for(int j=0; j < m_AImage[threadId].size(); ++j) // For each variable, perform a copy
    {

       switch (m_AImage[threadId][j].type) 
        {

          case 0 : //idxX
            m_AImage[threadId][j].value = static_cast<double>(Vit[0].GetIndex()[0]);
          break;

          case 1 : //idxY
            m_AImage[threadId][j].value = static_cast<double>(Vit[0].GetIndex()[1]);
          break;

          case 2 : //Spacing X (imiPhyX)
            //Nothing to do (already set inside BeforeThreadedGenerateData)"
          break;

          case 3 : //Spacing Y (imiPhyY)
            //Nothing to do (already set inside BeforeThreadedGenerateData)"
          break;

          case 4 : //vector       
            // m_AImage[threadId][j].info[0] : Input image #ID
            for(int p=0; p < m_AImage[threadId][j].value.GetCols(); ++p)
              m_AImage[threadId][j].value.At(0,p) = Vit[m_AImage[threadId][j].info[0]].Get()[p];
          break;

          case 5 : //pixel
            // m_AImage[threadId][j].info[0] : Input image #ID
            // m_AImage[threadId][j].info[1] : Band #ID
            m_AImage[threadId][j].value = Vit[m_AImage[threadId][j].info[0]].Get()[m_AImage[threadId][j].info[1]];
          break;

          case 6 : //neighborhood
            // TODO
          break;

          default :
            itkExceptionMacro(<< "Type of the variable is unknown");
          break;
        }
    }

  for(int IDExpression=0; IDExpression<m_Expression.size(); ++IDExpression)
  {

        m_VParser[threadId]->SetExpr(m_Expression[IDExpression]);

        try
        {
          value = m_VParser[threadId]->Eval();
        }
        catch(itk::ExceptionObject& err)
        {
          itkExceptionMacro(<< err);
        }

        switch (value.GetType())
        {   //ValueType
            case 'i':
            VoutIt[IDExpression].Get()[0] = value.GetInteger();
            break;

            case 'f':
            VoutIt[IDExpression].Get()[0] = value.GetFloat();
            break;

            case 'c':
            itkExceptionMacro(<< "Complex numbers not supported." << std::endl);
            break;

            case 'm':
            mup::matrix_type vect = value.GetArray();
            for(int p=0; p<vect.GetCols(); ++p)
              VoutIt[IDExpression].Get()[p] = vect.At(0,p); 
            break;

        }


        for(int p=0; p<VoutIt[IDExpression].Get().GetSize(); ++p)
        {
            // Case value is equal to -inf or inferior to the minimum value
            // allowed by the pixelType cast
            if (VoutIt[IDExpression].Get()[p] < double(itk::NumericTraits<typename PixelType::ValueType>::NonpositiveMin()))
            {
                VoutIt[IDExpression].Get()[p] = itk::NumericTraits<typename PixelType::ValueType>::NonpositiveMin();
                m_ThreadUnderflow[threadId]++;
            }
            // Case value is equal to inf or superior to the maximum value
            // allowed by the pixelType cast
            else if (VoutIt[IDExpression].Get()[p] > double(itk::NumericTraits<typename PixelType::ValueType>::max()))
            {
               VoutIt[IDExpression].Get()[p] = itk::NumericTraits<typename PixelType::ValueType>::max();
               m_ThreadOverflow[threadId]++;
            }  
        }

    }

    for(int j=0; j < nbInputImages; ++j) {++Vit[j];}
    for(int j=0; j < m_Expression.size(); ++j) {++VoutIt[j];}
    
    progress.CompletedPixel();
  }

}

}// end namespace otb

#endif
