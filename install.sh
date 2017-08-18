compile_array=('memory' 'map' 'json' 'log' 'main' 'scheduler')

for moudle in ${compile_array[*]}
do
	cd ./src/$moudle
	echo "compiling module => [`pwd`]"
	echo "====================================================="
	make clean
	make
	if [ $? != 0 ]
	then
		echo 'compiling modulw failed...'
		exit -1
	fi
	echo "====================================================="
	cd -
done
