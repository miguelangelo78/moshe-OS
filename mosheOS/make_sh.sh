# GO TO MAKEFILE DIRECTORY
	cd "$1"
	
# SHOW BUILD MESSAGE
	printf "===========\n   Building\n===========\n\n"
	
#BUILD IT:
	make -f make.mak all

printf "\n===========\n   Press ENTER to launch the Kernel!\n===========\n"
read
	
#LAUNCH IT:
	make -f make.mak launch
