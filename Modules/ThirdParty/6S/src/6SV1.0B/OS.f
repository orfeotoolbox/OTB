      subroutine os (iaer_prof,tamoy,trmoy,pizmoy,tamoyp,trmoyp,palt,
     s               phirad,nt,mu,np,rm,gb,rp,
     s                     xl,xlphim,nfi,rolut)

c - to vary the number of quadratures
      include "paramdef.inc"
      integer nquad
      common /num_quad/ nquad
      real pha,qha,uha,alphal,betal,gammal,zetal
      common /sixs_polar/ pha(nqmax_p),qha(nqmax_p),uha(nqmax_p),
     &alphal(0:nqmax_p),betal(0:nqmax_p),gammal(0:nqmax_p),
     &zetal(0:nqmax_p)
      real nbmu
c - to vary the number of quadratures


c  dimension for gauss integration
      integer mu,np,nfi
      real rm(-mu:mu),gb(-mu:mu),rp(np),xlphim(nfi)
c  dimension for os computation
      real  xl(-mu:mu,np)
c array for sos computation
c THOMAS
c      double precision xpl(-mu:mu),bp(0:mu,-mu:mu),
c     s xdel(0:nt),ydel(0:nt),ch(0:nt),h(0:nt),altc(0:nt)
c      double precision i1(0:nt,-mu:mu),i2(0:nt,-mu:mu),i3(-mu:mu),
c     s   i4(-mu:mu),in(-mu:mu),inm1(-mu:mu),inm2(-mu:mu)

      double precision xpl(-25:25),bp(0:25,-25:25),
     s xdel(0:30),ydel(0:30),ch(0:30),h(0:30),altc(0:30)
      double precision i1(0:30,-25:25),i2(0:30,-25:25),i3(-25:25),
     s   i4(-25:25),in(-25:25),inm1(-25:25),inm2(-25:25)
      
CCCC Begin Variable for Look up table generation      
C azimuth or scattering angle variable for LUT computation (rolut)
C azimuth table for look up table computation (filut), nb of fi in each case (nfilut)
      real    luttv,lutmuv,iscama,iscami,nbisca,its,scaa
      real    rolut(mu,41)
c THOMAS
c			real    filut(mu,41)
			real    filut(25,41)
c      double precision psl(-1:nqmax_p,-mu:mu) 
      double precision psl(-1:1000,-25:25) 
c      integer nfilut(mu)
      integer nfilut(25)
CCCC End Variable for Look up table generation      
      
      
      Real tamoy,trmoy,pizmoy
      Real tamoyp,trmoyp,palt,phirad
      real delta,sigma
      double precision hr,ta,tr,trp
      double precision tap,piz,accu,accu2,ha,xmus,zx,yy,dd
      double precision ppp2,ppp1,ca,cr,ratio
      double precision taup,th,xt1,xt2,pi,phi,aaaa,ron
      double precision roavion1,roavion2,roavion,spl,sa1
      double precision beta0,beta2,roavion0
      double precision sa2,c,zi1,f,d,xpk,y
      double precision a1,d1,g1,y1,delta0s
      integer snt
      integer nt,iwr,iplane,mum1,ntp,j,it,itp,i,l,m,iborm
      integer is,isp,ig,k,jj,index
      logical ier
      double precision xx,xdb,bpjk,bpjmk,z,xi1,xi2,x,xpj,ypk,a,b,ii1,ii2
      integer igmax,iaer_prof


     
      common/sixs_del/delta,sigma
      common/sixs_ier/iwr,ier
      common /multorder/ igmax

      nbmu=nquad
c the optical thickness above plane are recomputed to give o.t above pla
c     write(6,*) 'tamoy,trmoy,tamoyp,trmoyp,palt,pizmoy'
c      write(6,*) tamoy,trmoy,tamoyp,trmoyp,palt,pizmoy
c      write(6,*) 'betal 0:80'
c      do i=0,80
c        write(6,*) i,betal(i)
c      enddo
c      write(6,*) 'phase function 83 terms'
c      do i=1,83
c        write(6,*) pha(i)
c      enddo
      snt=nt
      hr=8.0
      ta=tamoy
      tr=trmoy
      trp=trmoy-trmoyp
      tap=tamoy-tamoyp
      piz=pizmoy
c     print *, 'ta,tr,piz,tap,trp,palt,nt'
c     print *,ta,tr,piz,tap,trp,palt,nt
      iplane=0
      accu=1.e-20
      accu2=1.e-3
      mum1=mu-1
c if plane observations recompute scale height for aerosol knowing:
c the aerosol optical depth as measure from the plane 	= tamoyp
c the rayleigh scale   height = 			= hr (8km)
c the rayleigh optical depth  at plane level 		= trmoyp
c the altitude of the plane 				= palt
c the rayleigh optical depth for total atmos		= trmoy
c the aerosol  optical depth for total atmos		= tamoy
c if not plane observations then ha is equal to 2.0km
c ntp local variable: if ntp=nt     no plane observation selected
c                        ntp=nt-1   plane observation selected
c     it's a mixing rayleigh+aerosol
      if(palt.le.900..and.palt.gt.0.0) then
      if (tap.gt.1.e-03) then
         ha=-palt/log(tap/ta)
         else
         ha=2.
         endif
      ntp=nt-1
      else
      ha=2.0
      ntp=nt
      endif
c
      xmus=-rm(0)
c
c compute mixing rayleigh, aerosol
c case 1: pure rayleigh
c case 2: pure aerosol
c case 3: mixing rayleigh-aerosol
c
      if((ta.le.accu2).and.(tr.gt.ta)) then
      do j=0,ntp
      h(j)=j*tr/ntp
      ch(j)=exp(-h(j)/xmus)/2.
      ydel(j)=1.0
      xdel(j)=0.0
      if (j.eq.0) then
         altc(j)=300.
         else
         altc(j)=-log(h(j)/tr)*hr
         endif
      enddo
      endif
      if((tr.le.accu2).and.(ta.gt.tr)) then
      do j=0,ntp
      h(j)=j*ta/ntp
      ch(j)=exp(-h(j)/xmus)/2.
      ydel(j)=0.0
      xdel(j)=piz
      if (j.eq.0) then
         altc(j)=300.
         else
         altc(j)=-log(h(j)/ta)*ha
         endif
      enddo
      endif
c
      if(tr.gt.accu2.and.ta.gt.accu2.and.iaer_prof.eq.0)then
      ydel(0)=1.0
      xdel(0)=0.0
      h(0)=0.
      ch(0)=0.5
      altc(0)=300.
      zx=300.
      iplane=0
      do 14 it=0,ntp
      if (it.eq.0) then
         yy=0.
         dd=0.
         goto 111
      endif
      yy=h(it-1)
      dd=ydel(it-1)
 111  ppp2=300.0
      ppp1=0.0
      itp=it
      call discre(ta,ha,tr,hr,itp,ntp,yy,dd,ppp2,ppp1,
     s    zx)
      if(ier)return
      xx=-zx/ha
      if (xx.le.-20) then
         ca=0.
         else
         ca=ta*dexp(xx)
         endif
      xx=-zx/hr
      cr=tr*dexp(xx)
      h(it)=cr+ca
      altc(it)=zx
      ch(it)=exp(-h(it)/xmus)/2.
      cr=cr/hr
      ca=ca/ha
      ratio=cr/(cr+ca)
      xdel(it)=(1.e+00-ratio)*piz
      ydel(it)=ratio
c     print *,'discre ',it,cr,ca,xdel(it),ydel(it),zx
  14  continue
      endif

      if(tr.gt.acu2.and.ta.gt.acu2.and.iaer_prof.eq.1)then
       call aero_prof(ta,piz,tr,hr,ntp,xmus,
     s   h,ch,ydel,xdel,altc)
      endif

c update plane layer if necessary
      if (ntp.eq.(nt-1)) then
c compute position of the plane layer
         taup=tap+trp
         iplane=-1
         do i=0,ntp
         if (taup.ge.h(i)) iplane=i
         enddo
c update the layer from the end to the position to update if necessary
         th=0.0005
         xt1=abs(h(iplane)-taup)
         xt2=abs(h(iplane+1)-taup)
         if ((xt1.gt.th).and.(xt2.gt.th)) then
         do i=nt,iplane+1,-1
            xdel(i)=xdel(i-1)
            ydel(i)=ydel(i-1)
            h(i)=h(i-1)
            altc(i)=altc(i-1)
            ch(i)=ch(i-1)
         enddo
         else
         nt=ntp
         if (xt2.lt.xt1) iplane=iplane+1
         endif
         h(iplane)=taup
         if ( tr.gt.accu2.and.ta.gt.accu2) then
         ca=ta*exp(-palt/ha)
         cr=tr*exp(-palt/hr)
         h(iplane)=ca+cr
         cr=cr/hr
         ca=ca/ha
         ratio=cr/(cr+ca)
         xdel(iplane)=(1.e+00-ratio)*piz
         ydel(iplane)=ratio
         altc(iplane)=palt
         ch(iplane)=exp(-h(iplane)/xmus)/2.
         endif
         if ( tr.gt.accu2.and.ta.le.accu2) then
         ydel(iplane)=1.
         xdel(iplane)=0.
         altc(iplane)=palt
         endif
         if ( tr.le.accu2.and.ta.gt.accu2) then
         ydel(iplane)=0.
         xdel(iplane)=1.*piz
         altc(iplane)=palt
         endif
      endif
c
c
c     print *,ha,hr,palt,ta,tr,tap,trp
c     do i=0,nt
c     print *,i,h(i),ch(i),xdel(i),ydel(i),altc(i)
c     enddo
c
      pi=acos(-1.)
      phi=phirad
      do 615 l=1,np
      do 615 m=-mu,mu
 615  xl(m,l)=0.
      do ifi=1,nfi
      xlphim(ifi)=0.
      enddo
      
CCC initialization of look up table variable
      do i=1,mu
      nfilut(i)=0
      do j=1,(nbmu-1)/2
      rolut(i,j)=0.
      filut(i,j)=0.
      enddo
      enddo
      its=acos(xmus)*180.0/pi
      do i=1,mu
         lutmuv=rm(i)
         luttv=acos(lutmuv)*180./pi
         iscama=(180-abs(luttv-its))
         iscami=(180-(luttv+its))
         nbisca=int((iscama-iscami)/4)+1
         nfilut(i)=nbisca
         filut(i,1)=0.0
         filut(i,nbisca)=180.0
	 scaa=iscama
         do j=2,nfilut(i)-1
          scaa=scaa-4.0
          cscaa=cos(scaa*pi/180.)
          cfi=-(cscaa+xmus*lutmuv)/(sqrt(1-xmus*xmus)
     S	  *sqrt(1.-lutmuv*lutmuv))
          filut(i,j)=acos(cfi)*180.0/pi
         enddo
      enddo
CCCC Check initialization  (debug)     
C      do i=1,mu
C        lutmuv=rm(i)
C        luttv=acos(lutmuv)*180./pi
C       do j=1,nfilut(i)
C      cscaa=-xmus*lutmuv-cos(filut(i,j)*pi/180.)*sqrt(1.-xmus*xmus)
C    S  *sqrt(1.-lutmuv*lutmuv)
C      scaa=acos(cscaa)*180./pi
C      write(6,*) its,luttv,filut(i,j),scaa
C      enddo
C      enddo
CCCC Check initialization  (debug)     
CCC end initialization of look up table variable
      
      
      
c
c     ************ incident angle mus *******
c
c
      aaaa=delta/(2-delta)
      ron=(1-aaaa)/(1+2*aaaa)
c     write(6,*) 'ron ',ron
c
c     rayleigh phase function
c
      beta0=1.
      beta2=0.5*ron
c
c     fourier decomposition
c
      do 17 j=-mu,mu
      i4(j)=0.
   17 continue
      iborm=nbmu-3
      if( abs (xmus-1.000000) .lt.1.e-06)iborm=0
      do 24 is=0,iborm
c
c    primary scattering
c
      ig=1
      roavion0=0.
      roavion1=0.
      roavion2=0.
      roavion=0.
      do 16 j=-mu,mu
      i3(j)=0.
   16 continue
c
c     kernel computations
c
      isp=is
      call kernel(isp,mu,rm,xpl,psl,bp)
      if(is.gt.0)beta0=0.
      do 100 j=-mu,mu
      if(is-2)200,200,201
 200  spl=xpl(0)
      sa1=beta0+beta2*xpl(j)*spl
      sa2=bp(0,j)
      goto 202
 201  sa2=bp(0,j)
      sa1=0.
c
c     primary scattering source function at every level within the layer
c
 202  do 101 k=0,nt
      c=ch(k)
      a=ydel(k)
      b=xdel(k)
      i2(k,j)=c*(sa2*b+sa1*a)
  101 continue
  100 continue
c
c     vertical integration, primary upward radiation
c
 
      do 108 k=1,mu
      i1(nt,k)=0.
      zi1=i1(nt,k)
      yy=rm(k)
      do 108 i=nt-1,0,-1
      jj=i+1
      f=h(jj)-h(i)
      a=(i2(jj,k)-i2(i,k))/f
      b=i2(i,k)-a*h(i)
      c=exp(-f/yy)
      d=1.0e+00-c
      xx=h(i)-h(jj)*c
      zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00
      i1(i,k)=zi1
  108 continue
c
c     vertical integration, primary downward radiation
c
      do 109 k=-mu,-1
      i1(0,k)=0.
      zi1=i1(0,k)
      yy=rm(k)
      do 109 i=1,nt
      jj=i-1
      f=h(i)-h(jj)
      c=exp(f/yy)
      d=1.0e+00-c
      a=(i2(i,k)-i2(jj,k))/f
      b=i2(i,k)-a*h(i)
      xx=h(i)-h(jj)*c
      zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00
      i1(i,k)=zi1
  109 continue
c
c     inm2 is inialized with scattering computed at n-2
c     i3 is inialized with primary scattering
c
      do 20 k=-mu,mu
      if(k) 21,20,23
   21 index=nt
      go to 25
   23 index=0
   25 continue
      inm1(k)=i1(index,k)
      inm2(k)=i1(index,k)
      i3(k)=i1(index,k)
   20 continue
      roavion2=i1(iplane,mu)
      roavion=i1(iplane,mu)
c
c     loop on successive order
c
  503 ig=ig+1
c     write(6,*) 'ig ',ig
c
c     successive orders
c
c     multiple scattering source function at every level within the laye
c
c     if is < ou = 2 kernels are a mixing of aerosols and molecules kern
c     if is >2 aerosols kernels only
c
      if(is-2)210,210,211
  210 do455 k=1,mu
      xpk=xpl(k)
      ypk=xpl(-k)
      do 455 i=0,nt
      ii1=0.
      ii2=0.
      x=xdel(i)
      y=ydel(i)
      do477 j=1,mu
      xpj=xpl(j)
      z=gb(j)
      xi1=i1(i,j)
      xi2=i1(i,-j)
      bpjk=bp(j,k)*x+y*(beta0+beta2*xpj*xpk)
      bpjmk=bp(j,-k)*x+y*(beta0+beta2*xpj*ypk)
      xdb=z*(xi1*bpjk+xi2*bpjmk)
      ii2=ii2+xdb
      xdb=z*(xi1*bpjmk+xi2*bpjk)
      ii1=ii1+xdb
 477  continue
      if (abs(ii2).lt.1.E-30) ii2=0.
      if (abs(ii1).lt.1.E-30) ii1=0.
      i2(i,k)=ii2
      i2(i,-k)=ii1
 455  continue
      goto 213
 211  do45 k=1,mu
      do 45 i=0,nt
      ii1=0.
      ii2=0.
      x=xdel(i)
      do47 j=1,mu
      z=gb(j)
      xi1=i1(i,j)
      xi2=i1(i,-j)
      bpjk=bp(j,k)*x
      bpjmk=bp(j,-k)*x
      xdb=z*(xi1*bpjk+xi2*bpjmk)
      ii2=ii2+xdb
      xdb=z*(xi1*bpjmk+xi2*bpjk)
      ii1=ii1+xdb
   47 continue
      if (abs(ii2).lt.1.E-30) ii2=0.
      if (abs(ii1).lt.1.E-30) ii1=0.
      i2(i,k)=ii2
      i2(i,-k)=ii1
   45 continue
c
c     vertical integration, upward radiation
c
 213  do 48 k=1,mu
      i1(nt,k)=0.
      zi1=i1(nt,k)
      yy=rm(k)
      do 48 i=nt-1,0,-1
      jj=i+1
      f=h(jj)-h(i)
      a=(i2(jj,k)-i2(i,k))/f
      b=i2(i,k)-a*h(i)
      c=exp(-f/yy)
      d=1.e+00-c
      xx=h(i)-h(jj)*c
      zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00
      if (abs(zi1).le.1.E-20) zi1=0.
      i1(i,k)=zi1
   48 continue
c
c     vertical integration, downward radiation
c
      do 50 k=-mu,-1
      i1(0,k)=0.
      zi1=i1(0,k)
      yy=rm(k)
      do 50 i=1,nt
      jj=i-1
      f=h(i)-h(jj)
      c=exp(f/yy)
      d=1.e+00-c
      a=(i2(i,k)-i2(jj,k))/f
      b=i2(i,k)-a*h(i)
      xx=h(i)-h(jj)*c
      zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00
      if (abs(zi1).le.1.E-20) zi1=0.
      i1(i,k)=zi1
   50 continue
c
c     in is the nieme scattering order
c
      do 30 k=-mu,mu
      if(k) 31,30,33
   31 index=nt
      go to 34
   33 index=0
   34 continue
      in(k)=i1(index,k)
   30 continue
      roavion0=i1(iplane,mu)
c
c   convergence test (geometrical serie)
c
      if(ig.gt.2) then
      z=0.
      a1=roavion2
      d1=roavion1
      g1=roavion0
      if(a1.ge.accu.and.d1.ge.accu.and.roavion.ge.accu) then
      y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/roavion))
      y=abs(y)
      z=dmax1(dble(y),z)
      endif
      do 99 l=-mu,mu
      if (l.eq.0) goto 99
      a1=inm2(l)
      d1=inm1(l)
      g1=in(l)
      if(a1.le.accu) go to 99
      if(d1.le.accu) go to 99
      if(i3(l).le.accu) go to 99
      y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/i3(l)))
      y=abs(y)
      z=dmax1(dble(y),z)
  99  continue
      if(z.lt.0.0001) then
c
c     successful test (geometrical serie)
c
      do 606 l=-mu,mu
      y1=1.
      d1=inm1(l)
      g1=in(l)
      if(d1.le.accu) go to 606
      y1=1-g1/d1
      if(abs(g1-d1).le.accu) then
      go to 606
      endif
      g1=g1/y1
      i3(l)=i3(l)+g1
  606 continue
      d1=roavion1
      g1=roavion0
      y1=1.
      if(d1.ge.accu) then
      if(abs(g1-d1).ge.accu) then
      y1=1-g1/d1
      g1=g1/y1
      endif
      roavion=roavion+g1
      endif
      go to 505
      endif
c
c     inm2 is the (n-2)ieme scattering order
c
      do 26 k=-mu,mu
      inm2(k)=inm1(k)
   26 continue
      roavion2=roavion1
      endif
c
c     inm1 is the (n-1)ieme scattering order
c
      do 27 k=-mu,mu
      inm1(k)=in(k)
   27 continue
      roavion1=roavion0
c
c     sum of the n-1 orders
c
      do 610 l=-mu,mu
      i3(l)=i3(l)+in(l)
  610 continue
      roavion=roavion+roavion0
c
c     stop if order n is less than 1% of the sum
c
      z=0.
      do 611 l=-mu,mu
      if (abs(i3(l)).ge.accu) then
      y=abs(in(l)/i3(l))
      z=dmax1(z,dble(y))
      endif
  611 continue

c     if(z.lt.0.00001) go to 505    # 6SV4.0 choice
      if(z.lt.0.00001) go to 505
c
c      stop if order n is greater than 20 in any case
c
      if(ig-igmax) 503,503,505
  505 continue
c
c     sum of the fourier component s
c
      delta0s=1
      if(is.ne.0) delta0s=2
      do 612 l=-mu,mu
      i4(l)=i4(l)+delta0s*i3(l)
  612 continue
c
c     stop of the fourier decomposition
c
      do 614 l=1,np
      phi=rp(l)
      do 614 m=-mum1,mum1
      if(m.gt.0) then
      xl(m,l)=xl(m,l)+delta0s*i3(m)*cos(is*(phi+pi))
      else
      xl(m,l)=xl(m,l)+delta0s*i3(m)*cos(is*phi)
      endif
 614  continue
 
C Look up table generation 
      do m=1,mu
      do l=1,nfilut(m)
      phimul=filut(m,l)*pi/180.
      rolut(m,l)=rolut(m,l)+delta0s*i3(m)*cos(is*(phimul+pi))
      enddo
      enddo
C end of look up table generation 
 
 
 
      if(is.eq.0) then
      do k=1,mum1
      xl(0,1)=xl(0,1)+rm(k)*gb(k)*i3(-k)
      enddo
      endif
      xl(mu,1)=xl(mu,1)+delta0s*i3(mu)*cos(is*(phirad+pi))
      do ifi=1,nfi
      phimul=(ifi-1)*pi/(nfi-1)
      xlphim(ifi)=xlphim(ifi)+delta0s*roavion*cos(is*(phimul+pi))
      enddo
      xl(-mu,1)=xl(-mu,1)+delta0s*roavion*cos(is*(phirad+pi))
      z=0.
      do 613 l=-mu,mu
       if (abs(i4(l)).lt.accu) goto 613
      x=abs(i3(l)/i4(l))
      z=dmax1(z,x)
  613 continue

c     if(z.gt.0.001) go to 24     #6Sv4.0 choice
      if(z.gt.0.001) go to 24
      goto 243

   24 continue
  243 continue
      nt=snt
c     write(6,*) 'reflectance ', xl(mu,1)/xmus
      return
      end
