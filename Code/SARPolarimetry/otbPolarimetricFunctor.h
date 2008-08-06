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
#ifndef __otbPolarimetricFunctor_h
#define __otbPolarimetricFunctor_h


namespace otb
{
namespace Functor
  {
    /** \class PolarimetricFunctor2Channels
     *  \brief This functor calculate the
     *  
     *  
     *
     *  \ingroup Functor
     */
    template <class TInput1, class TInput2, class TOutput>
      class PolarimetricFunctor2Channels
      {
      public:
	PolarimetricFunctor2Channels() {};
	~PolarimetricFunctor2Channels() {};
	inline TOutput operator()(const TInput1 &r, const TInput2 &nir)
	{

                return ( static_cast<TOutput>(0) );
	}
      };

    /** \class PolarimetricSynthesis3Channels
     *  \brief This functor calculate the
     *  
     *  
     *
     *  \ingroup Functor
     */
/*    template <class TInput1, class TInput2, class TInput3, class TOutput>
      class PolarimetricSynthesis3Channels
      {
      public:
	PolarimetricSynthesis3Channels() {};
	~PolarimetricSynthesis3Channels() {};
	inline TOutput operator()(const TInput1 &r, const TInput2 &nir)
	{
                double dr = static_cast<double>(r);
                double dnir = static_cast<double>(nir);
                if( r == 0 )
                {
                        return static_cast<TOutput>(0.);
                }
                return ( static_cast<TOutput>(dnir/dr));
	}
      };
*/      
    /** \class PolarimetricSynthesis4Channels
     *  \brief This functor calculate the
     *  
     *  
     *
     *  \ingroup Functor
     */
/*    template <class TInput1, class TInput2, class TInput3, class TInput4, class TOutput>
      class PolarimetricSynthesis4Channels
      {
      public:
	PolarimetricSynthesis4Channels() {};
	~PolarimetricSynthesis4Channels() {};
	inline TOutput operator()(const TInput1 &r, const TInput2 &nir)
	{
                double dnir = static_cast<double>(nir);
                double dr = static_cast<double>(r);
                return ( static_cast<TOutput>(  (dnir - m_A*dr - m_B)*m_Coeff) );
	 }

       private:
*/          

  } // namespace Functor
} // namespace otb

#endif

