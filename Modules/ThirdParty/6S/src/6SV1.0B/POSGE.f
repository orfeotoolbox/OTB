      subroutine posge  (month,jday,tu,nc,nl,
     a                   asol,phi0,avis,phiv,xlon,xlat)
      logical ier
      real tu,asol,phi0,avis,phiv,xlon,xlat,yr,xr,alti
      real re,aaa,rp,pi,cdr,crd,deltax,deltay,x,y,rs
      real tanx,tany,val1,val2,yk,cosx2,sn,zt,xt,yt
      real teta,ylat,ylon,gam
      integer month,jday,nc,nl,iwr

      common/sixs_ier/iwr,ier
 
c     goes east definition
 
      yr=float(nl)-8665.5
      xr=float(nc)-6498.5
      alti=42107.0-6378.155
      re=6378.155
      aaa=1./297.
      rp=re/(1.+aaa)
      pi=3.1415926
      cdr=pi/180.
      crd=180./pi
      deltax=18.0/12997.0
      deltay=20.0/17331.0
      x=xr*deltax*cdr
      y=yr*deltay*cdr
      rs=re+alti
      tanx=tan(x)
      tany=tan(y)
      val1=1.0+(tanx**2)
      val2=1.0+(tany*(1.0+aaa))**2
      yk=rs/re
      cosx2=1./(val1*val2)
      if((1./cosx2).gt.((yk**2)/(yk**2-1.))) goto 1000
      sn=(rs-(re*(sqrt((yk**2)-(yk**2-1.)*(1./cosx2)))))/(1./cosx2)
      zt=rs-sn
      xt=-(sn*tanx)
      yt=sn*tany/cos(x)
      teta=asin(yt/rp)
      ylat=(atan(((tan(teta))*rp)/re))
      ylon=atan(xt/zt)
      goto 1001
 1000 call print_error(
     s 'no possibility to compute lat. and long.')
      return
 1001 xlat=ylat*crd
      xlon=ylon*crd-75.
 
      call possol(month,jday,tu,xlon,xlat,
     a            asol,phi0)
      if(ier)return
 
      ylon=xlon*pi/180.+75.*cdr
      ylat=xlat*pi/180.
      gam=sqrt(((1./cosx2)-1.)*cosx2)
      avis=asin((1.+alti/re)*(gam))
      avis=avis*180./pi
      phiv=atan2(tan(ylon),sin(ylat))+pi
      phiv=phiv*180./pi
      return
      end
