#!/usr/bin/env python
# encoding: utf-8
"""
untitled.py

Created by Daniel Fairchild on 2011-03-14.
Copyright (c) 2011 __MyCompanyName__. All rights reserved.
"""

import sys
import os


def main():
	fr = open(sys.argv[1], 'r')
	fw = open(sys.argv[1]+".ln", 'w')

	i = -1
	for line in fr:
		fw.write(line.rstrip() + " "+str(i)+"\n")
		i+=1

if __name__ == '__main__':
	main()
