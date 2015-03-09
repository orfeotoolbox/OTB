      subroutine oda550 (iaer,v,
     a                   taer55)
 
      double precision bnz,bnz1
      common /sixs_atm/ z(34),p(34),t(34),wh(34),wo(34)
      common /sixs_del/ delta,sigma
      real an5(34),an23(34)
      Real v,taer55,z,p,t,wh
      Real wo,delta,sigma,dz,bn5,bn51,bn23,bn231,az
      Real az1,bz,bz1,ev
      Integer iaer,k
c    aerosol optical depth at wl=550nm
c     vertical repartition of aerosol density for v=23km
c                     ( in nbr of part/cm3 )
 
      data an23 /2.828e+03,1.244e+03,5.371e+02,2.256e+02,1.192e+02
     a,8.987e+01,6.337e+01,5.890e+01,6.069e+01,5.818e+01,5.675e+01
     a,5.317e+01,5.585e+01,5.156e+01,5.048e+01,4.744e+01,4.511e+01
     a,4.458e+01,4.314e+01,3.634e+01,2.667e+01,1.933e+01,1.455e+01
     a,1.113e+01,8.826e+00,7.429e+00,2.238e+00,5.890e-01,1.550e-01
     a,4.082e-02,1.078e-02,5.550e-05,1.969e-08,0.000e+00/
 
c     vertical repartition of aerosol density for v=5km
c                     ( in nbr of part/cm3 )
 
      data  an5 /1.378e+04,5.030e+03,1.844e+03,6.731e+02,2.453e+02
     a,8.987e+01,6.337e+01,5.890e+01,6.069e+01,5.818e+01,5.675e+01
     a,5.317e+01,5.585e+01,5.156e+01,5.048e+01,4.744e+01,4.511e+01
     a,4.458e+01,4.314e+01,3.634e+01,2.667e+01,1.933e+01,1.455e+01
     a,1.113e+01,8.826e+00,7.429e+00,2.238e+00,5.890e-01,1.550e-01
     a,4.082e-02,1.078e-02,5.550e-05,1.969e-08,0.000e+00/
 
 
      taer55=0.
 
      if(abs(v).le.0.) return
      if(iaer.eq.0) return
 
      do 1 k=1,32
      dz=z(k+1)-z(k)
      bn5=an5(k)
      bn51=an5(k+1)
      bn23=an23(k)
      bn231=an23(k+1)
      az=(115./18.)*(bn5-bn23)
      az1=(115./18.)*(bn51-bn231)
      bz=(5.*bn5/18.)-(23.*bn23/18.)
      bz1=(5.*bn51/18.)-(23.*bn231/18.)
      bnz=az/v-bz
      bnz1=az1/v-bz1
      ev=dz*exp((dlog(bnz)+dlog(bnz1))*.5)
      taer55=taer55+ev*sigma*1.0e-03
    1 continue
      return
      end
