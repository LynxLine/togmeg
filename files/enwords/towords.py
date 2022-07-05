#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import re
import sys
import codecs

fi = codecs.open('all.txt', 'r', 'utf-8', 'ignore')
fo = codecs.open('words.txt', 'w', 'utf-8', 'ignore')
line = fi.readline()
while line:
    ws = re.findall(r'\w+', line, re.U)
    for w in ws: 
        if w != "":
            print >>fo,w.lower()
    try:
        line = fi.readline()
    except:
        fi.read(1)
fi.close()
