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
#include "otbMissingNumericTraits.h"


namespace itk
{
const std::complex< char >  NumericTraits< std::complex< char > >:: Zero = std::complex< char >(0, 0);
const std::complex< char >  NumericTraits< std::complex< char > >:: One  = std::complex< char >(1, 0);

const std::complex< unsigned char >  NumericTraits< std::complex< unsigned char > >:: Zero = std::complex< unsigned char >(0, 0);
const std::complex< unsigned char >  NumericTraits< std::complex< unsigned char > >:: One  = std::complex< unsigned char >(1, 0);

const std::complex< short >  NumericTraits< std::complex< short > >:: Zero = std::complex< short >(0, 0);
const std::complex< short >  NumericTraits< std::complex< short > >:: One  = std::complex< short >(1, 0);

const std::complex< unsigned short >  NumericTraits< std::complex< unsigned short > >:: Zero = std::complex< unsigned short >(0, 0);
const std::complex< unsigned short >  NumericTraits< std::complex< unsigned short > >:: One  = std::complex< unsigned short >(1, 0);

const std::complex< int >  NumericTraits< std::complex< int > >:: Zero = std::complex< int >(0, 0);
const std::complex< int >  NumericTraits< std::complex< int > >:: One  = std::complex< int >(1, 0);

const std::complex< unsigned int >  NumericTraits< std::complex< unsigned int > >:: Zero = std::complex< unsigned int >(0, 0);
const std::complex< unsigned int >  NumericTraits< std::complex< unsigned int > >:: One  = std::complex< unsigned int >(1, 0);

const std::complex< long >  NumericTraits< std::complex< long > >:: Zero = std::complex< long >(0UL, 0UL);
const std::complex< long >  NumericTraits< std::complex< long > >:: One  = std::complex< long >(1UL, 0UL);

const std::complex< unsigned long >  NumericTraits< std::complex< unsigned long > >:: Zero = std::complex< unsigned long >(0, 0);
const std::complex< unsigned long >  NumericTraits< std::complex< unsigned long > >:: One  = std::complex< unsigned long >(1, 0);


} // end namespace otb
