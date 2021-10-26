#include "bitoperation.h"
#include "DES.h"
#include <iostream>
#include<math.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

typedef unsigned char BYTE; 
typedef unsigned long long WORD; //Taking for 64 bits 

#define QLEN (sizeof(WORD))

int convert_number2array(WORD arr[16], WORD number)
{
	int digit = floor(log10(number)) + 1;
	int i;
	for (i = digit; i > 0; i--) {
		arr[digit - i] = (WORD)(number / pow(10, i - 1)) % 10;
	}

	return digit;
}

WORD convert_arraytonum(WORD* arr, int num)
{
	int i = 0;
	WORD final_num=0;
	for (i = 0; i<num; i++)
	{
		final_num = final_num * 10 + arr[i];
	}

	return final_num;
}


static void convert_word64_to_bytes(BYTE* into, WORD outof)
{
	*into++ = (BYTE)((outof >> 56) & 0xffL);
	*into++ = (BYTE)((outof >> 48) & 0xffL);
	*into++ = (BYTE)((outof >> 40) & 0xffL);
	*into++ = (BYTE)((outof >> 32) & 0xffL);
	*into++ = (BYTE)((outof >> 24) & 0xffL);
	*into++ = (BYTE)((outof >> 16) & 0xffL);
	*into++ = (BYTE)((outof >> 8) & 0xffL);
	*into++ = (BYTE)(outof & 0xffL);
}

static WORD convert_word64_from_bytes(const BYTE* outof)
{
	WORD into;
	into = (WORD)(*outof++ & 0xffL) << 56;
	into |= (WORD)(*outof++ & 0xffL) << 48;
	into |= (WORD)(*outof++ & 0xffL) << 40;
	into |= (WORD)(*outof++ & 0xffL) << 32;
	into |= (WORD)(*outof++ & 0xffL) << 24;
	into |= (WORD)(*outof++ & 0xffL) << 16;
	into |= (WORD)(*outof++ & 0xffL) << 8;
	into |= (WORD)(*outof++ & 0xffL);
	return into;
}


void encrypt_DES(unsigned char* key, BYTE data[16])
{

	key_set* key_sets = (key_set*)malloc(17 * sizeof(key_set));
	generate_sub_keys(key, key_sets); //Generate key set
	process_message(data, data, key_sets, 1); //DES 

}


void pseudo_random_function(BYTE* Encrypt, const BYTE* input, size_t nbytes, size_t nbits, unsigned char* key, size_t tweak)
{
	BYTE Block[16];	/* 128-bit encryption block */
	size_t block_length = sizeof(Block);
	memset(Block, 0, block_length);
	memcpy(Block, input, nbytes);
	Block[block_length - 1] = (BYTE)tweak; //Adding tweak here at end 
	bit_pr_bytes("B=      ", Block, block_length, "\n");
	encrypt_DES(key, Block);	/* B = E(key, B) */
	bit_pr_bytes("E(k,B)= ", Block, block_length, "\n");
	bit_substring(Encrypt, Block, nbytes, nbits, 0, nbits);
}


void feistel_encrypt(BYTE* data, size_t nbytes, size_t nbits, unsigned char* key, size_t tweak)
{
	size_t halfbits = nbits / 2;	//To get u , v as per text book .
	size_t hlen = nbytes / 2;
	size_t n, i, index, length;
	BYTE Encrypt[8];
	BYTE right[8], left[8];

	index = 0;
	length = halfbits;
	n = bit_substring(left, data, nbytes, nbits, index, length);
	bit_pr_bytes("L=", left, hlen, "\t");
	bit_print("", left, hlen, n, "\n");
	index = halfbits;
	length = halfbits;
	n = bit_substring(right, data, nbytes, nbits, index, length);
	bit_pr_bytes("R=", right, hlen, "\t");
	bit_print("", right, hlen, n, "\n");

	 pseudo_random_function(Encrypt, right, hlen, halfbits, key, tweak);
	 bit_pr_bytes("E=      ", Encrypt, hlen, "\n");
	 bit_pr_bytes("L=      ", left, hlen, "\n");
	 for (i = 0; i < hlen; i++)
		left[i] = left[i] ^ Encrypt[i];
	 bit_substring(left, left, hlen, halfbits, 0, halfbits);
	 bit_pr_bytes("L XOR E=", left, hlen, "\n");
	
		/* Concatenate bitstrings L' || R' */
	 bit_pr_bytes("L'=", right, hlen, "\t");
	 bit_print("", right, hlen, halfbits, "\n");
	 bit_pr_bytes("R'=", left, hlen, "\t");
	 bit_print("", left, hlen, halfbits, "\n");
	 bit_cat(data, nbytes, right, hlen, halfbits, left, hlen, halfbits);
	 bit_print("", data, nbytes, nbits, "\n");
	 bit_pr_index("", nbits, "\n");
}

void feistel_decrypt(BYTE* data, size_t nbytes, size_t nbits, unsigned char* key, size_t tweak)
{
	size_t halfbits = nbits / 2;	
	size_t hlen = nbytes / 2;
	size_t n, i, index, length;
	BYTE Encrypt[8];
	BYTE right[8], left[8];

	index = 0;
	length = halfbits;
	n = bit_substring(left, data, nbytes, nbits, index, length);
	bit_pr_bytes("L=", left, hlen, "\t");
	bit_print("", left, hlen, n, "\n");
	index = halfbits;
	length = halfbits;
	n = bit_substring(right, data, nbytes, nbits, index, length);
	bit_pr_bytes("R=", right, hlen, "\t");
	bit_print("", right, hlen, n, "\n");

	pseudo_random_function(Encrypt, left, hlen, halfbits, key, tweak);
	bit_pr_bytes("E=      ", Encrypt, hlen, "\n");
	bit_pr_bytes("R=      ", right, hlen, "\n");
	for (i = 0; i < hlen; i++)
		right[i] = right[i] ^ Encrypt[i];
	bit_substring(right, right, hlen, halfbits, 0, halfbits);
	bit_pr_bytes("R XOR E=", right, hlen, "\n");

	bit_pr_bytes("L'=", right, hlen, "\t");
	bit_print("", right, hlen, halfbits, "\n");
	bit_pr_bytes("R'=", left, hlen, "\t");
	bit_print("", left, hlen, halfbits, "\n");
	bit_cat(data, nbytes, right, hlen, halfbits, left, hlen, halfbits);
	bit_print("", data, nbytes, nbits, "\n");
	bit_pr_index("", nbits, "\n");
}


void encrypt(WORD* final_output, WORD card_number, WORD max_number, size_t maxbits, unsigned char* key, size_t nrounds)
{
	BYTE a[QLEN], b[QLEN], c[QLEN];
	size_t n, nbits, r;
	int loops = 0;

	do
	{
		loops++;
		convert_word64_to_bytes(a, card_number);
		bit_pr_bytes("arr=", a, QLEN, "\n");
		// Encode to 54-bit string
		nbits = maxbits;
		n = bit_encode(b, a, QLEN, nbits);
		cout << "Number of bits" << " " << n << endl;
		bit_pr_bytes("", b, n, "\n");
		bit_print("", b, n, nbits, "\n");

		
	   /* Fiestel encryption on blocks of nbits */
	    for (r = 1; r <= nrounds; r++)
	    {
		    cout << "Round: " << r << endl;
			bit_pr_bytes("input=  ", b, n, "\n");
			feistel_encrypt(b, QLEN, nbits, key, r);
			bit_pr_bytes("output= ", b, n, "\n");
		}
	
		// Decode to 64-bit QWORD
		n = bit_decode(c, b, QLEN, nbits);
		cout << "Number of bits:" << n << endl;
		bit_pr_bytes("", c, QLEN, "\n");
		bit_print("", c, QLEN, QLEN * 8, "\n");
		card_number = convert_word64_from_bytes(c);
		cout << "final value : " << card_number << endl;
	} while (card_number > max_number);

	*final_output = card_number;

}
void decrypt(WORD* final_output, WORD card_number, WORD max_number, size_t maxbits, unsigned char* key, size_t nrounds)
{
	BYTE a[QLEN], b[QLEN], c[QLEN];
	size_t n, nbits, r;
	int loops = 0;

	do
	{
		loops++;
		convert_word64_to_bytes(a, card_number);
		bit_pr_bytes("arr=", a, QLEN, "\n");
		// Encode to 54-bit string
		nbits = maxbits;
		n = bit_encode(b, a, QLEN, nbits);
		cout << "Number of bits" << " " << n << endl;
		bit_pr_bytes("", b, n, "\n");
		bit_print("", b, n, nbits, "\n");

	
		/* Fiestel decryption on blocks of nbits */
		for (r = nrounds; r >= 1; r--)
		{
			cout << "Round: " << r << endl;
			bit_pr_bytes("input=  ", b, n, "\n");
			feistel_decrypt(b, QLEN, nbits, key, r);
			bit_pr_bytes("output= ", b, n, "\n");
		}

		// Decode to 64-bit QWORD
		n = bit_decode(c, b, QLEN, nbits);
		cout << "Number of bits:" << n << endl;
		bit_pr_bytes("", c, QLEN, "\n");
		bit_print("", c, QLEN, QLEN * 8, "\n");
		card_number = convert_word64_from_bytes(c);
		cout << "final value : " << card_number << endl;
	} while (card_number > max_number);

	*final_output = card_number;
	
}

int stop_on_multiple_loops = 0;

int main()
{
	const int len_encrypt = 6;
	unsigned char x[] = "000102030405060708090A0B0C0D0E0F"; //Assuming a key 
	unsigned char* key = (unsigned char*)x;

	WORD input_card_numer;
	WORD plaintext[len_encrypt] = { 0,0,0,0,0,0 };
	WORD tweak[len_encrypt] = { 0,0,0,0,0,0 };
	WORD cipher[len_encrypt] = { 0,0,0,0,0,0 };
	WORD input_value;
	WORD arr[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int digit;
	int option = 0;
	const WORD ccmax = 999999; //Maximum possible value for 6 digits number
	const size_t maxbits = 21; //log(6)/log(2)

	const size_t nrounds = 7; 

	cout << "Please enter 1 for encryption and 2 for decryption" << endl;
	cin >> option;

	cout << "Enter the card number to be encrypted/decrypted";
	cin >>input_card_numer;

	digit=convert_number2array(arr, input_card_numer);

	cout << "Card Number entered:";

	for (int i = 0; i < 16; i++)
		cout << arr[i];
	cout << endl;

	WORD cce, ccd = 0;
	/*Finding tweak here for the card number*/
	cout << "Tweak:";
	for (int i = 0; i <2; i++)
	{
		tweak[i] = arr[i];
		cout << tweak[i];
	}

	for (int j = 2; j <6; j++)
	{
		tweak[j] = arr[j+10 ];
		cout << tweak[j];
	}
    cout << endl;

	for (int i = 0; i < len_encrypt; i++)
	{
		plaintext[i] = arr[i + len_encrypt];
		cipher[i]= arr[i + len_encrypt];
	}

	if (option == 1)
	{
	   cout << "Option picked is encryption:" << endl;

	   for (int i = 0; i < len_encrypt; i++)  //Adding tweak here to the 6 digit number
	   {
		  plaintext[i] = (plaintext[i] + tweak[i]) % 10;
	   }
	   input_value = convert_arraytonum(plaintext, len_encrypt); //converting from array back to WORD

	   cout << "Plain text + Tweak for encryption: " << input_value << endl;
        
	   encrypt(&cce, input_value, ccmax, maxbits, key, nrounds);

	   cout << "Encrypted value is as follows: " << cce << endl;

	   digit = convert_number2array(plaintext, cce); //Converting to array 
	}

	else if (option == 2)
	{
		cout << "Option picked is decryption :" << endl;

		WORD decrpt_input= convert_arraytonum(cipher, len_encrypt);

		decrypt(&ccd, decrpt_input, ccmax, maxbits, key, nrounds);

		digit = convert_number2array(plaintext, ccd);

		cout << "Decrypted data:" << endl;
		for (int i = 0; i < len_encrypt; i++)
		{
			plaintext[i] = (plaintext[i] - tweak[i]+10) % 10;  //using tweak here to get back the plain text during decryption
			cout << plaintext[i];
		}
		cout << endl;
	}

	else
	{
		cout << "Invalid option"<<endl;
	}
	
	cout << "Final Card number:" << endl;

	for (int i = 0; i < 3; i++)
		cout << arr[i];

	for(int i=3;i<6;i++)
		cout<< arr[i];

	for (int i = 0; i < 6; i++)
		cout << plaintext[i];

	cout << arr[12];

	for(int i=3 ; i<6;i++)
		cout<< tweak[i];

	cout << endl;
	   
	
	return 0;
}
