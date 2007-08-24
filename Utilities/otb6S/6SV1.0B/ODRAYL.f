      subroutine odrayl ( wl,
     a                   tray)
      double precision a1,a2,a3,a4,awl,an,a
      real wl,tray,z,p,t,wh,wo,delta,sigma,pi,ak,dppt,sr
      integer k
c     molecular optical depth
 
      common /sixs_atm/ z(34),p(34),t(34),wh(34),wo(34)
      common /sixs_del/ delta,sigma
      real ns
      data pi /3.1415926/
      ak=1/wl
      awl=wl

c     air refraction index edlen 1966 / metrologia,2,71-80  putting pw=0
      a1=130.-ak*ak
      a2=38.9-ak*ak
      a3=2406030./a1
      a4=15997./a2
      an=(8342.13+a3+a4)*1.0e-08
      an=an+1.d+00
      a=(24.*pi**3)*((an*an-1.)**2)*(6.+3.*delta)/(6.-7.*delta)
     s        /((an*an+2.)**2)
      tray=0.
      do k=1,33
        ns=2.54743e+19
        dppt=(288.15/1013.25)*(p(k)/t(k)+p(k+1)/t(k+1))/2.
        sr=(a*dppt/(awl**4)/ns*1.e+16)*1.e+05
        tray=tray+(z(k+1)-z(k))*sr
      enddo

      return
      end
