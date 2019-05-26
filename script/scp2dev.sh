#!/bin/bash
curdir=`dirname $0`
scp $curdir/../../../lib/bin/liblicom.so user@10.190.3.239:/home/xcu/lib/bin/
scp $curdir/../../../lib/bin/libfpgasim.so user@10.190.3.239:/home/xcu/lib/bin/
scp $curdir/../../../lib/bin/libtestcommon.so user@10.190.3.239:/home/xcu/lib/bin/
scp $curdir/../../../lib/bin/libtestsim.so user@10.190.3.239:/home/xcu/lib/bin/
scp $curdir/../../../lib/bin/libtestdpdk.so user@10.190.3.239:/home/xcu/lib/bin/

