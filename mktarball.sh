#!/bin/bash

major=`grep MAJOR Version.h | awk 'BEGIN{} {print substr($3, 1, length($3)-1);}'`
minor=`grep MINOR Version.h | awk 'BEGIN{} {print substr($3, 1, length($3)-1);}'`
release=`grep RELEASE Version.h | awk 'BEGIN{} {print substr($3, 1, length($3)-1);}'`
build=`grep BUILD Version.h | awk 'BEGIN{} {print substr($3, 1, length($3)-1);}'`

filename=BaySimulatorBin-$major.$minor.$release.$build.tar.gz

echo Creating $filename
tar czvf $filename Bin
