:: OUTPUT FIRST:
	cls
	@echo ===========
	@echo Building...
	@echo ===========

:: SET VARIABLES:
	:: GO TO PROJECT ROOT FOLDER
		@cd ../../
	@set moshe_root=%CD%
	@set build_dir=%CD%\tools\build
	@set system_dir=%moshe_root%\moshe-system
	
:: LAUNCH CYGWIN:
	@echo off
	C:
	chdir C:\cygwin\bin

:: EXECUTE SHELL SCRIPT:
	bash --login -i "%build_dir%/build.sh" "%system_dir%" "%build_dir%" "%moshe_root%"

:: RETURN TO ORIGINAL FOLDER:
	chdir "%build_dir%"

:: MOVE THE KERNEL TO THE FLOPPY
	@move /Y moshe-kernel.bin A:\moshe-kernel.bin
	
:: DONE:
	@echo !DONE!