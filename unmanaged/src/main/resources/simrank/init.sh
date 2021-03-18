#!/bin/bash
rm -rf /tmp/simrank/
mkdir -p /tmp/simrank/AL/
workdir=$(cd $(dirname $0); pwd)
cp -r $workdir/config /tmp/simrank/