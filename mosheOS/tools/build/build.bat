:: OUTPUT FIRST:
	cls
	@echo ===========
	@echo Building...
	@echo ===========

:: SET VARIABLES:
	@set original_dir=%~dp0
	@cd ../../moshe-system
	@set system_dir=%CD%
	
:: LAUNCH CYGWIN:
	@echo off
	C:
	chdir C:\cygwin\bin

:: EXECUTE SHELL SCRIPT:
	bash --login -i "%original_dir%/build.sh" "%system_dir%" "%original_dir%"

:: RETURN TO ORIGINAL FOLDER:
	chdir "%original_dir%"

:: MOVE THE KERNEL TO THE FLOPPY
	@move /Y moshe-kernel.bin A:\moshe-kernel.bin
	
:: DONE:
	@echo !DONE!