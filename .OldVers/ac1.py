#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys

VERSION = "0.1"
AUTHOR = "Jamie Courtes"
DATE = "7/23/2016"
SCRIPTNAME = "6j Builder"
FILENAME = __file__
KEYFILE = ""

def isNumber(s):
	try:
		int(s)
	except ValueError:
		return False
	return True

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
		return jfilename
		
def getInputFile(infile):
	if infile == "":
		jfilename = raw_input("Please enter input message file name (must be 2048 bytes!):       ")
	else:
		jfilename = infile
	if not os.path.isfile(jfilename):
		print "ERROR: file does not exist in path"
		return False	
	if os.path.getsize(jfilename) >= 2049:
		print "ERROR: input file size:", os.path.getsize(jfilename)
		print "       MUST BE NO GREATER THAN 2048"
		return False
	#Must confirm ascii only encoding...	
		
	
def getKeyfileData(filename):
	return open(filename, "rb").read()
def right(s, amount):
	return s[-amount:]
def left(s,amount):
	return s[amount]
def startup():
	print " "
	print "Script:                         ",SCRIPTNAME 
	print "Version                         ",VERSION
	print "Author:                         ",AUTHOR
	print "Date:                           ",DATE
	print "Filename:                       ",FILENAME
	print "Current path(getcwd():          ",os.getcwd()
	print "Current file:                   ",FILENAME
	print " "
	
def usage():
	cmdstring = "	python " + FILENAME + " keyfile.key inputmsgfile"
	print " "
	print "	Usage:"
	print "        ", cmdstring
	print " "
	print " <keyfile.key>	6j generated keyfile"
	print " <inputmsgfile>	input message"
	print " *Limitations for input file:"
	print "			ASCII 128 Charset"
	print "			256 character limit"
							
def EncryptMsg(sMessage):
	print "EncryptMsg() input: ", sMessage

def main(args):
	startup()
	print "Argument count detected:        ", len(args)
	#print args
	
	if len(args) != 3:
		print " "
		print "ERROR: Wrong number of arguments supplied!"
		usage()
		return
	
	#check keyfile
	keyFile = getKeyfile(args[1])
	if not keyFile:
		print "ERROR LOADING KEYFILE"
		return
	KEYFILE = keyFile

	#check input file
	INPUTFILE = 


if __name__ == '__main__':
    sys.exit(main(sys.argv))
