/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDecisionRuleBase.cxx,v $
  Language:  C++
  Date:      $Date: 2008-10-07 09:09:39 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkDecisionRuleBase.h"

namespace itk
{

DecisionRuleBase
::DecisionRuleBase() 
{
}

DecisionRuleBase
::~DecisionRuleBase()
{
}

void
DecisionRuleBase
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);
}// end PrintSelf

} // namespace itk
