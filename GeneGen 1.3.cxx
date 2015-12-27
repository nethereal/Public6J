#include <iostream>
#include <sys/stat.h>
#include <fstream>
//#include <string>
//#include <ctype.h>
//#include <unistd.h>
//#include <climits>

char* fileName;
const uint8_t geneLength = 8;
const uint16_t chromosomeLength = 256 * geneLength;
const int MB_DEFAULT = 1;

// utilfx
inline bool isInteger(const std::string & s) {
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
   char * p ;
   strtol(s.c_str(), &p, 10) ;
   return (*p == 0) ;
}
void errOutput(char* argv[]) {
		std::cout << "Usage: " << argv[0] << " <rndDepth> <Keyfile-name> <KeyfileMB>" << std::endl;
		std::cout << "  <rndDepth>	= 8 bit unsigned value (0-255) representing random depth" << std::endl;
		std::cout << "  <filename>	= filename to write to" << std::endl;
		std::cout << "  <KeyfileMB>	= optional, default 1, UInt16 value (0-65535)" << std::endl;
		std::cout << "                ^Desired MB size of keyfile" << std::endl;
		std::cout << std::endl;
}

// filefx
uint8_t CreateEmptyKeyFile(char *filename, bool dfy, unsigned long long MBs = 1ULL) {
	unsigned long long size = 128ULL*1024ULL;
	if (dfy) { size = size / geneLength; } 
	unsigned long long a[size];	
    std::ofstream myfile (filename, std::ios::out | std::ios::binary);
    // Set array to max values (1's)
    for(unsigned long long iy = 0; iy < size; iy++){ a[iy] = ULLONG_MAX; }
    // Writing routine
    for(unsigned long long i = 0; i < MBs; ++i){ myfile.write((char*)&a,size*sizeof(unsigned long long)); }
    myfile.close();
    return 0;
}

size_t getFilesize(const char* filename) {
    struct stat st;
    if(stat(filename, &st) != 0) { return 0; }
    return st.st_size;   
}
inline bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

// dnafx
unsigned char* genGene(uint8_t randomDepthStrength = 10) {
	unsigned char* gene = new unsigned char[geneLength];
	uint8_t randomDepth;
	for (uint8_t i = 0; i < geneLength; i++)
	{ 	
		randomDepth = arc4random_uniform(randomDepthStrength)+1;
		for (uint8_t depthCounter = 0; depthCounter < randomDepth; depthCounter++) { arc4random_buf(&gene[i], 1); }
	}
	return gene;
}
unsigned char* getGeneFromArray(unsigned char* geneArray, uint64_t pos) {
	unsigned char* getgene = new unsigned char[geneLength];
	for (uint64_t icnt = 0; icnt < geneLength; icnt++) {
		getgene[icnt] = geneArray[(pos + icnt)];
	}
	return getgene;
}

// mainfx
int main(int argc, char* argv[]) {
	//init vars
	unsigned char* newGene = new unsigned char[geneLength];
	//unsigned char* verify1 = new unsigned char[geneLength];
	//unsigned char* verify2 = new unsigned char[geneLength];
	
	uint64_t genomeLength = 1048576; // num bytes in 1 MB keyfile (base unit)

	// Verify arguments
	if (argc < 3 || argc > 4) { // wrong number of arguments
		std::cout << std::endl;
		std::cout << "Error: Incorrect amount of arguments" << std::endl;
		errOutput(argv);
		return 0;
	}
	if (!isInteger(argv[1])) { // arg 1 validation
		std::cout << std::endl;
		std::cout << "Error: Argument 1 not a valid integer" << std::endl;
		errOutput(argv);
		return 0;
	}
	unsigned int arg1 = atoi(argv[1]);
	if (arg1 > 255) { // arg 1 out of range (over)
		std::cout << std::endl;
		std::cout << "Error: Argument 1 outside allowed range (over)" << std::endl;
		errOutput(argv);
		return 0;
	}
	else if (arg1 < 1) { // arg 1 out of range (under)
		std::cout << std::endl;
		std::cout << "Error: Argument 1 outside allowed range (under)" << std::endl;
		errOutput(argv);
		return 0;		
	}
	unsigned int arg3;
	if (argc == 4) { // arg 3 validation
		if (!isInteger(argv[3])) { // arg 3 not a valid integer
			std::cout << std::endl;
			std::cout << "Error: Argument 3 not a valid integer" << std::endl;
			errOutput(argv);
			return 0;
		}
		arg3 = atoi(argv[3]);
		if (arg3 > 65535) { // arg 3 out of range
			std::cout << std::endl;
			std::cout << "Error: Argument 3 outside allowed range" << std::endl;
			errOutput(argv);
			return 0;
		}
	}
	else { arg3 = MB_DEFAULT; }
	char* arg2 = argv[2];
	for (unsigned int ix = 0; ix < strlen(arg2); ix++) { // arg 2 validation - alphanumeric
		if (!isalnum(arg2[ix])) {
			std::cout << std::endl;
			std::cout << "Error: Argument 2 is NOT alphanumeric!" << std::endl;
			errOutput(argv);
			return 0;
		}
	}
	std::string strarg2(arg2,strlen(arg2));
	std::string dupefyfilename;
	dupefyfilename.append("dupefy-");
	dupefyfilename.append(strarg2);
	if (file_exists(strarg2)) { // arg 2 validation - file already exists
		std::cout << std::endl;
		std::cout << "Error: Argument 2 already exists as a file!" << std::endl;
		errOutput(argv);
		return 0;
	}
	if (file_exists(dupefyfilename)) { // arg 2 validation - dupefy-file already exists
		std::cout << std::endl;
		std::cout << "Error: Argument 2 already exists as a dupefy-file!" << std::endl;
		errOutput(argv);
		return 0;
	}	

	// Verify & reserve free memory
	std::cout << "Checking mem..." << std::endl;
	genomeLength = genomeLength * arg3; // genomeLength was set to base, now X arg
	//std::cout << "gL: " << genomeLength << std::endl;
	unsigned char * genome;
	unsigned char * genomeVfy1;
	unsigned char * genomeVfy2;
	unsigned char * genomeVfy3;
	unsigned char * genomeVfy4;
	unsigned char * genomeVfy5;
	unsigned char * genomeVfy6;
	unsigned char * genomeVfy7;
	unsigned char * genomeVfy8;
	
	try {
		//std::cout << "try-gL: " << genomeLength << std::endl;
		genome = new unsigned char[genomeLength];
		//genomePTR = new unsigned char[(genomeLength / 4)];
		genomeVfy1 = new unsigned char[(genomeLength / geneLength)];
		genomeVfy2 = new unsigned char[(genomeLength / geneLength)];
		genomeVfy3 = new unsigned char[(genomeLength / geneLength)];
		genomeVfy4 = new unsigned char[(genomeLength / geneLength)];
		genomeVfy5 = new unsigned char[(genomeLength / geneLength)];
		genomeVfy6 = new unsigned char[(genomeLength / geneLength)];
		genomeVfy7 = new unsigned char[(genomeLength / geneLength)];
		genomeVfy8 = new unsigned char[(genomeLength / geneLength)];
	}
	catch (std::bad_alloc& ba) {
		std::cout << "try-gL: " << genomeLength << std::endl;
		// genomeLength = 2147483648 = 2048MB = bad_alloc!
		// genomeLength = 2146435072 = 2047MB = pass!
		std::cout << std::endl;
		std::cerr << "bad_alloc caught: " << ba.what() << " gL: " << genomeLength << std::endl;
		std::cout << "Error: Memory addressing issue , try lowering Argument 3 value!" << std::endl;
		errOutput(argv);
		return 0;
	}
	std::cout << "Mem check OK : key byte array reserved in memory" << std::endl;	
	
	// Verify & reserve disk space
	std::cout << "Reserving disk space..." << std::endl;
	uint8_t preseedFile; // Create empty file of appropriate size according to optional argv[3]
	uint8_t preseedDfy1;
	uint8_t preseedDfy2;
	uint8_t preseedDfy3;
	uint8_t preseedDfy4;
	uint8_t preseedDfy5;
	uint8_t preseedDfy6;
	uint8_t preseedDfy7;
	uint8_t preseedDfy8;
	std::string dfy1 = strarg2.append("-dfy1");
	std::string dfy2 = strarg2.append("-dfy2");
	std::string dfy3 = strarg2.append("-dfy3");
	std::string dfy4 = strarg2.append("-dfy4");
	std::string dfy5 = strarg2.append("-dfy5");
	std::string dfy6 = strarg2.append("-dfy6");
	std::string dfy7 = strarg2.append("-dfy7");
	std::string dfy8 = strarg2.append("-dfy8");
	
	uint64_t tmparg3;
	if (argc == 3) { 
		tmparg3 = 1;
		std::cout << "disk: 3 args detected " << std::endl;
	}
	else if (argc == 4) { 
		tmparg3 = arg3;
		std::cout << "disk: 4 args detected " << std::endl;
	}
	preseedFile = CreateEmptyKeyFile(arg2,false,tmparg3); 
	preseedDfy1 = CreateEmptyKeyFile((char *)dfy1.c_str(),true,tmparg3); 	
	preseedDfy2 = CreateEmptyKeyFile((char *)dfy2.c_str(),true,tmparg3); 	
	preseedDfy3 = CreateEmptyKeyFile((char *)dfy3.c_str(),true,tmparg3); 	
	preseedDfy4 = CreateEmptyKeyFile((char *)dfy4.c_str(),true,tmparg3); 	
	preseedDfy5 = CreateEmptyKeyFile((char *)dfy5.c_str(),true,tmparg3); 	
	preseedDfy6 = CreateEmptyKeyFile((char *)dfy6.c_str(),true,tmparg3); 	
	preseedDfy7 = CreateEmptyKeyFile((char *)dfy7.c_str(),true,tmparg3); 	
	preseedDfy8 = CreateEmptyKeyFile((char *)dfy8.c_str(),true,tmparg3); 	
		
	if (getFilesize(arg2) != genomeLength) { // Validate placeholder file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(arg2) << std::endl;
		std::cout << "Error while generating key file!" << std::endl;
		errOutput(argv);
		std::remove(arg2); // delete file
		bool delfail = !std::ifstream(arg2);
		if (delfail) { std::cout << "Error while attempting to delete " << arg2 << std::endl; }
		return 0;
	}
	if (getFilesize(dfy1.c_str()) != (genomeLength / 8)) { // Validate dfy1 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfy1.c_str()) << std::endl;
		std::cout << "Error while generating dfy1 file!" << std::endl;
		errOutput(argv);
		std::remove(dfy1.c_str()); // delete file
		bool delfail = !std::ifstream(dfy1.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfy1 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfy2.c_str()) != (genomeLength / 8)) { // Validate dfy2 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfy2.c_str()) << std::endl;
		std::cout << "Error while generating dfy2 file!" << std::endl;
		errOutput(argv);
		std::remove(dfy2.c_str()); // delete file
		bool delfail = !std::ifstream(dfy2.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfy2 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfy3.c_str()) != (genomeLength / 8)) { // Validate dfy3 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfy3.c_str()) << std::endl;
		std::cout << "Error while generating dfy3 file!" << std::endl;
		errOutput(argv);
		std::remove(dfy3.c_str()); // delete file
		bool delfail = !std::ifstream(dfy3.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfy3 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfy4.c_str()) != (genomeLength / 8)) { // Validate dfy4 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfy4.c_str()) << std::endl;
		std::cout << "Error while generating dfy4 file!" << std::endl;
		errOutput(argv);
		std::remove(dfy4.c_str()); // delete file
		bool delfail = !std::ifstream(dfy4.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfy4 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfy5.c_str()) != (genomeLength / 8)) { // Validate dfy5 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfy5.c_str()) << std::endl;
		std::cout << "Error while generating dfy5 file!" << std::endl;
		errOutput(argv);
		std::remove(dfy5.c_str()); // delete file
		bool delfail = !std::ifstream(dfy5.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfy5 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfy6.c_str()) != (genomeLength / 8)) { // Validate dfy6 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfy6.c_str()) << std::endl;
		std::cout << "Error while generating dfy6 file!" << std::endl;
		errOutput(argv);
		std::remove(dfy6.c_str()); // delete file
		bool delfail = !std::ifstream(dfy6.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfy6 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfy7.c_str()) != (genomeLength / 8)) { // Validate dfy7 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfy7.c_str()) << std::endl;
		std::cout << "Error while generating dfy7 file!" << std::endl;
		errOutput(argv);
		std::remove(dfy7.c_str()); // delete file
		bool delfail = !std::ifstream(dfy7.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfy7 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfy8.c_str()) != (genomeLength / 8)) { // Validate dfy8 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfy8.c_str()) << std::endl;
		std::cout << "Error while generating dfy8 file!" << std::endl;
		errOutput(argv);
		std::remove(dfy8.c_str()); // delete file
		bool delfail = !std::ifstream(dfy8.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfy8 << std::endl; }
		return 0;
	}	
	
	/*
	char *dupeFN2 = new char[dupefyfilename.length()+1];
	std::strcpy(dupeFN2, dupefyfilename.c_str());
	uint64_t fsize = getFilesize(dupeFN2);
	std::cout << fsize << " " << (genomeLength / 4) << std::endl;
	if (fsize != (genomeLength / 4)) {
		std::cout << std::endl;
		std::cout << "gF: " << fsize << std::endl;
		std::cout << "Error while generating dupify file!" << std::endl;
		errOutput(argv);
		std::remove(dupeFN2); // delete file
		bool delfail = !std::ifstream(dupeFN2);
		if (delfail) { std::cout << "Error while attempting to delete " << dupeFN2 << std::endl; }
		return 0;		
	}*/
	std::cout << "Disk space reserved OK ..." << std::endl;
	
	// Create keyfile in memory
	std::cout << "Creating key & dfy tables in memory..." << std::endl;
	for (uint64_t genCnt = 0; genCnt < genomeLength; genCnt = genCnt + 8) { // Main memory object creation loop
		newGene = genGene(arg1);
		//genomePTR[(genCnt / 8)] = newGene[0]; 
		//genomePTR[((genCnt / 8)+1)] = newGene[1]; 
		/* if(genCnt == 0) { 
			genomePTR[genCnt] = newGene[0]; 
			genomePTR[(genCnt+1)] = newGene[1]; 
		} else { 
			genomePTR[(genCnt / 8)] = newGene[0]; 
			genomePTR[((genCnt / 8)+1)] = newGene[1]; 
		} */
		
		genome[(genCnt + 0)] = genomeVfy1[genCnt / 8] = newGene[0]; 
		//genomeVfy1[genCnt] = newGene[0];		
		genome[(genCnt + 1)] = genomeVfy2[genCnt / 8] = newGene[1]; 		
		//genomeVfy2[genCnt] = newGene[1];
		genome[(genCnt + 2)] = genomeVfy3[genCnt / 8] = newGene[2]; 	
		//genomeVfy3[genCnt] = newGene[2];	
		genome[(genCnt + 3)] = genomeVfy4[genCnt / 8] = newGene[3]; 	
		//genomeVfy4[genCnt] = newGene[3];
		genome[(genCnt + 4)] = genomeVfy5[genCnt / 8] = newGene[4]; 
		//genomeVfy5[genCnt] = newGene[4];		
		genome[(genCnt + 5)] = genomeVfy6[genCnt / 8] = newGene[5];
		//genomeVfy6[genCnt] = newGene[5]; 		
		genome[(genCnt + 6)] = genomeVfy7[genCnt / 8] = newGene[6]; 
		//genomeVfy7[genCnt] = newGene[6];		
		genome[(genCnt + 7)] = genomeVfy8[genCnt / 8] = newGene[7]; 
		//genomeVfy8[genCnt] = newGene[7];		

		//for (uint8_t ie = 0; ie < geneLength; ie++) { 
		//	genome[(genCnt + ie)] = newGene[ie]; 
		//}
		delete newGene;
	}
	
	std::cout << "Finished creating keyfile in memory OK ..." << std::endl;

	// Dupify (chk for dupes)



	/* std::cout << "Dupifying keyfile in memory..." << std::endl;
	bool matchesFound = false;
	uint64_t totalelems = (((genomeLength / 4) * ((genomeLength / 4) - 1)) / 2);
	uint64_t curlvl2;	
	for (uint64_t uiPTR	= 0; uiPTR < (genomeLength / 4); uiPTR = uiPTR + 2) { // initial for statement for PTR array
		curlvl2 = uiPTR + 2;
		for (uint64_t uiPTR2 = curlvl2; uiPTR2 < (genomeLength / 4); uiPTR2 = uiPTR2 + 2) { // initial check through remaing PTR array elements
			if ((genomePTR[uiPTR] == genomePTR[uiPTR2]) && (genomePTR[(uiPTR+1)] == genomePTR[(uiPTR2+1)])) { // found maching gene PTRs!
				if (getGeneFromArray(genome,(uiPTR * 8)) == getGeneFromArray(genome,(uiPTR2 * 8))) {
					if (!matchesFound) { matchesFound = true; }
					std::cout << "FOUND MATCHING GENES!" << std::endl;
					genomePTR[uiPTR] = UCHAR_MAX;
					genomePTR[uiPTR2] = UCHAR_MAX;
					for(uint8_t rstGene = 0; rstGene < geneLength; rstGene++) { // mark matching gene locations as FF..
						std::cout << "MARKED AS FF" << std::endl;
						genome[rstGene + (uiPTR * 4)] = UCHAR_MAX; // set first gene to FF..
						genome[rstGene + (uiPTR2 * 4)] = UCHAR_MAX; // set matching gene to FF
					}
					
				}
				//delete verify1;
				//delete verify2;
			}
			std::cout << totalelems << " : " << ((uiPTR * (genomeLength / 4)) + uiPTR2) << "\r";
		}
		
	}
	std::cout << "Finished dupifying keyfile in memory!" << std::endl;
	if (matchesFound) { std::cout << "Matches set to UCHAR_MAX" << std::endl; }
	else { std::cout << "No gene matches found" << std::endl; }
	*/

	// DUPE CHECK HERE
	

	// Write keyfile in memory, to file
	std::cout << "Writing memory to file..." << std::endl;
	std::cout << "Size of genomeLength: " << genomeLength << std::endl;
	std::ofstream outFileOBJ;
	outFileOBJ.open(arg2, std::ios::out | std::ios::binary);
	outFileOBJ.write((char *)genome, genomeLength);
	outFileOBJ.close();
	std::cout << "Finished writing memory to keyfile OK ..." << std::endl;
	
	// Write dupefyles in memory, to file
	std::cout << "Writing dupefy memory to dupefyles..." << std::endl;
	std::cout << "Calculated size of each dupefy-file: " << (genomeLength / 8) << std::endl;
	
	std::string dfybase(arg2);
	std::string dfyFN1 = dfybase + "-dfy1";
	std::string dfyFN2 = dfybase + "-dfy2";
	std::string dfyFN3 = dfybase + "-dfy3";
	std::string dfyFN4 = dfybase + "-dfy4";
	std::string dfyFN5 = dfybase + "-dfy5";
	std::string dfyFN6 = dfybase + "-dfy6";
	std::string dfyFN7 = dfybase + "-dfy7";
	std::string dfyFN8 = dfybase + "-dfy8";

	outFileOBJ.open(dfyFN1.c_str(), std::ofstream::out | std::ofstream::binary);
	outFileOBJ.write((char *)genomeVfy1, (genomeLength / 8));
	outFileOBJ.close();

	outFileOBJ.open(dfyFN2.c_str(), std::ofstream::out | std::ofstream::binary);
	outFileOBJ.write((char *)genomeVfy2, (genomeLength / 8));
	outFileOBJ.close();

	outFileOBJ.open(dfyFN3.c_str(), std::ofstream::out | std::ofstream::binary);
	outFileOBJ.write((char *)genomeVfy3, (genomeLength / 8));
	outFileOBJ.close();

	outFileOBJ.open(dfyFN4.c_str(), std::ofstream::out | std::ofstream::binary);
	outFileOBJ.write((char *)genomeVfy4, (genomeLength / 8));
	outFileOBJ.close();

	outFileOBJ.open(dfyFN5.c_str(), std::ofstream::out | std::ofstream::binary);
	outFileOBJ.write((char *)genomeVfy5, (genomeLength / 8));
	outFileOBJ.close();

	outFileOBJ.open(dfyFN6.c_str(), std::ofstream::out | std::ofstream::binary);
	outFileOBJ.write((char *)genomeVfy6, (genomeLength / 8));
	outFileOBJ.close();

	outFileOBJ.open(dfyFN7.c_str(), std::ofstream::out | std::ofstream::binary);
	outFileOBJ.write((char *)genomeVfy7, (genomeLength / 8));
	outFileOBJ.close();

	outFileOBJ.open(dfyFN8.c_str(), std::ofstream::out | std::ofstream::binary);
	outFileOBJ.write((char *)genomeVfy8, (genomeLength / 8));
	outFileOBJ.close();

	
	std::cout << "Finished writing memory to dupefyles OK ..." << std::endl;
	
	return 0;
}

