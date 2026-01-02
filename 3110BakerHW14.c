/*
 * IPv6 Homework exercise
 * @author Jacob Baker
 * @date   11/19/2025
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage();
int fix_endianness(int x);
void to_ipv6_address_string(char* dest, int* nums);
void run_tests();

int main(int argc, char* argv[])
{
	int i;
	int nums[4];
	char addr[40]; // 8x(4 hex fields)+(7 colons)+nulchar

	if (argc != 5) {
		usage();
		exit(-1);
	}

	for (i = 0; i < 4; i++) {
		nums[i] = atoi(argv[i+1]);
	}

	// start your code here
	run_tests();
	
	// don't write code past this comment
	return 0;
} 

void to_ipv6_address_string(char* dest, int* nums) {
    unsigned short parts[8];

    for (int i = 0; i < 4; i++) {
        int fixed = fix_endianness(nums[i]);

        parts[(3 - i) * 2]     = (fixed >> 16) & 0xFFFF;
        parts[(3 - i) * 2 + 1] = fixed & 0xFFFF;
    }

    sprintf(dest,
        "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
        parts[7], parts[6], parts[5], parts[4],
        parts[3], parts[2], parts[1], parts[0]
    );
}

int fix_endianness(int x) {
	unsigned int b0 = (x >> 24) & 0xFF;
    unsigned int b1 = (x >> 16) & 0xFF;
    unsigned int b2 = (x >> 8) & 0xFF;
    unsigned int b3 = x & 0xFF;

    return (b2 << 24) | (b3 << 16) | (b0 << 8) | b1;
}

void test_fix_endianness(char* name, int input, int expected) {
	int actual;
	

	actual = fix_endianness(input);
	
	if (actual == expected) {
		printf("[passed]");
	} else {
		printf("[failed]");
	}
	printf(" test_fix_endianness %s\n", name);
	printf("\t    x was: 0x%08x\n", input);
	printf("\texpected = 0x%08x\n\t  actual = 0x%08x\n\n", expected, actual);
}

void test_to_ipv6_address_string(char* name, int input[], char* expected) {
	char actual[40];
	int rawnum[4];

	rawnum[0] = input[0];
	rawnum[1] = input[1];
	rawnum[2] = input[2];
	rawnum[3] = input[3];

	to_ipv6_address_string(actual, input);
	if (strncmp(actual, expected, 40) == 0) {
		printf("[passed]");
	} else {
		printf("[failed]");
	}
	printf(" test_to_ipv6_address_string %s\n", name);
	printf("\tinput int's: { 0x%08x, 0x%08x, 0x%08x, 0x%08x }\n",
		rawnum[0], rawnum[1], rawnum[2], rawnum[3]);
	printf("\texpected= \"%s\"\n\t  actual= \"%s\"\n\n", expected, actual);
}

void run_tests() {
	int allzeroes[4] = { 0, 0, 0, 0 };
	int allones[4] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
	int field0[4] = { 0, 0, 0, 0x0000abcd };
	int field1[4] = { 0, 0, 0, 0xabcd0000 };
	int field2[4] = { 0, 0, 0x0000abcd, 0 };
	int field3[4] = { 0, 0, 0xabcd0000, 0 };
	int field4[4] = { 0, 0x0000abcd, 0, 0 };
	int field5[4] = { 0, 0xabcd0000, 0, 0 };
	int field6[4] = { 0x0000abcd, 0, 0, 0 };
	int field7[4] = { 0xabcd0000, 0, 0, 0 };

	int fulladdress[4] = {
		0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff
	};

	test_fix_endianness("all zeroes", 0, 0);
	test_fix_endianness("all ones", 0xFFFFFFFF, 0xFFFFFFFF);
	test_fix_endianness("byte0 to byte2", 0xFF000000, 0x0000FF00);
 	test_fix_endianness("byte1 to byte3", 0x00FF0000, 0x000000FF);
	test_fix_endianness("byte2 to byte0", 0x0000FF00, 0xFF000000);
	test_fix_endianness("byte3 to byte1", 0x000000FF, 0x00FF0000);
	test_fix_endianness("swap byte0 and byte2", 0xFF00AA00, 0xAA00FF00);
	test_fix_endianness("swap byte1 and byte3", 0x00FF00AA, 0x00AA00FF);
	test_fix_endianness("rearrange all bytes", 0x00112233, 0x22330011);

	
	test_to_ipv6_address_string("all zeroes", allzeroes,
		"0000:0000:0000:0000:0000:0000:0000:0000");
	test_to_ipv6_address_string("all ones", allones,
		"ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff");
	test_to_ipv6_address_string("field 0", field0,
		"0000:0000:0000:0000:0000:0000:0000:abcd");
	test_to_ipv6_address_string("field 1", field1,
			"0000:0000:0000:0000:0000:0000:abcd:0000");
	test_to_ipv6_address_string("field 2", field2,
			"0000:0000:0000:0000:0000:abcd:0000:0000");
	test_to_ipv6_address_string("field 3", field3,
		"0000:0000:0000:0000:abcd:0000:0000:0000");
	test_to_ipv6_address_string("field 4", field4,
			"0000:0000:0000:abcd:0000:0000:0000:0000");
	test_to_ipv6_address_string("field 5", field5,
			"0000:0000:abcd:0000:0000:0000:0000:0000");
	test_to_ipv6_address_string("field 6", field6,
			"0000:abcd:0000:0000:0000:0000:0000:0000");
	test_to_ipv6_address_string("field 7", field7,
		"abcd:0000:0000:0000:0000:0000:0000:0000");

	test_to_ipv6_address_string("full address", fulladdress,
		"0011:2233:4455:6677:8899:aabb:ccdd:eeff");
}

void usage() {
	printf("Usage: ./asipv6 <int> <int> <int> <int>");
}