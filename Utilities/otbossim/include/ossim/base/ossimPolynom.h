///////////////////////////////////////////////////////////////////////////////
// ossimPolynom.h
// Author: Frederic Claudel Meraka/CSIR, 2005
///////////////////////////////////////////////////////////////////////////////
//
//TODO : check if automatically removing small monoms is actually viable
//
//TBD : normalization for Least Mean Square fit
//TBD : copy constructor for different DIM
//TBD : LOW PRIORITY redo serialization so that doesn't have to use strings (streams only)

#ifndef ossimPolynom_HEADER
#define ossimPolynom_HEADER

#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatap.h>

/**
 * template class for multivariate polynom algebra
 *
 * T   : the storage type, constraints: must have 0 (zero) as value, support ops fabs + - * /
 * DIM : the dimension of the input space, integer (>=1, default : 1)
 *
 * stores a set of monoms, a monom is (exponent tuples + coefficient)
 * requires a precion (epsilon) for comparisons
 * note: monoms absolute values below epsilon are removed from the map
 */
template < class T, int DIM = 1 >
class  ossimPolynom
{
public:
   /**
    * inner types
    */
   typedef std::vector< int > EXP_TUPLE; //! type to store exponent tuples
   typedef std::vector< T >   VAR_TUPLE; //! type to store multivariate input

   struct EXP_TUPLE_LESSTHAN //! inner functor for sorting exponent tuples
   {
      //warning both EXP_TUPLE should have same dimension (not necessarily DIM)
      bool operator()(const EXP_TUPLE& o1, const EXP_TUPLE& o2)const
      {
         for(unsigned int i=0;i<o1.size();++i)
         {
            if (o1[i]<o2[i])
            {
               return true;
            } else if (o1[i]>o2[i])
            {
               return false;
            }
         }
         return false;
      }
   };

   typedef std::map< EXP_TUPLE, T , EXP_TUPLE_LESSTHAN > MONOM_MAP; //! for storing polynom

   typedef std::set< EXP_TUPLE, EXP_TUPLE_LESSTHAN >     EXPT_SET; //! for storing set of exponent tuples

   /** 
    * construction : 
    * 
    */
    //TBD : by default, adapt epsilon to template type T
   ossimPolynom(const T& epsilon = 0) //! must supply epsilon value, default 0
      : theEpsilon(epsilon)
   {}

   ossimPolynom(const ossimPolynom& p) : 
      theMonoms(p.getMonoms()),
      theEpsilon(p.getEpsilon())
   {}

   ~ossimPolynom()
   {}

   const ossimPolynom& operator=(const ossimPolynom< T, DIM >& pt)
   {
      if (this != &pt)
      {
         theEpsilon = pt.getEpsilon();
         theMonoms  = pt.getMonoms();
      }
      return *this;
   }

   void setMonom(const EXP_TUPLE& m, const T& v)
   {
      if (isNegligible(v))
      {
         theMonoms.erase(m); //TBC TBD: what happens if m is not in the map?
      } else {
         theMonoms[m] = v;
      }
   }

   void setMonom(const int mexp[DIM], const T& v)
   {
      EXP_TUPLE mexpV(mexp,mexp+DIM);
      if (isNegligible(v))
      {
         theMonoms.erase(mexpV); //TBC TBD: what happens if m is not in the map?
      } else {
         theMonoms[mexpV] = v;
      }
   }


   inline void delMonom(const EXP_TUPLE& m)
   {
      theMonoms.erase(m); //TBC TBD: what happens if m is not in the map?
   }

   T getCoeff(const EXP_TUPLE& m)const
   {
      typename MONOM_MAP::const_iterator it = theMonoms.find(m);
      if (it != theMonoms.end())
      {
         return it->second;
      } else {
         return 0;
      }
   }

   void nullify()  //set to 0
   {
      theMonoms.clear();
   }

   inline bool isNull()const
   {
      return (theMonoms.size() == 0);
   }
  
   inline const MONOM_MAP& getMonoms()const 
   {
      return theMonoms;
   }

   inline const T& getEpsilon()const  //! no setEpsilon beacause might erase monoms
   {
      return theEpsilon;
   }

   /**
    * comparison operators
    * -don't compare theEpsilon values
    * -use my own epsilon in comparisons (not the compared to's epsilon)
    */
   bool operator==(const ossimPolynom& pt)const
   {
      if (getMonoms().size() != pt.getMonoms().size()) return false;

      // loop on my monoms
      typename MONOM_MAP::const_iterator it;
      for ( it = getMonoms().begin(); it != getMonoms().end() ; ++it )
      {
         if ( !isNegligible(it->second - pt.getCoeff(it->first)) ) return false;
      }
      return true; //same number of identical monoms
   }

   bool operator!=(const ossimPolynom& pt)const
   {
      return !operator==(pt);
   }

   inline bool isNegligible(const T& v)const //! can v be considered as zero?
   {
      return ( fabs(v) <= theEpsilon );
   }   

   /** 
    * orders
    */
   int getOrder(int d)const //! returns maximum order of monoms for a specific dimension (d starts at 0)
   {
      if ((d>=DIM) || (d<0)) return -1; //error = no dimension

      // loop on monoms
      int order = -1; //for null polynom
      int corder;
      typename MONOM_MAP::const_iterator it;
      for ( it = getMonoms().begin(); it != getMonoms().end() ; ++it )
      {
         corder = (it->first)[d];
         if ( corder > order ) order = corder;
      }
      return order;
   }

   int getTotalOrder()const //! returns maximum order of monoms
   {
      int order = -1; //for null polynom
      int sorder;
      typename MONOM_MAP::const_iterator it;
      for ( it = getMonoms().begin(); it != getMonoms().end() ; ++it )
      {
         sorder = 0;
         for (int d=0;d<DIM;++d) sorder+=(it->first)[d];
         if ( sorder > order ) order = sorder;
      }
      return order;
   }

   /**
    * evaluation : needs DIM values as input
    */
   T eval(const VAR_TUPLE& v)const
   {
      T ev = 0;
      //loop on monoms. TBD optimize powers using map order
      typename MONOM_MAP::const_iterator it;
      int p;
      for ( it = getMonoms().begin(); it != getMonoms().end() ; ++it )
      {
         //compute powers
         T mv = it->second;
         for(int d=0;d<DIM;++d)
         {
            p = (it->first)[d];
            if (p != 0)
            {
               mv *= std::pow( v[d], p );
            }
         }
         //add momom value
         ev += mv;
      }
      return ev;
   }

   /** 
    * partial differentiation polynom
    */
   void pdiff(int pdim, ossimPolynom& result)const
   {
      result.nullify();
      int ord;
      //loop on monoms
      for ( typename MONOM_MAP::const_iterator it = getMonoms().begin(); it != getMonoms().end() ; ++it )
      {
         ord = it->first[pdim];
         if (ord>=1)
         {
            EXP_TUPLE expDiff(it->first);
            expDiff[pdim] -= 1;
            result.setMonom(expDiff, it->second * ord);
         }
      }
   }

   /**
    * operations with scalar
    */
   const ossimPolynom& operator*=(const T& k)
   {
      //loop on monoms
      for ( typename MONOM_MAP::const_iterator it = getMonoms().begin(); it != getMonoms().end() ; ++it )
      {
         it->second *= k;
      }
      discardNullMonoms();
   }

   /**
    * arithmetic operators
    */
   ossimPolynom operator+(const ossimPolynom& p)const
   {
      ossimPolynom< T , DIM > sum(*this);
      return (sum+=p);
   }
   ossimPolynom operator-(const ossimPolynom& p)const
   {
      ossimPolynom< T , DIM > diff(*this);
      return (diff-=p);
   }

   const ossimPolynom& operator+=(const ossimPolynom& p)
   {
      typename MONOM_MAP::const_iterator it;
      //loop on p monoms
      for ( it = p.getMonoms().begin(); it != p.getMonoms().end() ; ++it )
      {
         setMonom( it->first, getCoeff(it->first) + it->second );
      }
      return *this;
   }

   const ossimPolynom& operator-=(const ossimPolynom& p)
   {
      typename MONOM_MAP::const_iterator it;
      //loop on p monoms
      for ( it = p.getMonoms().begin(); it != p.getMonoms().end() ; ++it )
      {
         setMonom( it->first, getCoeff(it->first) - it->second );
      }
      return *this;
   }

   /**
    * product operator : use epsilon from left side
    */
   ossimPolynom operator*(const ossimPolynom& p)const
   {
      //do a stupid multiplication (sum all monom pairs)      
      ossimPolynom< T , DIM > prod(getEpsilon());
      T coeff;
      //loop on p monoms
      for ( typename MONOM_MAP::const_iterator it = getMonoms().begin(); it != getMonoms().end() ; ++it )
      {      
         for ( typename MONOM_MAP::const_iterator pit = p.getMonoms().begin(); pit != p.getMonoms().end() ; ++pit )
         {
            coeff = it->second * pit->second;
            if (coeff!=0)
            {
               EXP_TUPLE prodExp(it->first);
               addExpTuple(prodExp, pit->first);
               prod.addMonomQuick(prodExp, coeff);
            }
         }
      }
      //scan for null monoms and discard
      prod.discardNullMonoms();
      return prod;
   }

   const ossimPolynom& operator*=(const ossimPolynom& p)
   {
      return operator=( this->operator*(p) );
   }

   static void addExpTuple(EXP_TUPLE& m1, const EXP_TUPLE& m2)
   {
      for(int i=0;i<DIM;++i) {
         m1[i] += m2[i];
      }
   }
   
   /**
    * I/O
    * 
    * stream serialization format :
    *  [ k1 (e1_1,e1_2,...,e1_DIM) ; k2 (e2_1,e2_2,..,e2_DIM) ; kN (eN_1,...eN_DIM)]
    *
    * N is the number of monoms
    * ei_j is exponent for dimension j and monom i
    *
    * order is not important
    * all monoms should have the same dimension : DIM
    * you should add eps=xxxx at the beginning, separated by semi-colon ; (by default epsilon is 0)
    *
    * examples:
    *  [ ] is the null polynom, [ eps=1.0e-5 ] too
    *  [ 1.0 (0) 3.5 (1) ] is polynom 1.0 + 3.5*x, with epsilon = 0
    *  [ eps=1.0E-12 ;  2.0 (1,1,0) ; -1.0 (0,0,1) ] is polynom 2*x*y-z, with epsilon=10^-12
    */
   
   std::ostream& print(std::ostream& os)const
   {
      static const char* monom_sep = " ; ";
      static const char* no_sep    = "";

      const char* use_sep = no_sep;

      os<<"[";
      os<<std::setprecision(16); //16 for double, TBD TBC: adapt to epsilon
      os<<std::setiosflags(std::ios_base::scientific);
      //output epsilon if not null
      if (getEpsilon() > 0)
      {         
         os<<use_sep<<"eps="<<getEpsilon();
         use_sep=monom_sep;
      }
      //loop on monoms
      for ( typename MONOM_MAP::const_iterator it = getMonoms().begin(); it != getMonoms().end() ; ++it )
      {
         os<<use_sep<<it->second<<"(";
         for(int d=0 ; d<DIM ; ++d)
         {
            if (d>0)
            {
               os<<",";
            }
            os<<(it->first)[d];
         }
         os<<")";
         use_sep=monom_sep;
      }
      os<<"]";
      return os;
   }
 
   std::ostream& 
   printNice(std::ostream& os, const char symbols[DIM]) //!classic representation (bad accuracy, for display only)
   {
      if (getMonoms().size() == 0)
      {
         os<<"0"; //zero
      } else {
         os<<std::setiosflags(std::ios_base::fixed);
         os<<std::setprecision(14); //14 for double, TBD TBC: adapt to epsilon
         //loop on monoms (map order)
         for ( typename MONOM_MAP::const_iterator it = getMonoms().begin(); it != getMonoms().end() ; ++it )
         {
            T coeff = it->second;
            if (coeff>0)
            {
               os<<"+";
            }
            os<<coeff;
            for(int d=0;d<DIM;++d) 
            {
               int ord=(it->first)[d];
               if (ord>0)
               {
                  os<<""<<symbols[d];
                  if (ord != 1)
                  {
                     os<<ord;
                  }
               }
            }
         }
      }
      return os;
   }

   std::istream& import(std::istream& is) //! note that it can only import for the template type T and dimesnion DIM
   {
      nullify();

      //extract data in brackets [ ]
      //swallow bracket
      ossimString tempString;
      char tempChar;
      is.get(tempChar);
      if (!is) 
      {
         std::cerr<<"ossimPolynom::import ERROR, cannot read left bracket ["<<std::endl;
         return is;
      }
      if (tempChar != '[')
      {
         std::cerr<<"ossimPolynom::import ERROR, missing left bracket ["<<std::endl;
         return is;
      }
      const int BUFSIZE=32768;  //should be enough fro most apps (TBC TBD : allow loops if not enough)
      char buffer[BUFSIZE];
      is.getline(buffer, BUFSIZE, ']');
      if (!is)
      {
         std::cerr<<"ossimPolynom::import ERROR, cannot read after left bracket ["<<std::endl;
         return is;
      }     
      if (is.gcount() >= BUFSIZE-1)
      {
         std::cerr<<"ossimPolynom::import ERROR, cannot find right bracket ] after "<<BUFSIZE-1<<"characters"<<std::endl;
         return is;
      }
      tempString = buffer; //no more brackets

      //split string using semicolons
      std::vector< ossimString > subparts = tempString.explode(";");
      //loop on subparts
      for (typename std::vector< ossimString >::const_iterator it=subparts.begin() ; it!=subparts.end() ; ++it )
      {
         ossimString sp = it->trim();
         //check for epsilon
         ossimString aft_eps=sp.after("eps=");
         if (aft_eps.size()>0)
         {
            //get epsilon value
            aft_eps.trim();
            theEpsilon = static_cast< T >(aft_eps.toDouble());
         } else {
            //no epsilon : must be a monom
            ossimString scalpart=sp.before("(");
            if (scalpart.size() < sp.size())
            {
              T coeff = static_cast< T >(scalpart.toDouble());
              ossimString expopart = ((sp.before(")")).after("(")).trim();
              if (expopart.size() == 0)
              {
                 std::cerr<<"ossimPolynom::import ERROR, cannot find ) in monom or empty monom"<<std::endl;
                 return is;
              }
              std::vector< ossimString > vexpo = expopart.explode(",");
              if (vexpo.size() != DIM)
              {
                 std::cerr<<"ossimPolynom::import ERROR, bad number of exponents in monom (need "<<DIM<<"): "<<vexpo.size()<<std::endl;
                 return is;
              }
              //store all exponents
              EXP_TUPLE expt(DIM);
              int d;
              std::vector< ossimString >::const_iterator eit;
              for (eit=vexpo.begin() , d=0 ; eit != vexpo.end() ; ++eit, ++d )
              {
                 expt[d] = eit->toInt(); //TBD : could check that value is integer, but how?
              }
              //add new monom (if duplicate...error)
              if (getMonoms().find(expt) != getMonoms().end())
              {
                 std::cerr<<"ossimPolynom::import ERROR, duplicate exponent tuple in polynom"<<std::endl;
                 return is;
              }
              theMonoms[expt] = coeff;

            } else {
               std::cerr<<"ossimPolynom::import ERROR, cannot find left parenthesis ( in monom "<<std::endl;
               return is;
            }
         }
      }

      return is;
   }
/**
 * constructs simple exponent tuples set for using LMSfit
 * need order for each dimension
 */
 EXPT_SET builExpSet(const EXP_TUPLE& orders)const
 {
    EXPT_SET eset;
    if (orders.size() != DIM)
    {
       std::cerr<<"ossimPolynom::import ERROR bad dimension for parameter, need "<<DIM<<" elements"<<std::endl;
       return eset;
    }
    //initialise variable exponent tuple
    EXP_TUPLE et(DIM);
    for(int d=0;d<DIM;++d) et[d]=0;
    while (et[0] <= orders[0])
    {
      //add tuple to set
      eset.insert(et);

      //increment tuple within bounds
      et[DIM-1]++;
      for(int d=DIM-1 ; d>=0 ; --d)
      {
         if ((et[d] > orders[d]) && (d>0))
         {
            et[d] = 0;
            et[d-1]++;
         }
      }
    }
    return eset;
 }

 /** 
  * concatenate exponents (at the right) to existing tuple set, for a given maximum total order
  * eg: with eset={(0,1),(0,0)} , 
  * then addExpTuple(2,1,eset) = {(0,1,0,0),(0,1,0,1),(0,1,1,0), (0,0,0,0),(0,0,0,1),(0,0,1,0)}
  */
 void addExpTupleRight(int newdim, int totalOrder, EXPT_SET& eset )const
 {
    EXPT_SET newset;
    // add a copy off eset for each order with the specific last dim
    for(int o=0; o<=totalOrder ; ++o)
    {
       EXPT_SET extset; //extended set
       if (eset.size()==0)
       {
          EXP_TUPLE tu(1);
          tu[0]=o;
          extset.insert(tu);
       } else {
          //we have to construct a new set from eset, extending dimension
          // cause: stored tuples cannot be compared at different dimensions
          for(typename EXPT_SET::iterator sit = eset.begin(); sit != eset.end(); ++sit)
          {
             EXP_TUPLE tu(*sit);
             tu.push_back(o);
             extset.insert(tu);
          }
       }
       //recursively add remaining dimensions
       if (newdim>1)
       {
          addExpTupleRight(newdim-1, totalOrder-o, extset); //only dimension decreases
       }
       //add full set for the specific order
       newset.insert(extset.begin(), extset.end());
    }
    eset=newset; //overwrite
 }

/**
 * fits the polynom to the observations using Least Mean Squares
 * returns true on success (can fail if not enough observations)
 *  + also updates rms error(root mean square)
 * NOTES: inputs must have same size and must be ordered the same way
 *        use builExpSet() to construct classic polynoms 
 * TODO: add weights to observations
 */
bool
LMSfit(const EXPT_SET&                expset,
       const std::vector< VAR_TUPLE > obs_input,
       const std::vector< T >         obs_output,
       T*                             prms = NULL)
{
   //init
   nullify();

   //check size
   int nobs = (int)obs_input.size();
   if (nobs != (int)obs_output.size())
   {
      std::cerr<<"ossimPolynom::LMSfit ERROR observation input/output must have the same size"<<std::endl;
      return false;
   }
   if (nobs<=0)
   {
      std::cerr<<"ossimPolynom::LMSfit ERROR observation count is zero"<<std::endl;
      return false;
   }
   int ncoeff = (int)expset.size();  
   if (ncoeff<=0)
   {
      std::cerr<<"ossimPolynom::LMSfit ERROR exponent count is zero"<<std::endl;
      return false;
   }
   
   //construct LMS linear system (using OSSIM matrices)
   // M.k = v
   // M : monom matrix
   // k : polynbm coefficients
   // v : output_obs
   NEWMAT::Matrix M(nobs, ncoeff);
   double elt;
   int p;
   typename EXPT_SET::const_iterator cit;
   typename std::vector< VAR_TUPLE >::const_iterator oit;
   int i,j;
   for (cit=expset.begin(), j=1; cit != expset.end() ; ++cit, ++j) //loop on exponent tuples
   {
      for(oit=obs_input.begin(), i=1; oit!=obs_input.end();++oit, ++i) //loop on observations
      {
         //compute powers using observation position
         elt=1.0;
         for(int d=0;d<DIM;++d)
         {
            p = (*cit)[d];
            if (p != 0)
            {
               elt *= std::pow( (*oit)[d], p );
            }
         }
         //init M
         M(i,j) = elt; //NEWMAT indices start at 1
      }
   }

   NEWMAT::ColumnVector v(nobs);
   for(int o=0;o<nobs;++o)
   {
      v(o+1) = obs_output[o];
   }

   //build LMS symmetric matrix tM.M
   //build best fit
   NEWMAT::ColumnVector bfit = invert(M.t()*M)*M.t()*v; //TBD : check inversion

   //compute RMS (optional, if rms non null)
   if (prms!=NULL)
   {
      NEWMAT::ColumnVector delta = M*bfit - v;
      *prms = std::sqrt( delta.SumSquare() / nobs);
   }

   //init polynom
   for (cit=expset.begin(), j=1; cit != expset.end() ; ++cit, ++j) //loop on exponent tuples
   {
      setMonom(*cit, bfit(j));
   }
   return true;
}

protected:
   /**
    * protected data members
    */
   MONOM_MAP theMonoms; //!associate a scalar to each tuple of orders : monom

   T theEpsilon; //! positive values below epsilon are considered 0 

   /**
    * method to erase all negligible monoms : user don't need that (automatic)
    */
   void
   discardNullMonoms()
   {     
      std::vector< typename MONOM_MAP::iterator > erasev; //storage for iterators on elements to erase
      for (typename MONOM_MAP::iterator it = theMonoms.begin(); it != theMonoms.end() ; ++it )
      {
         if (isNegligible(it->second)) erasev.push_back(it);
      }
      //erase all elements afterwards
      for ( typename std::vector< typename MONOM_MAP::iterator >::const_iterator vit = erasev.begin(); vit != erasev.end() ; ++vit )
      {
         theMonoms.erase(*vit); //*vit is  an iterator in theMonoms
      }      
   }

   /**
    * add value without testing for negligible
    */
   void 
   addMonomQuick(const EXP_TUPLE& m, const T& v)
   {
      typename MONOM_MAP::iterator it = theMonoms.find(m);
      if (it != theMonoms.end())
      {
         it->second  += v;
      } else {
         theMonoms.insert( MONOM_MAP::value_type(m,v));
      }
   }

   /** 
    * invert stolen from ossimRpcSolver
    */
   NEWMAT::Matrix 
   invert(const NEWMAT::Matrix& m)const
   {
      ossim_uint32 idx = 0;
      NEWMAT::DiagonalMatrix d;
      NEWMAT::Matrix u;
      NEWMAT::Matrix v;

      // decompose m.t*m which is stored in Temp into the singular values and vectors.
      //
      NEWMAT::SVD(m, d, u, v, true, true);
      
      // invert the diagonal
      // this is just doing the reciprical fo all diagonal components and store back int
      // d.  ths compute d inverse.
      //
      for(idx=0; idx < (ossim_uint32)d.Ncols(); ++idx)
      {
         if(d[idx] > getEpsilon()) //adpated here for epsilon
         {
            d[idx] = 1.0/d[idx];
         }
         else
         {
            d[idx] = 0.0;
         }
      }

      //compute inverse of decomposed m;
      return v*d*u.t();
   }

}; //class ossimPolynom



/**
 * stream operators
 */
template < class T, int DIM  > std::ostream& 
operator<<(std::ostream& os, const ossimPolynom<T,DIM>& pt)
{
   return pt.print(os);
}

template < class T, int DIM > std::istream&
operator>>(std::istream& is, ossimPolynom<T,DIM>& pt)
{
   return pt.import(is);
}

#endif
