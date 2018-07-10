/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbBandMathXImageFilter_hxx
#define otbBandMathXImageFilter_hxx
#include "otbBandMathXImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageScanlineConstIterator.h"
#include "itkImageScanlineIterator.h"
#include "itkImageRegionConstIteratorWithOnlyIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

#include <iostream>
#include <fstream>
#include <string>

namespace otb
{

/** Constructor */
template <class TImage>
BandMathXImageFilter<TImage>
::BandMathXImageFilter()
{
  //This number will be incremented each time an image
  //is added over the one minimumrequired
  this->SetNumberOfRequiredInputs( 1 );

  m_UnderflowCount = 0;
  m_OverflowCount = 0;
  m_ThreadUnderflow.SetSize(1);
  m_ThreadOverflow.SetSize(1);


  //idxX and idxY
  adhocStruct ahcX;
  ahcX.name = "idxX";
  ahcX.type = 0;
  m_VAllowedVarNameAuto.push_back(ahcX);

  adhocStruct ahcY;
  ahcY.name = "idxY";
  ahcY.type = 1;
  m_VAllowedVarNameAuto.push_back(ahcY);

  m_SizeNeighbourhood=10;
  
  m_ManyExpressions = true;

}

/** Destructor */
template <class TImage>
BandMathXImageFilter<TImage>
::~BandMathXImageFilter()
{
  m_Expression.clear();
  m_VParser.clear();

  for(unsigned int i=0; i<m_AImage.size(); ++i)
    m_AImage[i].clear();
  m_AImage.clear();

  m_VVarName.clear();
  m_VAllowedVarNameAuto.clear();
  m_VAllowedVarNameAddedByUser.clear();
  m_VFinalAllowedVarName.clear();
  m_VNotAllowedVarName.clear();
  m_outputsDimensions.clear();


}


template <class TImage>
void BandMathXImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Expressions: " << std::endl;
  for (unsigned int i=0; i<m_Expression.size(); i++)
    os << indent << m_Expression[i] << std::endl;
  os << indent << "Computed values follow:"                            << std::endl;
  os << indent << "UnderflowCount: "  << m_UnderflowCount              << std::endl;
  os << indent << "OverflowCount: "   << m_OverflowCount               << std::endl;
  os << indent << "itk::NumericTraits<typename PixelValueType>::NonpositiveMin()  :  "
               << itk::NumericTraits<PixelValueType>::NonpositiveMin()      << std::endl;
  os << indent << "itk::NumericTraits<typename PixelValueType>::max()  :             "
         << itk::NumericTraits<PixelValueType>::max()                 << std::endl;
}

template <class TImage>
void BandMathXImageFilter<TImage>
::SetNthInput(DataObjectPointerArraySizeType idx, const ImageType * image)
{

  std::stringstream sstm;
  sstm << "im" << (idx+1);
  this->SetNthInput(idx, image, sstm.str());
}

template <class TImage>
void BandMathXImageFilter<TImage>
::SetNthInput(DataObjectPointerArraySizeType idx, const ImageType * image, const std::string& varName)
{

  ImageType * imagebis = const_cast<ImageType *>( image ); // Useful for call of UpdateOutputInformation() (see below)
  this->SetInput(idx, imagebis );

  //imiPhyX and imiPhyY
  std::stringstream sstmPhyX;
  adhocStruct ahcPhyX;
  sstmPhyX << varName << "PhyX";
  ahcPhyX.name = sstmPhyX.str();
  ahcPhyX.type = 2;
  ahcPhyX.info[0] = idx; // Input image #ID
  m_VAllowedVarNameAuto.push_back(ahcPhyX);

  std::stringstream sstmPhyY;
  adhocStruct ahcPhyY;
  sstmPhyY << varName << "PhyY";
  ahcPhyY.name = sstmPhyY.str();
  ahcPhyY.type = 3;
  ahcPhyY.info[0] = idx; // Input image #ID
  m_VAllowedVarNameAuto.push_back(ahcPhyY);

  //imi
  std::stringstream sstm_glob;
  adhocStruct ahc_glob;
  sstm_glob << varName;
  ahc_glob.name = sstm_glob.str();
  ahc_glob.type = 4;
  ahc_glob.info[0] = idx; // Input image #ID
  m_VAllowedVarNameAuto.push_back(ahc_glob);

  //Mandatory before call of GetNumberOfComponentsPerPixel
  //Really important not to call the filter's UpdateOutputInformation method here:
  //this method is not ready until all inputs, variables and expressions are set.
  imagebis->UpdateOutputInformation();

  //imibj
  for (unsigned int j=0; j<imagebis->GetNumberOfComponentsPerPixel(); j++)
  {
    std::stringstream sstm;
    adhocStruct ahc;
    sstm << varName << "b" << (j+1);
    ahc.name = sstm.str();
    ahc.type = 5;
    ahc.info[0] = idx; // Input image #ID
    ahc.info[1] = j; // Band #ID
    m_VAllowedVarNameAuto.push_back(ahc);
  }

  //imibjNkxp
  for (unsigned int j=0; j<imagebis->GetNumberOfComponentsPerPixel(); j++)
    for(unsigned int x=0; x<=m_SizeNeighbourhood; x++)
      for(unsigned int y=0; y<=m_SizeNeighbourhood; y++)
      {
        std::stringstream sstm;
        adhocStruct ahc;
        sstm << varName << "b" << (j+1) << "N" << 2*x+1 << "x" << 2*y+1;
        ahc.name = sstm.str();
        ahc.type = 6;
        ahc.info[0] = idx; // Input image #ID
        ahc.info[1] = j;   // Band #ID
        ahc.info[2] = 2*x+1;  // Size x direction (matrix convention = cols)
        ahc.info[3] = 2*y+1;  // Size y direction (matrix convention = rows)
        m_VAllowedVarNameAuto.push_back(ahc);
      }

  //imibjSTATS
  std::vector< std::string > statsNames;
  statsNames.push_back("Mini");
  statsNames.push_back("Maxi");
  statsNames.push_back("Mean");
  statsNames.push_back("Sum");
  statsNames.push_back("Var");

  for (unsigned int j=0; j<imagebis->GetNumberOfComponentsPerPixel(); j++)
    for (unsigned int t=0; t<statsNames.size(); t++)
    {
      std::stringstream sstm;
      adhocStruct ahc;
      sstm << varName << "b" << (j+1) << statsNames[t];
      ahc.name = sstm.str();
      ahc.type = 8;
      ahc.info[0] = idx; // Input image #ID
      ahc.info[1] = j;   // Band #ID
      ahc.info[2] = t;   // Sub-type : 0 Mini, 1 Maxi, 2 Mean ...
      m_VAllowedVarNameAuto.push_back(ahc);
    }

}

template <typename TImage>
TImage * BandMathXImageFilter<TImage>
::GetNthInput(DataObjectPointerArraySizeType idx)
{
  return const_cast<TImage *>(this->GetInput(idx));
}

template< typename TImage >
void BandMathXImageFilter<TImage>
::SetManyExpressions(bool flag)
{
    m_ManyExpressions = flag;
}

template< typename TImage >
void BandMathXImageFilter<TImage>
::SetExpression(const std::string& expression)
{
  std::string expressionToBePushed = expression;

  if (expression.find(";") != std::string::npos)
  {
    std::ostringstream oss;
    oss << "cat(";
    for(unsigned int i=0; i < expression.size(); ++i)
      if (expression[i] == ';')
        oss << ",";
      else
        oss << expression[i];

    oss << ")";
    expressionToBePushed = oss.str();
  }

  if (m_ManyExpressions)
    m_Expression.push_back(expressionToBePushed);
  else if (m_Expression.size() == 0)
    m_Expression.push_back(expressionToBePushed);

  if (m_Expression.size()>1)
    this->SetNthOutput( (DataObjectPointerArraySizeType) (m_Expression.size()) -1, ( TImage::New() ).GetPointer() );

  this->Modified();
}


template< typename TImage >
void BandMathXImageFilter<TImage>
::SetMatrix(const std::string& name, const std::string& definition)
{

  for(unsigned int i=0; i<m_VAllowedVarNameAddedByUser.size(); i++)
    if (name.compare(m_VAllowedVarNameAddedByUser[i].name) == 0)
      itkExceptionMacro(<< "Variable name '"<< name << "' already used." << std::endl);


  if ( (definition.find("{") != 0) || (definition.find("}")) != definition.size()-1 )
    itkExceptionMacro(<< "Definition of a matrix must begin with { and end with } characters." << std::endl);

  //Get rid of { and } characters
  std::string def;
  for(unsigned int i=1; i<definition.size()-1; ++i)
    def.push_back(definition[i]);


  std::vector< std::vector<double> > mat;
  std::istringstream iss( def );
  std::string rows;
  while (std::getline( iss, rows, ';' ) )
  {
      mat.push_back(std::vector<double>(0));
      std::istringstream iss2( rows );
      std::string elmt;
        while (std::getline( iss2, elmt, ',' ) )
        {
            std::istringstream iss3( elmt );
            double val;
            iss3 >> val;
            mat.back().push_back(val);
        }
  }
  

  //Check dimensions of the matrix
  for (unsigned int i=0; i<mat.size()-1; i++)
    if (mat[i].size() != mat[i+1].size())
      itkExceptionMacro(<< "Each row must have the same number of cols : " << definition << std::endl);
  

  //Registration
  adhocStruct ahc;
  ahc.name = name;
  ahc.type = 7;
  ahc.info[0] = mat[0].size();  // Size x direction (matrix convention = cols)
  ahc.info[1] = mat.size();     // Size y direction (matrix convention = rows)

  ahc.value = ValueType(ahc.info[1],ahc.info[0],0.0);
  for(unsigned int i=0; i<mat.size(); i++)
    for(unsigned int j=0; j<mat[i].size(); j++)
      ahc.value.At(i,j)=mat[i][j];
 
  m_VAllowedVarNameAddedByUser.push_back(ahc);

}


template< typename TImage >
void BandMathXImageFilter<TImage>
::SetConstant(const std::string& name, double value)
{
  for(unsigned int i=0; i<m_VAllowedVarNameAddedByUser.size(); i++)
    if (name.compare(m_VAllowedVarNameAddedByUser[i].name) == 0)
      itkExceptionMacro(<< "Variable name '"<< name << "' already used." << std::endl);

  adhocStruct ahc;
  ahc.name = name;
  ahc.type = 7;
  ahc.value = value;
 
  m_VAllowedVarNameAddedByUser.push_back(ahc);

}


template< typename TImage >
void BandMathXImageFilter<TImage>
::ExportContext(const std::string& filename)
{

  std::vector< std::string > vectI,vectF,vectM, vectFinal;

  for(unsigned int i=0; i<m_VAllowedVarNameAddedByUser.size(); i++)
           {
        std::ostringstream iss;
        std::string        str;

        switch (m_VAllowedVarNameAddedByUser[i].value.GetType())
        {
          case 'i':
            iss << "#I " << m_VAllowedVarNameAddedByUser[i].name << " " << m_VAllowedVarNameAddedByUser[i].value.GetInteger();
            str=iss.str();
            vectI.push_back(str);
          break;
          case 'f':
            iss << "#F " << m_VAllowedVarNameAddedByUser[i].name << " " << m_VAllowedVarNameAddedByUser[i].value.GetFloat();
            str=iss.str();
            vectF.push_back(str);
          break;
          case 'c':
            itkExceptionMacro(<< "Complex numbers not supported." << std::endl);
          break;
          case 'm':
            iss << "#M " << m_VAllowedVarNameAddedByUser[i].name << " " << "{";
            for(int k=0; k<m_VAllowedVarNameAddedByUser[i].value.GetRows(); k++)
            {
              iss << " " << m_VAllowedVarNameAddedByUser[i].value.At(k,0);
              for(int p=1; p<m_VAllowedVarNameAddedByUser[i].value.GetCols(); p++)
                iss << " , " <<  m_VAllowedVarNameAddedByUser[i].value.At(k,p);
              iss << ";";
            }
            str=iss.str();
            str.erase(str.size()-1);
            str.push_back('}');
            vectM.push_back(str);
          break;
        }

    }

  // Sorting : I F M and E
  for(unsigned int i=0; i<vectI.size(); ++i)
    vectFinal.push_back(vectI[i]);
  for(unsigned int i=0; i<vectF.size(); ++i)
    vectFinal.push_back(vectF[i]);
  for(unsigned int i=0; i<vectM.size(); ++i)
    vectFinal.push_back(vectM[i]);
  for(unsigned int i=0; i < m_Expression.size(); ++i)
    {
      std::ostringstream iss;
      iss << "#E " << m_Expression[i] << std::endl;
      std::string str=iss.str();
      vectFinal.push_back(str);
    }

  std::ofstream exportFile(filename, std::ios::out | std::ios::trunc);
  if(exportFile)
    {
      for(unsigned int i=0; i<vectFinal.size(); ++i)
        exportFile << vectFinal[i] << std::endl;

      exportFile.close();
    }
    else
      itkExceptionMacro(<< "Could not open " << filename << "." << std::endl);
}

template< typename TImage >
void BandMathXImageFilter<TImage>
::ImportContext(const std::string& filename)
{
  std::ifstream importFile(filename, std::ios::in);

  std::string wholeline,line,name,matrixdef;
  int pos,pos2,lineID=0,nbSuccesses=0;
  double value;

  if(importFile)
    {
 
      while(std::getline(importFile,wholeline))
      {
        lineID++;

        pos = wholeline.find_first_not_of(' ');
        
        if (pos != (int) std::string::npos)
        {
          line = wholeline.substr(pos);

          if ( (line[0] == '#') && ((line[1] == 'I') || (line[1] == 'i') || (line[1] == 'F') || (line[1] == 'f')) )
            {
              
              pos = line.find_first_not_of(' ',2);

              if (pos == (int) std::string::npos)
                itkExceptionMacro(<< "In file '"<< filename << "', line " << lineID << " : please, set the name and the value of the constant." << std::endl);

              std::string sub = line.substr(pos);

              pos = sub.find_first_of(' ');
              name = sub.substr(0,pos);
          
              if (sub.find_first_of('{',pos) != std::string::npos)
                itkExceptionMacro(<< "In file '"<< filename << "', line " << lineID
                << " : symbol #F found, but find vector/matrix definition. Please, set an integer or a float number." << std::endl);

              if (sub.find_first_not_of(' ',pos) == std::string::npos )
                itkExceptionMacro(<< "In file '"<< filename << "', line " << lineID << " : please, set the value of the constant." << std::endl)

              std::istringstream iss( sub.substr(pos) );
              iss >> value;
                
              SetConstant(name,value);
              nbSuccesses++;

            }
          else if ( (line[0] == '#') && ((line[1] == 'M') || (line[1] == 'm')) )
            {

              pos = line.find_first_not_of(' ',2);

              if (pos == (int) std::string::npos)
                itkExceptionMacro(<< "In file '"<< filename << "', line " << lineID << " : please, set the name and the definition of the vector/matrix." << std::endl);

              std::string sub = line.substr(pos);

              pos = sub.find_first_of(' ');
              name = sub.substr(0,pos);
              pos2 = sub.find_first_of('{');
              if (pos2 != (int) std::string::npos)
                matrixdef = sub.substr(pos2);
              else
                itkExceptionMacro(<< "In file '"<< filename << "', line " << lineID << " : symbol #M found, but couldn't not find vector/matrix definition." << std::endl);

              SetMatrix(name,matrixdef);
              nbSuccesses++;

            }
          else if ( (line[0] == '#') && ((line[1] == 'E') || (line[1] == 'e')) )
            {
              pos = line.find_first_not_of(' ',2);

              if (pos == (int) std::string::npos)
                itkExceptionMacro(<< "In file '"<< filename << "', line " << lineID << " : symbol #E found, but couldn't not find any expression." << std::endl);

              std::string sub = line.substr(pos);

              SetExpression(sub);
              nbSuccesses++;

            }
 
        }
      }//while

      importFile.close();


    if (nbSuccesses == 0)
      itkExceptionMacro(<< "No constant or expression could be set; please, ensure that the file '" << filename << "' is correct." << std::endl);

    }
    else
      itkExceptionMacro(<< "Could not open " << filename << "." << std::endl);


}


template< typename TImage >
std::string BandMathXImageFilter<TImage>
::GetExpression(int IDExpression) const
{
  return m_Expression[IDExpression];
}


template< typename TImage >
std::vector<std::string> BandMathXImageFilter<TImage>
::GetVarNames() const
{
  std::vector<std::string> res;
  for(int y=0; y<m_VVarName.size(); y++)
    res.push_back(m_VVarName[y].name);

  return res;
}


template< typename TImage >
void BandMathXImageFilter<TImage>
::AddVariable(adhocStruct &ahc)
{
    bool found=false;
    for(unsigned int i=0; i<m_VVarName.size(); ++i)
      if (m_VVarName[i].name == ahc.name)
        found=true;

    if (!found)
      m_VVarName.push_back(ahc);
    
}


template< typename TImage >
void BandMathXImageFilter<TImage>
::PrepareParsers()
{

  if (m_Expression.size() == 0)
    itkExceptionMacro(<< "No expression set; please set at least one expression." << std::endl);


  // Generate variables names
  m_VVarName.clear();
  m_VNotAllowedVarName.clear();
  m_VFinalAllowedVarName.clear();

  // m_VFinalAllowedVarName = m_VAllowedVarNameAuto + m_VAllowedVarNameAddedByUser
  // m_VFinalAllowedVarName = variable names dictionary
  for(unsigned int i=0; i<m_VAllowedVarNameAddedByUser.size(); i++)
    m_VFinalAllowedVarName.push_back(m_VAllowedVarNameAddedByUser[i]);
  for(unsigned int i=0; i<m_VAllowedVarNameAuto.size(); i++)
    m_VFinalAllowedVarName.push_back(m_VAllowedVarNameAuto[i]);

  unsigned int nbExpr = m_Expression.size();
  for(unsigned int IDExpression=0; IDExpression < nbExpr; ++IDExpression) // For each expression
  {
      ParserType::Pointer dummyParser = ParserType::New();
      dummyParser->SetExpr(this->GetExpression(IDExpression));

      mup::var_maptype vmap = dummyParser->GetExprVar();
      for (mup::var_maptype::iterator item = vmap.begin(); item!=vmap.end(); ++item)
      {
        bool OK=false; int i=0;
        while( ( !OK ) && (i < (int) m_VFinalAllowedVarName.size()) )
        {
          if (item->first == m_VFinalAllowedVarName[i].name)
            OK=true;
          else
            i++;
        }
        
        if (OK) {AddVariable(m_VFinalAllowedVarName[i]); }
        else {
                adhocStruct ahc;
                ahc.name = item->first;
                m_VNotAllowedVarName.push_back(ahc);
              }
      }

  }// At this step, m_VVarName has been built


  //Checking formulas consistency
  if (m_VNotAllowedVarName.size()>0)
  {
    std::stringstream sstm;
    sstm << "Following variables not allowed : ";
    for(unsigned int i=0; i<m_VNotAllowedVarName.size(); ++i)
      sstm << m_VNotAllowedVarName[i].name << " ";
    sstm << std::endl;
    itkExceptionMacro(<< sstm.str());
  }


  // Register variables for each parser (important : one parser per thread and per expression)
  m_VParser.clear();
  unsigned int nbThreads = this->GetNumberOfThreads();
  for (unsigned int k=0 ; k<nbThreads ; k++)
    {
    std::vector<ParserType::Pointer> parserList;
    for (unsigned int i=0 ; i<nbExpr ; i++)
      {
      parserList.push_back(ParserType::New());
      }
    m_VParser.push_back(parserList);
    }

  // Important to remember that variables of m_VVarName come from a call of GetExprVar method
  // Only useful variables are allocated in this filter
  int nbVar = m_VVarName.size();

  m_StatsVarDetected.clear();
  m_NeighDetected.clear();
  m_NeighExtremaSizes.clear();
  unsigned int nbInputImages = this->GetNumberOfInputs();
  RadiusType dummyRadius; dummyRadius[0]=1; dummyRadius[1]=1;
  m_NeighExtremaSizes.resize(nbInputImages,dummyRadius);

  //Reset
  for(unsigned int i=0; i<m_AImage.size(); ++i)
    m_AImage[i].clear();
  m_AImage.clear();

  m_AImage.resize(nbThreads);

  double initValue = 0.1;
  for(unsigned int i = 0; i < nbThreads; ++i) // For each thread
  {
 
    m_AImage[i].resize(nbVar); // For each variable

    for(int j=0; j < nbVar; ++j)
      {
        m_AImage[i][j].name = m_VVarName[j].name;
        m_AImage[i][j].type  = m_VVarName[j].type;
        for (int t=0; t<5; ++t)
          m_AImage[i][j].info[t]=m_VVarName[j].info[t];

       

        if ( (m_AImage[i][j].type == 0 ) || (m_AImage[i][j].type == 1) ) // indices (idxX & idxY)
        {
            m_AImage[i][j].value = ValueType(initValue);
        }

        if (m_AImage[i][j].type == 2) //imiPhyX
        {
          SpacingType spacing = this->GetNthInput(m_AImage[i][j].info[0])->GetSignedSpacing();
          m_AImage[i][j].value = ValueType(static_cast<double>(spacing[0]));
        }

        if (m_AImage[i][j].type == 3) //imiPhyY
        {
          SpacingType spacing = this->GetNthInput(m_AImage[i][j].info[0])->GetSignedSpacing();
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
            m_AImage[i][j].value = ValueType(m_AImage[i][j].info[3],m_AImage[i][j].info[2],initValue);

            //m_AImage[i][j].info[0] = Image ID
            bool found = false;
            for (unsigned int r=0; r<m_NeighDetected.size() && !found; r++)
                if (m_NeighDetected[r] == (unsigned int) m_AImage[i][j].info[0])
                  found = true;
            if (!found)
              m_NeighDetected.push_back(m_AImage[i][j].info[0]);

            // find biggest radius for a given input image (idis given by info[0])
            if (m_NeighExtremaSizes[m_AImage[i][j].info[0]][0] < (unsigned int) ((m_VVarName[j].info[2]-1)/2) )  // Size x direction (otb convention)
              m_NeighExtremaSizes[m_AImage[i][j].info[0]][0] = (unsigned int) ((m_VVarName[j].info[2]-1)/2);

            if (m_NeighExtremaSizes[m_AImage[i][j].info[0]][1] < (unsigned int) ((m_VVarName[j].info[3]-1)/2) )  // Size y direction (otb convention)
              m_NeighExtremaSizes[m_AImage[i][j].info[0]][1] = (unsigned int) ((m_VVarName[j].info[3]-1)/2);

        }

        if (m_AImage[i][j].type == 7 ) // user defined variables
        {

          for(int t=0; t<(int) m_VAllowedVarNameAddedByUser.size(); t++)
            if (m_VAllowedVarNameAddedByUser[t].name.compare(m_AImage[i][j].name) == 0)
              m_AImage[i][j].value = m_VAllowedVarNameAddedByUser[t].value;

              
        }

        if (m_AImage[i][j].type == 8 ) // global stats
        {
            m_AImage[i][j].value = ValueType(initValue);
            //m_AImage[i][j].info[0] = Image ID
            bool found = false;
            for (unsigned int r=0; r<m_StatsVarDetected.size() && !found; r++)
                if (m_StatsVarDetected[r] == m_AImage[i][j].info[0])
                  found = true;
            if (!found)
              m_StatsVarDetected.push_back(m_AImage[i][j].info[0]);
        }


        //Register variable
        for (unsigned int k=0 ; k<nbExpr ; k++)
          {
          m_VParser[i][k]->DefineVar(m_AImage[i][j].name, &(m_AImage[i][j].value));
          }


        initValue += 0.001;
        if (initValue>1.0)
          initValue=0.1;
      }
  }

  // Set expressions
  for (unsigned int k=0 ; k<nbThreads ; k++)
    {
    for (unsigned int i=0 ; i<nbExpr ; i++)
      {
      m_VParser[k][i]->SetExpr(m_Expression[i]);
      }
    }

}


template< typename TImage >
void BandMathXImageFilter<TImage>
::PrepareParsersGlobStats()
{
  // Must instantiate stats variables of the parsers
  // Note : at this stage, inputs have already been set to largest possible regions.
    for (unsigned int i=0; i<m_StatsVarDetected.size(); i++)
    {

      StreamingStatisticsVectorImageFilterPointerType filter = StreamingStatisticsVectorImageFilterType::New();

      filter->SetInput( this->GetNthInput(m_StatsVarDetected[i]) );
      filter->Update();

      PixelType pix; //Variable length vector
      MatrixType mat;
      
      for(unsigned int t=0; t<m_AImage.size(); t++) // for each thread
        for(unsigned int v=0; v<m_AImage[t].size(); v++) // for each variable
          if ( (m_AImage[t][v].type == 8) && (m_AImage[t][v].info[0] == m_StatsVarDetected[i]) )// type 8 : flag identifying a glob stat; info[0] : input ID
          {
              switch ( m_AImage[t][v].info[2] ) // info[2] sub-type (see also SetNthInput method above)
              {
                  case 0: // mini

                    pix = filter->GetMinimum();
                    for (int b=0; b<(int) pix.GetSize(); b++) // for each band
                      if (m_AImage[t][v].info[1] == b) // info[1] : band ID
                        m_AImage[t][v].value = pix[b];

                  break;

                  case 1: // maxi

                    pix = filter->GetMaximum();
                    for (int b=0; b<(int) pix.GetSize(); b++) // for each band
                      if (m_AImage[t][v].info[1] == b) // info[1] : band ID
                        m_AImage[t][v].value = pix[b];

                  break;

                  case 2: // mean

                    pix = filter->GetMean();
                    for (int b=0; b<(int) pix.GetSize(); b++) // for each band
                      if (m_AImage[t][v].info[1] == b) // info[1] : band ID
                        m_AImage[t][v].value = pix[b];

                  break;

                  break;

                  case 3: // sum

                    pix = filter->GetSum();
                    for (int b=0; b<(int) pix.GetSize(); b++) // for each band
                      if (m_AImage[t][v].info[1] == b) // info[1] : band ID
                        m_AImage[t][v].value = pix[b];

                  break;

                  case 4: // stddev

                   mat = filter->GetCovariance();
                    for (int b=0; b<(int) mat.Cols(); b++) // for each band
                      if (m_AImage[t][v].info[1] == b) // info[1] : band ID
                        m_AImage[t][v].value = mat(b,b);

                  break;
              }
          }
    }
}

template< typename TImage >
void BandMathXImageFilter< TImage >
::OutputsDimensions()
{

  this->SetNumberOfRequiredOutputs((int) m_Expression.size());

  m_outputsDimensions.clear();

  for(int i=0; i<(int) m_Expression.size(); ++i)
  {
    ValueType value = m_VParser[0][i]->EvalRef();

    switch (value.GetType())
    {   //ValueType
        case 'b':
        itkExceptionMacro(<< "Booleans not supported." << std::endl);
        break;
        
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
        {
        const mup::matrix_type &vect = value.GetArray();
        if ( vect.GetRows() == 1 ) //Vector
          m_outputsDimensions.push_back(vect.GetCols());
        else //Matrix
          itkExceptionMacro(<< "Result of the evaluation can't be a matrix." << std::endl);
        }
        break;

        default:
          itkExceptionMacro(<< "Unknown output type : "<< value.GetType() << std::endl);
        break;
    }

    //std::cout << "Type = " << value.GetType() << " dimension = " << m_outputsDimensions.back() << std::endl;
  }

}

template< typename TImage >
void BandMathXImageFilter< TImage >
::CheckImageDimensions(void)
{
  // Check if input image dimensions match
  unsigned int nbInputImages = this->GetNumberOfInputs();
  unsigned int inputSize[2];
  inputSize[0] = this->GetNthInput(0)->GetLargestPossibleRegion().GetSize(0);
  inputSize[1] = this->GetNthInput(0)->GetLargestPossibleRegion().GetSize(1);

  for(unsigned int p = 1; p < nbInputImages; p++)
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

template< typename TImage >
void BandMathXImageFilter< TImage >
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();


  CheckImageDimensions();
  PrepareParsers();
  if (globalStatsDetected())
    PrepareParsersGlobStats();
  OutputsDimensions();


  typedef itk::ImageBase< TImage::ImageDimension > ImageBaseType;
  typename ImageBaseType::Pointer outputPtr;

  int i=0;
  for ( itk::OutputDataObjectIterator it(this); !it.IsAtEnd(); i++, it++ )
    {
      // Check whether the output is an image of the appropriate
      // dimension (use ProcessObject's version of the GetInput()
      // method since it returns the input as a pointer to a
      // DataObject as opposed to the subclass version which
      // static_casts the input to an TImage).
      outputPtr = dynamic_cast< ImageBaseType * >( it.GetOutput() );

        if ( outputPtr )
          outputPtr->SetNumberOfComponentsPerPixel(m_outputsDimensions[i]);
    }

}


template< typename TImage >
void BandMathXImageFilter< TImage >
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  for(unsigned int i=0; i<m_NeighDetected.size(); i++)
    if ( m_NeighDetected[i] < this->GetNumberOfInputs() )
      {

        // get pointers to the input and output
        typename Superclass::InputImagePointer  inputPtr   =  const_cast<TImage *>( this->GetNthInput(m_NeighDetected[i]) );


        ImageRegionType inputRequestedRegion;
        inputRequestedRegion = inputPtr->GetRequestedRegion();

        // pad the input requested region by the operator radius
        inputRequestedRegion.PadByRadius(  m_NeighExtremaSizes[m_NeighDetected[i]]  );

        // crop the input requested region at the input's largest possible region
        if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
          {
            inputPtr->SetRequestedRegion(inputRequestedRegion);
            return;
          }
        else
          {
          // Couldn't crop the region (requested region is outside the largest
          // possible region).  Throw an exception.

          // store what we tried to request (prior to trying to crop)
          inputPtr->SetRequestedRegion(inputRequestedRegion);

          // build an exception
          itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
          std::ostringstream msg,msg2;
          msg << static_cast<const char *>(this->GetNameOfClass())
              << "::GenerateInputRequestedRegion()";
          e.SetLocation(msg.str());
          msg2 << "Requested region is (at least partially) outside the largest possible region (input #" << m_NeighDetected[i] << ").";
          e.SetDescription(msg2.str());
          e.SetDataObject(inputPtr);
          throw e;
          }
      }
    else
      itkExceptionMacro(<< "Requested input #" << m_NeighDetected[i] << ", but only " << this->GetNumberOfInputs() << " inputs are available." << std::endl);

}


template< typename TImage >
void BandMathXImageFilter<TImage>
::BeforeThreadedGenerateData()
{

  unsigned int nbThreads = this->GetNumberOfThreads();
  // Allocate and initialize the thread temporaries
  m_ThreadUnderflow.SetSize(nbThreads);
  m_ThreadUnderflow.Fill(0);
  m_ThreadOverflow.SetSize(nbThreads);
  m_ThreadOverflow.Fill(0);

}


template< typename TImage >
void BandMathXImageFilter<TImage>
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

  if((m_UnderflowCount != 0) || (m_OverflowCount!=0))
  {
    std::stringstream sstm;
    sstm << std::endl
        << "The Following Parsed Expression  :  ";
    for(unsigned int t=0; t<m_Expression.size(); ++t)
        sstm << this->GetExpression(t) << std::endl;
    sstm << "Generated " << m_UnderflowCount << " Underflow(s) "
        << "And " << m_OverflowCount        << " Overflow(s) "   << std::endl
        << "The Parsed Expression, The Inputs And The Output "
        << "Type May Be Incompatible !";

    otbWarningMacro(<< sstm.str());
  }
}

template< typename TImage >
void BandMathXImageFilter<TImage>
::ThreadedGenerateData(const ImageRegionType& outputRegionForThread,
           itk::ThreadIdType threadId)
{

  ValueType value;
  unsigned int nbInputImages = this->GetNumberOfInputs();

  //----------------- --------- -----------------//
  //----------------- Iterators -----------------//
  //----------------- --------- -----------------//
  typedef itk::ImageScanlineConstIterator<TImage> ImageScanlineConstIteratorType;
  typedef itk::ImageScanlineIterator<TImage> ImageScanlineIteratorType;
  typedef itk::ImageRegionConstIteratorWithOnlyIndex<TImage> IndexIteratorType;
  std::vector< ImageScanlineConstIteratorType > Vit;
  Vit.resize(nbInputImages);
  for(unsigned int j=0; j < nbInputImages; ++j)
    Vit[j] = ImageScanlineConstIteratorType (this->GetNthInput(j), outputRegionForThread);
    

  std::vector< ImageScanlineIteratorType > VoutIt;
  VoutIt.resize(m_Expression.size());
  for(unsigned int j=0; j < VoutIt.size(); ++j)
    VoutIt[j] = ImageScanlineIteratorType (this->GetOutput(j), outputRegionForThread);
    

  //Special case : neighborhoods
  std::vector< itk::ConstNeighborhoodIterator<TImage> > VNit;
  for(unsigned int j=0; j<m_VVarName.size(); ++j)
    if (m_VVarName[j].type == 6)
     {
        RadiusType radius;
        radius[0]=(int) ((m_VVarName[j].info[2]-1)/2); // Size x direction (otb convention)
        radius[1]=(int) ((m_VVarName[j].info[3]-1)/2); // Size y direction (otb convention)
        VNit.push_back( itk::ConstNeighborhoodIterator<TImage>(radius, this->GetNthInput(m_VVarName[j].info[0]),outputRegionForThread)); // info[0] = Input image ID
        VNit.back().NeedToUseBoundaryConditionOn();
     }

  // Index only iterator
  IndexIteratorType indexIterator(this->GetNthInput(0), outputRegionForThread);

  // Support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // iterator on variables
  typename std::vector<adhocStruct>::iterator iterVarStart =
    m_AImage[threadId].begin();
  typename std::vector<adhocStruct>::iterator iterVarEnd =
    m_AImage[threadId].end();
  typename std::vector<adhocStruct>::iterator iterVar =
    iterVarStart;

  // temporary output vectors
  std::vector<PixelType> tmpOutputs(m_Expression.size());
  for(unsigned int k=0; k<m_Expression.size(); ++k)
    tmpOutputs[k].SetSize(m_outputsDimensions[k]);

  //----------------- --------------------- -----------------//
  //----------------- Variable affectations -----------------//
  //----------------- --------------------- -----------------//
  for(unsigned int j=0; j < nbInputImages; ++j)       {  Vit[j].GoToBegin();     }
  for(unsigned int j=0; j < m_Expression.size(); ++j) {  VoutIt[j].GoToBegin();  }
  for(unsigned int j=0; j < VNit.size(); ++j)         {  VNit[j].GoToBegin();    }
  indexIterator.GoToBegin();

  while(!Vit[0].IsAtEnd()) // For each pixel
    {
    while(!Vit[0].IsAtEndOfLine()) // For each line
      {
      int ngbhNameIndex=0; int index;

      iterVar = iterVarStart;
      while (iterVar != iterVarEnd)
        {
        switch (iterVar->type)
          {
          case 0 : //idxX
            iterVar->value = static_cast<double>(indexIterator.GetIndex()[0]);
          break;

          case 1 : //idxY
            iterVar->value = static_cast<double>(indexIterator.GetIndex()[1]);
          break;

          case 2 : //Spacing X (imiPhyX)
            //Nothing to do (already set inside BeforeThreadedGenerateData)"
          break;

          case 3 : //Spacing Y (imiPhyY)
            //Nothing to do (already set inside BeforeThreadedGenerateData)"
          break;

          case 4 : //vector
            // iterVar->info[0] : Input image #ID
            for(int p=0; p < iterVar->value.GetCols(); ++p)
              iterVar->value.At(0,p) = Vit[iterVar->info[0]].Get()[p];
          break;

          case 5 : //pixel
            // iterVar->info[0] : Input image #ID
            // iterVar->info[1] : Band #ID
            iterVar->value = Vit[iterVar->info[0]].Get()[iterVar->info[1]];
          break;

          case 6 : //neighborhood
    
          // iterVar->info[1] : Band #ID
          if (iterVar->info[2]*iterVar->info[3] != (int) VNit[ngbhNameIndex].Size() )
            itkExceptionMacro(<< "Size of muparserx variable is different from its related otb neighborhood iterator")

          index=0;
          for(int rows=0; rows<iterVar->info[3]; ++rows)
            for(int cols=0; cols<iterVar->info[2]; ++cols)
              {
                iterVar->value.At(rows,cols) = VNit[ngbhNameIndex].GetPixel(index)[iterVar->info[1]];
                index++;
              }

          ngbhNameIndex++;
          break;

          case 7 :
          //Nothing to do : user defined variable or constant, which have already been set inside PrepareParsers (see above)
          break;

          case 8 :
          //Nothing to do : variable has already been set inside PrepareParsersGlobStats method (see above)
          break;

          default :
            itkExceptionMacro(<< "Type of the variable is unknown");
          break;
          }

        iterVar++;
        }//End while on vars

      //----------------- ----------- -----------------//
      //----------------- Evaluations -----------------//
      //----------------- ----------- -----------------//
      for(unsigned int IDExpression=0; IDExpression<m_Expression.size(); ++IDExpression)
        {
        value = m_VParser[threadId][IDExpression]->EvalRef();

        switch (value.GetType())
          {   //ValueType
          case 'i':
          tmpOutputs[IDExpression][0] = value.GetInteger();
          break;

          case 'f':
          tmpOutputs[IDExpression][0] = value.GetFloat();
          break;

          case 'c':
          itkExceptionMacro(<< "Complex numbers are not supported." << std::endl);
          break;

          case 'm':
            {
            const mup::matrix_type &vect = value.GetArray();
  
            if ( vect.GetRows() == 1 ) //Vector
              for(int p=0; p<vect.GetCols(); ++p)
                tmpOutputs[IDExpression][p] = vect.At(0,p).GetFloat();
            else //Matrix
              itkExceptionMacro(<< "Result of the evaluation can't be a matrix." << std::endl);
            }
          break;
          }
 
        //----------------- Pixel affectations -----------------//
        for(unsigned int p=0; p<m_outputsDimensions[IDExpression]; ++p)
          {
          // Case value is equal to -inf or inferior to the minimum value
          // allowed by the PixelValueType cast
          if (tmpOutputs[IDExpression][p] < double(itk::NumericTraits<PixelValueType>::NonpositiveMin()))
            {
            tmpOutputs[IDExpression][p] = itk::NumericTraits<PixelValueType>::NonpositiveMin();
            m_ThreadUnderflow[threadId]++;
            }
          // Case value is equal to inf or superior to the maximum value
          // allowed by the PixelValueType cast
          else if (tmpOutputs[IDExpression][p] > double(itk::NumericTraits<PixelValueType>::max()))
            {
            tmpOutputs[IDExpression][p] = itk::NumericTraits<PixelValueType>::max();
            m_ThreadOverflow[threadId]++;
            }
          }
        VoutIt[IDExpression].Set(tmpOutputs[IDExpression]);
        }

      for(unsigned int j=0; j < nbInputImages; ++j)        {   ++Vit[j];    }
      for(unsigned int j=0; j < m_Expression.size(); ++j)  {   ++VoutIt[j]; }
      for(unsigned int j=0; j < VNit.size(); ++j)          {   ++VNit[j];   }
      ++indexIterator;

      progress.CompletedPixel();
      }
    for(unsigned int j=0; j < nbInputImages; ++j)        {   Vit[j].NextLine();    }
    for(unsigned int j=0; j < m_Expression.size(); ++j)  {   VoutIt[j].NextLine(); }
    }

}

}// end namespace otb

#endif
