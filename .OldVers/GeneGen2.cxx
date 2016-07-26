#include <iostream>
#include <sys/stat.h>
#include <fstream>

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
		std::cout << std::endl;
		std::cout << "  Usage: " << argv[0] << " <rndDepth> <Filename> <KeyfileMB>" << std::endl;
		std::cout << std::endl;
		std::cout << "  Required: " << std::endl; 
		std::cout << "    <rndDepth> :16 bit unsigned value (1-65535), random depth" << std::endl;
		std::cout << "    <Filename> :filename to write to" << std::endl;
		std::cout << std::endl;
		std::cout << "  Optional: " << std::endl;
		std::cout << "    <KeyfileMB>:16 bit unsigned value (1-65535), keyfile MB" << std::endl;
		std::cout << "                +Desired MB size of keyfile, default(MB): 1" << std::endl;
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
unsigned char* genGene(uint16_t randomDepthStrength = 10) {
	unsigned char* gene = new unsigned char[geneLength];
	uint16_t randomDepth;
	for (uint8_t i = 0; i < geneLength; i++)
	{ 	
		randomDepth = arc4random_uniform(randomDepthStrength)+1;
		for (uint16_t depthCounter = 0; depthCounter < randomDepth; depthCounter++) { arc4random_buf(&gene[i], 1); }
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
	if (arg1 > 65535) { // arg 1 out of range (over)
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
	std::cout << "Reserving memory for objects..." << std::endl;
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
	std::cout << "Memory reservation OK ..." << std::endl;	
	
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
	std::string dfybase(arg2);
	std::string dfyFN1 = dfybase + "-dfy1";
	std::string dfyFN2 = dfybase + "-dfy2";
	std::string dfyFN3 = dfybase + "-dfy3";
	std::string dfyFN4 = dfybase + "-dfy4";
	std::string dfyFN5 = dfybase + "-dfy5";
	std::string dfyFN6 = dfybase + "-dfy6";
	std::string dfyFN7 = dfybase + "-dfy7";
	std::string dfyFN8 = dfybase + "-dfy8";
	uint64_t tmparg3;
	if (argc == 3) { 
		tmparg3 = 1;
		//std::cout << "disk: 3 args detected " << std::endl;
	}
	else if (argc == 4) { 
		tmparg3 = arg3;
		//std::cout << "disk: 4 args detected " << std::endl;
	}
	preseedFile = CreateEmptyKeyFile(arg2,false,tmparg3); 
	preseedDfy1 = CreateEmptyKeyFile((char *)dfyFN1.c_str(),true,tmparg3); 	
	preseedDfy2 = CreateEmptyKeyFile((char *)dfyFN2.c_str(),true,tmparg3); 	
	preseedDfy3 = CreateEmptyKeyFile((char *)dfyFN3.c_str(),true,tmparg3); 	
	preseedDfy4 = CreateEmptyKeyFile((char *)dfyFN4.c_str(),true,tmparg3); 	
	preseedDfy5 = CreateEmptyKeyFile((char *)dfyFN5.c_str(),true,tmparg3); 	
	preseedDfy6 = CreateEmptyKeyFile((char *)dfyFN6.c_str(),true,tmparg3); 	
	preseedDfy7 = CreateEmptyKeyFile((char *)dfyFN7.c_str(),true,tmparg3); 	
	preseedDfy8 = CreateEmptyKeyFile((char *)dfyFN8.c_str(),true,tmparg3); 		
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
	if (getFilesize(dfyFN1.c_str()) != (genomeLength / 8)) { // Validate dfyFN1 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfyFN1.c_str()) << std::endl;
		std::cout << "Error while generating dfy1 file!" << std::endl;
		errOutput(argv);
		std::remove(dfyFN1.c_str()); // delete file
		bool delfail = !std::ifstream(dfyFN1.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfyFN1 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfyFN2.c_str()) != (genomeLength / 8)) { // Validate dfyFN2 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfyFN2.c_str()) << std::endl;
		std::cout << "Error while generating dfyFN2 file!" << std::endl;
		errOutput(argv);
		std::remove(dfyFN2.c_str()); // delete file
		bool delfail = !std::ifstream(dfyFN2.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfyFN2 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfyFN3.c_str()) != (genomeLength / 8)) { // Validate dfyFN3 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfyFN3.c_str()) << std::endl;
		std::cout << "Error while generating dfyFN3 file!" << std::endl;
		errOutput(argv);
		std::remove(dfyFN3.c_str()); // delete file
		bool delfail = !std::ifstream(dfyFN3.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfyFN3 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfyFN4.c_str()) != (genomeLength / 8)) { // Validate dfyFN4 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfyFN4.c_str()) << std::endl;
		std::cout << "Error while generating dfyFN4 file!" << std::endl;
		errOutput(argv);
		std::remove(dfyFN4.c_str()); // delete file
		bool delfail = !std::ifstream(dfyFN4.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfyFN4 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfyFN5.c_str()) != (genomeLength / 8)) { // Validate dfyFN5 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfyFN5.c_str()) << std::endl;
		std::cout << "Error while generating dfyFN5 file!" << std::endl;
		errOutput(argv);
		std::remove(dfyFN5.c_str()); // delete file
		bool delfail = !std::ifstream(dfyFN5.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfyFN5 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfyFN6.c_str()) != (genomeLength / 8)) { // Validate dfyFN6 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfyFN6.c_str()) << std::endl;
		std::cout << "Error while generating dfyFN6 file!" << std::endl;
		errOutput(argv);
		std::remove(dfyFN6.c_str()); // delete file
		bool delfail = !std::ifstream(dfyFN6.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfyFN6 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfyFN7.c_str()) != (genomeLength / 8)) { // Validate dfyFN7 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfyFN7.c_str()) << std::endl;
		std::cout << "Error while generating dfyFN7 file!" << std::endl;
		errOutput(argv);
		std::remove(dfyFN7.c_str()); // delete file
		bool delfail = !std::ifstream(dfyFN7.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfyFN7 << std::endl; }
		return 0;
	}	
	if (getFilesize(dfyFN8.c_str()) != (genomeLength / 8)) { // Validate dfyFN8 file
		std::cout << std::endl;
		std::cout << "gF: " << getFilesize(dfyFN8.c_str()) << std::endl;
		std::cout << "Error while generating dfyFN8 file!" << std::endl;
		errOutput(argv);
		std::remove(dfyFN8.c_str()); // delete file
		bool delfail = !std::ifstream(dfyFN8.c_str());
		if (delfail) { std::cout << "Error while attempting to delete " << dfyFN8 << std::endl; }
		return 0;
	}	
	std::cout << "Disk space reserved OK ..." << std::endl;
	
	// Create keyfile in memory
	std::cout << "Creating key & dfy tables in memory..." << std::endl;
	
	uint64_t pcSteps = 100;
	uint64_t pcStepval = genomeLength / pcSteps;
	uint64_t pc[pcSteps];
	for (uint64_t psb = 0; psb <= pcSteps; psb++) {
		if(psb == pcSteps) { break; } 
		pc[psb] = (psb * pcStepval);
	}
	uint64_t pElem = 0;
	for (uint64_t genCnt = 0; genCnt < genomeLength; genCnt = genCnt + 8) { // Main memory object creation loop
		if (genCnt >= pc[pElem]) {
			std::cout << "  " << pElem << "%" << "\r" << std::flush;
			pElem++;
		}
		newGene = genGene(arg1);
		genome[(genCnt + 0)] = genomeVfy1[genCnt / 8] = newGene[0]; 
		genome[(genCnt + 1)] = genomeVfy2[genCnt / 8] = newGene[1]; 		
		genome[(genCnt + 2)] = genomeVfy3[genCnt / 8] = newGene[2]; 	
		genome[(genCnt + 3)] = genomeVfy4[genCnt / 8] = newGene[3]; 	
		genome[(genCnt + 4)] = genomeVfy5[genCnt / 8] = newGene[4]; 
		genome[(genCnt + 5)] = genomeVfy6[genCnt / 8] = newGene[5];
		genome[(genCnt + 6)] = genomeVfy7[genCnt / 8] = newGene[6]; 
		genome[(genCnt + 7)] = genomeVfy8[genCnt / 8] = newGene[7]; 
		delete newGene;
	}
	std::cout << "  100%" << std::endl;
	std::cout << "Finished creating tables in memory OK ..." << std::endl;

	// Dupefy!
	std::cout << "Dupifying key table in memory..." << std::endl;
	uint64_t vfytop = (genomeLength / 8);
	uint64_t totalvfys = (vfytop / 2 ) * (vfytop - 1);
	uint64_t percSteps = 100;
	uint64_t percStepval = totalvfys / percSteps;
	uint64_t perc[percSteps];
	for (uint64_t ps = 0; ps <= percSteps; ps++) {
		if(ps == percSteps) { break; } 
		perc[ps] = (ps * percStepval);
	}
	uint64_t currelem = 0;
	bool matches = false;
	uint64_t percElem = 0;
	for (uint64_t lvl1a = 0; lvl1a < vfytop; lvl1a++) {
		if (currelem >= perc[percElem]) {
			std::cout << "  " << percElem << "%" << "\r" << std::flush;
			percElem++;
		}
		for (uint64_t lvl1b = (lvl1a + 1); lvl1b < vfytop; lvl1b++) {
			if (genomeVfy1[lvl1a] == genomeVfy1[lvl1b]) {
				if (genomeVfy2[lvl1a] == genomeVfy2[lvl1b]) {
					if (genomeVfy3[lvl1a] == genomeVfy3[lvl1b]) {
						if (genomeVfy4[lvl1a] == genomeVfy4[lvl1b]) {
							if (genomeVfy5[lvl1a] == genomeVfy5[lvl1b]) {
								if (genomeVfy6[lvl1a] == genomeVfy6[lvl1b]) {
									if (genomeVfy7[lvl1a] == genomeVfy7[lvl1b]) {
										if (genomeVfy8[lvl1a] == genomeVfy8[lvl1b]) {
											if (!matches) { matches = true; }
											std::cout << std::endl;
											std::cout << "MATCHING GENE FOUND" << std::endl;
											std::cout << "Matchine gene pos1: " << (lvl1a * 8) << std::endl;
											std::cout << "Matchine gene pos2: " << (lvl1b * 8) << std::endl;
										}
									}
								}
							}
						}
					}
				}
			}
			currelem++;
		}
	}
	std::cout << "  100%" << std::endl;
	if (matches) {
		std::cout << "Dupify finished with errors, matches were found!" << std::endl;
		// add meat to output / recovery options
	}
	else {
		std::cout << "Dupifying key table in memory OK ..." << std::endl;
	}

	// Write keyfile in memory, to file
	std::cout << "Writing key table memory to file..." << std::endl;
	std::ofstream outFileOBJ;
	outFileOBJ.open(arg2, std::ios::out | std::ios::binary);
	outFileOBJ.write((char *)genome, genomeLength);
	outFileOBJ.close();
	std::cout << "Finished writing key table memory to keyfile OK ..." << std::endl;
	
	// Write dupefyles in memory, to file
	std::cout << "Writing dupefy memory to dfyX files..." << std::endl;
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
	std::cout << "Finished writing memory to dfyX files OK ..." << std::endl;
	
	return 0;
}

