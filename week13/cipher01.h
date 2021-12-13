/********************************************************************
* Header:
*    Implement your cipher here. You can view 'example.h' to see the
*    completed Caesar Cipher example.
********************************************************************/
#ifndef CIPHER01_H
#define CIPHER01_H

/********************************************************************
 * CLASS
 *******************************************************************/
class Cipher01 : public Cipher
{
public:
   virtual std::string getPseudoAuth()  { return "Carlos N Reina"; }
   virtual std::string getCipherName()  { return "XTEA (eXtended TEA)"; }
   virtual std::string getEncryptAuth() { return "Carlos N Reina"; }
   virtual std::string getDecryptAuth() { return "Carlos N Reina"; }

   /***********************************************************
    * GET CIPHER CITATION
    * Returns the citation from which we learned about the cipher
    ***********************************************************/
   virtual std::string getCipherCitation()
   {
         std::string str;
         str =  "Designers:       Roger Needham, David Wheeler\n";
         str += "                 University of Cambridge\n";
         str += "                 Computer Laboratory\n";
         str += "\n";
         str += "First published: 1997 Tea extensions (xtea.pdf)\n";
         str += "URL:             http://www.cix.co.uk/~klockstone/xtea.pdf\n";
         str += "URL:             https://en.wikipedia.org/wiki/XTEA\n";

      return str;
   }
   
   /**********************************************************
    * GET PSEUDOCODE
    * Returns the pseudocode as a string to be used by the caller.
    **********************************************************/
   virtual std::string getPseudocode()
   {
      std::string str;

      // The encrypt pseudocode
      str + "Sum = 0\n";
      str += "delta = 0x9E3779B9\n";
      str += "for i = 0 to N do\n";
      str += "\tv0+ = ((v1 << 4) xor (v1 >> 5) + v1) xor (sum + key[sum & 3])\n";
      str += "\tsum+ = delta\n";
      str += "\tv1+ = ((v0 << 4) xor (v0 >> 5) + v0) (sum + key[sum >> 11 & 3])\n";
      str += "end for\n";

      // The decrypt pseudocode
      str + "Sum = 0\n";
      str += "delta = 0x9E3779B9\n";
      str += "for i = 0 to N do;\n";
      str += "\tv1- = ((v0 << 4) xor (v1 >> 5) + v0) xor (sum + key[sum >> 11 & 3])\n";
      str += "\tsum+ = delta\n";
      str += "\tv0- = ((v1 << 4) xor (v1 >> 5) + v1) (sum + Key[sum &3])\n";
      str += "end for\n";

      return str;
   }

   /**********************************************************
    * ENCRYPT
    * 
    * Implements encryption using a 64-bit block split into two
    * 32-bit halves, v0 and v1.
    * The algorithm performs 32 rounds.
    * Four 32-bit subkeys are used.
    * Basic addition and subtraction operations follow modulo 232.
    * **********************************************************/
   virtual std::string encrypt(const std::string & plainText, 
                               const std::string & password)
   {
      // generate key
      uint32_t    passHashUint32_t    = getKeyFromPassword(password);
      uint32_t    key[4];
      key[0]      = passHashUint32_t;
      key[1]      = passHashUint32_t;
      key[2]      = passHashUint32_t;
      key[3]      = passHashUint32_t;

      // encrypt plain text
      std::string textPlainString = plainText;
      std::string textPaddedString = textPlainString;
      // append padding
      if(textPaddedString.size() % 8 != 0){
         std::string paddingChar = "_";
         int padLength = 8 - (textPaddedString.size() % 8);
         int padCounter = 0;
         for (padCounter = 0; padCounter < padLength; padCounter++){
               textPaddedString.append(paddingChar);
         };
      };

      // encrypt 8 byte blocks
      std::vector<char>       charVector(textPaddedString.begin(), textPaddedString.end());
      uint32_t                encryptData[2];
      std::vector<uint32_t>   encryptedBufferVector(0);
      std::string             textEncryptedString;
      int                     encryptLength = charVector.size();
      int                     encryptCounter = 0;
      for (encryptCounter = 0; encryptCounter < encryptLength; encryptCounter++){
         // convert char block to uint32_t
         if((encryptCounter + 7) < encryptLength){
               encryptData[0] =    (uint32_t)charVector[encryptCounter] << 24 | (uint32_t)charVector[encryptCounter + 1] << 16 | 
                                 (uint32_t)charVector[encryptCounter + 2] << 8 | (uint32_t)charVector[encryptCounter + 3];

               encryptData[1] =    (uint32_t)charVector[encryptCounter + 4] << 24 | (uint32_t)charVector[encryptCounter + 5] << 16 | 
                                 (uint32_t)charVector[encryptCounter + 6] << 8 | (uint32_t)charVector[encryptCounter + 7];

               // encript
               xteaEncrypt(32, encryptData, (uint32_t*)&key);
               // add to buffer
               encryptedBufferVector.push_back(encryptData[0]);
               encryptedBufferVector.push_back(encryptData[1]);

               textEncryptedString.append(std::to_string(encryptData[0]));
               textEncryptedString.append("-");
               textEncryptedString.append(std::to_string(encryptData[1]));
               textEncryptedString.append("-");

               encryptCounter = encryptCounter + 7;
         };

      };
      
      for (uint32_t& cnrItem : encryptedBufferVector){
        textEncryptedString.append(std::to_string(cnrItem));
      };

      return textEncryptedString;
   }

   /**********************************************************
    * DECRYPT
    * 
    * Implements decryption using a 64-bit block split into two
    * 32-bit halves, v0 and v1.
    * The algorithm performs 32 rounds.
    * Four 32-bit subkeys are used.
    * Basic addition and subtraction operations follow modulo 232.
    * **********************************************************/
   virtual std::string decrypt(const std::string & cipherText, 
                               const std::string & password)
   {
      // generate decryption key
      uint32_t    passHashUint32_t    = getKeyFromPassword(password);
      uint32_t    key[4];
      key[0]      = passHashUint32_t;
      key[1]      = passHashUint32_t;
      key[2]      = passHashUint32_t;
      key[3]      = passHashUint32_t;

      // DECRYPT
      std::string             cipherString = cipherText;
      std::string             cipherCleanString;
      std::string             tempString;
      std::vector<uint32_t>   cipherBufferVector(0);
      char                    tempChar[100];
      int                     cipherLength = cipherString.length();
      int                     cipherCounter = 0;
      for (cipherCounter = 0; cipherCounter < cipherLength; cipherCounter++){
         char tempChar = cipherString[cipherCounter];
         if(tempChar == '-'){            
               cipherCleanString.append(tempString);
               cipherBufferVector.push_back((uint32_t)atoi(&tempString[0]));
               tempString.clear();
               continue;
         };
         tempString.push_back(tempChar);
      };

      uint32_t                    decryptData[2];
      std::vector<uint32_t>       decryptedBufferVector(0);
      std::vector<char>           decryptedCharVector(8);
      std::vector<std::string>    decryptedBufferStringVector(0);
      int                         decryptLength = cipherBufferVector.size();
      int                         decryptCounter = 0;
      for (decryptCounter = 0; decryptCounter < decryptLength; decryptCounter++){
         if((decryptCounter + 1) < decryptLength){
            decryptData[0] =    (uint32_t)cipherBufferVector[decryptCounter];
            decryptData[1] =    (uint32_t)cipherBufferVector[decryptCounter + 1];
            
            // decrypt
            xteaDecrypt(32, decryptData, (uint32_t*)&key);

            // add to buffer
            decryptedBufferVector.push_back(decryptData[0]);
            decryptedBufferVector.push_back(decryptData[1]);

            // get text back
            decryptedCharVector[0] = decryptData[0] >> 24;
            decryptedCharVector[1] = decryptData[0] >> 16;
            decryptedCharVector[2] = decryptData[0] >> 8;
            decryptedCharVector[3] = decryptData[0];
            decryptedCharVector[4] = decryptData[1] >> 24;
            decryptedCharVector[5] = decryptData[1] >> 16;
            decryptedCharVector[6] = decryptData[1] >> 8;
            decryptedCharVector[7] = decryptData[1];

            std::string textDecryptedString(decryptedCharVector.begin(), decryptedCharVector.end());
            char paddingCharacter = '_';
            textDecryptedString.erase(remove(textDecryptedString.begin(), textDecryptedString.end(), paddingCharacter), textDecryptedString.end());

            decryptedBufferStringVector.push_back(textDecryptedString);

            decryptCounter = decryptCounter + 1;
         };

      };

      std::string plainTextDecryptedString;
      for (std::string cnrItem : decryptedBufferStringVector){
         plainTextDecryptedString.append(cnrItem);
      };

      return plainTextDecryptedString;
   }

   uint32_t getKeyFromPassword(std::string password){
      std::string passPlain            = password;
      uint32_t    passHashUint32_t     = cnrHashStringToUint32_t(passPlain);
      uint32_t    key                  = passHashUint32_t;
      return      key;
   };

   uint32_t cnrHashStringToUint32_t(std::string input){
      size_t hash = std::hash<std::string>{}(input);
      return (uint32_t) hash;
   };

   /**	XTEA (eXtended TEA)
    * Key size:        128 bits
    * Block size:      64 bits
    * Structure:       Feistel cipher
    * Rounds:          variable (64 Feistel rounds - 32 cycles)
    * 
    * Designers:       Roger Needham, David Wheeler
    *                  University of Cambridge
    *                  Computer Laboratory
    * First published: 1997 Tea extensions (xtea.pdf)
    * URL: http://www.cix.co.uk/~klockstone/xtea.pdf
    * URL: https://en.wikipedia.org/wiki/XTEA
   */
   void xteaEncrypt(unsigned int rounds, uint32_t encryptData[2], uint32_t const key[4])
   {
      unsigned int    counter = 0;
      uint32_t        data0   = encryptData[0];
      uint32_t        data1   = encryptData[1];
      uint32_t        sum     = 0;
      uint32_t        delta   = 0x9E3779B9;
      for (counter = 0; counter < rounds; counter++)
      {
         data0    += (((data1 << 4) ^ (data1 >> 5)) + data1) ^ (sum + key[sum & 3]);
         sum      += delta;
         data1    += (((data0 << 4) ^ (data0 >> 5)) + data0) ^ (sum + key[(sum>>11) & 3]);
      }
      encryptData[0] = data0;
      encryptData[1] = data1;
   };

   void xteaDecrypt(unsigned int rounds, uint32_t encryptData[2], uint32_t const key[4]) {
      unsigned int    counter = 0;
      uint32_t        data0   = encryptData[0];
      uint32_t        data1   = encryptData[1];
      uint32_t        delta   = 0x9E3779B9;
      uint32_t        sum     = delta*rounds;
      for (counter = 0; counter < rounds ; counter++) {
         data1    -= (((data0 << 4) ^ (data0 >> 5)) + data0) ^ (sum + key[(sum>>11) & 3]);
         sum      -= delta;
         data0    -= (((data1 << 4) ^ (data1 >> 5)) + data1) ^ (sum + key[sum & 3]);
      }
      encryptData[0] = data0;
      encryptData[1] = data1;
   };

};

#endif // CIPHER01_H