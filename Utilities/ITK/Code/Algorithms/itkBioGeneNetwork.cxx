/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBioGeneNetwork.cxx,v $
  Language:  C++
  Date:      $Date: 2008-10-20 21:28:21 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkBioGeneNetwork.h"

namespace itk {

namespace bio {

/**
 *    Constructor 
 */ 
GeneNetwork
::GeneNetwork()
{
}

/**
 *    Destructor   
 */ 
GeneNetwork
::~GeneNetwork()
{
}

/**
 *    Copy from another genome
 */ 
void
GeneNetwork
::Copy( const GeneNetwork & geneNetwork )
{

  m_ProteinConcentration.clear();
  m_ProteinConcentration.insert( 
    m_ProteinConcentration.begin(),
    geneNetwork.m_ProteinConcentration.begin(),
    geneNetwork.m_ProteinConcentration.end()    );

  m_DomainConcentration.clear();
  m_DomainConcentration.insert( 
    m_DomainConcentration.begin(),
    geneNetwork.m_DomainConcentration.begin(),
    geneNetwork.m_DomainConcentration.end()    );
}
 
}  // end namespace bio

}  // end namespace itk
