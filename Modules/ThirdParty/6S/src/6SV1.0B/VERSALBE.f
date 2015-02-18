      subroutine versalbe(option,optics,struct,
     s           brdfalb)
c
c interface between the computer code of the model of verstraete and pin
c the computer code is courtesy of michel verstraete.
c see subroutine mvbp1 for a complete description
c
c
c  variables angles, error, optics, option, struct and mvbp1 are fully
c  described in the file mvbp1.for.
c     angdeg       angles in degrees
c     brdf         variable to receive the bidirectional reflectance
c                  value returned by the function mvbp1.
c     fmt          formats.
c     errmsg       error messages.
      parameter (nta=24,nfa=48)
      real teta1,teta2,phi1,phi2,ta(nta),fa(nfa),wta(nta),wfa(nfa)
      real mu1,mu2,fi
      real pi,y
c
      integer  option(5), error
      real angles(3), optics(3), struct(4)
      real         mvbp1
      character*60 fmt (6)
      character*80 errmsg (15)
      external     mvbp1
      logical ier
      real brdfalb,summ,si2,si1,pond
      integer iwr,k,j,l
      common/sixs_ier/iwr,ier
      data fmt (1) /'(i10)'/
      data fmt (2) /'(e10.3)'/
      data fmt (3) /'(1x, a10, 6 (i8, 2x))'/
      data fmt (4) /'(1x, a10, 6 (f8.3, 2x))'/
      data fmt (5) /'(1x, a, f9.6)'/
      data fmt (6) /'(1x, a)'/
      data errmsg (1)
     *   /'101: option (1) not an acceptable value, reset to 1'/
      data errmsg (2)
     *   /'102: option (2) not an acceptable value, reset to 1'/
      data errmsg (3)
     *   /'103: option (3) not an acceptable value, reset to 1'/
      data errmsg (4)
     *   /'104: option (4) not an acceptable value, reset to 1'/
      data errmsg (5)
     *   /'105: option (5) not an acceptable value, reset to 1'/
      data errmsg (6)
     *   /'201: illumination zenith angle (angles (1)) not acceptable'/
      data errmsg (7)
     *   /'202: observation zenith angle (angles (2)) not acceptable'/
      data errmsg (8)
     *   /'203: relative azimuth angle (angles (3)) not acceptable'/
      data errmsg (9)
     *   /'301: single scattering albedo (optics (1)) not acceptable'/
      data errmsg (10)
     *   /'302: phase function parameter (optics (2) not acceptable'/
      data errmsg (11)
     *   /'401: leaf area density (struct (1)) not acceptable'/
      data errmsg (12)
     *   /'402: radius of sun flecks (struct (2)) not acceptable'/
      data errmsg (13)
     *   /'411: kappa1 (struct (3) with option (3) = 0) not acceptable'/
      data errmsg (14)
     *   /'412: kappa2 (struct (4) with option (3) = 0) not acceptable'/
      data errmsg (15)
     *   /'421: chil (struct (3) with option (3) = 1) not acceptable'/
c
      pi=atan(1.)*4.
      teta1=0.
      teta2=pi/2.
      call gauss(teta1,teta2,ta,wta,nta)
      phi1=0.
      phi2=2.*pi
      call gauss(phi1,phi2,fa,wfa,nfa)
      brdfalb=0.
      summ=0.
      do 1 k=1,nfa
      do 2 j=1,nta
      do 3 l=1,nta
      mu2=cos(ta(j))
      mu1=cos(ta(l))
      si2=sin(ta(j))
      si1=sin(ta(l))
      fi=fa(k)
c compute geometrical parameter for mvbp1
      angles(1)=acos(mu1)
      angles(2)=acos(mu2)
      angles(3)=fi
      y=mvbp1(option,angles,optics,struct,error)
      if (error .eq. 101) write (iwr, fmt (6)) errmsg (1)
      if (error .eq. 102) write (iwr, fmt (6)) errmsg (2)
      if (error .eq. 103) write (iwr, fmt (6)) errmsg (3)
      if (error .eq. 104) write (iwr, fmt (6)) errmsg (4)
      if (error .eq. 105) write (iwr, fmt (6)) errmsg (5)
      if (error .eq. 201) then
         write (iwr, fmt (6)) errmsg (6)
         write(iwr,*) angles(1)
         endif
      if (error .eq. 202) then
         write(iwr,fmt (6)) errmsg (7)
         write(iwr,*) angles(2)
         endif
      if (error .eq. 203) write (iwr, fmt (6)) errmsg (8)
      if (error .eq. 301) write (iwr, fmt (6)) errmsg (9)
      if (error .eq. 302) write (iwr, fmt (6)) errmsg (10)
      if (error .eq. 401) write (iwr, fmt (6)) errmsg (11)
      if (error .eq. 402) write (iwr, fmt (6)) errmsg (12)
      if (error .eq. 411) write (iwr, fmt (6)) errmsg (13)
      if (error .eq. 412) write (iwr, fmt (6)) errmsg (14)
      if (error .eq. 421) write (iwr, fmt (6)) errmsg (15)
      pond=mu1*mu2*si1*si2*wfa(k)*wta(j)*wta(l)
      brdfalb=brdfalb+pond*y
      summ=summ+pond
  3   continue
  2   continue
  1   continue
      brdfalb=brdfalb/summ
      return
      end
