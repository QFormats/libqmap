if [[ $1 == "" ]]; then
    echo "please give path to dist source files Ö.ö"
    exit 1
fi

echo "copy source and includes to $1"

mkdir -p $1/qmap
cp -R include $1
cp src/*.cpp $1/qmap/
cp src/*.h $1/qmap/