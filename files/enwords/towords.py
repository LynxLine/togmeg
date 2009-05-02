#!/usr/bin/env python

import os
import re
import sys

fi = sys.stdin #open('book.txt', 'r')
line = fi.readline()
while line:
    ws = re.split(r'[^a-zA-Z]+', line)
    for w in ws: 
        if w != "":
            print w.lower()
    line = fi.readline()
fi.close()
