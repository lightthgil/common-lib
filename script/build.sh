#!/bin/bash
curdir=`dirname $0`
cd $curdir/..
make clean
make
