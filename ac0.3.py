#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  ac0.3.py
#  
#  Copyright 2016 doon <doon@xulu.my.domain>
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  
#  
from array import *
import random
import operator


def usage():
	print " "
	print "	Usage "
	print " "
	print " 	python script.py <keyfile.key>"
	print " "
	print " 	<keyfile.key> must be a valid 6j keyfile"
	print " "
	
def getKeyfile(infile):
	if infile == "":
		jfilename = raw_input("Please enter keyfile name including .key:       ")
	else:
		jfilename = infile
	if not os.path.isfile(jfilename):
		print "ERROR: file does not exist in path"
		return False	
	if right(jfilename, 4) != ".key":
		print "ERROR: must end in .key!"
		return False
	if os.path.getsize(jfilename) < 1000:
		print "ERROR: size:", os.path.getsize(jfilename)
		return False
	keyfileBytes = getKeyfileData(jfilename)
	#ensure that keyfileBytes is a clean multiple of 1MB		
	if not isNumber(len(keyfileBytes)/1048576):
		print "ERROR: keyfile is an improper file size"
		return false
	if os.path.isfile(jfilename):
		print "Filesize detected in bytes:     ", os.path.getsize(jfilename)
		return jfilename	

def getRndChrom():
	r = random.SystemRandom()
	return r.randrange(128,255)

def isNumber(s):
	try:
		int(s)
	except ValueError:
		return False
	return True
def getKeyfileData(filename):
	return open(filename, "rb").read()
def right(s, amount):
	return s[-amount:]
def left(s,amount):
	return s[amount]

def main(args):
	if len(args) != 2:
		print "ERROR: See usage:"
		usage()
		return
	#verify keyfile here
	KEYFILE = getKeyfile(args[1])
	if not KEYFILE:
		print "ERROR LOADING KEYFILE"
		usage()
		return	
	jinput = raw_input("Enter msg:\n")
	if len(jinput) > 256:
		print "ERROR Message length too long!"
		usage()
		return
	strMesg = ""
	for char in jinput:
		strMesg = strMesg + char
	charMesg = ""
	for char in jinput:
		charMesg = charMesg + str(ord(char)) + " "
	#print charMesg
	#print str.split(charMesg)
	chrArray = str.split(charMesg)
	print "Length of message: ",len(chrArray)
	print "adding padding with whitespace genes..."
	while True:
		charMesg = charMesg + str(getRndChrom()) + " "
		chrArray = str.split(charMesg)
		if len(chrArray) >= 255:
			break
	print "Length of message: ",len(chrArray)
	#print chrArray
	
	incr = 8 * 256
	with open(KEYFILE, 'rb') as f:
		data = f.read()
	print "length of data: ",len(data)	
	
	encryptedMsg = array('i')
	
	for ltrGene in chrArray:
		tmpGene = ltrGene
		while True:
			tGene = data.itemgetter(slice(tmpGene, tmpGene)+8)
			if tGene[tmpGene] == 0 and tGene[tmpGene+1] == 0 and tGene[tmpGene+2] == 0 and tGene[tmpGene+3] == 0 and tGene[tmpGene+4] == 0 and tGene[tmpGene+5] == 0 and tGene[tmpGene+6] == 0 and tGene[tmpGene+7] == 0:
				tGene = tGene + incr
				continue
			if tGene[tmpGene] != 0 and tGene[tmpGene+1] != 0 and tGene[tmpGene+2] != 0 and tGene[tmpGene+3] != 0 and tGene[tmpGene+4] != 0 and tGene[tmpGene+5] != 0 and tGene[tmpGene+6] != 0 and tGene[tmpGene+7] != 0:
				encryptedMsg.append(tGene[tmpGene])
				encryptedMsg.append(tGene[tmpGene+1])
				encryptedMsg.append(tGene[tmpGene+2])
				encryptedMsg.append(tGene[tmpGene+3])
				encryptedMsg.append(tGene[tmpGene+4])
				encryptedMsg.append(tGene[tmpGene+5])
				encryptedMsg.append(tGene[tmpGene+6])
				encryptedMsg.append(tGene[tmpGene+7])
				break
	print len(encryptedMsg)

	#os.urandom()
	#

if __name__ == '__main__':
    import sys
    import os
    
    sys.exit(main(sys.argv))
