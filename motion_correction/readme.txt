# System calls by motion_qc.cpp, and what to do with them.

## Shell builtins to replace with Posix functions
rename
mkdir

## HRRT programs requiring precise path
e7_fwd_u
e7_sino_u
ecat_alignlinear
ecat_reslice
gsmooth_ps
hrrt_osem_3d_u or param '-x'  (hrrt_osem3d is current)
lmhistogram                   (lmhistogram_mp is current)
matcopy
motion_distance

## Other programs requiring precise path
gnuplot

## HRRT programs called by motion_correct_recon
MAF_join
e7_fwd_u
e7_sino_u
ecat_alignlinear
ecat_make_air
ecat_reslice
gnuplot
gsmooth_ps
if2e7
invert_air
je_hrrt_osem3d
lmhistogram_u
matcopy
mkdir
rename
volume_reslice
