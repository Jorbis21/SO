if test -d $1; then
    rmdir $1;
else
    mkdir $1;
fi

cd $1;
mkdir subdir;
touch fichero1;
echo 1234567890 > fichero2;
ln -s fichero2 enlaceS;
ln fichero1 enlaceH;

stat subdir;
stat fichero1;
stat fichero2;
stat enlaceS;
stat enlaceH;