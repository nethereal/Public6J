#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  test2jcl.py
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

incr = 8 * 256


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
	


def main(args):
	bytesPerGene = 8
	genesPerRow = 256
	retdata = get_bytes_from_file(args[1])
	genome = bytearray(retdata)
	tmpGene = bytearray(8)
	
	print "keyfile data: ",type(genome)
	print "keyfile bytes:",len(genome)
	print "genome length:",((len(genome) / bytesPerGene) / genesPerRow)
	print "Genome loaded"
	ptmsg = "The secret message haiku"

	for ch in ptmsg:
		chartoget = ord(ch)
		curGene = getGene(genome,chartoget)
		print str(chartoget),":  ",curGene[0],",",curGene[1],",",curGene[2],",",curGene[3],",",curGene[4],",",curGene[5],",",curGene[6],",",curGene[7]
	
	newFile = open(args[1],"wb")
	newFile.write(genome)

    
    

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
