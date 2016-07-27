#!/usr/bin/env python
# -*- coding: utf-8 -*-
#  Gen6jMessage-1.0.py
#  Copyright 2016 doon <doon@xulu.my.domain>

import random
import binascii
bytesPerGene = 8
charsetLen = 256
incr = bytesPerGene * charsetLen
def startup(args):
	if len(args) != 3:
		usage()
		print "ERROR: Wrong number of arguments:",len(args)
		return False
def usage():
	print " "
	print " $ python script.py <keyfile.key> <inputfile>"
	print " "
	print " <keyfile.key> = must be 6j formatted keyfile."
	print " <inputfile> = ASCII character input file, max 256 chars"
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
def padMsg(encmsg,genome,ptmsg):
	paddedmsg = encmsg
	for i in range(0,(256-len(ptmsg)),1):
		ascchar = random.randint(128,255)
		paddedmsg = paddedmsg + getGene(genome,ascchar)
	return paddedmsg
def main(args):
	if startup(args) == False:
		print "Error on startup, quitting.."
		return
	bytesPerGene = 8
	genesPerRow = 256
	retdata = get_bytes_from_file(args[1])
	genome = bytearray(retdata)
	tmpGene = bytearray(8)
	ptmsg = open(args[2], "r").read()
	for cr in ptmsg:
		if ord(cr) >= 128 or ord(cr) < 0:
			print "ERROR in input file - non ASCII char detected - quitting"
			return
	if len(ptmsg) > 128:
		print "ERROR in input file - more than 128 characters: ",len(ptmsg)
		return
	encmsg = bytearray()
	for ch in ptmsg:
		chartoget = ord(ch)
		curGene = getGene(genome,chartoget)
		if curGene == False:
			print "ERROR: keyfile expired! Quitting..."
			return
		encmsg = encmsg + curGene
	rdyMsg = padMsg(encmsg,genome,ptmsg)
	msgFilename = str(binascii.hexlify(bytearray(rdyMsg[0:8]))).upper()
	msgFilename = 'devmsg-' + msgFilename + '.6jm'

	newMsgFile = open(msgFilename,"wb")
	newMsgFile.write(rdyMsg)
	print "File created: ",msgFilename
	newFile = open(args[1],"wb")
	newFile.write(genome)
if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
