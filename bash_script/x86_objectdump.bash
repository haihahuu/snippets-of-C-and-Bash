#!/bin/bash
for i in "$@"
do
	case $i in
		-b=* | --binary=* )
			binary="${i#*=}"
			shift
			;;
		-c=* | --code=*)
			bitcod="${i#*=}"
			shift
			;;
		--default )
			DEFAULT=YES
			shift
			;;
		*)
			;;
	esac
done
bitcode=${bitcod:-16}
echo "bitcode: $bitcode"
execute_file='../C/create_binary_file_from_input'
filename=`exec $execute_file $binary`
if [ $bitcode -eq 16 ]; then
	para='-D -Mintel,i8086 -b binary -m i386'
else
	if [ $bitcode -eq 32 ]; then
		para='-D -Mintel,i386 -b binary -m i386'
	else
		para='-D -Mintel,x86-64 -b binary -m i386'
	fi
fi
echo "objdump $para $filename"
exec objdump $para $filename
rm $filename
