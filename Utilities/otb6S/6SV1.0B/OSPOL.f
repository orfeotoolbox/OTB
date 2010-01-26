      subroutine ospol (iaer_prof,tamoy,trmoy,pizmoy,tamoyp,trmoyp,palt,
     s               phirad,nt,mu,np,rm,gb,rp,
     s                     xli,xlq,xlu,xlphim,nfi,
     s                     nfilut,filut,rolut,rolutq,rolutu)


c - to vary the number of quadratures
      include "paramdef.inc"
      integer nquad
      common /num_quad/ nquad
      real pha,qha,uha,alphal,betal,gammal,zetal
      common /sixs_polar/ pha(nqmax_p),qha(nqmax_p),uha(nqmax_p),
     &alphal(0:nqmax_p),betal(0:nqmax_p),gammal(0:nqmax_p),
     &zetal(0:nqmax_p)
c - to vary the number of quadratures


c  dimension for gauss integration
      integer mu,np,nfi
      real rm(-mu:mu),gb(-mu:mu),rp(np),xlphim(nfi)
c  dimension for os computation
      real xli(-mu:mu,np),xlq(-mu:mu,np),xlu(-mu:mu,np)
c array for sos computation
c THOMAS
c      double precision xpl(-mu:mu),xrl(-mu:mu),xtl(-mu:mu)
c      double precision bp(0:mu,-mu:mu),gr(0:mu,-mu:mu),gt(0:mu,-mu:mu)
c      double precision arr(0:mu,-mu:mu),art(0:mu,-mu:mu)
c      double precision att(0:mu,-mu:mu),altc(0:nt)
c      double precision xdel(0:nt),ydel(0:nt),ch(0:nt),h(0:nt)
c      real i1(0:nt,-mu:mu),q1(0:nt,-mu:mu)
c      real u1(0:nt,-mu:mu),i2(0:nt,-mu:mu)
c      real q2(0:nt,-mu:mu),u2(0:nt,-mu:mu)
c      real i3(-mu:mu),q3(-mu:mu),u3(-mu:mu)
c      real i4(-mu:mu),q4(-mu:mu),u4(-mu:mu)
c      real in(0:2,-mu:mu),qn(0:2,-mu:mu),un(0:2,-mu:mu)

      double precision xpl(-25:25),xrl(-25:25),xtl(-25:25)
      double precision bp(0:25,-25:25),gr(0:25,-25:25),gt(0:25,-25:25)
      double precision arr(0:25,-25:25),art(0:25,-25:25)
      double precision att(0:25,-25:25),altc(0:30)
      double precision xdel(0:30),ydel(0:30),ch(0:30),h(0:30)
      real i1(0:30,-25:25),q1(0:30,-25:25)
      real u1(0:30,-25:25),i2(0:30,-25:25)
      real q2(0:30,-25:25),u2(0:30,-25:25)
      real i3(-25:25),q3(-25:25),u3(-25:25)
      real i4(-25:25),q4(-25:25),u4(-25:25)
      real in(0:2,-25:25),qn(0:2,-25:25),un(0:2,-25:25)

      real roIavion(-1:2),roQavion(-1:2),roUavion(-1:2)
      
CCCC Begin Variable for Look up table generation      
C azimuth or scattering angle variable for LUT computation (rolut)
C azimuth table for look up table computation (filut), nb of fi in each case (nfilut)
      real    luttv,lutmuv,iscama,iscami,its,scaa
      integer nbisca
      real    rolut(mu,41),rolutq(mu,41),rolutu(mu,41)
      real    filut(mu,41)
      integer nfilut(mu)
CCCC End Variable for Look up table generation      
      
      real tamoy,trmoy,pizmoy
      real tamoyp,trmoyp,palt,phirad
      real delta,sigma
      double precision hr,ta,tr,trp
      double precision ppp2,ppp1,ca,cr,ratio
      double precision tap,piz,acu,acu2,ha,xmus,zx,yy,dd
      double precision taup,th,xt1,xt2,pi,phi,aaaa,ron,spl
      double precision sa1,sa2,sb1,sb2,sc1,sc2
      double precision beta0,beta2,gamma2,alpha2
      double precision zi1,zq1,zu1,c,f,d,y
      double precision a1,d1,g1,y1,r1,delta0s
      integer snt
      integer nt,iwr,iplane,mum1,ntp,j,it,itp,i,l,m,iborm
      integer is,isp,ig,k,jj,index
      logical ier
      double precision xx,xdb,z,x,a,b
      double precision bpjk,bpjmk,gtjk,gtjmk,gtkj,gtkmj,grjk,grjmk
      double precision grkj,grkmj,arrjk,arrjmk,artjk,artjmk,artkj
      double precision artkmj,attjk,attjmk
      double precision ii1,ii2,qq1,qq2,uu1,uu2,xi1,xi2,xq1,xq2,xu1,xu2
      double precision xpj,xrj,xtj,ypj,yrj,ytj,xpk,xrk,xtk,ypk,yrk,ytk
      integer igmax,iaer_prof

      common/sixs_del/delta,sigma

      common/sixs_ier/iwr,ier

      common /multorder/ igmax

c the optical thickness above plane are recomputed to give o.t above pla
      
      snt=nt
      hr=8.0
      ta=tamoy
      tr=trmoy
      trp=trmoy-trmoyp
      tap=tamoy-tamoyp
      piz=pizmoy

      iplane=0
      acu=1.e-20
      acu2=1.e-4
      mum1=mu-1
c if plane observations recompute scale height for aerosol knowing:
c the aerosol optical depth as measure from the plane 	= tamoyp
c the rayleigh scale height = 			= hr (8km)
c the rayleigh optical depth  at plane level 		= trmoyp
c the altitude of the plane 				= palt
c the rayleigh optical depth for total atmos		= trmoy
c the aerosol optical depth for total atmos		= tamoy
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
      if((ta.le.acu2).and.(tr.gt.ta)) then
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

      if((tr.le.acu2).and.(ta.gt.tr)) then
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
c        write(6,901) j,altc(j),h(j)
        enddo
      endif
c
      if(tr.gt.acu2.and.ta.gt.acu2.and.iaer_prof.eq.0)then
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
 111      ppp2=300.0
          ppp1=0.0
          itp=it
          call discre(ta,ha,tr,hr,itp,ntp,yy,dd,ppp2,ppp1,zx)
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
  14    continue
       endif
 901    Format(i2,f10.5,f10.5,f10.5)
 

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
        if ( tr.gt.acu2.and.ta.gt.acu2) then
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
c
c     print *,ha,hr,palt,ta,tr,tap,trp
c     do i=0,nt
c       print *,i,h(i),ch(i),xdel(i),ydel(i),altc(i)
c     enddo
c
      pi=acos(-1.)
      phi=phirad
      do 716 l=1,np
        do 716 m=-mu,mu
	  xli(m,l)=0.
	  xlq(m,l)=0.
	  xlu(m,l)=0.
 716  continue
      do ifi=1,nfi
      xlphim(ifi)=0.
      enddo
      
CCC initialization of look up table variable
      do i=1,mu
      do j=1,41
      rolut(i,j)=0.
      rolutq(i,j)=0.
      rolutu(i,j)=0.
      enddo
      enddo
      
c
c     ************ incident angle mus *******
c
c
      aaaa=delta/(2-delta)
      ron=(1-aaaa)/(1+2*aaaa)
C  TEST FOR COMPARING TO SIMPLE APPROXIMATION
C     WRITE(6,*) "WARNING DEPOLARIZATION=0"
C     ron=1.0
c
c     rayleigh phase function
c
      beta0=1.
      beta2=0.5*ron
      gamma2=-ron*sqrt(1.5)
      alpha2=3.*ron
c
c     fourier decomposition
c
      do 17 j=-mu,mu
        i4(j)=0.
        q4(j)=0.
        u4(j)=0.
   17 continue
      iborm=nquad
      if (ta.le.acu2) iborm=2
      if( abs (xmus-1.000000) .lt.1.e-06)iborm=0
 
      do 24 is=0,iborm
c
c    primary scattering
c
        ig=1
        do k=0,3
          roIavion(k-1)=0.
          roQavion(k-1)=0.
          roUavion(k-1)=0.
        enddo
        do 16 j=-mu,mu
          i3(j)=0.
          q3(j)=0.
          u3(j)=0.
   16   continue
c
c     kernel computations + mixture rayleigh(1)-aerosol(2)
c
        isp=is
        call kernelpol(isp,mu,rm,xpl,xrl,xtl,bp,gr,gt,arr,art,att)
        if(is.gt.0)beta0=0.
        do 100 j=-mu,mu
          if(is-2)200,200,201
 200      spl=xpl(0)
          sa1=beta0+beta2*xpl(j)*spl
          sa2=bp(0,j)
          sb1=gamma2*xrl(j)*spl
          sb2=gr(0,j)
          sc1=gamma2*xtl(j)*spl
          sc2=gt(0,j)
          goto 202
 201      sa2=bp(0,j)
          sa1=0.
          sb2=gr(0,j)
          sb1=0.
          sc2=gt(0,j)
          sc1=0.
c
c     primary scattering source function at every level within the layer
c
 202      do 101 k=0,nt
            c=ch(k)
            a=ydel(k)
            b=xdel(k)
            i2(k,j)=c*(sa2*b+sa1*a)
            q2(k,j)=c*(sb2*b+sb1*a)
            u2(k,j)=-c*(sc2*b+sc1*a)
  101     continue
  100   continue
c
c     vertical integration, primary upward radiation
c
 
        do 108 k=1,mu
           i1(nt,k)=0.
           q1(nt,k)=0.
           u1(nt,k)=0.
           zi1=i1(nt,k)
           zq1=q1(nt,k)
           zu1=u1(nt,k)
           yy=rm(k)
           do 108 i=nt-1,0,-1
              jj=i+1
              f=h(jj)-h(i)
              c=exp(-f/yy)
              d=1.0e+00-c
              xx=h(i)-h(jj)*c

              a=(i2(jj,k)-i2(i,k))/f
              b=i2(i,k)-a*h(i)
              zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00
              i1(i,k)=zi1
  
              a=(q2(jj,k)-q2(i,k))/f
              b=q2(i,k)-a*h(i)
              zq1=c*zq1+(d*(b+a*yy)+a*xx)*0.5e+00
              q1(i,k)=zq1

              a=(u2(jj,k)-u2(i,k))/f
              b=u2(i,k)-a*h(i)
              zu1=c*zu1+(d*(b+a*yy)+a*xx)*0.5e+00
              u1(i,k)=zu1
  108   continue
c
c     vertical integration, primary downward radiation
c
        do 109 k=-mu,-1
          i1(0,k)=0.
          q1(0,k)=0.
          u1(0,k)=0.
          zi1=i1(0,k)
          zq1=q1(0,k)
          zu1=u1(0,k)
          yy=rm(k)
          do 109 i=1,nt
            jj=i-1
            f=h(i)-h(jj)
            c=exp(f/yy)
            d=1.0e+00-c
            xx=h(i)-h(jj)*c
 
            a=(i2(i,k)-i2(jj,k))/f
            b=i2(i,k)-a*h(i)
            zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00
            i1(i,k)=zi1
 
            a=(q2(i,k)-q2(jj,k))/f
            b=q2(i,k)-a*h(i)
            zq1=c*zq1+(d*(b+a*yy)+a*xx)*0.5e+00
            q1(i,k)=zq1
 
            a=(u2(i,k)-u2(jj,k))/f
            b=u2(i,k)-a*h(i)
            zu1=c*zu1+(d*(b+a*yy)+a*xx)*0.5e+00
            u1(i,k)=zu1
  109   continue
c
c     in(2,?) is inialized with scattering computed at n-2
c     i3 is inialized with primary scattering
c
        do 20 k=-mu,mu
          if(k) 21,20,23
   21     index=nt
          go to 25
   23     index=0
   25     continue
          in(1,k)=i1(index,k)
          in(2,k)=i1(index,k)
          i3(k)=i1(index,k)
          qn(1,k)=q1(index,k)
          qn(2,k)=q1(index,k)
          q3(k)=q1(index,k)
          un(1,k)=u1(index,k)
          un(2,k)=u1(index,k)
          u3(k)=u1(index,k)
   20   continue
        roIavion(2)=i1(iplane,mu)
        roIavion(-1)=i1(iplane,mu)
        roQavion(2)=q1(iplane,mu)
        roQavion(-1)=q1(iplane,mu)
        roUavion(2)=u1(iplane,mu)
        roUavion(-1)=u1(iplane,mu)
c
c     loop on successive order
c
  503   ig=ig+1
C       WRITE(6,*) "WARNING ONLY PRIMARY SCATTERING"
C       goto 505
c
c     successive orders
c
c     multiple scattering source function at every level within the laye
c
c     if is < ou = 2 kernels are a mixing of aerosols and molecules kern
c     if is >2 aerosols kernels only
c
        if(is-2)210,210,211
  210   do455 k=1,mu
          xpk=xpl(k)
          xrk=xrl(k)
          xtk=xtl(k)
          ypk=xpl(-k)
          yrk=xrl(-k)
          ytk=xtl(-k)
          do 455 i=0,nt
            ii1=0.
            ii2=0.
            qq1=0.
            qq2=0.
            uu1=0.
            uu2=0.
            x=xdel(i)
            y=ydel(i)
            do477 j=1,mu
              z=gb(j)
              xpj=xpl(j)
              xrj=xrl(j)
              xtj=xtl(j)
              ypj=xpl(-j)
              yrj=xrl(-j)
              ytj=xtl(-j)
              xi1=i1(i,j)
              xi2=i1(i,-j)
              xq1=q1(i,j)
              xq2=q1(i,-j)
              xu1=u1(i,j)
              xu2=u1(i,-j)

              bpjk=bp(j,k)*x+y*(beta0+beta2*xpj*xpk)
              bpjmk=bp(j,-k)*x+y*(beta0+beta2*xpj*ypk)
              gtjk=gt(j,k)*x+y*gamma2*xpj*xtk
              gtjmk=gt(j,-k)*x+y*gamma2*xpj*ytk
              gtkj=gt(k,j)*x+y*gamma2*xpk*xtj
              gtkmj=gt(k,-j)*x+y*gamma2*xpk*ytj
              grjk=gr(j,k)*x+y*gamma2*xpj*xrk
              grjmk=gr(j,-k)*x+y*gamma2*xpj*yrk
              grkj=gr(k,j)*x+y*gamma2*xpk*xrj
              grkmj=gr(k,-j)*x+y*gamma2*xpk*yrj

              arrjk=arr(j,k)*x+y*alpha2*xrj*xrk
              arrjmk=arr(j,-k)*x+y*alpha2*xrj*yrk
              artjk=art(j,k)*x+y*alpha2*xtj*xrk
              artjmk=art(j,-k)*x+y*alpha2*xtj*yrk
              artkj=art(k,j)*x+y*alpha2*xtk*xrj
              artkmj=art(k,-j)*x+y*alpha2*xtk*yrj
              attjk=att(j,k)*x+y*alpha2*xtj*xtk
              attjmk=att(j,-k)*x+y*alpha2*xtj*ytk

              xdb=xi1*bpjk+xi2*bpjmk+xq1*grkj+xq2*grkmj
              xdb=xdb-xu1*gtkj-xu2*gtkmj
              ii2=ii2+xdb*z
              xdb=xi1*bpjmk+xi2*bpjk+xq1*grkmj+xq2*grkj
	      xdb=xdb+xu1*gtkmj+xu2*gtkj
              ii1=ii1+xdb*z
              xdb=xi1*grjk+xi2*grjmk+xq1*arrjk+xq2*arrjmk
              xdb=xdb-xu1*artjk+xu2*artjmk
	      qq2=qq2+xdb*z
	      xdb=xi1*grjmk+xi2*grjk+xq1*arrjmk+xq2*arrjk
	      xdb=xdb-xu1*artjmk+xu2*artjk
	      qq1=qq1+xdb*z
              xdb=xi1*gtjk-xi2*gtjmk+xq1*artkj+xq2*artkmj
              xdb=xdb-xu1*attjk-xu2*attjmk
	      uu2=uu2-xdb*z
              xdb=xi1*gtjmk-xi2*gtjk-xq1*artkmj-xq2*artkj
              xdb=xdb-xu1*attjmk-xu2*attjk
	      uu1=uu1-xdb*z
 477        continue
            if (abs(ii2).lt.1.E-30) ii2=0.
            if (abs(ii1).lt.1.E-30) ii1=0.
            if (abs(qq2).lt.1.E-30) qq2=0.
            if (abs(qq1).lt.1.E-30) qq1=0.
            if (abs(uu2).lt.1.E-30) uu2=0.
            if (abs(uu1).lt.1.E-30) uu1=0.
            i2(i,k)=ii2
            i2(i,-k)=ii1
            q2(i,k)=qq2
            q2(i,-k)=qq1
            u2(i,k)=uu2
            u2(i,-k)=uu1
 455    continue
        goto 213


 211    do45 k=1,mu
          do 45 i=0,nt
            ii1=0.
            ii2=0.
            qq1=0.
            qq2=0.
            uu1=0.
            uu2=0.
            x=xdel(i)
            do47 j=1,mu
              z=gb(j)
              xi1=i1(i,j)
              xi2=i1(i,-j)
              xq1=q1(i,j)
              xq2=q1(i,-j)
              xu1=u1(i,j)
              xu2=u1(i,-j)

              bpjk=bp(j,k)*x
              bpjmk=bp(j,-k)*x
              gtjk=gt(j,k)*x
              gtjmk=gt(j,-k)*x
              gtkj=gt(k,j)*x
              gtkmj=gt(k,-j)*x
              grjk=gr(j,k)*x
              grjmk=gr(j,-k)*x
              grkj=gr(k,j)*x
              grkmj=gr(k,-j)*x
	      arrjk=arr(j,k)*x
	      arrjmk=arr(j,-k)*x
	      artjk=art(j,k)*x
	      artjmk=art(j,-k)*x
	      artkj=art(k,j)*x
	      artkmj=art(k,-j)*x
	      attjk=att(j,k)*x
	      attjmk=att(j,-k)*x

              xdb=xi1*bpjk+xi2*bpjmk+xq1*grkj+xq2*grkmj
              xdb=xdb-xu1*gtkj-xu2*gtkmj
              ii2=ii2+xdb*z
              xdb=xi1*bpjmk+xi2*bpjk+xq1*grkmj+xq2*grkj
	      xdb=xdb+xu1*gtkmj+xu2*gtkj
              ii1=ii1+xdb*z
              xdb=xi1*grjk+xi2*grjmk+xq1*arrjk+xq2*arrjmk
              xdb=xdb-xu1*artjk+xu2*artjmk
	      qq2=qq2+xdb*z
	      xdb=xi1*grjmk+xi2*grjk+xq1*arrjmk+xq2*arrjk
	      xdb=xdb-xu1*artjmk+xu2*artjk
	      qq1=qq1+xdb*z
              xdb=xi1*gtjk-xi2*gtjmk+xq1*artkj+xq2*artkmj
              xdb=xdb-xu1*attjk-xu2*attjmk
	      uu2=uu2-xdb*z
              xdb=xi1*gtjmk-xi2*gtjk-xq1*artkmj-xq2*artkj
              xdb=xdb-xu1*attjmk-xu2*attjk
	      uu1=uu1-xdb*z
   47       continue
            if (abs(ii2).lt.1.E-30) ii2=0.
            if (abs(ii1).lt.1.E-30) ii1=0.
            if (abs(qq2).lt.1.E-30) qq2=0.
            if (abs(qq1).lt.1.E-30) qq1=0.
            if (abs(uu2).lt.1.E-30) uu2=0.
            if (abs(uu1).lt.1.E-30) uu1=0.
            i2(i,k)=ii2
            i2(i,-k)=ii1
            q2(i,k)=qq2
            q2(i,-k)=qq1
            u2(i,k)=uu2
            u2(i,-k)=uu1
   45   continue
c
c     vertical integration, upward radiation
c
 213    do 48 k=1,mu
          i1(nt,k)=0.
          q1(nt,k)=0.
          u1(nt,k)=0.
          zi1=i1(nt,k)
          zq1=q1(nt,k)
          zu1=u1(nt,k)
          yy=rm(k)
          do 48 i=nt-1,0,-1
            jj=i+1
            f=h(jj)-h(i)
            c=exp(-f/yy)
            d=1.e+00-c
            xx=h(i)-h(jj)*c

            a=(i2(jj,k)-i2(i,k))/f
            b=i2(i,k)-a*h(i)
            zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00
            if (abs(zi1).le.1.E-20) zi1=0.
            i1(i,k)=zi1

            a=(q2(jj,k)-q2(i,k))/f
            b=q2(i,k)-a*h(i)
            zq1=c*zq1+(d*(b+a*yy)+a*xx)*0.5e+00
            if (abs(zq1).le.1.E-20) zq1=0.
            q1(i,k)=zq1

            a=(u2(jj,k)-u2(i,k))/f
            b=u2(i,k)-a*h(i)
            zu1=c*zu1+(d*(b+a*yy)+a*xx)*0.5e+00
            if (abs(zu1).le.1.E-20) zu1=0.
            u1(i,k)=zu1
   48   continue
c
c     vertical integration, downward radiation
c
        do 50 k=-mu,-1
          i1(0,k)=0.
          q1(0,k)=0.
          u1(0,k)=0.
          zi1=i1(0,k)
          zq1=q1(0,k)
          zu1=u1(0,k)
          yy=rm(k)
          do 50 i=1,nt
            jj=i-1
            f=h(i)-h(jj)
            c=exp(f/yy)
            d=1.e+00-c
            xx=h(i)-h(jj)*c

            a=(i2(i,k)-i2(jj,k))/f
            b=i2(i,k)-a*h(i)
            zi1=c*zi1+(d*(b+a*yy)+a*xx)*0.5e+00
            if (abs(zi1).le.1.E-20) zi1=0.
            i1(i,k)=zi1

            a=(q2(i,k)-q2(jj,k))/f
            b=q2(i,k)-a*h(i)
            zq1=c*zq1+(d*(b+a*yy)+a*xx)*0.5e+00
            if (abs(zq1).le.1.E-20) zq1=0.
            q1(i,k)=zq1

            a=(u2(i,k)-u2(jj,k))/f
            b=u2(i,k)-a*h(i)
            zu1=c*zu1+(d*(b+a*yy)+a*xx)*0.5e+00
            if (abs(zu1).le.1.E-20) zu1=0.
            u1(i,k)=zu1
   50   continue
c
c     in(0,?) is the nieme scattering order
c
        do 30 k=-mu,mu
          if(k) 31,30,33
   31     index=nt
          go to 34
   33     index=0
   34     continue
          in(0,k)=i1(index,k)
          qn(0,k)=q1(index,k)
          un(0,k)=u1(index,k)
   30   continue
        roIavion(0)=i1(iplane,mu)
        roQavion(0)=Q1(iplane,mu)
        roUavion(0)=U1(iplane,mu)
c
c   convergence test (geometrical serie)
c
        if(ig.gt.2) then
          z=0.
            a1=abs(roIavion(2))
            d1=abs(roIavion(1))
            g1=abs(roIavion(0))
	    r1=abs(roIavion(-1))
            if(a1.ge.acu.and.d1.ge.acu.and.r1.ge.acu) then
              a1=roIavion(2)
              d1=roIavion(1)
              g1=roIavion(0)
	      r1=roIavion(-1)
              y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/r1))
              y=abs(y)
              z=dmax1(dble(y),z)
            endif
            a1=abs(roQavion(2))
            d1=abs(roQavion(1))
            g1=abs(roQavion(0))
            r1=abs(roQavion(-1))
            if(a1.ge.acu.and.d1.ge.acu.and.r1.ge.acu) then
              a1=roQavion(2)
              d1=roQavion(1)
              g1=roQavion(0)
              r1=roQavion(-1)
              y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/r1))
              y=abs(y)
              z=dmax1(dble(y),z)
            endif
            a1=abs(roUavion(2))
            d1=abs(roUavion(1))
            g1=abs(roUavion(0))
            r1=abs(roUavion(-1))
            if(a1.ge.acu.and.d1.ge.acu.and.r1.ge.acu) then
              a1=roUavion(2)
              d1=roUavion(1)
              g1=roUavion(0)
              r1=roUavion(-1)
              y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/r1))
              y=abs(y)
              z=dmax1(dble(y),z)
            endif
            do 99 l=-mu,mu
              if (l.eq.0) goto 99
              a1=in(2,l)
              d1=in(1,l)
              g1=in(0,l)
              if(abs(a1).le.acu) go to 98
              if(abs(d1).le.acu) go to 98
              if(abs(i3(l)).le.acu) go to 98
              y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/i3(l)))
              y=abs(y)
              z=dmax1(dble(y),z)
 98           a1=qn(2,l)
              d1=qn(1,l)
              g1=qn(0,l)
              if(abs(a1).le.acu) go to 97
              if(abs(d1).le.acu) go to 97
              if(abs(q3(l)).le.acu) go to 97
              y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/q3(l)))
              y=abs(y)
              z=dmax1(dble(y),z)
 97           a1=un(2,l)
              d1=un(1,l)
              g1=un(0,l)
              if(abs(a1).le.acu) go to 99
              if(abs(d1).le.acu) go to 99
              if(abs(u3(l)).le.acu) go to 99
              y=((g1/d1-d1/a1)/((1-g1/d1)**2)*(g1/u3(l)))
              y=abs(y)
              z=dmax1(dble(y),z)
  99        continue

          if(z.lt.0.01) then
c
c     successful test (geometrical serie)
            do 606 l=-mu,mu
              y1=1.
              d1=in(1,l)
              if(abs(d1).le.acu) go to 605
              g1=in(0,l)
              if(abs(g1-d1).le.acu) go to 605
              y1=1-g1/d1
              g1=g1/y1
              i3(l)=i3(l)+g1
 605          y1=1.
              d1=qn(1,l)
              if(abs(d1).le.acu) go to 604
              g1=qn(0,l)
              if(abs(g1-d1).le.acu) go to 604
              y1=1-g1/d1
              g1=g1/y1
              q3(l)=q3(l)+g1
 604          y1=1.
              d1=un(1,l)
              if(abs(d1).le.acu) go to 606	
              g1=un(0,l)
              if(abs(g1-d1).le.acu) go to 606
              y1=1-g1/d1
              g1=g1/y1
              u3(l)=u3(l)+g1
  606       continue
            y1=1.
            d1=roIavion(1)
            if(abs(d1).ge.acu) then
              g1=roIavion(0)
              if(abs(g1-d1).ge.acu) then
                y1=1-g1/d1
                g1=g1/y1
              endif
              roIavion(-1)=roIavion(-1)+g1
            endif
            y1=1.
            d1=roQavion(1)
            if(abs(d1).ge.acu) then
              g1=roQavion(0)
              if(abs(g1-d1).ge.acu) then
                y1=1-g1/d1
                g1=g1/y1
              endif
              roQavion(-1)=roQavion(-1)+g1
            endif
            y1=1.
            d1=roUavion(1)
            if(abs(d1).ge.acu) then
              g1=roUavion(0)
              if(abs(g1-d1).ge.acu) then
                y1=1-g1/d1
                g1=g1/y1
              endif
              roUavion(-1)=roUavion(-1)+g1
            endif
            go to 505
          endif
c
c     in(2,?) is the (n-2)ieme scattering order
c
          do 26 k=-mu,mu
            in(2,k)=in(1,k)
            qn(2,k)=qn(1,k)
            un(2,k)=un(1,k)
   26     continue
          roIavion(2)=roIavion(1)
          roQavion(2)=roQavion(1)
          roUavion(2)=roUavion(1)
        endif
c
c     in(1,?) is the (n-1)ieme scattering order
c
        do 27 k=-mu,mu
          in(1,k)=in(0,k)
          qn(1,k)=qn(0,k)
          un(1,k)=un(0,k)
   27   continue
        roIavion(1)=roIavion(0)
        roQavion(1)=roQavion(0)
        roUavion(1)=roUavion(0)
c
c     sum of the n-1 orders
        do 610 l=-mu,mu
          i3(l)=i3(l)+in(0,l)
          q3(l)=q3(l)+qn(0,l)
          u3(l)=u3(l)+un(0,l)
  610   continue
        roIavion(-1)=roIavion(-1)+roIavion(0)
        roQavion(-1)=roQavion(-1)+roQavion(0)
        roUavion(-1)=roUavion(-1)+roUavion(0)
c
c     stop if order n is less than 1% of the sum
          z=0.
          do 612 l=-mu,mu
            if (abs(i3(l)).ge.acu) then
              y=abs(in(0,l)/i3(l))
              z=dmax1(z,dble(y))
            endif
            if (abs(q3(l)).ge.acu) then
              y=abs(qn(0,l)/q3(l))
              z=dmax1(z,dble(y))
            endif
            if (abs(u3(l)).ge.acu) then
              y=abs(un(0,l)/u3(l))
              z=dmax1(z,dble(y))
            endif
  612     continue
c       if(z.lt.0.00001) go to 505   ###originally
c       if(z.lt.0.001) go to 505     ###6sV4.0 choice
c        if(z.lt.0.01) go to 505 
       if(z.lt.0.00000001) go to 505 ! - like in a modified file 

c
c      stop if order n is greater than 20 in any case
c       WRITE(6,*) "WRITE WARNING PRIMARY SCATTERING"
c       goto 505
        if(ig-igmax) 503,503,505
  505   continue
c
c     sum of the fourier component s
c
        delta0s=1
        if(is.ne.0) delta0s=2
        do 613 l=-mu,mu
          i4(l)=i4(l)+abs(delta0s*i3(l))
          q4(l)=q4(l)+abs(q3(l))
          u4(l)=u4(l)+abs(u3(l))
  613   continue
c
c     stop of the fourier decomposition
c

        do 614 l=1,np
          phi=rp(l)
          do 614 m=-mum1,mum1
            if(m.gt.0) then
              xli(m,l)=xli(m,l)+delta0s*i3(m)*cos(is*(phi+pi))
              xlq(m,l)=xlq(m,l)+delta0s*q3(m)*cos(is*(phi+pi))
              xlu(m,l)=xlu(m,l)+delta0s*u3(m)*sin(is*(phi+pi))
            else
              xli(m,l)=xli(m,l)+delta0s*i3(m)*cos(is*phi)
              xlq(m,l)=xlq(m,l)+delta0s*q3(m)*cos(is*phi)
              xlu(m,l)=xlu(m,l)+delta0s*u3(m)*sin(is*phi)
            endif

 614    continue
 
 
C Look up table generation 
      do m=1,mu
      do l=1,nfilut(m)
      phimul=filut(m,l)*pi/180.
      rolut(m,l)=rolut(m,l)+delta0s*i3(m)*cos(is*(phimul+pi))
      rolutq(m,l)=rolutq(m,l)+delta0s*q3(m)*cos(is*(phimul+pi))
      rolutu(m,l)=rolutu(m,l)+delta0s*u3(m)*sin(is*(phimul+pi))
      enddo
      enddo
C end of look up table generation 
 
 
 
 
 
        if(is.eq.0) then
          do k=1,mum1
            xli(0,1)=xli(0,1)+rm(k)*gb(k)*i3(-k)
            xlq(0,1)=xlq(0,1)+rm(k)*gb(k)*q3(-k)
            xlu(0,1)=xlu(0,1)+rm(k)*gb(k)*u3(-k)
          enddo
        endif
        xli(mu,1)=xli(mu,1)+delta0s*i3(mu)*cos(is*(phirad+pi))
        xlq(mu,1)=xlq(mu,1)+delta0s*q3(mu)*cos(is*(phirad+pi))
        xlu(mu,1)=xlu(mu,1)+delta0s*u3(mu)*sin(is*(phirad+pi))
        xli(-mu,1)=xli(-mu,1)+delta0s*roIavion(-1)*cos(is*(phirad+pi))
        xlq(-mu,1)=xlq(-mu,1)+delta0s*roQavion(-1)*cos(is*(phirad+pi))
        xlu(-mu,1)=xlu(-mu,1)+delta0s*roUavion(-1)*sin(is*(phirad+pi))
        do ifi=1,nfi
        phimul=(ifi-1)*pi/(nfi-1)
        xlphim(ifi)=xlphim(ifi)+delta0s*roIavion(-1)*cos(is*(phimul+pi))
        enddo
	
          z=0.
          do 616 l=-mu,mu
            if (abs(i4(l)).lt.acu) goto 617
            x=abs(delta0s*i3(l)/i4(l))
            z=dmax1(z,x)
 617        if (abs(q4(l)).lt.acu) goto 618
            x=abs(q3(l)/q4(l))
            z=dmax1(z,x)
 618        if (abs(u4(l)).lt.acu) goto 616
            x=abs(u3(l)/u4(l))
            z=dmax1(z,x)
  616     continue
c       if(z.gt.0.0001) go to 24     #### originally
c       if(z.gt.0.001) go to 24    #### 6s choice

         if(z.gt.0.0005) go to 24 
       goto 243 

   24 continue
  243 continue

      nt=snt
      return
      end
