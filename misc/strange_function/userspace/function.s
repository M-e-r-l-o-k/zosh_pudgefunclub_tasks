	.file	"function.c"
	.ignore	ld_st_style
	.ignore	strict_delay
	.text
	.global	func
	.type	func, #function
	.align	8
func:
	{
	  setwd	wsz = 0x4, nfx = 0x1, dbl = 0x0
	  return	%ctpr3
	  adds,0,sm	%r5, %r4, %g16
	  adds,1	%r0, %r1, %g17
	  adds,2,sm	%r6, %r3, %g18
	  adds,3,sm	%r2, %r7, %g19
	}
	{
	  adds,0,sm	%g17, %g16, %g16
	}
	{
	  adds,0,sm	%g19, %g18, %g17
	}
	{
	  adds,0	%g16, %g17, %g16
	}
	{
	  nop 1
	  sxt,0	0x2, %g16, %r0
	}
	{
	  ct	%ctpr3
	}
	.size	func, .- func
	.weak	elbrus_optimizing_compiler_v1.23.19_Jun_19_2019
	elbrus_optimizing_compiler_v1.23.19_Jun_19_2019 = 0x0
