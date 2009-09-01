/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMinimumDecisionRule2.cxx,v $
  Language:  C++
  Date:      $Date: 2009-05-02 05:43:58 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkMinimumDecisionRule2.h"

namespace itk { 
namespace Statistics { 

void 
MinimumDecisionRule2::PrintSelf(std::ostream& os, Indent indent) const 
{ 
  Superclass::PrintSelf(os, indent); 
}
 
unsigned int 
MinimumDecisionRule2::Evaluate(const MembershipVectorType &discriminantScores) const
{
  double min = discriminantScores[0];
  unsigned int minIndex = 0;
  unsigned int i;
  for (i = 1; i < discriminantScores.size(); i++)
    {
    if (discriminantScores[i] < min) 
      {
      min = discriminantScores[i];
      minIndex = i;
      }
    }
  return minIndex;
}

} // end of namespace itk
} // end of namespace Statistics
