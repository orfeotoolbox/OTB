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
#ifndef __otbVegetationIndex_h
#define __otbVegetationIndex_h


namespace otb
{
namespace Functor
  {
    /** \class NDVI
     *  \brief This functor calculate the NormalizeD Vegetation Index (NDVI)
     *  
     *  [Pearson et Miller, 1972]
     *
     *  \ingroup Functor
     */
    template <class TInput1, class TInput2, class TOutput>
      class NDVI
      {
      public:
	NDVI() {};
	~NDVI() {};
	inline TOutput operator()(const TInput1 &r, const TInput2 &nri)
	{
                double dr = static_cast<double>(r);
                double dnri = static_cast<double>(nri);
                if( (nri + r) == 0 )
                {
                        return static_cast<TOutput>(0.);
                }

                return ( static_cast<TOutput>((dnri-dr)/(dnri+dr)));
	}
      };

    /** \class RVI
     *  \brief This functor calculate the Ratio Vegetation Index (RVI)
     *  
     *  [Rouse et al., 1973]
     *
     *  \ingroup Functor
     */
    template <class TInput1, class TInput2, class TOutput>
      class RVI
      {
      public:
	RVI() {};
	~RVI() {};
	inline TOutput operator()(const TInput1 &r, const TInput2 &nri)
	{
                double dr = static_cast<double>(r);
                double dnri = static_cast<double>(nri);
                if( r == 0 )
                {
                        return static_cast<TOutput>(0.);
                }
                return ( static_cast<TOutput>(dnri/dr));
	}
      };
    /** \class PVI
     *  \brief This functor calculate the Perpendicular Vegetation Index (PVI)
     *  
     *  [Richardson et Wiegand, 1977]
     *
     *  \ingroup Functor
     */
    template <class TInput1, class TInput2, class TOutput>
      class PVI
      {
      public:
	PVI() {};
	~PVI() {};
	inline TOutput operator()(const TInput1 &r, const TInput2 &nri)
	{
                double dnri = static_cast<double>(nri);
                double dr = static_cast<double>(r);
                return ( static_cast<TOutput>(  (dnri - m_A*dr - m_B)*m_Coeff) );
	 }
         /** Set/Get A and B parameters */
         void SetA(const double A)
         { 
                m_A = A;
                m_Coeff = 1./(vcl_sqrt(m_A*m_A + 1.));
         }
         double GetA(void)const    { return (  m_A ); }
         void SetB(const double B) { m_B = B; }
         double GetB(void)const    { return (  m_B ); }

       private:
          
          /** A and B parameters */
          double  m_A;
          double  m_B;
          /** Denominator, pre-calculed when the A variable is set */
          double  m_Coeff;

      };


    /** \class SAVI
     *  \brief This functor calculate the Soil Adjusted Vegetation Index (SAVI)
     *  
     *  [Huete, 1988]
     *
     *  \ingroup Functor
     */
    template <class TInput1, class TInput2, class TOutput>
      class SAVI
      {
      public:
	SAVI() : m_L(0.5) {};
	~SAVI() {};
	inline TOutput operator()(const TInput1 &r, const TInput2 &nri)
	{
                double dnri = static_cast<double>(nri);
                double dr = static_cast<double>(r);
                double denominator = dnri + dr + m_L;
                if( denominator == 0. )
                {
                        return static_cast<TOutput>(0.);
                }
                return ( static_cast<TOutput>(  ((dnri-dr)*(1+m_L))/denominator ) );
	 }
         /** Set/Get L correction */
         void SetL(const double L) { m_L = L; }
         double GetL(void)const    { return (  m_L ); }

       private:
          
          /** L correction */
          double  m_L;

      };
    /** \class TSAVI
     *  \brief This functor calculate the Transformed Soil Adjusted Vegetation Index (TSAVI)
     *  
     *  [Baret et al. 1989, Baret et Guyot, 1991]
     *
     *  \ingroup Functor
     */
    template <class TInput1, class TInput2, class TOutput>
      class TSAVI
      {
      public:
	TSAVI() : m_X(0.08) {};
	~TSAVI() {};
	inline TOutput operator()(const TInput1 &r, const TInput2 &nri)
	{
                double dnri = static_cast<double>(nri);
                double dr = static_cast<double>(r);
                double denominator = m_A*dnri + dr + m_X*(1.+m_A*m_A);
                if( denominator == 0. )
                {
                        return static_cast<TOutput>(0.);
                }
                return ( static_cast<TOutput>(  (m_A*(dnri - m_A*dr - m_B))/denominator ) );
	 }
         /** Set/Get A and B parameters */
         void SetA(const double A) { m_A = A; }
         double GetA(void)const    { return (m_A); }
         void SetB(const double B) { m_B = B; }
         double GetB(void)const    { return (m_B); }
         /** Set/Get X parameter */
         void SetX(const double X) { m_X = X; }
         double GetX(void)const    { return (m_X); }

       private:
          
          /** A and B parameters */
          double  m_A;
          double  m_B;
          /** X parameter */
          double  m_X;

      };

    /** \class MSAVI
     *  \brief This functor calculate the Modified Soil Adjusted Vegetation Index (MSAVI)
     *  
     *  [Qi et al., 1994]
     *
     *  \ingroup Functor
     */
    template <class TInput1, class TInput2, class TOutput>
      class MSAVI
      {
      public:
	MSAVI() {};
	~MSAVI() {};
	inline TOutput operator()(const TInput1 &r, const TInput2 &nri)
	{
                double dnri = static_cast<double>(nri);
                double dr = static_cast<double>(r);
                double sqrt_value = (2*dr+1)*(2*dr+1) - 8*(dr-dnri);
                if( sqrt_value < 0. )
                {
                        return static_cast<TOutput>(0.);
                }
                return ( static_cast<TOutput>(  (2*dr + 1 - vcl_sqrt(sqrt_value))/2. ) );
	 }

      };

  } // namespace Functor
} // namespace otb

#endif

