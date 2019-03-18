#!/bin/bash

KIT_ROOT=${PWD}

build()
{
echo ""
echo "------------------------------------------"
echo "Building Lumidigm vCOM Integration Kit"

cd VCOMExample
make

cd ${KIT_ROOT}

cd SampleCode/VCOMExampleApp
make

cd ${KIT_ROOT}
}


clean()
{
echo ""
echo "------------------------------------------"
echo "Cleaning Build of Lumidigm vCOM Integration Kit"

cd VCOMExample
make clean

cd ${KIT_ROOT}

cd SampleCode/VCOMExampleApp
make clean

cd ${KIT_ROOT}
}

usage ()
{ 
echo "-b: build the application,"
echo "-c: clean the previous build of the application,"
echo "-h: print usage,"
}

options='bch'
while getopts $options option
do
    case $option in
        b  ) build; exit;;
        c  ) clean; exit;;
        h  ) usage; exit;;
        *  ) usage; exit;;
    esac
done
build;
