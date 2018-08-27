#!/bin/bash

#sudo -s
directory='/home/saldana/Documentos/linux-source-4.4.0/*'
myfile='/home/saldana/os-labs/lab1.5/linux_source_structure.txt'

if [ -f "$myfile" ]
then
	rm $myfile
fi
for dir in $directory; do
	if [ -d "$dir" ]; then
		echo 'Directorio: '$dir >> $myfile
		echo 'Archivos .c' >> $myfile
		cd $dir && find | grep '\.c$' >> $myfile
		echo 'Archivos .h'  >> $myfile
		cd $dir && find | grep '\.h$' >> $myfile
	fi
	done	


