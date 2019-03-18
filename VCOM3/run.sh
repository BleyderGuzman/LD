#! /bin/sh
run()
{
rootdir=`pwd`
testexec="SampleCode/VCOMExampleApp/bin/VCOMExampleApp"

if [ ! -f $testexec ]; then
    echo "Test executable not found. Rebuilding from source."
    cd $rootdir
    ./build.sh -b
fi

echo ""
echo "------------------------------------------"
echo "Running test executable"

$testexec
}


usage ()
{ 
echo "-b: build the application,"
echo "-c: clean the previous build of the application,"
echo "-h: print usage,"
}

options='rdh'
while getopts $options option
do
    case $option in
        r  ) run; exit;;
        h  ) usage; exit;;
        *  ) usage; exit;;
    esac
done
run
