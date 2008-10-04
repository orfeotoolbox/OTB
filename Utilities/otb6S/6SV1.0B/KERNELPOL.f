      subroutine kernelpol(is,mu,rm,xpl,xrl,xtl,bp,gr,gt,arr,art,att)

c - to vary the number of quadratures
      include "paramdef.inc"
      integer nquad
      common /num_quad/ nquad
      real pha,qha,uha,alphal,betal,gammal,zetal
      common /sixs_polar/ pha(nqmax_p),qha(nqmax_p),uha(nqmax_p),
     &alphal(0:nqmax_p),betal(0:nqmax_p),gammal(0:nqmax_p),
     &zetal(0:nqmax_p)
c THOMAS
c      double precision psl(-1:nqmax_p,-mu:mu),rsl(-1:nqmax_p,-mu:mu)
c      double precision tsl(-1:nqmax_p,-mu:mu)
      double precision psl(-1:nqmax_p,-25:25),rsl(-1:nqmax_p,-25:25)
      double precision tsl(-1:nqmax_p,-25:25)
c - to vary the number of quadratures

      integer mu
      real rm(-mu:mu)
      double precision xpl(-mu:mu),xrl(-mu:mu),xtl(-mu:mu)
      double precision bp(0:mu,-mu:mu),gr(0:mu,-mu:mu),gt(0:mu,-mu:mu)
      double precision arr(0:mu,-mu:mu),art(0:mu,-mu:mu)
      double precision att(0:mu,-mu:mu)
      integer is,ip1,j,i,k,ip,ig,l,lp,lm,ij
      double precision xdb,a,b,c,d,e,f,xx,rac3,x
      double precision sbp,satt,sarr,sgr,sgt,sart
      double precision r1,r2,r3



      ip1=nquad-3
      rac3=dsqrt(3.D+00)
      if(is.ne.0)go to 700
      do 25 j=0,mu
        c=dble(rm(j))
        psl(0,j)=1.D+00
        psl(0,-j)=1.D+00
        psl(1,j)=c
        psl(1,-j)=-c
        xdb=(3.D+00*c*c-1.D+00)*0.5D+00
	if (abs(xdb).lt.1.e-30) xdb =0.0D+00
        psl(2,j)=xdb
        psl(2,-j)=xdb
        rsl(1,j)=0.0D+00
        rsl(1,-j)=0.0D+00
        xdb=3.D+00*(1.D+00-c*c)/2.D+00/sqrt(6.D+00)
	if (abs(xdb).lt.1.e-30) xdb =0.0D+00
        rsl(2,j)=xdb
        rsl(2,-j)=xdb
        tsl(1,j)=0.0D+00
        tsl(1,-j)=0.0D+00
        tsl(2,j)=0.0D+00
        tsl(2,-j)=0.0D+00
   25 continue
      psl(1,0)=rm(0)
      rsl(1,0)=0.0D+00
      goto 501
c
  700 if(is.ne.1)go to 701
      do 26 j=0,mu
        c=dble(rm(j))
        x=1.d+00-c*c
        psl(0,j)=0.D+00
        psl(0,-j)=0.D+00
        psl(1,j)=dsqrt(x*0.5D+00)
        psl(1,-j)=dsqrt(x*0.5d+00)
        psl(2,j)=c*psl(1,j)*rac3
        psl(2,-j)=-psl(2,j)
        rsl(1,j)=0.0d+00
        rsl(1,-j)=0.0d+00
        rsl(2,j)=-c*dsqrt(x)*0.5d+00
        rsl(2,-j)=-rsl(2,j)
        tsl(1,j)=0.0d+00
        tsl(1,-j)=0.0d+00
        tsl(2,j)=-dsqrt(x)*0.5d+00
        tsl(2,-j)=-dsqrt(x)*0.5d+00
   26 continue
      psl(2,0)=-psl(2,0)
      rsl(2,0)=-rsl(2,0)
      rsl(1,0)=0.0d+00
      tsl(1,0)=0.0d+00
      goto 501
c
  701 a=1.0d+00
      do 27 i=1,is
        x=i
        a=a*dsqrt((i+is)/x)*0.5d+00
 27   continue
      b=a*dsqrt(is/(is+1.d+00))*dsqrt((is-1.d+00)/(is+2.d+00))
      do 28 j=0,mu
        c=dble(rm(j))
        xx=1.d+00-c*c
        psl(is-1,j)=0.d+00
        rsl(is-1,j)=0.d+00
        tsl(is-1,j)=0.d+00
        xdb=a*xx**(is*0.5d+00)
	if (abs(xdb).lt.1.e-30) xdb =0.0d+00
        psl(is,-j)=xdb
        psl(is,j)=xdb
        xdb=b*(1.+c*c)*xx**(is*0.5-1.d+00)
	if (abs(xdb).lt.1.e-30) xdb =0.0d+00
        rsl(is,-j)=xdb
        rsl(is,j)=xdb
        xdb=2.d+00*b*c*xx**(is*0.5-1.d+00)
	if (abs(xdb).lt.1.e-30) xdb =0.0d+00
        tsl(is,-j)=-xdb
        tsl(is,j)=xdb
   28 continue

  501 k=2
      ip=ip1
      if(is.gt.2)k=is
      if(k.eq.ip)goto 502
      ig=-1
      if(is.eq.1)ig=1
      do 30 l=k,ip-1
        lp=l+1
        lm=l-1
        a=(2*l+1.d+00)/sqrt((l+is+1.d+00)*(l-is+1.d+00))
        b=dsqrt(1.d+00*(l+is)*(l-is))/(2.*l+1.d+00)
        d=(l+1.d+00)*(2*l+1.d+00)
  	d=d/dsqrt((l+3.d+00)*(l-1)*(l+is+1.d+00)*(l-is+1.))
	e=dsqrt((l+2.d+00)*(l-2.)*(l+is)*(l-is))/(l*(2.*l+1.))
	f=2.d+00*is/(l*(l+1.))
        do 31 j=0,mu
          c=dble(rm(j))
          xdb=a*(c*psl(l,j)-b*psl(lm,j))
	  if (abs(xdb).lt.1.e-30) xdb =0.0
          psl(lp,j)=xdb
          xdb=d*(c*rsl(l,j)-f*tsl(l,j)-e*rsl(lm,j))
	  if (abs(xdb).lt.1.e-30) xdb =0.0
          rsl(lp,j)=xdb
          xdb=d*(c*tsl(l,j)-f*rsl(l,j)-e*tsl(lm,j))
	  if (abs(xdb).lt.1.e-30) xdb =0.0
          tsl(lp,j)=xdb
          if(j.eq.0) go to 31
          psl(lp,-j)=ig*psl(lp,j)
          rsl(lp,-j)=ig*rsl(lp,j)
          tsl(lp,-j)=-ig*tsl(lp,j)
   31   continue
        ig=-ig
   30 continue
  502 continue
      do 1005 j=-mu,mu
        xpl(j)=psl(2,j)
        xrl(j)=rsl(2,j)
        xtl(j)=tsl(2,j)
c      write(6,*) "xpl,xrl,xtl ",xpl(j),xrl(j),xtl(j),j
 1005 continue
c      stop
      
      ij=ip1
      do 32 j=0,mu
        do 32 k=-mu,mu
          sbp=0.
	  sgr=0.
	  sgt=0.
	  satt=0.
	  sarr=0.
	  sart=0.
          if(is.gt.ij) goto 1
c	  write(6,*) "is,ij ",is,ij
          do 33 l=is,ij
	    r1=tsl(l,j)*tsl(l,k)
	    r2=rsl(l,j)*rsl(l,k)
	    r3=psl(l,j)*gammal(l)
            sbp=sbp+psl(l,j)*psl(l,k)*betal(l)
            sgr=sgr+rsl(l,k)*r3
            sgt=sgt+tsl(l,k)*r3
c 	  if (is.eq.3.and.j.eq.1)then
c 	      write(6,*)j,k,l,gammal(l),psl(l,j),tsl(l,k)
c 	      endif
            satt=satt+r1*alphal(l)
     &          +r2*zetal(l)
            sarr=sarr+r1*zetal(l)
     &          +r2*alphal(l)
            sart=sart+tsl(l,j)*rsl(l,k)*alphal(l)
     &          +rsl(l,j)*tsl(l,k)*zetal(l)
  33      continue
 1        continue
 	  if (abs(sbp).lt.1.e-30) sbp =0.0
          bp(j,k)=sbp
 	  if (abs(sgr).lt.1.e-30) sgr =0.0
          gr(j,k)=sgr
 	  if (abs(sgt).lt.1.e-30) sgt =0.0
          gt(j,k)=sgt
 	  if (abs(satt).lt.1.e-30) satt =0.0
          att(j,k)=satt
 	  if (abs(sart).lt.1.e-30) sart =0.0
          art(j,k)=sart
 	  if (abs(sarr).lt.1.e-30) sarr =0.0
          arr(j,k)=sarr
   32 continue
   35 continue
c      stop
      return
      end
