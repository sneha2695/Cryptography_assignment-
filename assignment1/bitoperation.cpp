#include <stdio.h>
#include "bitoperation.h"
typedef unsigned char BYTE;


void bit_print(const char* prefix, const unsigned char* data, size_t nbytes, size_t nbits, const char* suffix)
{
	size_t i, j;
	unsigned char mask;
	size_t nb = nbits / 8;
	size_t odd = nbits % 8;

	if (nb > nbytes)
	{
		nb = nbytes;
		odd = 0;
	}
	if (prefix) printf("%s", prefix);
	for (i = 0; i < nb; i++)
	{
		for (j = 0, mask = 0x80; j < 8; j++)
		{
			printf("%d", (data[i] & mask) ? 1 : 0);
			mask >>= 1;
		}
	}
	if (odd)
	{
		for (j = 0, mask = 0x80; j < odd; j++)
		{
			printf("%d", (data[i] & mask) ? 1 : 0);
			mask >>= 1;
		}
	}
	if (suffix) printf("%s", suffix);
}


void bit_pr_index(const char* prefix, size_t nbits, const char* suffix)
{
	size_t i;
	if (prefix) printf("%s", prefix);
	for (i = 0; i < nbits; i++)
	{
		printf("%1d", i % 10);
	}
	if (suffix) printf("%s", suffix);
}


void bit_pr_bytes(const char* prefix, const unsigned char* b, size_t n, const char* suffix)
{
	size_t i;
	if (prefix) printf("%s", prefix);
	for (i = 0; i < n; i++)
	{
		printf("%02X", b[i]);
	}
	if (suffix) printf("%s", suffix);
}

size_t bit_encode(BYTE* out, const BYTE* data, size_t nbytes, size_t nbits)
{
	size_t n, byteshift, bitshift, i;

	if (0 == nbytes) return 0;
	n = (nbits + 7) / 8;			
	byteshift = nbytes - n;			
	bitshift = n * 8 - nbits;		

	if (n > nbytes) //error condition
	{
		n = nbytes;
		byteshift = bitshift = 0;
	}

	for (i = 0; i < nbytes - byteshift; i++)
		out[i] = data[i + byteshift];
	for (; i < nbytes; i++)
		out[i] = 0;

	if (bitshift)
	{	/* Left shift */
		for (i = 0; i < nbytes - 1; i++)
		{
			out[i] = (out[i] << bitshift) | (out[i + 1] >> (8 - bitshift));
		}
		out[i] = out[i] << bitshift;
	}

	return n;	
}

size_t bit_decode(BYTE* out, const BYTE* data, size_t nbytes, size_t nbits)
{
	size_t n, byteshift, bitshift, i;

	if (0 == nbytes) return 0;
	n = (nbits + 7) / 8;			
	byteshift = nbytes - n;			
	bitshift = n * 8 - nbits;		

	if (n > nbytes) //error condition
	{	
		n = nbytes;
		byteshift = bitshift = 0;
	}

	for (i = nbytes; i > byteshift; i--)
		out[i - 1] = data[i - byteshift - 1];
	for (; i > 0; i--)
		out[i - 1] = 0;

	if (bitshift)
	{	/* Right shift */
		for (i = nbytes - 1; i > 0; i--)
		{
			out[i] = (out[i] >> bitshift) | (out[i - 1] << (8 - bitshift));
		}
		out[0] = out[0] >> bitshift;
	}

	return n;	
}


size_t bit_substring(BYTE* out, const BYTE* data, size_t nbytes, size_t nbits,
	size_t index, size_t length)
{
	size_t n, bitshift, i, j, t;
	size_t first, last;
	BYTE mask;

	if (0 == nbytes) return 0; //return 
	if (index >= nbits || 0 == length) //error condition
	{	
		for (i = 0; i < nbytes; i++)
			out[i] = 0;
		return 0;
	}
	if (index + length > nbits)
	{	
		length = nbits - index;
	}

	first = index / 8;
	last = (index + length) / 8;
	n = last - first + 1;
	bitshift = index % 8;

	for (i = 0; i < n; i++)
		out[i] = data[i + first];
	for (; i < nbytes; i++)
		out[i] = 0;

	if (bitshift)
	{	/* Left shift */
		for (i = 0; i < nbytes - 1; i++)
		{
			out[i] = (out[i] << bitshift) | (out[i + 1] >> (8 - bitshift));
		}
		out[i] = out[i] << bitshift;
	}
	/* Clear any extra bits on the right */
	for (t = 0, i = 0; i < n; i++)
	{
		for (mask = 0x80, j = 0; j < 8; j++, t++, mask >>= 1)
		{
			if (t >= length)
				out[i] &= (~mask);
		}
	}

	return length;
}

size_t bit_cat(BYTE* out, size_t outbytes,
	const BYTE* data1, size_t nbytes1, size_t nbits1,
	const BYTE* data2, size_t nbytes2, size_t nbits2)
{
	size_t n, n2, bitshift, i, j, t, length;

	n = (nbits1 + nbits2 + 7) / 8;
	if (n > outbytes)	//error condition
	{	
		for (i = 0; i < outbytes; i++)
			out[i] = 0;
		return 0;
	}

	
	n = (nbits1 + 7) / 8;
	bitshift = n * 8 - nbits1;
	for (i = 0; i < n; i++)
		out[i] = data1[i];
	for (; i < outbytes; i++)
		out[i] = 0;
	
	n2 = (nbits2 + 7) / 8;
	
	if (bitshift)
	{
		out[n - 1] |= data2[0] >> (8 - bitshift);
		for (i = 0; i < n2 - 1; i++)
			out[i + n] = (data2[i] << bitshift) | (data2[i + 1] >> (8 - bitshift));
	}
	else
	{	
		for (i = 0; i < n2; i++)
			out[i + n] = data2[i];
	}


	length = nbits1 + nbits2;
	for (t = 0, i = 0; i < outbytes; i++)
	{
		BYTE mask;
		for (mask = 0x80, j = 0; j < 8; j++, t++, mask >>= 1)
		{
			if (t >= length)
				out[i] &= (~mask);
		}
	}

	return length;
}


