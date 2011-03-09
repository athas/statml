#!/bin/bash
pdf2ps $1 $1.ps
ps2eps $1.ps $1.eps
