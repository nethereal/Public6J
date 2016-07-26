#!/usr/bin/env python
# -*- coding: utf-8 -*-
#  test2jcl.py
#  Copyright 2016 doon <doon@xulu.my.domain>

import random

incr = 8 * 256

def startup(args):
	if len(args) != 3:
		print "ERROR: Wrong number of arguments:",len(args)
		usage()
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
	print "keyfile data: ",type(genome)
	print "keyfile bytes:",len(genome)
	print "genome length:",((len(genome) / bytesPerGene) / genesPerRow)
	print "Genome loaded"
	ptmsg = open(args[2], "r").read()
	encmsg = bytearray()
	for ch in ptmsg:
		chartoget = ord(ch)
		curGene = getGene(genome,chartoget)
		encmsg = encmsg + curGene
		print str(chartoget),":  ",curGene[0],",",curGene[1],",",curGene[2],",",curGene[3],",",curGene[4],",",curGene[5],",",curGene[6],",",curGene[7]
	print "nmlmsg len: ",len(ptmsg)
	print "encmsg len: ",len(encmsg)
	rdyMsg = padMsg(encmsg,genome,ptmsg)
	print "rdyMsg len: ",len(rdyMsg)
	print "bakchk len: ",(len(rdyMsg)/8)
	print "*above number should be 256"
	print "Writing msg file..."
	newMsgFile = open("encmsg.6jm","wb")
	newMsgFile.write(rdyMsg)
	print "Writing keyfile modifications.."
	newFile = open(args[1],"wb")
	newFile.write(genome)

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
