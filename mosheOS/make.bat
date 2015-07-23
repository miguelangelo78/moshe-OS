cls

:: SET MAKE DIRECTORY:
	@set make_root=%~dp0%

:: LAUNCH CYGWIN:
	@echo off
	C:
	chdir C:\cygwin\bin
	
:: EXECUTE SHELL SCRIPT:
	bash --login -i "%make_root%/make_sh.sh" "%make_root%"