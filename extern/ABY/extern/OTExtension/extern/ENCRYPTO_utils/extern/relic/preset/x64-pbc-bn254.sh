#!/bin/bash
cmake -DWSIZE=64 -DRAND=UDEV -DSHLIB=OFF -DSTBIN=ON -DTIMER=CYCLE -DCHECK=off -DVERBS=off -DARITH=x64-asm-254 -DFP_PRIME=254 -DFP_METHD="INTEG;INTEG;INTEG;MONTY;LOWER;SLIDE" -DCOMP="-O3 -funroll-loops -fomit-frame-pointer -finline-small-functions -march=native -mtune=native" -DFP_PMERS=off -DFP_QNRES=on -DFPX_METHD="INTEG;INTEG;LAZYR" -DPP_METHD="LAZYR;OATEP" $1
