      subroutine msrm
c
c   MultiSpectral Reflectance Model 93         A.Kuusk   24.03.1993
c
      implicit double precision (a-h, o-z)
      save /count/, /soildata/, /aaa/, /ggg/, /ladak/
c
      dimension u1(10), u2(10), a1(10), a2(10)
      common /count/ jl, jj, lg, jg, lf, nnx, n1, n2, u1, u2, a1, a2
c
      double precision nnl, kk
      common /leafin/ nnl, vai, kk
      common /leafout/ refl, tran
c
      double precision ke, kab, kw
      dimension refr(200), ke(200), kab(200), kw(200)
      common /dat/ refr, ke, kab, kw
c
      dimension phis1(200), phis2(200), phis3(200), phis4(200)
      common /soildata/ phis1, phis2, phis3, phis4, rsl1, rsl2, rsl3,
     & rsl4, th2, rsl, rsoil, rr1soil, rrsoil
c
      common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint
      common /ggg/ gr, gt, g, g1, th, sth, cth, th1, sth1, cth1,
     & phi, sp, cp, th22, st, ct, st1, ct1, t10, t11, e1, e2,
     & s2, s3, ctg, ctg1, ctt1, stt1, calph, alp2, salp2, calp2,
     & alph, salph, alpp, difmy, difsig
      common /cfresn/ rn, rk
      common /ladak/ ee, thm, sthm, cthm
      common /msrmdata/ th10, rncoef, cab, cw, bq
c
      data pi12/1.570796326794895d0/, pi/3.141592653589793d0/
      data eps4/.1d-3/
c
*           print *, 'msrm'
c
      sth10 = sin(th10)
      cth10 = cos(th10)
c
      sp    = sin(phi)
      cp    = cos(phi)
      th1   = th10
      sth1  = sth10
      cth1  = cth10
      sth   = sin(th)
      cth   = cos(th)
      rrls  = rrl
c
      call biz
c
      rrl  = refl
      rtp  = rrl + ttl
c
      call difr92
c
10    continue
c
      rrl = rrls
      bq  = bi + bd
c
      return
      end
*
******************************************************************
*
      subroutine akd
c  bdz   A.Kuusk    4.03.1988
c
      implicit double precision (a-h, o-z)
      save /count/, /aaa/, /ggg/
c
      dimension tt3(10), stt3(10), ctt3(10), tt2(10), stt2(10), ctt2(10)
c
      dimension u1(10), u2(10), a1(10), a2(10)
      common /count/ jl, jj, lg, jg, lf, nnx, n1, n2, u1, u2, a1, a2
c
      double precision nnl, kk
      common /leafin/ nnl, vai, kk
      common /leafout/ refl, tran
c
      common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint
      common /ggg/ gr, gt, g, g1, th, sth, cth, th1, sth1, cth1,
     & phi, sp, cp, th22, st, ct, st1, ct1, t10, t11, e1, e2,
     & s2, s3, ctg, ctg1, ctt1, stt1, calph, alp2, salp2, calp2,
     & alph, salph, alpp, difmy, difsig
c
      data pi/3.141592653589793d0/, pi1/1.5707963268d0/, eps/.005d0/
c
*                    print *, 'akd'
      bqint = 0.d0
      if (th .gt. eps) goto 4
      phi = 0.d0
      sp  = 0.d0
      cp  = 1.d0
c
      do 10 i2 = 1, n2
         th1  = (1.d0 - u2(i2))*pi1
         sth1 = sin(th1)
         cth1 = cos(th1)
         rrls = rrl
c
         call biz
c
         rrl = refl
         rtp = rrl + ttl
c
         call difr92
c
         rrl   = rrls
         bqint = bqint + a2(i2)*(bi + bd)*sth1*cth1
10    continue
c
      bqint = bqint*pi
      goto 1
c
4     continue
      do 14 i = 1, n1
         thi     = u1(i)*th
         tt3(i)  = thi
         stt3(i) = sin(thi)
         ctt3(i) = cos(thi)
14    continue
c
      do 15 i = 1, n2
         thi     = u2(i)*(th - pi1) + pi1
         tt2(i)  = thi
         stt2(i) = sin(thi)
         ctt2(i) = cos(thi)
15    continue
c
      do 11 j = 1, n1
         phi  = (1.d0 - u1(j))*pi
         sp   = sin(phi)
         cp   = cos(phi)
         bd1  = 0.d0
         bd2  = 0.d0
         do 12 i1 = 1, n1
            th1  = tt3(i1)
            sth1 = stt3(i1)
            cth1 = ctt3(i1)
c
         rrls = rrl
c
         call biz
c
         rrl = refl
         rtp = rrl + ttl
c
         call difr92
c
         rrl = rrls
c
            bd1 = bd1 + a1(i1)*(bi + bd)*sth1*cth1
12       continue
c
         do 13 i2 = 1, n2
            th1  = tt2(i2)
            sth1 = stt2(i2)
            cth1 = ctt2(i2)
c
         rrls = rrl
c
         call biz
c
         rrl = refl
         rtp = rrl + ttl
c
         call difr92
c
         rrl = rrls
c
            bd2 = bd2 + a2(i2)*(bi + bd)*sth1*cth1
13       continue
c
         bqint = bqint + ((pi1 - th)*bd2 + th*bd1)*a1(j)
11    continue
c
      bqint = bqint + bqint
c
1     return
      end
*
******************************************************************
*
      subroutine biz
c     canopy reflectance of single scattering for direct radiation
c     A. Kuusk   6.02.1992
c
      implicit double precision (a-h, o-z)
      double precision integr
      save /count/, /soildata/, /aaa/, /ggg/, /ladak/
c
*     dimension gj(2), g1j(2), grj(2), gtj(2), gfj(2)
c
      dimension u1(10), u2(10), a1(10), a2(10)
      common /count/ jl, jj, lg, jg, lf, nnx, n1, n2, u1, u2, a1, a2
c
      dimension phis1(200), phis2(200), phis3(200), phis4(200)
      common /soildata/ phis1, phis2, phis3, phis4, rsl1, rsl2,
     & rsl3, rsl4, th2, rsl, rsoil, rr1soil, rrsoil
c
      common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint
      common /ggg/ gr, gt, g, g1, th, sth, cth, th1, sth1, cth1,
     & phi, sp, cp, th22, st, ct, st1, ct1, t10, t11, e1, e2,
     & s2, s3, ctg, ctg1, ctt1, stt1, calph, alp2, salp2, calp2,
     & alph, salph, alpp, difmy, difsig
      common /ladak/ ee, thm, sthm, cthm
c
      data pi/3.14159265358979d0/, eps/.1d-4/, eps3/.01d0/
c
      integr(xx) = (1.d0 - exp(-xx))/xx
*           print *, 'biz in'
      ths   = th
      sths  = sth
      cths  = cth
      th1s  = th1
      sth1s = sth1
      cth1s = cth1
*     thms  = thm
c
      call soil
c
      if (ul .gt. eps) goto 2
      bi  = rsoil
      goto 1
c
2     continue
      if (th1 .lt. th) goto 12
      t11  = th1
      st   = sth
      st1  = sth1
      ct   = cth
      ct1  = cth1
      t10  = th
      jj   = 0
      goto 7
c
12    t10  = th1
      st   = sth1
      st1  = sth
      ct   = cth1
      ct1  = cth
      t11  = th
      jj   = 1
c
7     continue
      ctt1  = ct*ct1
      stt1  = st*st1
      calph = stt1*cp + ctt1
      catmp = calph
      alph  = acos(catmp)
      alp2  = alph*.5d0
*     if (lf .ne. 2) then
*        if( jg .gt. 2) then
*           print *, ' ***  biz3:  jg > 2  ***'
*           stop
*        endif
         e1   = st*ct1
         e2   = ct*st1
         s2   = e1*cp + e2
         s3   = e1*sp
         ctg  = 1.d30
         ctg1 = 1.d30
         if (st .ne. 0.d0) ctg = ct/st
         if (st1 .ne. 0.d0) ctg1 = ct1/st1
         salph = sin(alph)
         alpp  = pi - alph
         salp2 = sin(alp2)
         calp2 = cos(alp2)
c
         call gmf(gf)
c
         if (ee .le. eps3) goto 95
         y4  = abs(cth + cth1)*.5d0/calp2
         if (y4.lt.1.d0) thp = acos(y4)
c
95       call glak(glthp, thp)
c
         x2 = glthp*.125d0
         gf = gf*x2
c
         call gmd92
c
      gammd = gr*rrl + gt*ttl
c
      t11 = th1
      st  = sth
      st1 = sth1
      ct  = cth
      ct1 = cth1
      t10 = th
      if (jj .eq. 1) then
         x = g1
         g1 = g
         g = x
      endif
c
*           print *, 'biz:2'
      gg   = g*g1
      g    = g*clmp
      g1   = g1*clmp1
      gg1  = g*ct1 + g1*ct
      sct  = sqrt(ctt1)
      alpd = alp2/sl
      bam  = alpd*sct/ul
c
      if (ctt1 .gt. eps) then
         gma  = alpd/sct
         ulg  = gg1/ctt1*ul
      else
         gma  = 0.d0
         ulg  = ul
      endif
      ulg1 = ulg*.5d0
      xx1  = ulg + gma
      if ((xx1 .gt. 30.d0) .or. (ctt1 .le. eps)) then
         easte  = 0.d0
         easte2 = 0.d0
         easte4 = 0.d0
         bs1    = 0.d0
      else
         easte  = exp(-ulg)
         easte2 = exp(-ulg1 - gma)
         easte4 = exp(-ulg - gma)
         bs1    = (easte + easte2 - easte4)*rsoil
      endif
c
      xx1   = (1.d0 - easte)/gg1
      xx2   = (1.d0 - easte2)/(gg1*.5d0 + bam) -
     & (1.d0 - easte4)/(gg1 + bam)
      bc1d  = xx1*gammd
      bc1hs = xx2*(gammd + gf)
      bcsp  = xx1*gf
      bc1   = bc1d + bcsp + bc1hs
      bi    = bc1 + bs1
c
1     continue
      th    = ths
      sth   = sths
      cth   = cths
      th1   = th1s
      sth1  = sth1s
      cth1s = cth1
*     thm   = thms
c
      return
      end
*
******************************************************************
*
      subroutine difr92
c   diffuse fluxes according to SAIL for an elliptical LAD
c   A. Kuusk 16.06.1992
c
      implicit double precision (a-h, o-z)
      double precision ks, ko, m, m11, m12, m21, m22, integr
      save /soildata/, /aaa/, /ggg/, /ladak/
c
      dimension phis1(200), phis2(200), phis3(200), phis4(200)
      common /soildata/ phis1, phis2, phis3, phis4, rsl1, rsl2,
     & rsl3, rsl4, th2, rsl, rsoil, rr1soil, rrsoil
c
      common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint
      common /ggg/ gr, gt, g, g1, th, sth, cth, th1, sth1, cth1,
     & phi, sp, cp, th22, st, ct, st1, ct1, t10, t11, e1, e2,
     & s2, s3, ctg, ctg1, ctt1, stt1, calph, alp2, salp2, calp2,
     & alph, salph, alpp, difmy, difsig
      common /ladak/ ee, thm, sthm, cthm
c
      integr(x) = (1.d0 - exp(-x))/x
*           print *, 'difr92'
c
      tsun  = th1
      tview = th
      tants = sth1/cth1
      tanto = sth/cth
      rtp   = (rrl + ttl)/2.d0
c
      ks    = g1*ul/cth1
      ko    = g*ul/cth
      gg    = (1.289d0*difmy - 1.816d0*difsig)*(cthm**2 -
     & .33333333333d0) + .31823d0
      bf    = (rrl - ttl)/2.d0*ul*gg
      att   = (1.d0 - rtp)*ul + bf
      sig   = rtp*ul + bf
      sb    = ks*rtp + bf
      sf    = ks*rtp - bf
      ub    = ko*rtp + bf
      uf    = ko*rtp - bf
      m     = sqrt(att**2 - sig**2)
      h1    = (att + m)/sig
      h2    = 1.d0/h1
      c     = (sf*sig - sb*(ks - att))/(m**2 - ks**2)
      d     = (sb*sig + sf*(ks + att))/(m**2 - ks**2)
*     epso  = skyl - d*sq
      epso  =  - d
*     epss  = (rrsoil*(d + 1.d0) - c)*sq*exp(-ks)
      epss  = (rrsoil*(d + 1.d0) - c)*exp(-ks)
      m11   = h1
      m12   = h2
      m21   = (1.d0 - rrsoil*h1)*exp(-m)
      m22   = (1.d0 - rrsoil*h2)*exp(m)
      det   = m11*m22 - m12*m21
      a     = (m22*epso - m12*epss)/det
      b     = (-m21*epso + m11*epss)/det
      ep    = integr(ko + m)
      em    = integr(ko - m)
      ek    = integr(ko + ks)
*     gp    = a*ep + b*em + c*ek*sq
      gp    = a*ep + b*em + c*ek
*     gm    = h1*a*ep + h2*b*em + d*ek*sq
      gm    = h1*a*ep + h2*b*em + d*ek
*     ems   = h1*a*exp(-m) + h2*b*exp(m) + d*sq*exp(-ks)
      ems   = h1*a*exp(-m) + h2*b*exp(m) + d*exp(-ks)
      rplants = uf*gp + ub*gm
      rdsoil  = rrsoil*ems*exp(-ko)
      bd    = rplants + rdsoil
c
      return
      end
*
**********************************************************************
*
      subroutine glak(glth, th)
c  elliptical distribution
c  A.Kuusk   1.03.1988
c
      implicit double precision (a-h, o-z)
      save /aaa/, /ladak/
      save bb, es, tms
c
      common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint
      common /ladak/ ee, thm, sthm, cthm
c
      data bb/1.d0/, es/0.d0/, tms/0.d0/, eps/.1d0/
c
*           print *, 'gl'
c
      if (ee .lt. eps) then
      glth = 1.d0
      return
      endif
c
      if (ee .eq. 1.d0) ee = .999999d0
      if ((ee .ne. es) .or. (thm .ne. tms)) then
        u1  = ee*cthm
        u3  = ee*sthm
        u2  = sqrt(1.d0 - u1*u1)
        u4  = sqrt(1.d0 - u3*u3)
        x   = log((u4 + u1)/(u2 - u3))
        x1  = atan2(u3, u4) - atan2(u1, u2)
        x2  = sthm*x - cthm*x1
        bb  = ee/x2
        es  = ee
        tms = thm
      endif
c
      glth = bb/sqrt(1.d0 - (ee*cos(thm - th))**2)
c
      return
      end
*
******************************************************************
*
      subroutine gmf(gf)
c  Fresnel' reflection                    A.Kuusk 02.01.1991
c  input parameters are ca = cos(th_incident),  rn=refract.ind.,
c  rk = leaf hair index
c
      implicit double precision (a-h, o-z)
      save /aaa/, /ggg/
c
      common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint
      common /ggg/ gr, gt, g, g1, th, sth, cth, th1, sth1, cth1,
     & phi, sp, cp, th22, st, ct, st1, ct1, t10, t11, e1, e2,
     & s2, s3, ctg, ctg1, ctt1, stt1, calph, alp2, salp2, calp2,
     & alph, salph, alpp, difmy, difsig
      common /cfresn/ rn, rk
c
      data pi12/1.570796326794895d0/
c
*           print *, 'gmf'
c
      ca=calp2
      x2  = ca*ca
      ag  = x2*2.d0 - 1.d0 + rn*rn
      bg  = 1.d0 + (ag - 2.d0)*x2
      xy  = ag - x2
      cg  = 2.d0*ca*sqrt(xy)
      sa2 = 1.d0 - x2
      y   = (bg + sa2*cg)*(ag + cg)
      y   = (ag - cg)*bg/y
      yy  = sqrt(sa2)/pi12/ca*rk
      gf  = exp(-yy)*y
c
      return
      end
*
******************************************************************
*
      subroutine soil
c   Soil directional reflectance and reflectance (albedo)
c   th, th1, th2 in radianes,  cp = cos(phi)
c   A.Kuusk     1.03.1988
c
      implicit double precision (a-h, o-z)
      save a, b, c, cts, ths1, ths2
      save /soildata/, /aaa/, /ggg/
c
      dimension phis1(200), phis2(200), phis3(200), phis4(200)
      common /soildata/ phis1, phis2, phis3, phis4, rsl1, rsl2,
     & rsl3, rsl4, th2, rsl, rsoil, rr1soil, rrsoil
c
      common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint
      common /ggg/ gr, gt, g, g1, th, sth, cth, th1, sth1, cth1,
     & phi, sp, cp, th22, st, ct, st1, ct1, t10, t11, e1, e2,
     & s2, s3, ctg, ctg1, ctt1, stt1, calph, alp2, salp2, calp2,
     & alph, salph, alpp, difmy, difsig
c
      data a/.45098d0/, b/5.7829d0/, c, cts/2*13.7575d0/
      data ths1, ths2/2*.785398163d0/
c
*           print *, 'soil'
      if (th2 .ne. ths2) then
         cts  = 16.41d0 - th2*th2*4.3d0
         ths2 = th2
      endif
      if (th1 .ne. ths1) then
         ths1 = th1
         x    = th1*th1
         a    = x*7.702d0 - 4.3d0
         b    = th1*7.363d0
         c    = 16.41d0 - x*4.3d0
      endif
      x2      = rsl/cts
      rsoil   = ((a*th + b*cp)*th + c)*x2
      rr1soil = (.7337d0*a + c)*x2
      rrsoil  = 14.25d0*x2
c
      return
      end
*
******************************************************************
*
      subroutine soilspec
c
c   Soil spectral reflectance,  Price,  RSE 33:113 - 121 (1990)
c
      implicit double precision (a-h, o-z)
      save /count/, /soildata/
c
      dimension u1(10), u2(10), a1(10), a2(10)
      common /count/ jl, jj, lg, jg, lf, nnx, n1, n2, u1, u2, a1, a2
c
      dimension phis1(200), phis2(200), phis3(200), phis4(200)
      common /soildata/phis1, phis2, phis3, phis4, rsl1, rsl2,
     & rsl3, rsl4, th2, rsl, rsoil, rr1soil, rrsoil
c
      rsl = rsl1*phis1(jl) + rsl2*phis2(jl) +
     &      rsl3*phis3(jl) + rsl4*phis4(jl)
c
      return
      end
*
**********************************************************************
*
      subroutine gmd92
c  phase function and G-funktion
c  A. Kuusk    22.03.1988 & 16.06.1992



c  0< = th,  th1, th2<=pi/2,  0<=phi<=pi
c
      implicit double precision (a-h, o-z)
      dimension f(5)
      save /aaa/, /ggg/, /ladak/
c
      common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint
      common /ggg/ gr, gt, g, g1, th, sth, cth, th1, sth1, cth1,
     & phi, sp, cp, th22, st, ct, st1, ct1, t10, t11, e1, e2,
     & s2, s3, ctg, ctg1, ctt1, stt1, calph, alp2, salp2, calp2,
     & alph, salph, alpp, difmy, difsig
      common /ladak/ ee, thm, sthm, cthm
c
      data pi/3.14159265358979d0/, pi4/6.28318531717958d0/,
     & pi12/.159154943d0/, pi14/.636619773d0/, eps5/.1d-2/
     & , pi13/.1061032953d0/
c
*           print *, 'gmd92'
c
c                            ***  gammad,  e = 0.  ***
      gr0 = (salph + alpp*calph)*pi13
      gt0 = (salph - alph*calph)*pi13
      if (ee .lt. .4d0) then
         gr = gr0
         gt = gt0
         g  = .5d0
         g1 = .5d0
         return
      endif
c                            ***  gammad,  e = 1.  ***
      sg   = 0.d0
      sg1  = 0.d0
      sgmr = 0.d0
      sgmt = 0.d0
      if (th22 .lt. t11) goto 47
      assign 46 to l4
      goto 61
c
46    continue
      assign 48 to l4
      goto 64
c
47    continue
      if (th22 .lt. t10) goto 50
      assign 51 to l4
      goto 62
c
51    continue
      assign 46 to l4
      goto 65
c
50    continue
      assign 52 to l4
      goto 63
c
52    continue
      assign 48 to l4
      goto 65
c
48    continue
c
      gr1 = sgmr*pi12
      gt1 = sgmt*pi12
      gr  = gr0 - .0102d0 +
     &    (1.742d0*difmy - .4557d0*difsig)*(gr1 - gr0)
      gt  = gt0 + .00653d0 +
     &    (.2693d0*difmy + 5.821d0*difsig)*(gt1 - gt0)
      g   = (2.653d0*difmy + 1.432d0*difsig)*(sg  - .5d0) + .50072d0
      g1  = (2.653d0*difmy + 1.432d0*difsig)*(sg1 - .5d0) + .50072d0
c
49    continue
      return
c
c  ******************************    tl1 = 0.,  tl2=pi/2 - th1
c
61    assign 71 to l2
      goto 130
71    y = pp
      if (y .gt. 0.d0) sgmr = sgmr + y
      if (y .lt. 0.d0) sgmt = sgmt - y
      y1  = ct1*cthm
      sg1 = sg1 + abs(y1)
      goto l4
c
c  ******************************    tl1 = pi/2 - th1,  tl2=pi/2 - th
c
62    continue
      x2 = cthm/sthm
      x  = -ctg1*x2
      x1 = sqrt(1.d0 - x*x)
      fa = atan2(x1, x)
      fb = pi4 - fa
      assign 72 to l2
      goto 30
c
72    continue
      y = pp
      if (y .gt. 0.d0) sgmr = sgmr + y
      if (y .lt. 0.d0) sgmt = sgmt - y
      assign 73 to l2
      goto 130
c
73    y = pp - y
      if (y .gt. 0.d0) sgmr = sgmr + y
      if (y .lt. 0.d0) sgmt = sgmt - y
      goto l4
c
c  ******************************   tl1 = pi/2 - th,  tl2=pi/2
c
63    continue
      x2 = cthm/sthm
      x  = -ctg1*x2
      x1 = sqrt(1.d0 - x*x)
      fa = atan2(x1, x)
      f(2) = fa
      f(3) = pi4 - fa
      x  = -ctg*x2
      x1 = sqrt(1.d0 - x*x)
      fa = atan2(x1, x)
      fb = phi - fa
      if (fb .lt. 0.d0) fb = fb + pi4
      f(4) = fb
      f(5) = phi + fa
      do 75 ii = 2, 4
         i1 = ii + 1
         do 75 j = i1, 5
            fa = f(ii)
            fb = f(j)
            if (fb .gt. fa) goto 75
            f(ii) = fb
            f(j)  = fa
75    continue
      f(1) = f(5) - pi4
      i1   = 1
76    ii   = i1
      i1   = ii + 1
      fa   = f(ii)
      fb   = f(i1)
      assign 74 to l2
      goto 30
c
c  ******************************   tl1 = pi/2 - th,  tl2=pi/2
c
74    continue
      y = pp
      if (y .gt. 0.d0) sgmr = sgmr + y
      if (y .lt. 0.d0) sgmt = sgmt - y
      if (i1 .le. 4) goto 76
c
      x2 = ct*cthm
      x1 = st*sthm/x2
      x1 = sqrt(x1*x1 - 1.d0)
      x  = atan2(1.d0, x1)
      x  = (x + x1)*x2
      y  = x*pi14
      sg = sg + abs(y)
      goto l4
c
c  ******************************    tl1 = 0,  tl2=pi/2 - th
c
64    y1 = ct*cthm
      sg = sg + abs(y1)
      goto l4
c
c  ******************************    tl1 = pi/2 - th1,  tl2=pi/2
c
65    continue
      x2  = ct1*cthm
      x1  = st1*sthm/x2
      x1  = sqrt(x1*x1 - 1.d0)
      x   = atan2(1.d0, x1)
      x   = (x + x1)*x2
      y   = x*pi14
      sg1 = sg1 + abs(y)
      goto l4
c
c  ******************************    p(fa, fb)
c
30    x  = fb - fa
      if (x .gt. eps5) goto 31
      pp = 0.d0
      goto l2
31    if ((pi4 - x) .lt. eps5) goto 130
      sfa = sin(fa)
      sfb = sin(fb)
      cfa = cos(fa)
      cfb = cos(fb)
      pp  = x*ctt1*cthm*cthm
      y1  = x + sfb*cfb - sfa*cfa
      x   = cfa - cfb
      y1  = y1*cp + sp*x*(cfa + cfb)
      pp  = pp + stt1*.5d0*y1*sthm*sthm
      y1  = s2*(sfb - sfa) + s3*x
      pp  = pp + y1*sthm*cthm
      goto l2
c
130   x  = sthm*sthm
      pp = calph*x + ctt1*(2.d0 - 3.d0*x)
      pp = pp*pi
      goto l2
c
      end
*
******************************************************************
*
*
c     ******************************************************************
c     leaf reflectance and transmittance.
c     Input data are refractive index n,  a structure parameter N
c     and an absorption coefficient k:
c     the PROSPECT model,  Jacquemoud & Baret,  RSE 34:75-91 (1990)
c     ******************************************************************

      subroutine leaf
c
      implicit double precision (a-h, o-z)
c
      double precision nn, k, inex
      common /leafin/ nn, vai, k
      common /leafout/ refl, tran
      common /nagout/ inex
      common /tauin/ teta, ref
      common /tauout/ tau

c     ******************************************************************
c     determination of elementary reflectances et transmittances
c     ******************************************************************
c     ALLEN et al.,  1969,  Interaction of isotropic ligth with a compact
c     plant leaf,  J. Opt. Soc. Am.,  Vol.59,  10:1376-1379
c     JACQUEMOUD S. and BARET F.,  1990,  Prospect : a model of leaf
c     optical properties spectra,  Remote Sens. Environ.,  34:75-91
c     ******************************************************************

*                     print *, 'leaf'
      if (k .le. 0.d0) then
         k = 1.d0
      else
         call s13aaf
         k = (1.d0 - k)*exp(-k) + k**2*inex
      endif

      teta = 90.d0
      ref  = nn
c
      call tav
c
      t1   = tau
      teta = 59.d0
c
      call tav
c
      t2 = tau
      x1 = 1.d0 - t1
      x2 = t1**2*k**2*(nn**2 - t1)
      x3 = t1**2*k*nn**2
      x4 = nn**4 - k**2*(nn**2 - t1)**2
      x5 = t2/t1
      x6 = x5*(t1 - 1.d0) + 1.d0 - t2
      r  = x1 + x2/x4
      t  = x3/x4
      ra = x5*r + x6
      ta = x5*t

c     ******************************************************************
c     reflectances et transmittances corresponding to N elementary
c     layers
c     ******************************************************************
c     STOKES G.G.,  1862,  On the intensity of the light reflected from or
c     transmitted through a pile of plates,  Proceedings of the Royal
c     Society of London,  Vol.11,  545-556
c     ******************************************************************

      delta = (t**2 - r**2 - 1.d0)**2 - 4.d0*r**2
      alfa  = (1.d0 + r**2 - t**2 + sqrt(delta))/(2.d0*r)
      beta  = (1.d0 + r**2 - t**2 - sqrt(delta))/(2.d0*r)
      va    = (1.d0 + r**2 - t**2 + sqrt(delta))/(2.d0*r)
      vb    = sqrt(beta*(alfa - r)/(alfa*(beta - r)))
      s1    = ra*(va*vb**(vai - 1.d0) -
     & va**(-1.d0)*vb**(-(vai - 1.d0))) +
     & (ta*t - ra*r)*(vb**(vai - 1.d0) - vb**(-(vai - 1.d0)))
      s2    = ta*(va - va**(-1.d0))
      s3    = va*vb**(vai - 1.d0) - va**(-1.d0)*vb**(-(vai - 1.d0))
     &        - r*(vb**(vai - 1.d0) - vb**(-(vai - 1.d0)))
      refl  = s1/s3
      tran  = s2/s3
c
      return
      end


c     ******************************************************************
c     exponential integral: int(exp(-t)/t, t = x..inf)
c     ******************************************************************

      subroutine s13aaf
c
      implicit double precision (a-h, o-z)
c
      double precision nn, k, inex
      common /leafin/ nn, vai, k
      common /nagout/ inex
*                     print *, 's13aafin'

      if (k .gt. 4.d0) goto 10

      x  =  0.5d0 * k  -  1.d0
      y  =  (((((((((((((((-3.60311230482612224d-13
     &    *x + 3.46348526554087424d-12)*x - 2.99627399604128973d-11)
     &    *x + 2.57747807106988589d-10)*x - 2.09330568435488303d-9)
     &    *x + 1.59501329936987818d-8)*x - 1.13717900285428895d-7)
     &    *x + 7.55292885309152956d-7)*x - 4.64980751480619431d-6)
     &    *x + 2.63830365675408129d-5)*x - 1.37089870978830576d-4)
     &    *x + 6.47686503728103400d-4)*x - 2.76060141343627983d-3)
     &    *x + 1.05306034687449505d-2)*x - 3.57191348753631956d-2)
     &    *x + 1.07774527938978692d-1)*x - 2.96997075145080963d-1
      y  =  (y*x + 8.64664716763387311d-1)*x  +  7.42047691268006429d-1
      inex  =  y  -  log(k)
      goto 30

10    if (k .ge. 85.d0) go to 20
      x  =  14.5d0 / (k + 3.25d0)  -  1.d0
      y  =  (((((((((((((((-1.62806570868460749d-12
     &    *x - 8.95400579318284288d-13)*x - 4.08352702838151578d-12)
     &    *x - 1.45132988248537498d-11)*x - 8.35086918940757852d-11)
     &    *x - 2.13638678953766289d-10)*x - 1.10302431467069770d-9)
     &    *x - 3.67128915633455484d-9)*x - 1.66980544304104726d-8)
     &    *x - 6.11774386401295125d-8)*x - 2.70306163610271497d-7)
     &    *x - 1.05565006992891261d-6)*x - 4.72090467203711484d-6)
     &    *x - 1.95076375089955937d-5)*x - 9.16450482931221453d-5)
     &    *x - 4.05892130452128677d-4)*x - 2.14213055000334718d-3
      y  =  ((y*x - 1.06374875116569657d-2)*x -
     &     8.50699154984571871d-2)*x  +
     &     9.23755307807784058d-1
      inex  =  exp(-k) * y / k
      goto 30

20    inex  =  0.d0
      goto 30

30    continue
*                     print *, 's13aafout'
      return
      end

c     ******************************************************************
c     determination of tav for any solid angle
c     ******************************************************************
c     STERN F.,  1964,  Transmission of isotropic radiation across an
c     interface between two dielectrics,  Appl.Opt.,  Vol.3,  1:111-113
c     ALLEN W.A.,  1973,  Transmission of isotropic light across a
c     dielectric surface in two and three dimensions,  J.Opt.Soc.Am.,
c     Vol.63,  6:664-666
c     ******************************************************************

      subroutine tav
c
      implicit double precision (a-h, o-z)
      double precision k
c
      common /tauin/ teta, ref
      common /tauout/ tau
c
      data dr/1.745329251994330d-2/, eps/.1d-6/,
     &     pi12/1.570796326794895d0/

*                     print *, 'tavin'
      teta = teta*dr
      r2   = ref**2
      rp   = r2 + 1.d0
      rm   = r2 - 1.d0
      a    = (ref + 1.d0)**2/2.d0
      k    = -(r2 - 1.d0)**2/4.d0
      ds   = sin(teta)

      if (abs(teta) .le. eps) then
         tau = 4.d0*ref/(ref + 1.d0)**2
      else

         if (abs(teta - pi12) .le. eps) then
            b1 = 0.d0
         else
            xxx = (ds**2 - rp/2.d0)**2 + k
            b1 = sqrt(xxx)
         endif

         b2 = ds**2 - rp/2.d0
         b  = b1 - b2
         ts = (k**2/(6.d0*b**3) + k/b - b/2.d0) -
     &        (k**2/(6.d0*a**3) + k/a - a/2.d0)
         tp1 = -2.d0*r2*(b - a)/rp**2
         tp2 = -2.d0*r2*rp*log(b/a)/rm**2
         tp3 = r2*(1.d0/b - 1.d0/a)/2.d0
         tp4 = 16.d0*r2**2*(r2**2 + 1.d0)*dlog((2.d0*rp*b - rm**2)/
     &    (2.d0*rp*a - rm**2))/(rp**3*rm**2)
         tp5 = 16.d0*r2**3*(1.d0/(2.d0*rp*b - rm**2) - 1.d0/
     &    (2.d0*rp*a - rm**2))/rp**3
         tp  = tp1 + tp2 + tp3 + tp4 + tp5
         tau = (ts + tp)/(2.d0*ds**2)
      endif
*                     print *, 'tavout'
      return
      end
*
******************************************************************
*
c     constant values: refractive index (ref), albino and dry leaf
c     absorption (ke), chlorophyll a+b specific absorption coefficient
c     (kab), water specific absorption coefficient (kw),
*     and basis functions for soil spectral reflectance phis1, phis2,
*     phis3 and phis4 (Price, 1990)
c     ******************************************************************
c     JACQUEMOUD S. AND BARET F., 1990, Prospect : a model of leaf
c     optical properties spectra, Remote Sens. Environ., 34:75-91
c     JACQUEMOUD S. et al., 1991, Validation d'un modele de reflectance
c     spectrale et directionnnelle de sol, 5ieme Colloque International
c     Mesures Physiques et Signatures en Teledetection, Courchevel
c     (France), 14-18 Janvier 1991
c     ******************************************************************

      block data valeur
c
      implicit double precision (a-h, o-z)
c
      double precision ke, kab, kw
      dimension ref(200), ke(200), kab(200), kw(200)
      common /dat/ ref, ke, kab, kw
c
      dimension phis1(200), phis2(200), phis3(200), phis4(200)
      common /soildata/ phis1, phis2, phis3, phis4, rsl1, rsl2,
     & rsl3, rsl4, th2, rsl, rsoil, rr1soil, rrsoil
c
      data (ref(i), i = 1, 100)/
     & 1.5123,1.5094,1.5070,1.5050,1.5032,1.5019,1.5007,1.4997,1.4988,
     & 1.4980,1.4969,
     & 1.4959,1.4951,1.4943,1.4937,1.4930,1.4925,1.4920,1.4915,1.4910,
     & 1.4904,1.4899,1.4893,1.4887,1.4880,1.4873,1.4865,1.4856,1.4846,
     & 1.4836,1.4825,1.4813,1.4801,1.4788,1.4774,1.4761,1.4746,1.4732,
     & 1.4717,1.4701,1.4685,1.4670,1.4654,1.4639,1.4624,1.4609,1.4595,
     & 1.4582,1.4570,1.4559,1.4548,1.4538,1.4528,1.4519,1.4510,1.4502,
     & 1.4495,1.4489,1.4484,1.4480,1.4477,1.4474,1.4472,1.4470,1.4468,
     & 1.4467,1.4465,1.4463,1.4461,1.4458,1.4456,1.4453,1.4450,1.4447,
     & 1.4444,1.4440,1.4435,1.4430,1.4423,1.4417,1.4409,1.4402,1.4394,
     & 1.4387,1.4380,1.4374,1.4368,1.4363,1.4357,1.4352,1.4348,1.4345,
     & 1.4342,1.4341,1.4340,1.4340,1.4341,1.4342,1.4343,1.4345/

      data (ref(i), i = 101, 200)/
     & 1.4347,1.4348,1.4347,1.4345,1.4341,1.4336,1.4331,1.4324,1.4317,
     & 1.4308,1.4297,1.4284,1.4269,1.4253,1.4235,1.4216,1.4196,1.4176,
     & 1.4156,1.4137,1.4118,1.4100,1.4082,1.4065,1.4047,1.4029,1.4011,
     & 1.3993,1.3975,1.3958,1.3940,1.3923,1.3906,1.3888,1.3870,1.3851,
     & 1.3830,1.3808,1.3784,1.3758,1.3731,1.3703,1.3676,1.3648,1.3620,
     & 1.3592,1.3565,1.3537,1.3510,1.3484,1.3458,1.3433,1.3410,1.3388,
     & 1.3368,1.3350,1.3333,1.3317,1.3303,1.3289,1.3275,1.3263,1.3251,
     & 1.3239,1.3228,1.3217,1.3205,1.3194,1.3182,1.3169,1.3155,1.3140,
     & 1.3123,1.3105,1.3086,1.3066,1.3046,1.3026,1.3005,1.2985,1.2964,
     & 1.2944,1.2923,1.2902,1.2882,1.2863,1.2844,1.2826,1.2808,1.2793,
     & 1.2781,1.2765,1.2750,1.2738,1.2728,1.2719,1.2712,1.2708,1.2712,
     & 1.2736/

      data (ke(i), i = 1, 100)/
     &.1104,.0893,.0714,.0567,.0442,.0348,.0279,.0232,.0197,.0173,.0154,
     &.0142,.0120,.0108,.0093,.0092,.0092,.0092,.0092,.0092,.0091,.0091,
     &.0091,.0091,.0091,.0090,.0090,.0090,.0090,.0090,.0089,.0089,.0089,
     &.0089,.0088,.0088,.0088,.0088,.0088,.0087,.0087,.0087,.0087,.0087,
     &.0086,.0086,.0086,.0086,.0086,.0085,.0085,.0085,.0085,.0085,.0084,
     &.0084,.0084,.0084,.0084,.0083,.0083,.0083,.0082,.0082,.0082,.0082,
     &.0082,.0081,.0081,.0081,.0081,.0081,.0080,.0080,.0080,.0080,.0080,
     &.0079,.0079,.0079,.0079,.0079,.0078,.0078,.0078,.0078,.0078,.0077,
     &.0077,.0077,.0077,.0077,.0076,.0076,.0076,.0076,.0076,.0075,.0075,
     &.0075/

      data (ke(i), i = 101, 200)/
     &.0074,.0073,.0072,.0071,.0070,.0069,.0068,.0068,.0067,.0066,.0065,
     &.0064,.0063,.0062,.0062,.0061,.0060,.0059,.0058,.0057,.0056,.0056,
     &.0054,.0053,.0053,.0052,.0051,.0050,.0049,.0048,.0047,.0047,.0046,
     &.0045,.0044,.0043,.0042,.0041,.0040,.0039,.0039,.0037,.0037,.0036,
     &.0035,.0034,.0033,.0032,.0031,.0031,.0030,.0029,.0028,.0027,.0026,
     &.0025,.0025,.0024,.0023,.0022,.0021,.0020,.0019,.0019,.0018,.0017,
     &.0016,.0015,.0014,.0014,.0013,.0012,.0010,.0010,.0009,.0008,.0007,
     &.0006,.0006,.0005,.0004,.0003,.0002,.0002,.0001,15*.0000/

      data kab/
     & .04664,.04684,.04568,.04482,.04344,.04257,.04287,.04189,.04116,
     & .03847,.03409,
     & .03213,.03096,.03116,.03051,.03061,.02998,.02965,.02913,.02902,
     & .02769,.02707,.02539,.02409,.02150,.01807,.01566,.01317,.01095,
     & .00929,.00849,.00803,.00788,.00757,.00734,.00713,.00692,.00693,
     & .00716,.00758,.00815,.00877,.00938,.00976,.01041,.01089,.01105,
     & .01127,.01170,.01222,.01280,.01374,.01441,.01462,.01495,.01499,
     & .01506,.01580,.01686,.01810,.01961,.02112,.02336,.02702,.02880,
     & .02992,.03142,.03171,.02961,.02621,.02078,.01518,.01020,.00718,
     & .00519,.00390,.00298,.00218,.00163,.00116,.00083,.00057,.00039,
     & .00027,.00014,.00011,.00009,.00005,112*.00000/

      data kw/
     & 111*0.,00.100,00.200,00.278,00.206,00.253,00.260,00.313,00.285,
     & 00.653,00.614,00.769,00.901,00.872,00.812,00.733,00.724,00.855,
     & 00.900,01.028,01.500,02.026,02.334,03.636,08.942,14.880,17.838,
     & 19.497,19.419,17.999,12.024,10.709,08.384,07.081,06.155,05.619,
     & 05.112,04.512,04.313,04.064,03.804,03.709,03.877,04.348,04.574,
     & 05.029,05.804,06.345,05.823,05.886,06.315,08.432,15.588,32.247,
     & 51.050,58.694,55.135,50.454,42.433,40.670,36.030,29.771,25.153,
     & 24.378,22.008,20.608,18.576,17.257,15.921,14.864,12.861,12.773,
     & 12.426,13.090,14.013,15.066,15.857,16.776,19.113,21.066,22.125,
     & 26.438,28.391,28.920,31.754,36.375,40.056,41.019,45.471,43.126/

      data (phis1(i), i = 1, 100)/
     &  .088, .095, .102, .109, .116, .123, .130, .136, .143, .150,
     &  .157, .164, .171, .178, .185, .192, .199, .206, .213, .220,
     &  .227, .233, .240, .247, .254, .261, .268, .275, .282, .289,
     &  .295, .302, .309, .316, .326, .335, .345, .356, .366, .376,
     &  .386, .395, .404, .412, .421, .429, .436, .443, .450, .457,
     &  .464, .470, .476, .483, .489, .495, .502, .508, .514, .520,
     &  .526, .532, .538, .543, .549, .555, .561, .568, .574, .580,
     &  .587, .594, .601, .608, .615, .622, .629, .637, .644, .652,
     &  .659, .667, .674, .681, .689, .696, .702, .709, .716, .723,
     &  .729, .735, .742, .748, .754, .760, .766, .771, .777, .782/

      data (phis1(i), i = 101, 200)/
     &  .802, .819, .832, .842, .854, .868, .883, .899, .917, .935,
     &  .954, .974, .993,1.012,1.030,1.047,1.063,1.078,1.091,1.102,
     & 1.111,1.118,1.126,1.137,1.150,1.163,1.176,1.187,1.192,1.188,
     & 1.177,1.159,1.134,1.090, .979, .830, .764, .744, .748, .777,
     &  .823, .878, .932, .983,1.026,1.062,1.091,1.115,1.133,1.147,
     & 1.156,1.161,1.162,1.158,1.149,1.132,1.109,1.087,1.072,1.056,
     & 1.035, .989, .886, .659, .456, .350, .323, .335, .361, .396,
     &  .438, .484, .530, .576, .622, .664, .705, .740, .768, .788,
     &  .800, .802, .796, .794, .797, .789, .779, .756, .725, .715,
     &  .675, .635, .585, .535, .485, .435, .385, .335, .285, .235/

      data (phis2(i), i = 1, 100)/
     &  .249, .245, .241, .237, .232, .228, .222, .217, .211, .205,
     &  .199, .193, .186, .179, .171, .163, .155, .147, .139, .130,
     &  .121, .111, .102, .092, .081, .071, .060, .049, .038, .026,
     &  .014, .002,-.011,-.024,-.037,-.050,-.064,-.078,-.092,-.107,
     & -.121,-.137,-.152,-.168,-.184,-.200,-.216,-.232,-.246,-.259,
     & -.270,-.280,-.289,-.297,-.303,-.308,-.313,-.317,-.322,-.325,
     & -.329,-.332,-.335,-.338,-.340,-.342,-.345,-.347,-.350,-.352,
     & -.355,-.358,-.360,-.363,-.366,-.369,-.372,-.374,-.377,-.378,
     & -.380,-.381,-.382,-.382,-.383,-.382,-.382,-.381,-.380,-.378,
     & -.376,-.373,-.370,-.367,-.363,-.359,-.354,-.349,-.344,-.338/

      data (phis2(i), i = 101, 200)/
     & -.310,-.283,-.258,-.234,-.212,-.190,-.167,-.143,-.118,-.092,
     & -.066,-.039,-.014, .011, .034, .057, .083, .114, .151, .192,
     &  .233, .272, .311, .348, .380, .407, .438, .476, .521, .570,
     &  .624, .674, .708, .766, .824, .853, .854, .852, .858, .881,
     &  .916, .947, .973, .997,1.017,1.036,1.052,1.067,1.082,1.095,
     & 1.107,1.119,1.131,1.142,1.154,1.166,1.175,1.179,1.178,1.172,
     & 1.162,1.148,1.083, .900, .678, .538, .499, .515, .552, .598,
     &  .653, .716, .777, .834, .886, .932, .973,1.007,1.036,1.058,
     & 1.075,1.086,1.091,1.091,1.086,1.076,1.060,1.039,1.012, .980,
     &  .943, .900, .852, .799, .740, .676, .606, .532, .451, .366/

      data (phis3(i), i = 1, 100)/
     & -.417,-.384,-.351,-.318,-.285,-.253,-.221,-.189,-.157,-.126,
     & -.095,-.064,-.033,-.003, .027, .057, .087, .117, .146, .175,
     &  .204, .232, .260, .289, .316, .344, .371, .399, .425, .452,
     &  .478, .505, .525, .545, .566, .587, .606, .626, .652, .676,
     &  .699, .722, .744, .764, .784, .804, .822, .839, .856, .872,
     &  .886, .900, .913, .926, .937, .948, .957, .966, .974, .981,
     &  .988, .993, .998,1.002,1.006,1.009,1.012,1.014,1.016,1.017,
     & 1.018,1.018,1.018,1.017,1.016,1.014,1.012,1.010,1.007,1.003,
     &  .999, .995, .990, .984, .978, .972, .965, .957, .949, .941,
     &  .932, .923, .913, .902, .891, .880, .868, .855, .842, .829/

      data (phis3(i), i = 101, 200)/
     &  .766, .694, .620, .550, .484, .421, .361, .303, .247, .190,
     &  .134, .079, .023,-.031,-.086,-.140,-.190,-.235,-.275,-.310,
     & -.340,-.367,-.394,-.422,-.452,-.484,-.513,-.541,-.565,-.578,
     & -.575,-.556,-.525,-.468,-.323,-.115,-.018, .002,-.003,-.029,
     & -.076,-.142,-.211,-.274,-.333,-.386,-.432,-.471,-.503,-.528,
     & -.544,-.551,-.549,-.538,-.517,-.491,-.463,-.436,-.419,-.417,
     & -.401,-.348,-.216, .014, .160, .203, .209, .210, .207, .200,
     &  .189, .174, .155, .132, .105, .075, .043, .013,-.012,-.035,
     & -.053,-.068,-.078,-.082,-.080,-.073,-.060,-.041,-.017, .006,
     &  .035, .065, .097, .125, .168, .180, .168, .125, .097, .065/

      data (phis4(i), i = 1, 100)/
     &  .067, .077, .086, .094, .102, .111, .118, .126, .133, .140,
     &  .146, .152, .158, .164, .169, .174, .179, .184, .188, .192,
     &  .195, .198, .201, .204, .206, .208, .210, .212, .213, .214,
     &  .214, .214, .214, .214, .213, .212, .211, .210, .210, .209,
     &  .207, .205, .202, .198, .194, .189, .184, .179, .173, .167,
     &  .161, .155, .149, .143, .136, .130, .123, .116, .108, .101,
     &  .093, .085, .077, .068, .060, .051, .043, .034, .026, .018,
     &  .010, .002,-.006,-.014,-.022,-.030,-.037,-.045,-.052,-.060,
     & -.067,-.074,-.081,-.087,-.093,-.098,-.103,-.108,-.112,-.116,
     & -.120,-.123,-.126,-.129,-.132,-.134,-.136,-.138,-.140,-.141/

      data (phis4(i), i = 101, 200)/
     & -.147,-.152,-.158,-.166,-.170,-.165,-.157,-.151,-.144,-.128,
     & -.104,-.078,-.049,-.009, .038, .082, .122, .169, .222, .272,
     &  .317, .364, .413, .469, .532, .591, .642, .694, .748, .790,
     &  .810, .817, .819, .740, .494, .215, .110, .125, .155, .204,
     &  .291, .408, .521, .627, .724, .811, .884, .940, .987,1.025,
     & 1.053,1.071,1.077,1.072,1.046, .996, .941, .892, .857, .842,
     &  .809, .713, .509, .055,-.236,-.324,-.336,-.320,-.308,-.294,
     & -.275,-.248,-.205,-.144,-.094,-.048, .005, .058, .105, .132,
     &  .123, .079, .045, .024, .014, .018, .022,-.010,-.042,-.054,
     & -.055,-.060,-.060,-.055,-.050,-.046,-.042,-.038,-.034,-.030/

       end
*
******************************************************************
*
      subroutine dakg(u, a, nq)
c Gaussi kvadratuuri sqlmed ja kordajad, nq = 2*n, u=(-1., 1.)
      implicit double precision (a-h, o-z)
      dimension u(48), a(48)
c
*              print *,'dakg'
      n = nq/2
      goto (1, 2, 1, 4, 1, 6, 1, 8, 1, 10, 1, 12, 1, 14, 1, 16, 1,
     & 1, 1, 20, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     & 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 48), nq
1     continue
      print *,  ' ***   dakg - inacceptable nq'
      stop 'dakg'
c
2     continue
      u(2) = .577350269189626d0
      a(2) = 1.d0
      goto 13
c
4     continue
      u(3) = .339981043584856d0
      u(4) = .861136311594053d0
      a(3) = .652145154862546d0
      a(4) = .347854845137454d0
      goto 13
c
6     continue
      u(4) = .238619186083197d0
      u(5) = .661209386466265d0
      u(6) = .932469514203152d0
      a(4) = .467913934572691d0
      a(5) = .360761573048139d0
      a(6) = .171324492379170d0
      goto 13
c
8     continue
      u(5) = .183434642495650d0
      u(6) = .525532409916329d0
      u(7) = .796666477413627d0
      u(8) = .960289856497536d0
      a(5) = .362683783378362d0
      a(6) = .313706645877887d0
      a(7) = .222381034453374d0
      a(8) = .101228536290376d0
      goto 13
c
10    continue
      u(6)  = .148874338981631d0
      u(7)  = .433395394129247d0
      u(8)  = .679409568299024d0
      u(9)  = .865063366688985d0
      u(10) = .973906528517172d0
      a(6)  = .295524224714753d0
      a(7)  = .269266719309996d0
      a(8)  = .219086362515982d0
      a(9)  = .149451349150580d0
      a(10) = .666713443086881d-1
      goto 13
c
12    continue
      u(7)  = .125233408511469d0
      u(8)  = .367831498998180d0
      u(9)  = .587317954286617d0
      u(10) = .769902674194305d0
      u(11) = .904117256370475d0
      u(12) = .981560634246719d0
      a(7)  = .249147045813402d0
      a(8)  = .233492536538355d0
      a(9)  = .203167426723066d0
      a(10) = .160078328543346d0
      a(11) = .106939325995318d0
      a(12) = .471753363865118d-1
      goto 13
c
14    continue
      u( 8) = .108054948707344d0
      u( 9) = .319112368927890d0
      u(10) = .515248636358154d0
      u(11) = .687292904811685d0
      u(12) = .827201315069765d0
      u(13) = .928434883663574d0
      u(14) = .986283808696812d0
      a( 8) = .215263853463158d0
      a( 9) = .205198463721296d0
      a(10) = .185538397477938d0
      a(11) = .157203167158194d0
      a(12) = .121518570687903d0
      a(13) = .801580871597602d-1
      a(14) = .351194603317519d-1
      goto 13
c
16    continue
      u( 9) = .950125098376374d-1
      u(10) = .281603550779259d0
      u(11) = .458016777657227d0
      u(12) = .617876244402643d0
      u(13) = .755404408355003d0
      u(14) = .865631202387832d0
      u(15) = .944575023073233d0
      u(16) = .989400934991650d0
      a( 9) = .189450610455068d0
      a(10) = .182603415044924d0
      a(11) = .169156519395003d0
      a(12) = .149595988816577d0
      a(13) = .124628971255534d0
      a(14) = .951585116824928d-1
      a(15) = .622535239386479d-1
      a(16) = .271524594117541d-1
      goto 13
c
20    continue
      u(11) = .765265211334973d-1
      u(12) = .227785851141645d0
      u(13) = .373706088715420d0
      u(14) = .510867001950827d0
      u(15) = .636053680726515d0
      u(16) = .746331906460151d0
      u(17) = .839116971822219d0
      u(18) = .912234428251326d0
      u(19) = .963971927277914d0
      u(20) = .993128599185095d0
      a(11) = .152753387130726d0
      a(12) = .149172986472604d0
      a(13) = .142096109318382d0
      a(14) = .131688638449177d0
      a(15) = .118194531961518d0
      a(16) = .101930119817240d0
      a(17) = .832767415767047d-1
      a(18) = .626720483341091d-1
      a(19) = .406014298003869d-1
      a(20) = .176140071391521d-1
      goto 13
c
48    continue
      u(25) = .323801709628694d-1
      u(26) = .970046992094627d-1
      u(27) = .161222356068892d0
      u(28) = .224763790394689d0
      u(29) = .287362487355456d0
      u(30) = .348755886292161d0
      u(31) = .408686481990717d0
      u(32) = .466902904750958d0
      u(33) = .523160974722233d0
      u(34) = .577224726083973d0
      u(35) = .628867396776514d0
      u(36) = .677872379632664d0
      u(37) = .724034130923815d0
      u(38) = .767159032515740d0
      u(39) = .807066204029443d0
      u(40) = .843588261624394d0
      u(41) = .876572020274247d0
      u(42) = .905879136715570d0
      u(43) = .931386690706554d0
      u(44) = .952987703160431d0
      u(45) = .970591592546247d0
      u(46) = .984124583722827d0
      u(47) = .993530172266351d0
      u(48) = .998771007252426d0
      a(25) = .647376968126839d-1
      a(26) = .644661644359501d-1
      a(27) = .639242385846482d-1
      a(28) = .631141922862540d-1
      a(29) = .620394231598927d-1
      a(30) = .607044391658939d-1
      a(31) = .591148396983956d-1
      a(32) = .572772921004032d-1
      a(33) = .551995036999842d-1
      a(34) = .528901894851937d-1
      a(35) = .503590355538545d-1
      a(36) = .476166584924905d-1
      a(37) = .446745608566943d-1
      a(38) = .415450829434647d-1
      a(39) = .382413510658307d-1
      a(40) = .347772225647704d-1
      a(41) = .311672278327981d-1
      a(42) = .274265097083569d-1
      a(43) = .235707608393244d-1
      a(44) = .196161604573555d-1
      a(45) = .155793157229438d-1
      a(46) = .114772345792345d-1
      a(47) = .732755390127626d-2
      a(48) = .315334605230584d-2
13    continue
c
      nq1 = nq+1
      do 15 i = 1,n
         ii   = nq1-i
         u(i) = -u(ii)
         a(i) = a(ii)
15    continue
c
      return
      end
*
******************************************************************
c  akbrdf  -  an interface between 6s and msrm
c  MSRM93  -  MultiSpectral Reflectance Model   A. Kuusk   24.03.1993
c                                               Internet:  andres@aai.ee
c
c             A. Kuusk, A multispectral canopy reflectance model,
c                       Remote Sens. Environ.,  1994,  50(2):75-82.
c
      subroutine akbrdf(eei, thmi, uli, sli, rsl1i, wlmoy, rnci,
     & cabi, cwi, vaii, mu, np, rm, rp, brdfint)
c  See on tegelikult juba mcrm,  aga clx ja clz on fikseeritud
c
      implicit double precision (a-h, o-z)
      double precision integr
      integer np, mu
      integer k, j
      real eei, thmi, uli, sli, rsl1i, wlmoy, rnci, cabi, cwi,
     & vaii, pir
      real mu1, mu2, fi
      real rm(-mu:mu), rp(np), brdfint(-mu:mu, np)
      save /count/, /soildata/, /aaa/, /ggg/, /ladak/
c
      dimension u1(10), u2(10), a1(10), a2(10)
      common /count/ jl, jj, lg, jg, lf, nnx, n1, n2, u1, u2, a1, a2
c
      double precision nnl, kk
      common /leafin/ nnl, vai, kk
      common /leafout/ refl, tran
c
      double precision ke, kab, kw
      dimension refr(200), ke(200), kab(200), kw(200)
      common /dat/ refr, ke, kab, kw
c
      dimension phis1(200), phis2(200), phis3(200), phis4(200)
      common /soildata/ phis1, phis2, phis3, phis4, rsl1, rsl2,
     & rsl3, rsl4, th2, rsl, rsoil, rr1soil, rrsoil
c
      common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint
      common /ggg/gr, gt, g, g1, th, sth, cth, th1, sth1, cth1,
     & phi, sp, cp, th22, st, ct, st1, ct1, t10, t11, e1, e2,
     & s2, s3, ctg, ctg1, ctt1, stt1, calph, alp2, salp2, calp2,
     & alph, salph, alpp, difmy, difsig
      common /cfresn/ rn, rk
      common /ladak/ ee, thm, sthm, cthm
      common /msrmdata/ th10, rncoef, cab, cw, bq
c
c
      data pi/3.141592653589793d0/, pir/3.14159265/
      data pi12/1.570796326794895d0/, dr/1.745329251994330d-2/
      data eps/.1d-5/, eps4/.1d-3/
      data lf/1/
c
*           print *, 'msrm93'
c
      integr(xx) = (1.d0 - exp(-xx))/xx
      jg = 1
*     if (lf .eq. 1) then
c
        ee    = eei
        thm   = thmi*dr
        ul    = uli
        sl    = sli
        clz   = .9d0
        clx   = .1d0
        th2   = 45.d0*dr
        rsl1  = rsl1i
        rsl2  = -.48d0*rsl1 + .0862d0
        rsl3  = 0.d0
        rsl4  = 0.d0
        rlambda = wlmoy*1000.d0
c
        if ((rlambda .gt. 2500.d0) .or. (rlambda .lt. 404.d0)) then
           print *, 'AKBRDF: wavelength out of range'
           stop
        endif
c
        if (rlambda .le. 800.d0) then
           jl = nint((rlambda - 400.d0)/4.d0)
        else
           jl = nint((rlambda - 800.d0)/17.d0) + 100
        endif
c
        rncoef = rnci
        cab    = cabi
        cw     = cwi
        vai    = vaii
        nnl    = refr(jl)
        kk     = ke(jl) + cab*kab(jl) + cw*kw(jl)
        call leaf
c
        rn   = rncoef*nnl
        rrl  = refl - ((1.d0 - rn)/(1.d0 + rn))**2
        rrls = rrl
        ttl  = tran
c
        call soilspec
c
        cthm = cos(thm)
        sthm = sin(thm)
c
        th22 = pi12 - thm
        if (abs(th22) .lt. eps4) th22 = 0.d0
        eln = -log(1.d0 - ee)
        difmy  = abs(.059d0*eln*(thm - 1.02d0) + .02d0)
        difsig = abs(.01771d0 - .0216d0*eln*(thm - .256d0))
c
*       lf = 2
*     endif
c
      sth10 = sin(th10)
      cth10 = cos(th10)
c
      mu1 = rm(0)
      do 1 k = 1, np
      do 2 j = 1, mu
        mu2 = rm(j)
        if (j .eq. mu) then
           fi = rm(-mu)
        else
           fi = rp(k) + rm(-mu)
        endif
        th10 = acos(mu1)
        if (fi .lt. 0.) fi = fi + 2.*pir
        if (fi .gt. (2.*pir)) fi = fi - 2.*pir
        if (fi .gt. pir) fi = 2.*pir - fi
        tgt1 = tan(th10)
        xx   = tgt1*clx/sl
c
        if (xx .lt. eps) then
            clmp1 = clz
        else
            clmp1 = 1.d0 - (1.d0 - clz)*integr(xx)
        endif
c
        phi = fi
        th1 = th10
        th  = acos(mu2)
        tgt = tan(th)
        xx  = tgt*clx/sl
c
        if (xx .lt. eps) then
            clmp = clz
        else
            clmp = 1.d0 - (1.d0 - clz)*integr(xx)
        endif
c
        call msrm
        brdfint(j, k) = bq
c
  2   continue
  1   continue
c
      return
      end
*
*
******************************************************************
*

      subroutine akalbe
*    & (eei, thmi, uli, sli, rsl1i, wlmoy, rnci, cabi, cwi, vaii, albbrdf)
     & (albbrdf)
c
c   aa94.f   -  albeedo integrating msrm93 over the hemisphere
c   A. Kuusk    23.09.1994
c
      implicit double precision (a-h, o-z)
c
*     real eei, thmi, uli, sli, rsl1i, wlmoy, rnci, cabi, cwi, vaii, albbrdf
      real albbrdf
      save /count/, /soildata/, /aaa/, /ggg/, /ladak/
c
      dimension uu(20), aa(20)
c
      dimension u1(10), u2(10), a1(10), a2(10)
      common /count/ jl, jj, lg, jg, lf, nnx, n1, n2, u1, u2, a1, a2
c
      dimension phis1(200), phis2(200), phis3(200), phis4(200)
      common /soildata/ phis1, phis2, phis3, phis4, rsl1, rsl2,
     & rsl3, rsl4, th2, rsl, rsoil, rr1soil, rrsoil
c
      common /aaa/ rrl, ttl, ul, sl, clmp, clmp1, bi, bd, bqint
      common /ggg/gr, gt, g, g1, th, sth, cth, th1, sth1, cth1,
     & phi, sp, cp, th22, st, ct, st1, ct1, t10, t11, e1, e2,
     & s2, s3, ctg, ctg1, ctt1, stt1, calph, alp2, salp2, calp2,
     & alph, salph, alpp, difmy, difsig
      common /ladak/ ee, thm, sthm, cthm
c
      data pi/3.141592653589793d0/, pi1/1.5707963268d0/
c
*           print *, 'aa94'
c
      n1 = 6
      n2 = 8
c
      n  = n2 + n2
      ng = n + 1
      call dakg(uu, aa, n)
c
      do 20 i = 1, n2
         i1    = ng - i
         a2(i) = aa(i)
20       u2(i) = uu(i1)
c
      n  = n1 + n1
      ng = n + 1
      call dakg(uu, aa, n)
c
      do 21 i = 1, n1
         i1    = ng - i
         a1(i) = aa(i)
21       u1(i) = uu(i1)
c
      bdd = 0.d0
      do 10 i2 = 1, n2
         th  = (1.d0 - u2(i2))*pi1
         sth = sin(th)
         cth = cos(th)
c
         call akd
c
         bdd = bdd + a2(i2)*bqint*sth*cth
10    continue
c
      albbrdf = bdd*pi
c
      return
      end
*
******************************************************************
*
