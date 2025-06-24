#include "rsa.h"
#include <stdio.h>
#include "cli.h"


int main(){
	// BigInt c = big_int_constructor(0, 8,
	// 	0x3322116D , 0x77665544 , 0xBBAA9988 , 0xFFEEDDCC ,
	// 	0x33221100 , 0x77665544 , 0xB5A69788 , 0xF1E2D3C4);

    // BigInt message = big_int_constructor(0, 4,
	// 	0x89ABCDF9 , 0x12345678 , 0xCAFEBABE , 0x0BADF00D 
	//  );


	 
	//  BigInt e = big_int_constructor(0, 32,
	// 	0x3582461F, 0xB9F37C95, 0x94AA3396, 0xA605BE04, 
	// 	0xFD326DCB, 0xB159D35C, 0x5ECDFABF, 0x37E2A1D4, 
	// 	0x36361A4F, 0xEAC56C34, 0xEC8204A5, 0xA62FD25E, 
	// 	0x30CE364F, 0xD3DF93D1, 0xCA8207C6, 0xA7B46A47, 
	// 	0x70CAF02A, 0x1DF110DF, 0x681B2C03, 0x7E541BEE,
	// 	0x571C2BA7, 0xACFEC524, 0xE5A395BF, 0xF3A751C6, 
	// 	0xD1FB59DE, 0xC494D989, 0xA7B43554, 0x7D5C4432, 
	// 	0xCE405BD6, 0xA0B5209C, 0xC183ACBD, 0x725F7C79
	//  );
	//   BigInt n = big_int_constructor(0, 32,
	// 	0x389E7E37, 0xC0902132, 0x7A93ED7E, 0xFDC74E3A,
	// 	0xCB0C064C, 0x2B3ACC0E, 0x9D91AE87, 0x158AFCBD,
	// 	0x197F70EA, 0x09347693, 0xCB582B0C, 0xC4653DA6,
	// 	0x4FE3727B, 0xF3C4C708, 0x14BB99E5, 0xB84C1820,
	// 	0x4989F7B9, 0x99DDE6E4, 0x1AF8E180, 0x1808ECF3,
	// 	0x037D7F1D, 0xDECB8320, 0x862AE266, 0xCB2530DC,
	// 	0x03DAA630, 0xAFA8EDF3, 0xABE6F766, 0x39ACD611, 
	// 	0xFFEECEC9, 0xBFECEDEF, 0x223A9AA3, 0xC795895E
	//  );

	 
	 
	// BigInt d = big_int_constructor(0, 16,
	// 		0x24EE6B5F, 0xE8106B93, 0xE33D35C6, 0x9AEC7AD7,
	// 		0x4B15357E, 0x5AFB838A, 0xA3CCFA10, 0x8F802886,
	// 		0xCD3F0E82, 0x88E4520E, 0xAE5DFA2B, 0x795BEAED,
	// 		0xE2F36187, 0x0848B3DC, 0x4F638004, 0xFAAECC22
	//  );

	 

	// BigInt p = big_int_constructor(0, 4,
	// 	0x89ABCDF9 ,0x12345678, 0xCAFEBABE ,0x0BADF00D
	// );


	 
	// BigInt n = {NULL, 0, 0};
	// BigInt e = {NULL, 0, 0};
	// BigInt d = {NULL, 0, 0};
	
	// BigInt cipher = {NULL, 0, 0};
	// BigInt plain = {NULL, 0, 0};

	// char * tmpmsg = "string";
	
	// generate(&n,&e,&d,1024);

	// //big_int_print(&a,0);
	// big_int_print(&message,2);
	// big_int_print(&e,2);
	// big_int_print(&n,2);
	// big_int_print(&d,2);



	// encrypt(&message,&d,&n,&cipher);
	// //big_int_modpow(&message,&e,&n,&cipher);

	// printf("message = ");
	// big_int_print(&message,2);
		
	// printf("cipher = ");
	// big_int_print(&cipher,2);

	// decrypt(&cipher,&e,&n,&plain);

	// printf("plain = ");
	// big_int_print(&plain,2);

    // char * pubfile = "pubkey_out.txt";
	// char * privfile = "privkey_out.txt";
	// cli_generate(2048,pubfile,privfile);
	// size_t mod_len,exp_len;
	// BigInt mod,exp,mod2,priv;
	
	// read_rsa_key(pubfile,&mod_len,&exp_len,&mod,&exp);
	// read_rsa_key(privfile,&mod_len,&exp_len,&mod2,&priv);

	// printf("mod = ");
	// big_int_print(&mod,2);

	// printf("pubkey = ");
	// big_int_print(&exp,2);

	// printf("mod2 = ");
	// big_int_print(&mod2,2);

	// printf("pribkey = ");
	// big_int_print(&priv,2);
	
	char * pubKey = "pubkey_out.txt";
	char * privKey = "privkey_out.txt";
	char * inFile = "ex.bmp";
	char * outFile = "encrypted.txt";

	cli_encrypt(pubKey,inFile,outFile);
	 cli_decrypt(privKey,outFile,"decrypted.bmp");
	
	
	// big_int_destructor(&n);
	// big_int_destructor(&message);
}
