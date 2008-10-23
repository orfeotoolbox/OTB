        subroutine abstra (idatm,wl,xmus,xmuv,uw,uo3,uwus,uo3us,
     a               idatmp,uwpl,uo3pl,uwusp,uo3usp,
     a      dtwava,dtozon,dtdica,dtoxyg,dtniox,dtmeth,dtmoca,
     a      utwava,utozon,utdica,utoxyg,utniox,utmeth,utmoca,
     a      ttwava,ttozon,ttdica,ttoxyg,ttniox,ttmeth,ttmoca )
 
c     transmittance calculation for ozone, water vapor,
c     carbon dioxyde and oxygen.
 
c     downward absorption water vapor dtwava
c     downward absorption ozone       dtozon
c     downward absorption carbon diox dtdica
c     downward absorption oxygen      dtoxyg
c     downward absorption nitrous oxi dtniox
c     downward absorption methane     dtmeth
c     downward absorption carbon mono dtmoca
c     upward absorption water vapor   utwava
c     upward absorption ozone         utozon
c     upward absorption carbon diox   utdica
c     upward absorption oxygen        utoxyg
c     upward   absorption nitrous oxi utniox
c     upward   absorption methane     utmeth
c     upward   absorption carbon mono utmoca
c     total(on the two paths ) absorption water vapor ttwava
c     total(on the two paths ) absorption ozone       ttozon
c     total(on the two paths ) absorption carbon diox ttdica
c     total(on the two paths ) absorption oxygen      ttoxyg
c     total    absorption nitrous oxi ttniox
c     total    absorption methane     ttmeth
c     total    absorption carbon mono ttmoca
 
      common /sixs_atm/ z(34),p(34),t(34),wh(34),wo(34)
      common /sixs_planesim/ zpl(34),ppl(34),tpl(34),whpl(34),wopl(34)
      real z,p,t,wh,wo
      real zpl,ppl,tpl,whpl,wopl
      integer iv,ivli(6),idatm,idatmp,i,id,idgaz,inu,k,n,nh
      real co3(102),tnu(10,3),
     a          a(8),rm(34),r2(34),r3(34),tp(34),rat(10)
      real rmpl(34),r2pl(34),r3pl(34),ratpl(10)
      real cch2o(15)
      real wl,xmus,xmuv,uw,uo3,uwus,uo3us,uwpl,uo3pl,uwusp,uo3usp
      real accu,ah2o,xh,dtcont,utcont,ttcont
      double precision ptest1,ptest
      real dtoxyg,dtniox,dtmeth,dtmoca,dtwava,dtozon,dtdica
      real utniox,utdica,utoxyg,utwava,utozon
      real utmeth,utmoca,ttwava,ttozon,ttdica,ttoxyg,ttniox,ttmeth
      real ttmoca
      real p0,g,t0,ds,te,roair,air,roco2,rmo2,rmo3,rmn2o,rmch4
      real rmco,v,te2,phi,psi,uu,u,up,uud,uut,uuu
      real ud,ut,upd,upt,udp,updp,udtp,updtp
      real ds2,uupl,upl,uppl
      real xi,xd,ako3,test1,test2,test3,udt,atest
      real updt,tt,y,utt,uptt,tn
C
      data(ivli(i),i=1,6)/2500,5060,7620,10180,12740,15300/
      data(co3(i),i=   1, 102)/
     a 4.50e-03, 8.00e-03, 1.07e-02, 1.10e-02, 1.27e-02, 1.71e-02,
     a 2.00e-02, 2.45e-02, 3.07e-02, 3.84e-02, 4.78e-02, 5.67e-02,
     a 6.54e-02, 7.62e-02, 9.15e-02, 1.00e-01, 1.09e-01, 1.20e-01,
     a 1.28e-01, 1.12e-01, 1.11e-01, 1.16e-01, 1.19e-01, 1.13e-01,
     a 1.03e-01, 9.24e-02, 8.28e-02, 7.57e-02, 7.07e-02, 6.58e-02,
     a 5.56e-02, 4.77e-02, 4.06e-02, 3.87e-02, 3.82e-02, 2.94e-02,
     a 2.09e-02, 1.80e-02, 1.91e-02, 1.66e-02, 1.17e-02, 7.70e-03,
     a 6.10e-03, 8.50e-03, 6.10e-03, 3.70e-03, 3.20e-03, 3.10e-03,
     a 2.55e-03, 1.98e-03, 1.40e-03, 8.25e-04, 2.50e-04, 0.      ,
     a 0.      , 0.      , 5.65e-04, 2.04e-03, 7.35e-03, 2.03e-02,
     a 4.98e-02, 1.18e-01, 2.46e-01, 5.18e-01, 1.02e+00, 1.95e+00,
     a 3.79e+00, 6.65e+00, 1.24e+01, 2.20e+01, 3.67e+01, 5.95e+01,
     a 8.50e+01, 1.26e+02, 1.68e+02, 2.06e+02, 2.42e+02, 2.71e+02,
     a 2.91e+02, 3.02e+02, 3.03e+02, 2.94e+02, 2.77e+02, 2.54e+02,
     a 2.26e+02, 1.96e+02, 1.68e+02, 1.44e+02, 1.17e+02, 9.75e+01,
     a 7.65e+01, 6.04e+01, 4.62e+01, 3.46e+01, 2.52e+01, 2.00e+01,
     a 1.57e+01, 1.20e+01, 1.00e+01, 8.80e+00, 8.30e+00, 8.60e+00/

      DATA (CCH2O(I),I=1,15)/                                           
     a 0.00,0.19,0.15,0.12,0.10,0.09,0.10,0.12,0.15,0.17,0.20,0.24,      
     a 0.28,0.33,0.00/

      accu=1.E-10
      dtwava=1.
      utwava=1.
      ttwava=1.
      dtcont=1.
      utcont=1.
      ttcont=1.
      dtozon=1.
      utozon=1.
      ttozon=1.
      dtdica=1.
      utdica=1.
      ttdica=1.
      dtoxyg=1.
      utoxyg=1.
      ttoxyg=1.
      dtniox=1.
      utniox=1.
      ttniox=1.
      dtmeth=1.
      utmeth=1.
      ttmeth=1.
      dtmoca=1.
      utmoca=1.
      ttmoca=1.
      do 201 i=1,10
corig do 201 i=1,7
      rat(i)=1.
      tnu(i,1)=1.
      tnu(i,2)=1.
      tnu(i,3)=1.
  201 continue
 
      if (idatm.eq.0) return
      if((xmus.eq.0.).or.(xmuv.eq.0.)) goto 95
 
 
c     constants determination
 
      p0=1013.25
      g=98.1
      t0=250.
 
c     volumic mass in kilogrammes per m3
 
      ds=0.
      te=0.
      roair=0.
      air=0.028964/0.0224
      roco2=0.044/0.0224
      rmo2=0.032/0.0224
      rmo3=0.048/0.0224
      rmn2o=0.044/0.0224
      rmch4=0.016/0.0224
      rmco =0.028/0.0224
      uwus=1.424
      uo3us=.344
 
      if(idatm.eq.8) goto 80
      goto 90
   80 rat(1)=uw/uwus
      rat(2)=1.
      rat(3)=1.
      rat(4)=uo3/uo3us
      rat(5)=1.
      rat(6)=1.
      rat(7)=1.
      rat(8)=uw/uwus
      rat(9)=uw/uwus
      rat(10)=uw/uwus
 
   90 v=1.0e+04/wl
      iv=v/5.
      iv=iv*5
      id=((iv-2500)/10)/256+1
      do 40 idgaz=1,7
c
c
      if (id.le.6) inu=(iv-ivli(id))/10+1
      goto(101,102,103,104,105,106),id
      goto 270
  106 if(idgaz.eq.1) call wava6(a,inu)
      if(idgaz.eq.2) goto 270
      if(idgaz.eq.3) call oxyg6(a,inu)
      if(idgaz.eq.4) goto 270
      if(idgaz.eq.5) call niox6(a,inu)
      if(idgaz.eq.6) call meth6(a,inu)
      if(idgaz.eq.7) call moca6(a,inu)
      goto 271
  105 if(idgaz.eq.1) call wava5(a,inu)
      if(idgaz.eq.2) goto 270
      if(idgaz.eq.3) call oxyg5(a,inu)
      if(idgaz.eq.4) goto 270
      if(idgaz.eq.5) call niox5(a,inu)
      if(idgaz.eq.6) call meth5(a,inu)
      if(idgaz.eq.7) call moca5(a,inu)
      goto 271
  104 if(idgaz.eq.1) call wava4(a,inu)
      if(idgaz.eq.2) goto 270
      if(idgaz.eq.3) call oxyg4(a,inu)
      if(idgaz.eq.4) goto 270
      if(idgaz.eq.5) call niox4(a,inu)
      if(idgaz.eq.6) call meth4(a,inu)
      if(idgaz.eq.7) call moca4(a,inu)
      goto 271
  103 if(idgaz.eq.1) call wava3(a,inu)
      if(idgaz.eq.2) call dica3(a,inu)
      if(idgaz.eq.3) call oxyg3(a,inu)
      if(idgaz.eq.4) goto 270
      if(idgaz.eq.5) call niox3(a,inu)
      if(idgaz.eq.6) call meth3(a,inu)
      if(idgaz.eq.7) call moca3(a,inu)
      goto 271
  102 if(idgaz.eq.1) call wava2(a,inu)
      if(idgaz.eq.2) call dica2(a,inu)
      if(idgaz.eq.3) goto 270
      if(idgaz.eq.4) goto 270
      if(idgaz.eq.5) call niox2(a,inu)
      if(idgaz.eq.6) call meth2(a,inu)
      if(idgaz.eq.7) call moca2(a,inu)
      goto 271
  101 if(idgaz.eq.1) call wava1(a,inu)
      if(idgaz.eq.2) call dica1(a,inu)
      if(idgaz.eq.3) goto 270
      if(idgaz.eq.4) call ozon1(a,inu)
      if(idgaz.eq.5) call niox1(a,inu)
      if(idgaz.eq.6) call meth1(a,inu)
      if(idgaz.eq.7) call moca1(a,inu)
      goto 271
  270 do 200 i=1,8
      a(i)=0.
  200 continue
  271 continue
 
c     mixing ratio calculation for each gaseous constituents
      do k=1,33 
      roair=air*273.16*p(k)/(1013.25*t(k))
      tp(k)=(t(k)+t(k+1))/2.
      te=tp(k)-t0
      te2=te*te
      phi=exp(a(3)*te+a(4)*te2)
      psi=exp(a(5)*te+a(6)*te2)
      if(idgaz.eq.1) rm(k)=wh(k)/(roair*1000.)
      if(idgaz.eq.2) rm(k)=3.3e-04*roco2/air
      if(idgaz.eq.3) rm(k)=0.20947*rmo2/air
      if(idgaz.eq.4) rm(k)=wo(k)/(roair*1000.)
      if(idgaz.eq.5) rm(k)=310.e-09*rmn2o/air
      if(idgaz.eq.6) rm(k)=1.72e-06*rmch4/air
      if(idgaz.eq.7) rm(k)=1.00e-09*rmco /air
      r2(k)=rm(k)*phi
      r3(k)=rm(k)*psi
      enddo
c
      uu=0.
      u=0.
      up=0.
      uud=0.
      uut=0.
      uuu=0.
      ud=0.
      ut=0.
      upd=0.
      upt=0.
      udp=0.
      updp=0.
      udtp=0.
      updtp=0.
 
c     pressure scale integration sigma=p/p0
 
         do 50 k=2,33                                  
         ds=(p(k-1)-p(k))/p(1)                        
         ds2=(p(k-1)*p(k-1)-p(k)*p(k))/(2.*p(1)*p0)  
         uu=uu+((rm(k)+rm(k-1))/2.)*ds*rat(idgaz)   
         u =u +((r2(k)+r2(k-1))/2.)*ds*rat(idgaz)  
         up=up+((r3(k)+r3(k-1))/2.)*ds2*rat(idgaz)
   50    continue                                
         uu=uu*p(1)*100./g                      
         u=u*p(1)*100./g                       
         up=up*p(1)*100./g                    
                                             
         if(idgaz.eq.4) uu=1000.*uu/rmo3    
         if(idgaz.eq.2) uu=1000.*uu/roco2  
         if(idgaz.eq.5) uu=1000.*uu/rmn2o
         if(idgaz.eq.6) uu=1000.*uu/rmch4
         if(idgaz.eq.7) uu=1000.*uu/rmco
 
c     mixing ratio for plane calculation for each gaseous constituents
      if ((idatmp.eq.0).or.(idatmp.eq.4)) then
          uupl=uu
          upl=u
          uppl=up
          else
          do k=1,33
          roair=air*273.16*ppl(k)/(1013.25*tpl(k))
          tp(k)=(tpl(k)+tpl(k+1))/2.
          te=tp(k)-t0
          te2=te*te
          phi=exp(a(3)*te+a(4)*te2)
          psi=exp(a(5)*te+a(6)*te2)
          if(idgaz.eq.1) rmpl(k)=whpl(k)/(roair*1000.)
          if(idgaz.eq.2) rmpl(k)=3.3e-04*roco2/air
          if(idgaz.eq.3) rmpl(k)=0.20947*rmo2/air
          if(idgaz.eq.4) rmpl(k)=wopl(k)/(roair*1000.)
          if(idgaz.eq.5) rmpl(k)=310.e-09*rmn2o/air
          if(idgaz.eq.6) rmpl(k)=1.72e-06*rmch4/air
          if(idgaz.eq.7) rmpl(k)=1.00e-09*rmco /air
          r2pl(k)=rmpl(k)*phi
          r3pl(k)=rmpl(k)*psi
          enddo
c
          uupl=0.
          upl=0.
          uppl=0.
c update ratio plane
          do i=1,10
          ratpl(i)=1.
          enddo
          if (idatmp.eq.8) then
          ratpl(1)=uwpl/uwusp
          ratpl(2)=1.
          ratpl(3)=1.
          ratpl(4)=uo3pl/uo3usp
          ratpl(5)=1.
          ratpl(6)=1.
          ratpl(7)=1.
          ratpl(8)=uwpl/uwusp
          ratpl(9)=uwpl/uwusp
          ratpl(10)=uwpl/uwusp
          endif
c     pressure scale integration sigma=p/p0
 
c* all gases
         do k=2,33
         ds=(ppl(k-1)-ppl(k))/ppl(1)
         ds2=(ppl(k-1)*ppl(k-1)-ppl(k)*ppl(k))/(2.*ppl(1)*p0)
         uupl=uupl+((rmpl(k)+rmpl(k-1))/2.)*ds*ratpl(idgaz)
         upl =upl +((r2pl(k)+r2pl(k-1))/2.)*ds*ratpl(idgaz)
         uppl=uppl+((r3pl(k)+r3pl(k-1))/2.)*ds2*ratpl(idgaz)
         enddo
         uupl=uupl*ppl(1)*100./g
         upl=upl*ppl(1)*100./g
         uppl=uppl*ppl(1)*100./g
 
         if(idgaz.eq.4) uupl=1000*uupl/rmo3
         if(idgaz.eq.2) uupl=1000*uupl/roco2
         if(idgaz.eq.5) uupl=1000*uupl/rmn2o
         if(idgaz.eq.6) uupl=1000*uupl/rmch4
         if(idgaz.eq.7) uupl=1000*uupl/rmco
c endif test idatm =0,4
      endif
 
c     downward path
      uud=uu/xmus
c     upward path
      uuu=uupl/xmuv
c     total(down+up) path
      uut=uu/xmus+uupl/xmuv
 
      if(idgaz.eq.1) goto 146
      if(idgaz.eq.2.and.iv.gt.9620) goto 147
      if(idgaz.eq.3.and.iv.gt.15920) goto 147
      if(idgaz.eq.4) goto 146
      goto 145
  146 xi=0.
      n=0
      xd=0.
      if(iv.lt.2350.or.iv.gt.3000) goto 148
      xi=(v-2350.)/50.+1.
      NH=XI+1.001
      XH=XI-FLOAT(NH)
      AH2O=CCH2O(NH)+XH*(CCH2O(NH)-CCH2O(NH-1))
      DTCONT=EXP(-AH2O*UUD)
      UTCONT=EXP(-AH2O*UUU)
      TTCONT=EXP(-AH2O*UUT)
  148 if (idgaz.eq.1) goto 145
      if(iv.lt.13000) goto 145
      if(iv.le.23400) goto 130
      if(iv.ge.27500) goto 135
      tnu(4,1)=1.
      tnu(4,2)=1.
      tnu(4,3)=1.
      goto 45
  130 xi=(v-13000.)/200.+1.
      goto 140
  135 xi=(v-27500.)/500.+57.
  140 n=xi+1.001
      xd=xi-float(n)
      ako3=co3(n)+xd*(co3(n)-co3(n-1))
      test1=ako3*uud
      test2=ako3*uuu
      test3=ako3*uut
 
c     exponential overflow test
 
      if(test1.gt.86.0) test1=86.0
      if(test2.gt.86.0) test2=86.0
      if(test3.gt.86.0) test3=86.0
 
      tnu(4,1)=exp(-test1)
      tnu(4,2)=exp(-test2)
      tnu(4,3)=exp(-test3)
      goto 40
  145 continue
      if(idgaz.eq.4.and.iv.gt.3020) goto 147
c
c     downward path
c
      ud=u/xmus
      upd=up/xmus
      udt=ud
      if(ud.eq.0.and.upd.eq.0.) udt=1.
      tn=a(2)*upd/(2*udt)
      atest=a(2)
      if (a(2).eq.0.and.a(1).eq.0.) atest=1.
      updt=upd
      if(ud.eq.0.and.upd.eq.0.) updt=1.
      tt=1+4*(a(1)/atest)*((ud*ud)/updt)
      y=-tn*(sqrt(tt)-1)
      if(idgaz.eq.1) y=-a(1)*ud/sqrt(1+(a(1)/atest)*(ud*ud/updt))
      tnu(idgaz,1)=exp(y)
c
c     upward path modified to take account for plane content
c
      udp=upl/xmuv
      updp=uppl/xmuv
      udtp=udp
      if(udp.eq.0.and.updp.eq.0.) udtp=1.
      tn=a(2)*updp/(2*udtp)
      atest=a(2)
      if (a(2).eq.0.and.a(1).eq.0.) atest=1.
      updtp=updp
      if(udp.eq.0.and.updp.eq.0.) updtp=1.
      tt=1+4*(a(1)/atest)*((udp*udp)/updtp)
      y=-tn*(sqrt(tt)-1)
      if(idgaz.eq.1) y=-a(1)*udp/sqrt(1+(a(1)/atest)*(udp*udp/updtp))
      tnu(idgaz,2)=exp(y)
c
c     total(down+up) path modified on the way up
c
      ut=u/xmus+upl/xmuv
      upt=up/xmus+uppl/xmuv
      utt=ut
      if(ut.eq.0.and.upt.eq.0.) utt=1.
      tn=a(2)*upt/(2*utt)
      uptt=upt
      if(ut.eq.0.and.upt.eq.0.) uptt=1.
      tt=1+4*(a(1)/atest)*((ut*ut)/uptt)
      y=-tn*(sqrt(tt)-1)
      if(idgaz.eq.1) y=-a(1)*ut/sqrt(1+(a(1)/atest)*(ut*ut/uptt))
      tnu(idgaz,3)=exp(y)
      goto 40
  147 tnu(idgaz,1)=1.
      tnu(idgaz,2)=1.
      tnu(idgaz,3)=1.
   40 continue
C
   45 ptest1=tnu(1,1)*dtcont
      ptest=ptest1
      if (ptest.gt.accu) then
         dtwava=ptest
         else
         dtwava=0.
         endif
      ptest1=tnu(1,2)*utcont
      ptest=ptest1
      if (ptest.gt.accu) then
         utwava=ptest
         else
         utwava=0.
         endif
      ptest1=tnu(1,3)*ttcont
      ptest=ptest1
      if (ptest.gt.accu) then
         ttwava=ptest
         else
         ttwava=0.
         endif
      dtdica=tnu(2,1)
      utdica=tnu(2,2)
      ttdica=tnu(2,3)
      dtoxyg=tnu(3,1)
      utoxyg=tnu(3,2)
      ttoxyg=tnu(3,3)
      dtozon=tnu(4,1)
      utozon=tnu(4,2)
      ttozon=tnu(4,3)
      dtniox=tnu(5,1)
      utniox=tnu(5,2)
      ttniox=tnu(5,3)
      dtmeth=tnu(6,1)
      utmeth=tnu(6,2)
      ttmeth=tnu(6,3)
      dtmoca=tnu(7,1)
      utmoca=tnu(7,2)
      ttmoca=tnu(7,3)
      if (idatmp.eq.0) then
         ttwava=dtwava
         utwava=1.
         ttdica=dtdica
         utdica=1.
         ttoxyg=dtoxyg
         utoxyg=1.
         ttozon=dtozon
         utozon=1.
         ttniox=dtniox
         utniox=1.
         ttmeth=dtmeth
         utmeth=1.
         ttmoca=dtmoca
         utmoca=1.
         endif
      return
   95 call print_error(
     s 'Error on zenithal angle ( near 90 deg )')
      return
      end
