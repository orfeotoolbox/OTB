/* OTB : This file is a copy of v3p_f2c_mangle.h ITK file */
/* Replace v3p_netlib by otb_6S */

/* This header file mangles names added to the namespace by v3p_f2c.h.
   There are purposely no include blockers so translation units can
   mangle/unmangle repeatedly.  */

/* Functions */
#define F77_aloc otb_6s_F77_aloc
#define exit_ otb_6s_exit_
#define c_abs otb_6s_c_abs
#define c_div otb_6s_c_div
#define c_sqrt otb_6s_c_sqrt
#define d_cnjg otb_6s_d_cnjg
#define d_imag otb_6s_d_imag
#define d_lg10 otb_6s_d_lg10
#define d_sign otb_6s_d_sign
#define f__cabs otb_6s_f__cabs
#define i_dnnt otb_6s_i_dnnt
#define pow_dd otb_6s_pow_dd
#define pow_di otb_6s_pow_di
#define pow_ii otb_6s_pow_ii
#define pow_ri otb_6s_pow_ri
#define r_cnjg otb_6s_r_cnjg
#define r_imag otb_6s_r_imag
#define r_sign otb_6s_r_sign
#define s_cat otb_6s_s_cat
#define s_cmp otb_6s_s_cmp
#define s_copy otb_6s_s_copy
#define sig_die otb_6s_sig_die
#define z_abs otb_6s_z_abs
#define z_div otb_6s_z_div
#define z_sqrt otb_6s_z_sqrt

/* Types */
#define C_f otb_6s_C_f
#define C_fp otb_6s_C_fp
#define E_f otb_6s_E_f
#define E_fp otb_6s_E_fp
#define H_f otb_6s_H_f
#define H_fp otb_6s_H_fp
#define I_fp otb_6s_I_fp
#define J_fp otb_6s_J_fp
#define K_fp otb_6s_K_fp
#define L_fp otb_6s_L_fp
#define Multitype otb_6s_Multitype
#define Namelist otb_6s_Namelist
#define R_fp otb_6s_R_fp
#define S_fp otb_6s_S_fp
#define U_fp otb_6s_U_fp
#define Vardesc otb_6s_Vardesc
#define Z_f otb_6s_Z_f
#define Z_fp otb_6s_Z_fp
#define address otb_6s_address
#define alist otb_6s_alist
#define cilist otb_6s_cilist
#define cllist otb_6s_cllist

#if defined(complex)
#undef complex
#endif
#define complex otb_6s_complex

#define doublecomplex otb_6s_doublecomplex
#define doublereal otb_6s_doublereal
#define flag otb_6s_flag
#define ftnint otb_6s_ftnint
#define ftnlen otb_6s_ftnlen
#define icilist otb_6s_icilist
#define inlist otb_6s_inlist
#define integer otb_6s_integer
#define integer1 otb_6s_integer1
#define logical otb_6s_logical
#define logical1 otb_6s_logical1
#define longint otb_6s_longint
#define olist otb_6s_olist
#define real otb_6s_real
#define shortint otb_6s_shortint
#define shortlogical otb_6s_shortlogical
#define uinteger otb_6s_uinteger
#define ulongint otb_6s_ulongint
