/********************************************************************
* Header:
*    Implement your cipher here. You can view 'example.h' to see the
*    completed Caesar Cipher example.
********************************************************************/
#include <cmath>

#ifndef CIPHER02_H
#define CIPHER02_H

/********************************************************************
 * CLASS
 *******************************************************************/
class Cipher02 : public Cipher
{
public:
   virtual std::string getPseudoAuth()  { return "Caleb Georgeson"; }
   virtual std::string getCipherName()  { return "AES-128"; }
   virtual std::string getEncryptAuth() { return "Caleb Georgeson"; }
   virtual std::string getDecryptAuth() { return "Caleb Georgeson"; }

   /***********************************************************
    * GET CIPHER CITATION
    * Returns the citation from which we learned about the cipher
    ***********************************************************/
   virtual std::string getCipherCitation()
   {
      std::string s;
      s += "Dworkin et al. \"Advanced Encryption Standard\". NIST Publications. 2001.\n";
      s += "retrieved: https://www.nist.gov/publications/advanced-encryption-standard-aes";
      return s;
   }
   
   /**********************************************************
    * GET PSEUDOCODE
    * Returns the pseudocode as a string to be used by the caller.
    **********************************************************/
   virtual std::string getPseudocode()
   {
      std::string str;

      // The encrypt pseudocode
      str =  "encrypt(std::string plaintext, string keyString)\n";
      str += "  word key[4]\n";
      str += "  GetKey(keyString, key)\n\n";
      str += "  vector<block> blocks = getBlocks(plaintext)\n\n";
      str += "  word roundKeys[4*R]\n";
      str += "  expandKeys(key, roundKeys)\n\n";
      str += "  for state in blocks\n";
      str += "    AddRoundKey(state, roundKeys, 0)\n";
      str += "    for round = 1 step 1 to 9\n";
      str += "      SubBytes(state)\n";
      str += "      ShiftRows(state)\n";
      str += "      MixColumns(state)\n";
      str += "      AddRoundKey(state, roundKeys, round * 4)\n\n";
      str += "    SubBytes(state)\n";
      str += "    ShiftRows(state)\n";
      str += "    AddRoundKey(state, roundKeys, 40)\n\n";
      str += "  return getMessage(blocks)\n";
      str += "end\n\n";

      // The decrypt pseudocode
      str += "decrypt(std::string ciphertext, string keyString)\n";
      str += "  word key[4]\n";
      str += "  GetKey(keyString, key)\n\n";
      str += "  vector<block> blocks = getBlocks(ciphertext)\n\n";
      str += "  word roundKeys[4*R]\n";
      str += "  expandKeys(key, roundKeys)\n\n";
      str += "  for state in blocks\n";
      str += "    InverseAddRoundKey(state, roundKeys, 40)\n";
      str += "    InverseShiftRows(state)\n";
      str += "    InverseSubBytes(state)\n\n";
      str += "    for round = 9 step -1 to 1\n";
      str += "      InverseAddRoundKey(state, roundKeys, round * 4)\n";
      str += "      InverseMixColumns(state)\n";
      str += "      InverseShiftRows(state)\n";
      str += "      InverseSubBytes(state)\n\n";
      str += "    InverseAddRoundKey(state, roundKeys, 0)\n\n";
      str += "  return getMessage(blocks)\n";
      str += "end\n";

      return str;
   }

   // A byte is the same as an unsigned char in memory
   typedef unsigned char byte;

   // A block is a 2 dimensional array of bytes
   struct Block 
   {
      byte data[4][4];
   };

   // Length of key in words
   const int N = 4;

   // Number of round keys needed
   const int R = 11;

   // S-BOX
   const unsigned char sbox[16][16] = {
      { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
      { 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
      { 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
      { 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
      { 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
      { 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
      { 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
      { 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
      { 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
      { 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
      { 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
      { 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
      { 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
      { 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
      { 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf },
      { 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
   };

   // Inverse S-Box
   const unsigned char inverseSbox[16][16] = {
      { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb },
      { 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb },
      { 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e },
      { 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25 },
      { 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92 },
      { 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84 },
      { 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06 },
      { 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b },
      { 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73 },
      { 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e },
      { 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b },
      { 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4 },
      { 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f },
      { 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef },
      { 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61 },
      { 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d },
   };

   // Tables for mix column
   // Found here: https://en.wikipedia.org/wiki/Rijndael_MixColumns
   const byte table_2[256] = {
      0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e,
      0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e,
      0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e,
      0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e,	
      0x80,0x82,0x84,0x86,0x88,0x8a,0x8c,0x8e,0x90,0x92,0x94,0x96,0x98,0x9a,0x9c,0x9e,
      0xa0,0xa2,0xa4,0xa6,0xa8,0xaa,0xac,0xae,0xb0,0xb2,0xb4,0xb6,0xb8,0xba,0xbc,0xbe,
      0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde,
      0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,0xf0,0xf2,0xf4,0xf6,0xf8,0xfa,0xfc,0xfe,
      0x1b,0x19,0x1f,0x1d,0x13,0x11,0x17,0x15,0x0b,0x09,0x0f,0x0d,0x03,0x01,0x07,0x05,
      0x3b,0x39,0x3f,0x3d,0x33,0x31,0x37,0x35,0x2b,0x29,0x2f,0x2d,0x23,0x21,0x27,0x25,
      0x5b,0x59,0x5f,0x5d,0x53,0x51,0x57,0x55,0x4b,0x49,0x4f,0x4d,0x43,0x41,0x47,0x45,
      0x7b,0x79,0x7f,0x7d,0x73,0x71,0x77,0x75,0x6b,0x69,0x6f,0x6d,0x63,0x61,0x67,0x65,
      0x9b,0x99,0x9f,0x9d,0x93,0x91,0x97,0x95,0x8b,0x89,0x8f,0x8d,0x83,0x81,0x87,0x85,
      0xbb,0xb9,0xbf,0xbd,0xb3,0xb1,0xb7,0xb5,0xab,0xa9,0xaf,0xad,0xa3,0xa1,0xa7,0xa5,
      0xdb,0xd9,0xdf,0xdd,0xd3,0xd1,0xd7,0xd5,0xcb,0xc9,0xcf,0xcd,0xc3,0xc1,0xc7,0xc5,
      0xfb,0xf9,0xff,0xfd,0xf3,0xf1,0xf7,0xf5,0xeb,0xe9,0xef,0xed,0xe3,0xe1,0xe7,0xe5
   };

   const byte table_3[256] = {
      0x00,0x03,0x06,0x05,0x0c,0x0f,0x0a,0x09,0x18,0x1b,0x1e,0x1d,0x14,0x17,0x12,0x11,
      0x30,0x33,0x36,0x35,0x3c,0x3f,0x3a,0x39,0x28,0x2b,0x2e,0x2d,0x24,0x27,0x22,0x21,
      0x60,0x63,0x66,0x65,0x6c,0x6f,0x6a,0x69,0x78,0x7b,0x7e,0x7d,0x74,0x77,0x72,0x71,
      0x50,0x53,0x56,0x55,0x5c,0x5f,0x5a,0x59,0x48,0x4b,0x4e,0x4d,0x44,0x47,0x42,0x41,
      0xc0,0xc3,0xc6,0xc5,0xcc,0xcf,0xca,0xc9,0xd8,0xdb,0xde,0xdd,0xd4,0xd7,0xd2,0xd1,
      0xf0,0xf3,0xf6,0xf5,0xfc,0xff,0xfa,0xf9,0xe8,0xeb,0xee,0xed,0xe4,0xe7,0xe2,0xe1,
      0xa0,0xa3,0xa6,0xa5,0xac,0xaf,0xaa,0xa9,0xb8,0xbb,0xbe,0xbd,0xb4,0xb7,0xb2,0xb1,
      0x90,0x93,0x96,0x95,0x9c,0x9f,0x9a,0x99,0x88,0x8b,0x8e,0x8d,0x84,0x87,0x82,0x81,	
      0x9b,0x98,0x9d,0x9e,0x97,0x94,0x91,0x92,0x83,0x80,0x85,0x86,0x8f,0x8c,0x89,0x8a,
      0xab,0xa8,0xad,0xae,0xa7,0xa4,0xa1,0xa2,0xb3,0xb0,0xb5,0xb6,0xbf,0xbc,0xb9,0xba,
      0xfb,0xf8,0xfd,0xfe,0xf7,0xf4,0xf1,0xf2,0xe3,0xe0,0xe5,0xe6,0xef,0xec,0xe9,0xea,	
      0xcb,0xc8,0xcd,0xce,0xc7,0xc4,0xc1,0xc2,0xd3,0xd0,0xd5,0xd6,0xdf,0xdc,0xd9,0xda,	
      0x5b,0x58,0x5d,0x5e,0x57,0x54,0x51,0x52,0x43,0x40,0x45,0x46,0x4f,0x4c,0x49,0x4a,
      0x6b,0x68,0x6d,0x6e,0x67,0x64,0x61,0x62,0x73,0x70,0x75,0x76,0x7f,0x7c,0x79,0x7a,	
      0x3b,0x38,0x3d,0x3e,0x37,0x34,0x31,0x32,0x23,0x20,0x25,0x26,0x2f,0x2c,0x29,0x2a,
      0x0b,0x08,0x0d,0x0e,0x07,0x04,0x01,0x02,0x13,0x10,0x15,0x16,0x1f,0x1c,0x19,0x1a
   };

   const byte table_9[256] = {
      0x00,0x09,0x12,0x1b,0x24,0x2d,0x36,0x3f,0x48,0x41,0x5a,0x53,0x6c,0x65,0x7e,0x77,
      0x90,0x99,0x82,0x8b,0xb4,0xbd,0xa6,0xaf,0xd8,0xd1,0xca,0xc3,0xfc,0xf5,0xee,0xe7,
      0x3b,0x32,0x29,0x20,0x1f,0x16,0x0d,0x04,0x73,0x7a,0x61,0x68,0x57,0x5e,0x45,0x4c,
      0xab,0xa2,0xb9,0xb0,0x8f,0x86,0x9d,0x94,0xe3,0xea,0xf1,0xf8,0xc7,0xce,0xd5,0xdc,
      0x76,0x7f,0x64,0x6d,0x52,0x5b,0x40,0x49,0x3e,0x37,0x2c,0x25,0x1a,0x13,0x08,0x01,
      0xe6,0xef,0xf4,0xfd,0xc2,0xcb,0xd0,0xd9,0xae,0xa7,0xbc,0xb5,0x8a,0x83,0x98,0x91,
      0x4d,0x44,0x5f,0x56,0x69,0x60,0x7b,0x72,0x05,0x0c,0x17,0x1e,0x21,0x28,0x33,0x3a,
      0xdd,0xd4,0xcf,0xc6,0xf9,0xf0,0xeb,0xe2,0x95,0x9c,0x87,0x8e,0xb1,0xb8,0xa3,0xaa,	
      0xec,0xe5,0xfe,0xf7,0xc8,0xc1,0xda,0xd3,0xa4,0xad,0xb6,0xbf,0x80,0x89,0x92,0x9b,	
      0x7c,0x75,0x6e,0x67,0x58,0x51,0x4a,0x43,0x34,0x3d,0x26,0x2f,0x10,0x19,0x02,0x0b,
      0xd7,0xde,0xc5,0xcc,0xf3,0xfa,0xe1,0xe8,0x9f,0x96,0x8d,0x84,0xbb,0xb2,0xa9,0xa0,
      0x47,0x4e,0x55,0x5c,0x63,0x6a,0x71,0x78,0x0f,0x06,0x1d,0x14,0x2b,0x22,0x39,0x30,
      0x9a,0x93,0x88,0x81,0xbe,0xb7,0xac,0xa5,0xd2,0xdb,0xc0,0xc9,0xf6,0xff,0xe4,0xed,
      0x0a,0x03,0x18,0x11,0x2e,0x27,0x3c,0x35,0x42,0x4b,0x50,0x59,0x66,0x6f,0x74,0x7d,	
      0xa1,0xa8,0xb3,0xba,0x85,0x8c,0x97,0x9e,0xe9,0xe0,0xfb,0xf2,0xcd,0xc4,0xdf,0xd6,
      0x31,0x38,0x23,0x2a,0x15,0x1c,0x07,0x0e,0x79,0x70,0x6b,0x62,0x5d,0x54,0x4f,0x46
   };

   const byte table_11[256] = {
      0x00,0x0b,0x16,0x1d,0x2c,0x27,0x3a,0x31,0x58,0x53,0x4e,0x45,0x74,0x7f,0x62,0x69,
      0xb0,0xbb,0xa6,0xad,0x9c,0x97,0x8a,0x81,0xe8,0xe3,0xfe,0xf5,0xc4,0xcf,0xd2,0xd9,
      0x7b,0x70,0x6d,0x66,0x57,0x5c,0x41,0x4a,0x23,0x28,0x35,0x3e,0x0f,0x04,0x19,0x12,
      0xcb,0xc0,0xdd,0xd6,0xe7,0xec,0xf1,0xfa,0x93,0x98,0x85,0x8e,0xbf,0xb4,0xa9,0xa2,
      0xf6,0xfd,0xe0,0xeb,0xda,0xd1,0xcc,0xc7,0xae,0xa5,0xb8,0xb3,0x82,0x89,0x94,0x9f,
      0x46,0x4d,0x50,0x5b,0x6a,0x61,0x7c,0x77,0x1e,0x15,0x08,0x03,0x32,0x39,0x24,0x2f,
      0x8d,0x86,0x9b,0x90,0xa1,0xaa,0xb7,0xbc,0xd5,0xde,0xc3,0xc8,0xf9,0xf2,0xef,0xe4,
      0x3d,0x36,0x2b,0x20,0x11,0x1a,0x07,0x0c,0x65,0x6e,0x73,0x78,0x49,0x42,0x5f,0x54,
      0xf7,0xfc,0xe1,0xea,0xdb,0xd0,0xcd,0xc6,0xaf,0xa4,0xb9,0xb2,0x83,0x88,0x95,0x9e,
      0x47,0x4c,0x51,0x5a,0x6b,0x60,0x7d,0x76,0x1f,0x14,0x09,0x02,0x33,0x38,0x25,0x2e,
      0x8c,0x87,0x9a,0x91,0xa0,0xab,0xb6,0xbd,0xd4,0xdf,0xc2,0xc9,0xf8,0xf3,0xee,0xe5,
      0x3c,0x37,0x2a,0x21,0x10,0x1b,0x06,0x0d,0x64,0x6f,0x72,0x79,0x48,0x43,0x5e,0x55,
      0x01,0x0a,0x17,0x1c,0x2d,0x26,0x3b,0x30,0x59,0x52,0x4f,0x44,0x75,0x7e,0x63,0x68,
      0xb1,0xba,0xa7,0xac,0x9d,0x96,0x8b,0x80,0xe9,0xe2,0xff,0xf4,0xc5,0xce,0xd3,0xd8,
      0x7a,0x71,0x6c,0x67,0x56,0x5d,0x40,0x4b,0x22,0x29,0x34,0x3f,0x0e,0x05,0x18,0x13,
      0xca,0xc1,0xdc,0xd7,0xe6,0xed,0xf0,0xfb,0x92,0x99,0x84,0x8f,0xbe,0xb5,0xa8,0xa3
   };

   const byte table_13[256] = {
      0x00,0x0d,0x1a,0x17,0x34,0x39,0x2e,0x23,0x68,0x65,0x72,0x7f,0x5c,0x51,0x46,0x4b,
      0xd0,0xdd,0xca,0xc7,0xe4,0xe9,0xfe,0xf3,0xb8,0xb5,0xa2,0xaf,0x8c,0x81,0x96,0x9b,
      0xbb,0xb6,0xa1,0xac,0x8f,0x82,0x95,0x98,0xd3,0xde,0xc9,0xc4,0xe7,0xea,0xfd,0xf0,
      0x6b,0x66,0x71,0x7c,0x5f,0x52,0x45,0x48,0x03,0x0e,0x19,0x14,0x37,0x3a,0x2d,0x20,
      0x6d,0x60,0x77,0x7a,0x59,0x54,0x43,0x4e,0x05,0x08,0x1f,0x12,0x31,0x3c,0x2b,0x26,
      0xbd,0xb0,0xa7,0xaa,0x89,0x84,0x93,0x9e,0xd5,0xd8,0xcf,0xc2,0xe1,0xec,0xfb,0xf6,
      0xd6,0xdb,0xcc,0xc1,0xe2,0xef,0xf8,0xf5,0xbe,0xb3,0xa4,0xa9,0x8a,0x87,0x90,0x9d,
      0x06,0x0b,0x1c,0x11,0x32,0x3f,0x28,0x25,0x6e,0x63,0x74,0x79,0x5a,0x57,0x40,0x4d,
      0xda,0xd7,0xc0,0xcd,0xee,0xe3,0xf4,0xf9,0xb2,0xbf,0xa8,0xa5,0x86,0x8b,0x9c,0x91,
      0x0a,0x07,0x10,0x1d,0x3e,0x33,0x24,0x29,0x62,0x6f,0x78,0x75,0x56,0x5b,0x4c,0x41,
      0x61,0x6c,0x7b,0x76,0x55,0x58,0x4f,0x42,0x09,0x04,0x13,0x1e,0x3d,0x30,0x27,0x2a,
      0xb1,0xbc,0xab,0xa6,0x85,0x88,0x9f,0x92,0xd9,0xd4,0xc3,0xce,0xed,0xe0,0xf7,0xfa,
      0xb7,0xba,0xad,0xa0,0x83,0x8e,0x99,0x94,0xdf,0xd2,0xc5,0xc8,0xeb,0xe6,0xf1,0xfc,
      0x67,0x6a,0x7d,0x70,0x53,0x5e,0x49,0x44,0x0f,0x02,0x15,0x18,0x3b,0x36,0x21,0x2c,
      0x0c,0x01,0x16,0x1b,0x38,0x35,0x22,0x2f,0x64,0x69,0x7e,0x73,0x50,0x5d,0x4a,0x47,
      0xdc,0xd1,0xc6,0xcb,0xe8,0xe5,0xf2,0xff,0xb4,0xb9,0xae,0xa3,0x80,0x8d,0x9a,0x97
   };

   const byte table_14[256] = {
      0x00,0x0e,0x1c,0x12,0x38,0x36,0x24,0x2a,0x70,0x7e,0x6c,0x62,0x48,0x46,0x54,0x5a,
      0xe0,0xee,0xfc,0xf2,0xd8,0xd6,0xc4,0xca,0x90,0x9e,0x8c,0x82,0xa8,0xa6,0xb4,0xba,
      0xdb,0xd5,0xc7,0xc9,0xe3,0xed,0xff,0xf1,0xab,0xa5,0xb7,0xb9,0x93,0x9d,0x8f,0x81,
      0x3b,0x35,0x27,0x29,0x03,0x0d,0x1f,0x11,0x4b,0x45,0x57,0x59,0x73,0x7d,0x6f,0x61,
      0xad,0xa3,0xb1,0xbf,0x95,0x9b,0x89,0x87,0xdd,0xd3,0xc1,0xcf,0xe5,0xeb,0xf9,0xf7,
      0x4d,0x43,0x51,0x5f,0x75,0x7b,0x69,0x67,0x3d,0x33,0x21,0x2f,0x05,0x0b,0x19,0x17,
      0x76,0x78,0x6a,0x64,0x4e,0x40,0x52,0x5c,0x06,0x08,0x1a,0x14,0x3e,0x30,0x22,0x2c,
      0x96,0x98,0x8a,0x84,0xae,0xa0,0xb2,0xbc,0xe6,0xe8,0xfa,0xf4,0xde,0xd0,0xc2,0xcc,
      0x41,0x4f,0x5d,0x53,0x79,0x77,0x65,0x6b,0x31,0x3f,0x2d,0x23,0x09,0x07,0x15,0x1b,
      0xa1,0xaf,0xbd,0xb3,0x99,0x97,0x85,0x8b,0xd1,0xdf,0xcd,0xc3,0xe9,0xe7,0xf5,0xfb,
      0x9a,0x94,0x86,0x88,0xa2,0xac,0xbe,0xb0,0xea,0xe4,0xf6,0xf8,0xd2,0xdc,0xce,0xc0,
      0x7a,0x74,0x66,0x68,0x42,0x4c,0x5e,0x50,0x0a,0x04,0x16,0x18,0x32,0x3c,0x2e,0x20,
      0xec,0xe2,0xf0,0xfe,0xd4,0xda,0xc8,0xc6,0x9c,0x92,0x80,0x8e,0xa4,0xaa,0xb8,0xb6,
      0x0c,0x02,0x10,0x1e,0x34,0x3a,0x28,0x26,0x7c,0x72,0x60,0x6e,0x44,0x4a,0x58,0x56,
      0x37,0x39,0x2b,0x25,0x0f,0x01,0x13,0x1d,0x47,0x49,0x5b,0x55,0x7f,0x71,0x63,0x6d,
      0xd7,0xd9,0xcb,0xc5,0xef,0xe1,0xf3,0xfd,0xa7,0xa9,0xbb,0xb5,0x9f,0x91,0x83,0x8d
   };

   /************************
    * Get Key
    * Converts a string to an array of longs
    ************************/
   void  getKey(std::string stringKey, unsigned long * key)
   {
      int iKey = 0;
      for (int i = 0; i < 4; i++)
      {
         unsigned long num = 0;

         for (int j = 0; j < 4; j++)
         {
               num *= 256;
               num += stringKey[iKey++ % stringKey.length()];
         }

         key[i] = num;
      }
   }

   /***********************
    * Get Blocks
    * Converts a string into a block
    ***********************/
   std::vector<Block> getBlocks(std::string plainText)
   {
      std::vector<Block> blocks;
      std::string::iterator it = plainText.begin();
      while (it != plainText.end())
      {
         Block block;
         for (int col = 0; col < 4; col++)
         {
               for (int row = 0; row < 4; row++)
               {
                  if (it != plainText.end())
                  {
                     // cout << "Adding byte: " << *it << endl;
                     block.data[row][col] = *it;
                     ++it;
                  } else {
                     block.data[row][col] = 0;
                  }
               }
         }

         blocks.push_back(block);
      }

      return blocks;
   }

   /*****************
    * Get message
    * Converts a block into a string
    *****************/
   std::string getMessage(std::vector<Block> blocks)
   {
      std::string message = "";

      std::vector<Block>::iterator it = blocks.begin();
      for(it; it != blocks.end(); ++it)
      {
         Block block = *it;
         for (int col = 0; col < 4; col++)
               for (int row = 0; row < 4; row++)
                  message += block.data[row][col];
      }

      return message;
   }

   /********************
    * Add Round Key
    * XORs the round key from the first byte to the last byte
    ********************/
   Block addRoundKey(Block state, unsigned long * key, int offset)
   {
      for (int row = 0; row < 4; row++)
      {
         for (int col = 0; col < 4; col++)
         {
               long word = key[col + offset];
               int power = 8 * (3 - row);
               byte subKey = (word / (long) std::pow(2, power)) % 256;
               state.data[row][col] ^= subKey;
         }
      }

      return state;
   }

   /***************************************
    * Inverse Add Round Key
    * XORs the round key from the last byte to the first byte
    ***************************************/
   Block inverseAddRoundKey(Block state, unsigned long * key, int offset)
   {
      for (int row = 3; row >= 0; row--)
      {
         for (int col = 3; col >= 0; col--)
         {
               long word = key[col + offset];
               int power = 8 * (3 - row);
               byte subKey = (word / (long) std::pow(2, power)) % 256;
               state.data[row][col] ^= subKey;
         }
      }

      return state;
   }

   /*************************************
    * Mix Single Column
    * Mixes the bytes of the column by XORing each value with values from tables 2 and 3
    *************************************/
   void mixSingleColumn(byte * col)
   {
      byte temp[4];
      for (int i = 0; i < 4; i++)
      {
         temp[i] = col[i];
      } 

      col[0] = table_2[temp[0]] ^ table_3[temp[1]] ^ temp[2]          ^ temp[3]         ;
      col[1] = temp[0]          ^ table_2[temp[1]] ^ table_3[temp[2]] ^ temp[3]         ;
      col[2] = temp[0]          ^ temp[1]          ^ table_2[temp[2]] ^ table_3[temp[3]];
      col[3] = table_3[temp[0]] ^ temp[1]          ^ temp[2]          ^ table_2[temp[3]];
   }

   /***********************************
    * Inverse Mix Single Column
    * Mixes the bytes of the column by XORing each value with values from tables 9, 13, and 14
    ***********************************/
   void inverseMixSingleColumn(byte * col)
   {
      byte temp[4];
      for (int i = 0; i < 4; i++)
      {
         temp[i] = col[i];
      } 

      col[0] = table_14[temp[0]] ^ table_11[temp[1]] ^ table_13[temp[2]] ^ table_9[temp[3]] ;
      col[1] = table_9[temp[0]]  ^ table_14[temp[1]] ^ table_11[temp[2]] ^ table_13[temp[3]];
      col[2] = table_13[temp[0]] ^ table_9[temp[1]]  ^ table_14[temp[2]] ^ table_11[temp[3]];
      col[3] = table_11[temp[0]] ^ table_13[temp[1]] ^ table_9[temp[2]]  ^ table_14[temp[3]];
   }

   /******************************
    * Mix columns
    * Mix each column
    ******************************/
   Block mixColumns(Block state)
   {
      for (int col = 0; col < 4; col++)
      {
         byte temp[4];
         for (int row = 0; row < 4; row++)
         {
               temp[row] = state.data[row][col];
         }
         mixSingleColumn(temp);
         for (int row = 0; row < 4; row++)
         {
               state.data[row][col] = temp[row];
         }
      }

      return state;
   }

   /***********************************
    * Inverse Mix Columns
    * Reverse the mix columns
    ***********************************/
   Block inverseMixColumns(Block state)
   {
      for (int col = 0; col < 4; col++)
      {
         byte temp[4];
         for (int row = 0; row < 4; row++)
         {
               temp[row] = state.data[row][col];
         }
         inverseMixSingleColumn(temp);
         for (int row = 0; row < 4; row++)
         {
               state.data[row][col] = temp[row];
         }
      }

      return state;
   }

   /**************************
    * Sub Bytes
    * Substitute each byte of the state with a value from the S-Box
    **************************/
   Block subBytes(Block state)
   {
      for (int row = 0; row < 4; row++)
      {
         for (int col = 0; col < 4; col++)
         {
               unsigned char b = state.data[row][col];
               state.data[row][col] = sbox[b / 16][b % 16];
         }
      }

      return state;
   }

   /*****************************************
    * Inverse Sub Bytes
    * Substitute each byte of the state with a value from the inverse S-Box
    *****************************************/
   Block inverseSubBytes(Block state)
   {
      for (int row = 0; row < 4; row++)
      {
         for (int col = 0; col < 4; col++)
         {
               unsigned char b = state.data[row][col];
               state.data[row][col] = inverseSbox[b / 16][b % 16];
         }
      }

      return state;
   }

   /*************************************
    * Shift Rows
    * 
    * Row 1: No change
    * Row 2: 1 to the left
    * Row 3: 2 to the left
    * Row 4: 3 to the left
    *************************************/
   Block shiftRows(Block state)
   {
      for (int row = 0; row < 4; row++)
      {
         byte byteRow[4];
         for (int col = 0; col < 4; col++)
            byteRow[col] = state.data[row][(col + row) % 4];
         
         for (int col = 0; col < 4; col++)
         {
               state.data[row][col] = byteRow[col];
         }
      }

      return state;
   }

   /**************************************
    * Inverse Shift rows
    * 
    * Row 1: no change
    * Row 2: 3 to the left
    * Row 3: 2 to the left
    * Row 4: 1 to the left
    **************************************/
   Block inverseShiftRows(Block state)
   {
      for (int row = 0; row < 4; row++)
      {
         byte byteRow[4];
         for (int col = 0; col < 4; col++)
         {
               int mod = row;
               if (row == 1)
                  mod = 3;
               else if (row == 3)
                  mod = 1;
               byteRow[col] = state.data[row][(col + mod) % 4];
         }

         for (int col = 0; col < 4; col++)
         {
               state.data[row][col] = byteRow[col];
         }
      }

      return state;
   }

   /****************************************
    * Rot Word
    * Rotate the word one byte to the left
    ****************************************/
   long rotWord(long word)
   {
      long newWord = 0;

      long left = word / std::pow(2, 24);
      long right = word % (long) std::pow(2, 24);

      newWord = (right * 256) + left;

      return newWord;
   }

   /****************************************
    * Sub word
    * Substitute each byte in the word with a value from the S-Box
    ****************************************/
   long subWord(long word)
   {
      long newWord = 0;

      // cout << "Word: " << word << endl;
      for (int i = 0; i < 4; i++)
      {
         byte num = word % 256;
         word /= 256;
         newWord += sbox[num / 16][num % 16] * pow(2, 8 * i);
      }

      // cout << "Subword: " << newWord << endl;

      return newWord;
   }

   /*****************************************
    * Expand key
    * Create the round keys
    *****************************************/
   void expandKey(unsigned long * inputKey, unsigned long * outputKey)
   {
      const long rcon[11] = {
         0x01000000,
         0x02000000,        
         0x04000000,
         0x08000000,
         0x10000000,
         0x20000000,
         0x40000000,
         0x80000000,
         0x1B000000,
         0x36000000,
         0x6C000000     
      }; 

      for (int i = 0; i < 4 * R; i++)
      {
         if (i < N)
         {
               outputKey[i] = inputKey[i];
         } else if (i >= N && (i % N == 0)) {
               outputKey[i] = outputKey[i-N] ^ subWord(rotWord(outputKey[i-1])) ^ rcon[i/N];
         } else if (i >= N && N > 6 && (i % N == 4)) {
               // AES-128 doesn't need this, but I put it here anyway to implement the full algorithm
               outputKey[i] = outputKey[i-N] ^ subWord(outputKey[i-1]);
         } else {
               outputKey[i] = outputKey[i-N] ^ outputKey[i-1];
         }
      }

   }

   

   

/**********************************************************
    * ENCRYPT
    * Encrypts the plaintext following the AES algorithm.
    **********************************************************/
   virtual std::string encrypt(const std::string & plainText,
                               const std::string & password)
   {      
      unsigned long key[4];
      getKey(password, key);
      std::vector<Block> blockMessage = getBlocks(plainText);
      std::vector<Block> blockCipher;
      unsigned long roundKeys[4*R];
      expandKey(key, roundKeys);

      std::vector<Block>::iterator it = blockMessage.begin();
      for (it; it != blockMessage.end(); ++it)
      {
         Block state = *it;
         int keyOffset = 0;
         state = addRoundKey(state, roundKeys, keyOffset);
         
         // Rounds 1 - 9
         for (int i = 1; i <= 9; i++)
         {
               keyOffset = N * i;
               state = subBytes(state);
               state = shiftRows(state);
               state = mixColumns(state);
               state = addRoundKey(state, roundKeys, keyOffset);
         }

         // Final round
         state = subBytes(state);
         state = shiftRows(state);
         state = addRoundKey(state, roundKeys, N * 10);

         blockCipher.push_back(state);
      }

      return getMessage(blockCipher);
   }

   /**********************************************************
    * DECRYPT
    * Decrypt by running the encrypt algorithm in reverse
    **********************************************************/
   virtual std::string decrypt(const std::string & cipherText,
                               const std::string & password)
   {
      unsigned long key[4];
      getKey(password, key);

      std::vector<Block> blockCipher = getBlocks(cipherText);
      std::vector<Block> blockPlain;

      unsigned long roundKeys[4 * R];
      expandKey(key, roundKeys);

      std::vector<Block>::iterator it = blockCipher.begin();
      for (it; it != blockCipher.end(); ++it)
      {
         Block state = *it;
         int keyOffset = N * 10;

         state = inverseAddRoundKey(state, roundKeys, keyOffset);
         state = inverseShiftRows(state);
         state = inverseSubBytes(state);

         for (int i = 9; i >= 1; i--)
         {
               keyOffset = 4 * i;
               state = inverseAddRoundKey(state, roundKeys, keyOffset);
               state = inverseMixColumns(state);
               state = inverseShiftRows(state);
               state = inverseSubBytes(state);
         }

         state = inverseAddRoundKey(state, roundKeys, 0);

         blockPlain.push_back(state);
      }


      return getMessage(blockPlain);
   }

};

#endif // CIPHER02_H