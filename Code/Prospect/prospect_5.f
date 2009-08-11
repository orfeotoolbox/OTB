SUBROUTINE prospect_5(N,Cab,Car,Cw,Cm,LRT)

	USE dataSpec_P5
	IMPLICIT NONE

REAL(KIND=8),INTENT(IN) :: N
REAL(KIND=8),INTENT(IN) :: Cab
REAL(KIND=8),INTENT(IN) :: Car
REAL(KIND=8),INTENT(IN) :: Cw
REAL(KIND=8),INTENT(IN) :: Cm
REAL(KIND=8),INTENT(OUT) :: LRT(nl,3)

REAL(KIND=8) k,x,y,t1,t2
REAL(KIND=8) x1,x2,x3,x4,x5,x6,r,t,ra,ta
REAL(KIND=8) delta,beta,va,vb,s11,s12,s2,s3
REAL(KIND=8) ang1,ang2
REAL(KIND=8) t12,k2,nref2,t_2,r_2,vbNN,vbNN1,va1
REAL(KIND=8) eps
REAL(KIND=8) tav_abs
EXTERNAL tav_abs

INTEGER l

DO l=1,nl
	k=(Cab*aCab(l)+Car*aCar(l)+Cw*aCw(l)+Cm*aCm(l))/N

	! ***********************************************************************
	! reflectance and transmittance of one layer
	! ***********************************************************************
	! Allen W.A., Gausman H.W., Richardson A.J., Thomas J.R. (1969),
	! Interaction of isotropic ligth with a compact plant leaf, J. Opt.
	! Soc. Am., 59(10):1376-1379.
	! ***********************************************************************

	! ******************************************************************
	! exponential integral - NAG - S13AAF
	! ******************************************************************

	! error test
	  IF (k.le.0.0) GOTO 10

	! large range test
	  IF (k.gt.4.0) GOTO 20

	! small k - argument evaluation
	  x = 0.5 * k - 1.0
	  y = (((((((((((((((-3.60311230482612224d-13 &
			 *x+3.46348526554087424d-12)*x-2.99627399604128973d-11) &
			 *x+2.57747807106988589d-10)*x-2.09330568435488303d-9) &
			 *x+1.59501329936987818d-8)*x-1.13717900285428895d-7) &
			 *x+7.55292885309152956d-7)*x-4.64980751480619431d-6) &
			 *x+2.63830365675408129d-5)*x-1.37089870978830576d-4) &
			 *x+6.47686503728103400d-4)*x-2.76060141343627983d-3) &
			 *x+1.05306034687449505d-2)*x-3.57191348753631956d-2) &
			 *x+1.07774527938978692d-1)*x-2.96997075145080963d-1
	   y = (y*x+8.64664716763387311d-1)*x + 7.42047691268006429d-1
	   y = y - LOG(k)
	   GOTO 30

	! large k - asymptotic test
	20 IF (k.ge.85.0) GOTO 40
		  x = 14.5 / (k+3.25) - 1.0
		  y = (((((((((((((((-1.62806570868460749d-12 &
			 *x-8.95400579318284288d-13)*x-4.08352702838151578d-12) &
			 *x-1.45132988248537498d-11)*x-8.35086918940757852d-11) &
			 *x-2.13638678953766289d-10)*x-1.10302431467069770d-9) &
			 *x-3.67128915633455484d-9)*x-1.66980544304104726d-8) &
			 *x-6.11774386401295125d-8)*x-2.70306163610271497d-7) &
			 *x-1.05565006992891261d-6)*x-4.72090467203711484d-6) &
			 *x-1.95076375089955937d-5)*x-9.16450482931221453d-5) &
			 *x-4.05892130452128677d-4)*x-2.14213055000334718d-3
		  y = ((y*x-1.06374875116569657d-2)*x-8.50699154984571871d-2)*x + &
			  9.23755307807784058d-1
		  y = EXP(-k) * y / k
		  GOTO 30

	! asymptotic range
	40    y = 0.0

	30    k=(1.-k)*DEXP(-k)+k**2*y
		  GOTO 50
	10    k=1.

	50    CONTINUE

	ang1=90;ang2=40

	t1	=	tav_abs(ang1,nrefrac(l))
	t2	=	tav_abs(ang2,nrefrac(l))
	t12	=	t1**2
	k2=k**2
	nref2=nrefrac(l)**2

	x1=1-t1
	x2=t12*k2*(nref2-t1)
	x3=t12*k*nref2
	x4=nref2**2-k2*(nref2-t1)**2
	x5=t2/t1
	x6=x5*(t1-1)+1-t2

	r=x1+x2/x4
	t=x3/x4
	ra=x5*r+x6
	ta=x5*t

	! ***********************************************************************
	! reflectance and transmittance of N layers
	! ***********************************************************************
	! Stokes G.G. (1862), On the intensity of the light reflected from
	! or transmitted through a pile of plates, Proc. Roy. Soc. Lond.,
	! 11:545-556.
	! ***********************************************************************

	t_2		=	t**2
	r_2		=	r**2

	delta=(t_2-r_2-1)**2-4*r_2

	! correction of unstability if no absorption and N=1...
	eps=1e-16
	IF (delta .le. 0) THEN
		delta=eps
	ENDIF

	beta=(1+r_2-t_2-SQRT(delta))/(2*r)
	va=(1+r_2-t_2+SQRT(delta))/(2*r)
	! numerical unstabilities for VERY important absorptions
	! (k>12)
	! theorically impossible... (0.1cm water or 600 µg/cm² Chl...)
	IF ((beta*(va-r)/(va*(beta-r))) <0 ) THEN
		vb=eps
	ELSE
		vb=SQRT(beta*(va-r)/(va*(beta-r)))
	ENDIF

	vbNN	=	vb**(N-1)
	vbNN1	=	vb**(-(N-1))
	va1		=	va**(-1)

	s11=ra*(va*vbNN-va1*vbNN1)
	s12=(ta*t-ra*r)*(vbNN-vbNN1)
	s2=ta*(va-va1)
	s3=va*vbNN-va1*vbNN1-r*(vbNN-vbNN1)
	IF ((s11+s12)*s2*s3 .ne. 0) THEN
		LRT(l,2) = (s11+s12)/s3
		LRT(l,3) = s2/s3
	ELSE
		LRT(l,2) = 999.
		LRT(l,3) = 999.
	ENDIF
	LRT(l,1) = lambda(l)
ENDDO

RETURN
END
