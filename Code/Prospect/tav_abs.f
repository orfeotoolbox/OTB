!     ******************************************************************
!     TAV(teta0,ref0) computation of the transmittivity at the leaf 
!     surface for a given incidence solid angle. teta0 is the incidence
!     solid angle (in radian). The average angle that works in most 
!     cases is 59deg*pi/180. ref0 is the refaction index.
!
!     Jacquemoud S., 1992
! 
!     ******************************************************************
!     STERN F., 1964, Transmission of isotropic radiation across an
!     interface between two dielectrics, Appl.Opt., Vol.3, 1:111-113
!     ALLEN W.A., 1973, Transmission of isotropic light across a
!     dielectric surface in two and three dimensions, J.Opt.Soc.Am.,
!     Vol.63, 6:664-666
!	  FERET et al. (2008), PROSPECT-4 and 5: Advances in the Leaf Optical
!	  Properties Model Separating Photosynthetic Pigments, Remote Sensing of
!	  Environment
!     ******************************************************************

DOUBLE PRECISION FUNCTION tav_abs(teta0,ref0)

IMPLICIT NONE

REAL(KIND=8) teta,ref,teta0,ref0
REAL(KIND=8) ref2
REAL(KIND=8) a,b,b1,b2,k,pi
REAL(KIND=8) ts,tp,tp1,tp2,tp3,tp4,tp5

pi	=	3.1415926

teta=	teta0
ref	=	ref0

IF (teta.eq.0.) THEN
	tav_abs=4.*ref/(ref+1.)**2
	RETURN
ENDIF

ref2=	ref**2
a	=	(ref+1.)**2/2.
k	=	-(ref2-1.)**2/4.
teta=	pi*teta/180.


IF (teta.eq.pi/2.) THEN
	b1=0.
ELSE
	b1=dsqrt((sin(teta)*sin(teta)-(ref2+1.)/2.)**2+k)
ENDIF

b2	=	sin(teta)*sin(teta)-(ref2+1.)/2.
b	=	b1-b2
ts	=	(k**2/(6.*b**3)+k/b-b/2.)-(k**2/(6.*a**3)+k/a-a/2.)
tp1	=	-2.*ref2*(b-a)/(ref2+1.)**2
tp2	=	-2.*ref2*(ref2+1.)*dlog(b/a)/(ref2-1.)**2
tp3	=	ref2*(1./b-1./a)/2.
tp4	=	16.*ref2**2*(ref2**2+1.)*dlog((2.*(ref2+1.)*b-(ref2-1.)**2)/ &
(2.*(ref2+1.)*a-(ref2-1.)**2))/((ref2+1.)**3*(ref2-1.)**2)
tp5	=	16.*ref2**3*(1./(2.*(ref2+1.)*b-(ref2-1.)**2)-1./(2.*(ref2&
+1.)*a-(ref2-1.)**2))/(ref2+1.)**3
tp	=	tp1+tp2+tp3+tp4+tp5
tav_abs	=	(ts+tp)/(2.*sin(teta)*sin(teta))

RETURN
END
