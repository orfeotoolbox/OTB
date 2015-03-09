      subroutine iso(iaer_prof,tamoy,trmoy,pizmoy,
     s            tamoyp,trmoyp,
     s            palt,nt,mu,rm,gb,
     s                     xf)

      include "paramdef.inc"     
c THOMAS
c      double precision psl(-1:nqmax_p,-mu:mu)
      double precision psl(-1:1000,-25:25)
c  dimension for gauss integration
      integer mu
      real rm(-mu:mu),gb(-mu:mu)
c  dimension for os computation
      real  xf(-1:1)
c array for sos computation
c THOMAS
c      double precision xpl(-mu:mu),bp(0:mu,-mu:mu),ch(0:nt)
c      double precision xdel(0:nt),ydel(0:nt),h(0:nt),altc(0:nt)
c      double precision i1(0:nt,-mu:mu),i2(0:nt,-mu:mu),i3(-mu:mu)
c      double precision in(-mu:mu),inm1(-mu:mu),inm2(-mu:mu)

      double precision xpl(-25:25),bp(0:25,-25:25),ch(0:30)
      double precision xdel(0:30),ydel(0:30),h(0:30),altc(0:30)
      double precision i1(0:30,-25:25),i2(0:30,-25:25),i3(-25:25)
      double precision in(-25:25),inm1(-25:25),inm2(-25:25)

      double precision acu,acu2,ta,piz
      double precision tr,trp,tap,hr,ha,zx,yy,dd,ppp2,ppp1,ca
      double precision cr,ratio,taup,th,xt1,xt2,aaaa,ron,beta0,beta2
      double precision tavion0,tavion1,tavion2,tavion,zi1,xpk,ypk,x,y
      double precision z,xi1,xi2,bpjk,bpjmk,f,a,b,c,d,xx,a1,d1,g1
      double precision y1,xpj,xxx,ii1,ii2
      Real tamoy,trmoy,pizmoy
      Real tamoyp,trmoyp,palt
      Real delta,sigma
      integer snt,nt,iplane,ntp,j,it,itp,i,ig,k,index,iwr,m
      integer jj,l
      logical ier
      integer igmax,iaer_prof

      common/sixs_del/delta,sigma
      common/sixs_ier/iwr,ier
      common /multorder/ igmax
     
 
      snt=nt
      iplane=0
      acu=1.e-20
      acu2=1.e-3
      ta=tamoy
      piz=pizmoy
      tr=trmoy
      do 615 m=-1,1
 615  xf(m)=0.
c
c     molecular ratio within the layer
c     computations are performed assuming a scale of 8km for
c     molecules and 2km for aerosols
c
c the optical thickness above plane are recomputed to give o.t above pla
      trp=trmoy-trmoyp
      tap=tamoy-tamoyp
c     print *, 'tamoy,trmoy,pizmoy,tap,trp,palt,nt'
c     print *,tamoy,trmoy,pizmoy,tap,trp,palt,nt
      acu=1.e-20
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



      hr=8.0
c     it's a mixing rayleigh+aerosol
      if(palt.le.900..and.palt.gt.0.0)then
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
      ta=tamoy
      tr=trmoy
      piz=pizmoy
c
c compute mixing rayleigh, aerosol
c case 1: pure rayleigh
c case 2: pure aerosol
c case 3: mixing rayleigh-aerosol
c
      if((ta.le.acu2).and.(tr.gt.ta)) then
      do j=0,ntp
      h(j)=j*tr/ntp
      ydel(j)=1.0
      xdel(j)=0.0
      enddo
      endif
      if((tr.le.acu2).and.(ta.gt.tr)) then
      do j=0,ntp
      h(j)=j*ta/ntp
      ydel(j)=0.0
      xdel(j)=piz
      enddo
      endif
c
      if(tr.gt.acu2.and.ta.gt.acu2.and.iaer_prof.eq.0)then
      ydel(0)=1.0
      xdel(0)=0.0
      h(0)=0.
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
      xxx=-zx/ha
      if (xxx.lt.-18) then
         ca=0.
         else
         ca=ta*dexp(xxx)
         endif
      xxx=-zx/hr
      cr=tr*dexp(xxx)
      h(it)=cr+ca
      altc(it)=zx
c     print *,it,cr,ca,h(it),zx
      cr=cr/hr
      ca=ca/ha
      ratio=cr/(cr+ca)
      xdel(it)=(1.e+00-ratio)*piz
      ydel(it)=ratio
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
         th=0.005
         xt1=abs(h(iplane)-taup)
         xt2=abs(h(iplane+1)-taup)
         if ((xt1.gt.th).and.(xt2.gt.th)) then
         do i=nt,iplane+1,-1
            xdel(i)=xdel(i-1)
            ydel(i)=ydel(i-1)
            h(i)=h(i-1)
            altc(i)=altc(i-1)
         enddo
         else
         nt=ntp
         if (xt2.lt.xt1) iplane=iplane+1
         endif
         h(iplane)=taup
         if ( tr.gt.acu2.and.ta.gt.acu2) then
         ca=ta*exp(-palt/ha)
         cr=tr*exp(-palt/hr)
         cr=cr/hr
         ca=ca/ha
         ratio=cr/(cr+ca)
         xdel(iplane)=(1.e+00-ratio)*piz
         ydel(iplane)=ratio
         altc(iplane)=palt
         endif
         if ( tr.gt.acu2.and.ta.le.acu2) then
         ydel(iplane)=1.
         xdel(iplane)=0.
         altc(iplane)=palt
         endif
         if ( tr.le.acu2.and.ta.gt.acu2) then
         ydel(iplane)=0.
         xdel(iplane)=1.*piz
         altc(iplane)=palt
         endif
      endif
c
c     print *,ha,hr,palt,tamoy,trmoy,tap,trp
c     do i=0,nt
c     print *,i,h(i),xdel(i),ydel(i),altc(i)
c     enddo
c
      aaaa=delta/(2-delta)
      ron=(1-aaaa)/(1+2*aaaa)
c
c     rayleigh phase function
c
      beta0=1.
      beta2=0.5*ron
c
c    primary scattering
c
      ig=1
      tavion0=0.
      tavion1=0.
      tavion2=0.
      tavion=0.
      do 16 j=-mu,mu
      i3(j)=0.
   16 continue
c
c     kernel computations
c
      call kernel(0,mu,rm,xpl,psl,bp)
      do 100 j=-mu,mu
      do 101 k=0,nt
      i2(k,j)=0.0000
  101 continue
  100 continue
c
c     vertical integration, primary upward radiation
c
 
      do 108 k=1,mu
      i1(nt,k)=1.0
      zi1=i1(nt,k)
      yy=rm(k)
      do 108 i=nt-1,0,-1
      i1(i,k)=exp(-(ta+tr-h(i))/yy)
  108 continue
c
c     vertical integration, primary downward radiation
c
      do 109 k=-mu,-1
      do 109 i=0,nt
      i1(i,k)=0.00
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
      tavion=i1(iplane,mu)
      tavion2=i1(iplane,mu)
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
c
      do455 k=1,mu
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
      ii2=ii2+z*(xi1*bpjk+xi2*bpjmk)
      ii1=ii1+z*(xi1*bpjmk+xi2*bpjk)
 477  continue
      i2(i,k)=ii2
      i2(i,-k)=ii1
 455  continue
c
c     vertical integration, upward radiation
c
      do 48 k=1,mu
      i1(nt,k)=0.0
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
      tavion0=i1(iplane,mu)
c
c   convergence test (geometrical serie)
c
      if(ig.gt.2) then
      z=0.
      a1=tavion2
      d1=tavion1
      g1=tavion0
      if (a1.ge.acu.and.d1.ge.acu.and.tavion.ge.acu) then
         y=((g1/d1-d1/a1)/((1.-g1/d1)**2)*(g1/tavion))
         y=abs(y)
         z=max(y,z)
      endif
      do 99 l=-mu,mu
      if (l.eq.0) goto 99
      a1=inm2(l)
      d1=inm1(l)
      g1=in(l)
      if(a1.eq.0.) go to 99
      if(d1.eq.0.) go to 99
      if(i3(l).eq.0.) go to 99
      y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/i3(l)))
      y=abs(y)
      z=max(y,z)
  99  continue
      if(z.lt.0.0001) then
c
c     successful test (geometrical serie)
c
      do 606 l=-mu,mu
      if (l.eq.0) goto 606
      y1=1.
      d1=inm1(l)
      g1=in(l)
      if(d1.eq.0.0) go to 606
      y1=1-g1/d1
      g1=g1/y1
      i3(l)=i3(l)+g1
  606 continue
      d1=tavion1
      g1=tavion0
      y1=1.
      if (d1.ge.acu) then
      if (abs(g1-d1).ge.acu) then
         y1=1.-g1/d1
         g1=g1/y1
      endif
      tavion=tavion+g1
      endif
      go to 505
      endif
c
c     inm2 is the (n-2)ieme scattering order
c
      do 26 k=-mu,mu
      inm2(k)=inm1(k)
   26 continue
      tavion2=tavion1
      endif
c
c     inm1 is the (n-1)ieme scattering order
c
      do 27 k=-mu,mu
      inm1(k)=in(k)
   27 continue
      tavion1=tavion0
c
c     sum of the n-1 orders
c
      do 610 l=-mu,mu
      i3(l)=i3(l)+in(l)
  610 continue
      tavion=tavion+tavion0
c
c     stop if order n is less than 1% of the sum
c
      z=0.
      do 611 l=-mu,mu
      if(i3(l).ne.0)then
      y=abs(in(l)/i3(l))
      z=max(z,y)
      endif
  611 continue
      if(z.lt.0.00001) go to 505
c
c      stop if order n is greater than 20 in any case
c
      if(ig-igmax) 503,503,505
  505 continue
c
c
      xf(1)=xf(1)+i3(mu)
      xf(-1)=tavion
      do k=1,mu
      xf(0)=xf(0)+rm(k)*gb(k)*i3(-k)
      enddo
      nt=snt
      return

      end

