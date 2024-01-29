Use "comp.sh", comp.sh is situated in my_programs/shell_script/comp.sh .  



to correctly compile with comp.sh you must specify relative path to apue.h in your c file. situated in  [...]/include/apue.h

example (compiling foo.c) :
 
[...]/shell_script/comp.sh [...]path_too_foo.c/foo (not specify .c)

note that comp.sh works only on this folder hierarchy, so don't move these files .