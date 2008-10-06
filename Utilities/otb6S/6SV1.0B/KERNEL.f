      subroutine kernel(is,mu,rm,xpl,psl,bp)
      integer mu
      real rm(-mu:mu)
      double precision xpl(-mu:mu),bp(0:mu,-mu:mu)
      integer is,ip1,j,i,k,ip,ig,l,lp,lm,ij
      double precision xdb,a,b,c,xx,rac3,x,bt,sbp

c - to vary the number of quadratures
      include "paramdef.inc"
      integer nquad
      common /num_quad/ nquad
      real pha,qha,uha,alphal,betal,gammal,zetal
      common /sixs_polar/ pha(nqmax_p),qha(nqmax_p),uha(nqmax_p),
     &alphal(0:nqmax_p),betal(0:nqmax_p),gammal(0:nqmax_p),
     &zetal(0:nqmax_p)
      double precision psl(-1:nqmax_p,-mu:mu)
c - to vary the number of quadratures

      ip1=nquad-3
      rac3=dsqrt(3.D+00)
      if(is.ne.0)go to 700
      do 25 j=0,mu
        c=dble(rm(j))
        psl(0,-j)=1.
        psl(0,j)=1.
        psl(1,j)=c
        psl(1,-j)=-c
        xdb=(3.*c*c-1.)*0.5
        if (abs(xdb).lt.1.E-30) xdb=0.0
        psl(2,-j)=xdb
        psl(2,j)=xdb
   25 continue
      psl(1,0)=rm(0)
      goto 501
c
  700 if(is.ne.1)go to 701
      do 26 j=0,mu
        c=dble(rm(j))
        x=1.-c*c
        psl(0,j)=0.
        psl(0,-j)=0.
        psl(1,-j)=sqrt(x*0.5)
        psl(1,j)=sqrt(x*0.5)
        psl(2,j)=c*psl(1,j)*rac3
        psl(2,-j)=-psl(2,j)
   26 continue
      psl(2,0)=-psl(2,0)
      goto 501
c
  701 a=1
      do 27 i=1,is
        x=i
        a=a*sqrt((i+is)/x)*0.5
 27   continue
      b=a*sqrt(is/(is+1.))*sqrt((is-1.)/(is+2.))
      do 28 j=0,mu
        c=dble(rm(j))
        xx=1.-c*c
        psl(is-1,j)=0.
        xdb=a*xx**(is*0.5)
        if (abs(xdb).lt.1.E-30) xdb=0.0
        psl(is,-j)=xdb
        psl(is,j)=xdb
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
        a=(2*l+1.)/sqrt((l+is+1.)*(l-is+1.))
        b=sqrt(float((l+is)*(l-is)))/(2.*l+1.)
        do 31 j=0,mu
          c=dble(rm(j))
          xdb=a*(c*psl(l,j)-b*psl(lm,j))
          if (abs(xdb).lt.1.E-30) xdb=0.
          psl(lp,j)=xdb
          if(j.eq.0) go to 31
          psl(lp,-j)=ig*psl(lp,j)
   31   continue
        ig=-ig
   30 continue
  502 continue
      do 1005 j=-mu,mu
        xpl(j)=psl(2,j)
 1005 continue
      ij=ip1
      do 32 j=0,mu
        do 32 k=-mu,mu
          sbp=0.
          if(is.gt.ij) goto 1
          do 33 l=is,ij
            bt=betal(l)
            sbp=sbp+dble(psl(l,j))*psl(l,k)*bt
  33      continue
 1        continue
          if (abs(sbp).lt.1.E-30) sbp=0.
          bp(j,k)=sbp
   32 continue
      return
      end
