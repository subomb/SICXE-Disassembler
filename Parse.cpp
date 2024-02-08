#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

map<string, pair<string, bool>> opcodeMap = {
    {"18", {"ADD", false}},
    {"58", {"ADDF", false}},
    {"90", {"ADDR", true}},
    {"40", {"AND", false}},
    {"B4", {"CLEAR", true}},
    {"28", {"COMP", false}},
    {"88", {"COMPF", false}},
    {"A0", {"COMPR", true}},
    {"24", {"DIV", false}},
    {"64", {"DIVF", false}},
    {"9C", {"DIVR", true}},
    {"C4", {"FIX", false}},
    {"C0", {"FLOAT", false}},
    {"F4", {"HIO", false}},
    {"3C", {"J", false}},
    {"30", {"JEQ", false}},
    {"34", {"JGT", false}},
    {"38", {"JLT", false}},
    {"48", {"JSUB", false}},
    {"00", {"LDA", false}},
    {"68", {"LDB", false}},
    {"50", {"LDCH", false}},
    {"70", {"LDF", false}},
    {"08", {"LDL", false}},
    {"6C", {"LDS", false}},
    {"74", {"LDT", false}},
    {"04", {"LDX", false}},
    {"D0", {"LPS", false}},
    {"20", {"MUL", false}},
    {"60", {"MULF", false}},
    {"98", {"MULR", true}},
    {"C8", {"NORM", false}},
    {"44", {"OR", false}},
    {"D8", {"RD", false}},
    {"AC", {"RMO", true}},
    {"4C", {"RSUB", false}},
    {"A4", {"SHIFTL", true}},
    {"A8", {"SHIFTR", true}},
    {"F0", {"SIO", false}},
    {"EC", {"SSK", false}},
    {"0C", {"STA", false}},
    {"78", {"STB", false}},
    {"54", {"STCH", false}},
    {"80", {"STF", false}},
    {"D4", {"STI", false}},
    {"14", {"STL", false}},
    {"7C", {"STS", false}},
    {"E8", {"STSW", false}},
    {"84", {"STT", false}},
    {"10", {"STX", false}},
    {"1C", {"SUB", false}},
    {"5C", {"SUBF", false}},
    {"94", {"SUBR", true}},
    {"B0", {"SVC", true}},
    {"E0", {"TD", false}},
    {"F8", {"TIO", false}},
    {"2C", {"TIX", false}},
    {"B8", {"TIXR", true}},
    {"DC", {"WD", false}}
};

int hexStringToInt(const string &str) {
    stringstream ss(str);
    int result;
    ss >> hex >> result;
    return result;
}

string calculateAddressingMode(bool n, bool i) {
    if (!n && !i) return "SIC"; // Corrected as per your instruction
    if (n && i) return "simple";
    if (n) return "indirect";
    if (i) return "immediate";
    return "simple"; // Default case
}

string calculateIndexingMode(bool x, bool b, bool p, bool e) {
    if (e) return "absolute";
    if (x && b) return "base_indexed";
    if (x && p) return "pc_indexed";
    if (x) return "indexed";
    if (b) return "base";
    if (p) return "pc";
    return "absolute";
}

void processObjectCode(const string& objectCode, ofstream& outputFile, int format) {
    if (objectCode.empty()) return;
    
    string opcodeHex = objectCode.substr(0, 2);
    bitset<8> opBits(hexStringToInt(opcodeHex));
    int opcodeInt = (int)(opBits.to_ulong()) & 0xFC;  // Perform the masking op

    stringstream sstream;
    sstream << hex << uppercase << setw(2) << setfill('0') << opcodeInt;
    string opcode = sstream.str();

    auto search = opcodeMap.find(opcode);
    if (search == opcodeMap.end()) return;

    string mnemonic = search->second.first;
    outputFile << left << setw(15) << mnemonic << setw(18) << (format == 2 ? "2" : format == 3 ? "3" : "4");

    if (format == 2) {
        outputFile << setw(36) << " " << setw(8) << objectCode << endl;
        return;
    }

    bool n = opBits[1];
    bool i = opBits[0];

    int disp;
    bool x, b, p, e = false;
    if (format == 4) {
        e = ((hexStringToInt(objectCode.substr(3, 1)) & 0x1) != 0); 
        disp = hexStringToInt(objectCode.substr(3)) & 0x0FFFFF;
    } else {
        disp = hexStringToInt(objectCode.substr(2, 3)) & 0xFFF;
        bitset<12> dispBits(disp);
        x = dispBits[11];
        b = dispBits[10];
        p = dispBits[9];
        // For Format 3, this will always be 0, so no need to set e here.
    }

    string oat = calculateAddressingMode(n, i);
    string taam = calculateIndexingMode(x, b, p, e);

    outputFile << setw(18) << oat << setw(18) << taam << setw(18) << objectCode << endl;
}

int main(int argc, char** argv) {
    if(argc < 2) {
        cerr << "Usage: " << argv[0] << " [input files...]" << endl;
        return 1;
    }

    // Loop through each file passed as a command-line argument
    for(int i = 1; i < argc; ++i) {
        ifstream inputFile(argv[i]);
        if (!inputFile.is_open()) {
            cerr << "Error opening file: " << argv[i] << endl;
            continue; // Skip to the next file
        }

        // Create an output file name based on the input file name
        string inputFilename(argv[i]);
        size_t lastDotIndex = inputFilename.find_last_of(".");
        string baseFilename = (lastDotIndex == string::npos) ? inputFilename : inputFilename.substr(0, lastDotIndex);
        string outputFilename = baseFilename + "_struct.txt";

        ofstream outputFile(outputFilename);
        if (!outputFile.is_open()) {
            cerr << "Error opening output file: " << outputFilename << endl;
            inputFile.close(); // Don't forget to close the input file before continuing
            continue; // Skip to the next file
        }

        outputFile << left << setw(15) << "INSTR" << setw(18) << "FORMAT" << setw(18) << "OAT"
                   << setw(18) << "TAAM" << setw(18) << "OBJ" << endl;

        string line;
    while (getline(inputFile, line)) {
        if (line.empty() || line[0] == 'E' || line[0] != 'T') 
            continue; // Skip lines that are empty, start with 'E' or do not start with 'T'

        int idx = 9; // To skip the first 8 characters
        while (idx < (int)line.length()) {
            if (idx + 2 > line.length()) {
                break; // Break out of the while loop to avoid further processing on this line.
            }

            string opcodeHex = line.substr(idx, 2);
            bitset<8> opBits(hexStringToInt(opcodeHex));
            int opcodeInt = (int)(opBits.to_ulong()) & 0xFC;

            stringstream sstream;
            sstream << hex << uppercase << setw(2) << setfill('0') << opcodeInt;
            string maskedOpcode = sstream.str();

            auto search = opcodeMap.find(maskedOpcode);
            if (search == opcodeMap.end()) {
                idx += 2;
                continue;
            }

            int format = 3; // Default format
            if (search->second.second) format = 2;
            else {
                if (idx + 3 > line.length()) {
                    cerr << "Index " << idx + 3 << " is out of range for line: " << line << endl;
                    break;
                }
                string extendedBitCheck = line.substr(idx + 2, 1); // Moving to the proper position to check extended bit.
                if (hexStringToInt(extendedBitCheck) & 0x1) format = 4; // If e bit is set, it's format 4.
            }

            int len = format == 4 ? 8 : format == 2 ? 4 : 6; // Get the length depending on format
            if (idx + len > line.length()) {
                cerr << "Substr range [" << idx << ", " << idx + len << ") is out of range for line: " << line << endl;
                break; // Break out of the while loop to avoid further processing on this line.
            }

            string objectCodeToProcess = line.substr(idx, len); // Now, this should not throw std::out_of_range
            processObjectCode(objectCodeToProcess, outputFile, format);
            idx += len;
        }
    }

        inputFile.close();
        outputFile.close();
    }

    return 0;
}