      subroutine presplane(uw,uo3,xpp,ftray)

      real z,p,t,wh,wo,zpl,ppl,tpl,whpl,wopl,xa,xb,xalt
      real xtemp,xwo,xwh,g,air,ro3,rt,rp,roair,ds
      integer i,isup,iinf,k
      real rmo3(34),rmwh(34)
      real ps,xpp,uo3,uw,ftray

      common /sixs_atm/z(34),p(34),t(34),wh(34),wo(34)
      common /sixs_planesim/zpl(34),ppl(34),tpl(34),whpl(34),wopl(34)

c log linear interpolation
      xpp=xpp+z(1)
      if (xpp.ge.100.) xpp=1000.
      i=0
 10   i=i+1
      if (z(i).le.xpp) goto 10
      isup=i
      iinf=i-1
      xa=(z(isup)-z(iinf))/alog(p(isup)/p(iinf))
      xb=z(isup)-xa*alog(p(isup))
      ps=exp((xpp-xb)/xa)
c interpolating temperature wator vapor and ozone profile versus altitud
      xalt=xpp
      xtemp=(t(isup)-t(iinf))/(z(isup)-z(iinf))
      xtemp=xtemp*(xalt-z(iinf))+t(iinf)
      xwo=(wo(isup)-wo(iinf))/(z(isup)-z(iinf))
      xwo=xwo*(xalt-z(iinf))+wo(iinf)
      xwh=(wh(isup)-wh(iinf))/(z(isup)-z(iinf))
      xwh=xwh*(xalt-z(iinf))+wh(iinf)
c uptading atmospheric profile
c  last level: plane     , complete to 34
c  with interpolated layers
      do i=1,iinf
        zpl(i)=z(i)
        ppl(i)=p(i)
        tpl(i)=t(i)
        whpl(i)=wh(i)
        wopl(i)=wo(i)
      enddo
      zpl(iinf+1)=xalt
      ppl(iinf+1)=ps
      tpl(iinf+1)=xtemp
      whpl(iinf+1)=xwh
      wopl(iinf+1)=xwo
      do i=iinf+2,34
        zpl(i)=zpl(iinf+1)
        ppl(i)=ppl(iinf+1)
        tpl(i)=tpl(iinf+1)
        whpl(i)=whpl(iinf+1)
        wopl(i)=wopl(iinf+1)
      enddo
c compute modified h2o and o3 integrated content
c compute conversion factor for rayleigh optical thickness computation
c ftray=rp/rt
      uw=0.
      uo3=0.
      g=98.1
      air=0.028964/0.0224
      ro3=0.048/0.0224
      rt=0.
      rp=0.
      do k=1,33
        roair=air*273.16*ppl(k)/(1013.25*tpl(k))
        rmwh(k)=wh(k)/(roair*1000.)
        rmo3(k)=wo(k)/(roair*1000.)
        rt=rt+(p(k+1)/t(k+1)+p(k)/t(k))*(z(k+1)-z(k))
        rp=rp+(ppl(k+1)/tpl(k+1)+ppl(k)/tpl(k))*(zpl(k+1)-zpl(k))
      enddo
      ftray=rp/rt
      do k=2,33
        ds=(ppl(k-1)-ppl(k))/ppl(1)
        uw=uw+((rmwh(k)+rmwh(k-1))/2.)*ds
        uo3=uo3+((rmo3(k)+rmo3(k-1))/2.)*ds
      enddo
      uw=uw*ppl(1)*100./g
      uo3=uo3*ppl(1)*100./g
      uo3=1000.*uo3/ro3
      return
      end
