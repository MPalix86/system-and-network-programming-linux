#!/bin/bash

func(){
	path1="/Users/mircopalese/MEGAsync/Università/Magistrale/1 anno/Systems and network programming/my_programs"
	path2="/Users/mircopalese/MEGAsync/Università/Magistrale/1 anno/Systems and network programming/my_programs/exec";
	cur_path=$(pwd);
	
	
		
	if [ "$cur_path" == "$path2" ] || [ "$cur_path" == "$path1" ]; then
		sel_path="$path2"
	else
		echo "cannot move created file, to auto move created file you should copile from my_programs folder or from exec folder"
		exit 1
	fi
	

	if mv $1 "$sel_path" > /dev/null 2>&1  ; then 
		echo "FILE CREATED AND MOVED IN .../exec"
			if mv "$1.dSYM" "$sel_path" > /dev/null 2>&1  ; then 
				echo "debug FILE CREATED AND MOVED IN .../exec"
			else
				echo "debug file FILE CREATED BUT NOT MOVED"
			fi
	else
		echo "FILE CREATED BUT NOT MOVED"
	fi
	
	
}


if gcc  -g -I../include -Wall -DMACOS -D_DARWIN_C_SOURCE  $1.c -o $1 -Llib -lapue > erro 2>&1; then
	func $1
elif gcc  -g -I../include -Wall -DMACOS -D_DARWIN_C_SOURCE  $1.c -o $1 -L../lib -lapue; then
	func $1
else	
	echo "THERE ARE SOME COMPILING ERROR, CHECK ABOVE !"
fi



	
	
	
	
	
	
	
	
	
	
	
	