#!/usr/bin/env python
# -*- coding: utf-8 -*-
#  Read6jMessage-0.1.py
#  Copyright 2016 doon <doon@xulu.my.domain>

import random
import binascii
import os.path

bytesPerGene = 8
charsetLen = 256
incr = bytesPerGene * charsetLen
def startup(args):
	if len(args) != 3:
		usage()
		print "ERROR: Wrong number of arguments:",len(args)
		return False
	else:
		sKeyFile = args[1]
		sMsgFile = args[2]
		if chkMessage(sMsgFile) == False:
			return False
		if chkKeyfile(sKeyFile) == False:
			return False
		
def chkKeyfile(keyfile):
	if os.path.isfile(keyfile) == False:
		print "ERROR: keyfile supplied does not exist!"
		return False
	if os.path.getsize(keyfile) % 1048576 != 0:
		print "ERROR: incorrect file size for keyfile"
		return False
	return True
	
def chkMessage(message):
	if os.path.isfile(message) == False:
		print "ERROR: Not a valid message file!"
		return False
	if os.path.getsize(message) != 2048:
		print "ERROR: Message file is of an incorrect size!"
		return False
	return True

def usage():
	print " "
	print " $ python script.py <keyfile.6jk> <message.6jm>"
	print " "
	print " <keyfile.6jk> = must be 6j formatted keyfile."
	print " <message.6jm> = ASCII character input file, max 256 chars"
	print " "

def get_bytes_from_file(filename):  
    return open(filename, "rb").read()  

def getGene(genome,ascval):
	tmplocalgene = bytearray(8)
	current = 0
	while True:
		curr = (ascval*8) + (current * incr)
		tmplocalgene = genome[curr:curr+8]
		current = current + 1
		if tmplocalgene[0] != 255 and tmplocalgene[1] != 255 and tmplocalgene[2] != 255 and tmplocalgene[3] != 255 and tmplocalgene[4] != 255 and tmplocalgene[5] != 255 and tmplocalgene[6] != 255 and tmplocalgene[7] != 255:
			genome[curr] = 255
			genome[curr+1] = 255
			genome[curr+2] = 255
			genome[curr+3] = 255
			genome[curr+4] = 255
			genome[curr+5] = 255
			genome[curr+6] = 255
			genome[curr+7] = 255
			return tmplocalgene
		if (curr+8) > (len(genome)/2):
			print "Keyfile expired! Generate a new keyfile!"
			return False

def getAsciiFromGene(gene,keyArray):

	return ord(0)
	

def main(args):
	if startup(args) == False:
		print "Error on startup, quitting..."
		return

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
