#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "strfy.h"


/* cbuff: Clear Buffer */
void cbuff(char *buff, int l){
	for(int i = 0; i < l; ++i)
		buff[i] = '\0';
}


/* prtprt: formatted print in (color-full) */
int prtprt(int status, char *frmt, ...) {
	char buff[MALL];
	va_list args;
	va_start(args, frmt);
	vsprintf(buff, frmt, args);
	str_replace(buff, "[red]", "\x1B[31m");
	str_replace(buff, "[grn]", "\x1B[32m");
	str_replace(buff, "[yel]", "\x1B[33m");
	str_replace(buff, "[blu]", "\x1B[34m");
	str_replace(buff, "[nrm]", "\x1B[0m");
	printf("%s%s\n", buff, "\x1B[0m");
	va_end(args);
	return status;
}


/* show_help_info: show help menu (--help) */
void show_help_info(){
	prtprt(0, "Assembeler for a non-standard 8-bit CPU");
	prtprt(0, "[blu]'-v'[nrm]              [yel]Verbose   [nrm]To show more output than usual");
	prtprt(0, "[blu]'-o [filename]'[nrm]   [yel]Output    [nrm]For specifying output file");
	prtprt(0, "[blu]'--help'[nrm]          [yel]Help      [nrm]To see help menu (this menu)");
}


/* convert given 'hex' into int by base 16 (hexadecimal) */
int int_base16(char *hex) {

	str_replace(hex, "0x", "");  // Remove (0x)...
	str_replace(hex, "0X", "");  // Remove (0x)...

	int len = strlen(hex);
	int decimal = 0;

	for (int i = 0; i < len; i++) {
		char c = hex[len - i - 1]; // right to left
		int digit;

		if (isdigit(c)) {
			digit = c - '0';
		} else {
			digit = toupper(c) - 'A' + 10; // letters A..F
		}

		decimal += digit * pow(16, i);
	}

	return decimal;
}


char *decimal_to_binary(int decimal_num) {
	int len = 14;
	char *binary_str = malloc(len + 1);

	// Fill with '0'
	memset(binary_str, '0', len);
	binary_str[len] = '\0';

	// Add '0b' prefix
	binary_str[0] = '0';
	binary_str[1] = 'b';

	// Convert decimal to binary and fill from the end
	int index = len - 1;
	while (decimal_num > 0 && index > 1) {
		binary_str[index] = (decimal_num % 2) + '0';
		decimal_num /= 2;
		index--;
	}

	return binary_str;
}



/* update_gfalg: updage General-Flags with given arguments from user */
void update_gfalg(GFLAGS *gf, int argc, char *argv[]){
	int i;
	int saving = 0;

	gf->verbose = 0;
	gf->filename = malloc(MALL * sizeof(char));
	strcpy(gf->filename, "./out_bin");

	for(i = 0; i < argc; ++i){

		if(saving){
			strcpy(gf->filename, argv[i]);
			saving = 0;
			continue;
		}

		if(strcmp(argv[i], "-o") == 0){
			saving = 1;
		}

		// if(strcmp(argv[i], "--help") == 0)
		// 	show_help_info();

		if((saving == 1) && (i == argc - 1)){
			prtprt(0, "[yel]No output file!\n[nrm]after '-o' needs to be a file name! {%d}", saving);
			exit(0);
		}

		// Seach for short flags such as -v, etc...
		for(int j = 0; j < (int)strlen(argv[i]); ++j){
			if(argv[i][0] == '-'){
				switch (argv[i][j]) {
					case 'v':
						gf->verbose = 1;
						break;
					default: break;
				}
			}
		}

	}
}

