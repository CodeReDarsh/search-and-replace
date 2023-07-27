#include "../include/hw6.h"

struct Update {
    char *stop_ptr;
    int ptr_shift;
};

void advanced_search(char *stext, char *rtext, FILE *infile, FILE *outfile, char lflag, long start_line, long end_line, char wflag);

struct Update wildcard_helpr(char *stext, char *buffer);

int main(int argc, char *argv[]) {

    if (argc < 7) return MISSING_ARGUMENT;

    extern char *optarg;
	extern int optind;
	int c; 
	char sflag=0, rflag=0, lflag=0, wflag=0;
    char *stext=NULL, *rtext=NULL;
    bool s_err=false, r_err=false, l_err=false;
    long start_line=-1, end_line=-1;

	while ((c = getopt(argc, argv, "s:r:l:w")) != -1){
		switch (c) {
            case 's': {
                if (++sflag > 1) return DUPLICATE_ARGUMENT;
                if (optarg[0] == '-') s_err = true;
                else stext = optarg;
                break;
            }

            case 'r':{
                if (++rflag > 1) return DUPLICATE_ARGUMENT;
                if (optarg[0] == '-') r_err = true;
                else rtext = optarg;
                break;
            }

            case 'l':{
                if (++lflag > 1) return DUPLICATE_ARGUMENT;
                if (optarg[0] == '-') l_err = true;
                char *pch = strtok(optarg, ",");
                while (pch != NULL)
                {
                    if (start_line == -1) start_line = strtol(pch, NULL, 10);
                    else {
                        if (end_line == -1) end_line = strtol(pch, NULL, 10);
                    }
                    pch = strtok(NULL, ",");
                }
                if (start_line < 1 || end_line < 1 || start_line > end_line) l_err = true;
                break;
            }

            case 'w':{
                if(++wflag > 1) return DUPLICATE_ARGUMENT;
                break;
            }

            case '?':
                break;
		}
    }

    FILE *infile, *outfile;

    if (sflag == 0) s_err = true;
    if (rflag == 0) r_err = true;

    if ((infile = fopen(argv[argc - 2], "r")) == NULL) return INPUT_FILE_MISSING;
    else if ((outfile = fopen(argv[argc - 1], "w")) == NULL) return OUTPUT_FILE_UNWRITABLE;
    else if (s_err) return S_ARGUMENT_MISSING;
    else if (r_err) return R_ARGUMENT_MISSING;
    else if (l_err) return L_ARGUMENT_INVALID;
    else {
        if(wflag == 1 && ((stext[0] != '*' && stext[strlen(stext) - 1] != '*') || (stext[0] == '*' && stext[strlen(stext) - 1] == '*')) ) {return WILDCARD_INVALID;}
    }  

    advanced_search(stext, rtext, infile, outfile, lflag, start_line, end_line, wflag);

    fclose(infile);
    fclose(outfile);
    return 0;
}

struct Update wildcard_helpr(char *stext, char *buffr_ptr) {
    struct Update ptr_updt = {NULL, 0};
    char *ptr = NULL;

    // for prefixes
    if (stext[strlen(stext) - 1] == '*'){
        char new_stext[MAX_SEARCH_LEN];

        strncpy(new_stext, stext, strlen(stext) - 1);
        new_stext[strlen(stext) - 1] = '\0';

        ptr = strstr(buffr_ptr, new_stext);
        
        if (ptr != buffr_ptr){
            while(ptr != NULL && isalnum(*(ptr-1))) {
                if ((ptr + strlen(new_stext)) < buffr_ptr + (strlen(buffr_ptr))) {
                    ptr = strstr((ptr + strlen(new_stext)), new_stext);
                } else {
                    ptr = NULL;
                }
            }
        }

        if (ptr == NULL) return ptr_updt;

        ptr_updt.stop_ptr = ptr;
        
        int shift_amt = 0;

        while (*ptr != '\0' && !ispunct(*ptr) && !isspace(*ptr) && ptr < (buffr_ptr + strlen(buffr_ptr))) {
            shift_amt++;
            ptr++;
        }
        ptr_updt.ptr_shift = shift_amt;

    } else {    // for suffixes

        char new_stext[MAX_SEARCH_LEN];
        strcpy(new_stext, (stext + 1));

        ptr = strstr(buffr_ptr, new_stext);

        while(ptr != NULL && isalnum(*(ptr + strlen(new_stext)))){
            if ((ptr + strlen(new_stext)) < buffr_ptr + (strlen(buffr_ptr))) {
                ptr = strstr((ptr + strlen(new_stext)), new_stext);
            } else {
                ptr = NULL;
            }
        }

        if (ptr == NULL) return ptr_updt;

        int shift_amt = (int) strlen(new_stext);

        while (ptr >= buffr_ptr && *ptr != '\0' && !ispunct(*ptr) && !isspace(*ptr)) {
            shift_amt++;
            ptr--;
        }
        ptr++;
        ptr_updt.ptr_shift = --shift_amt;
        ptr_updt.stop_ptr = ptr;
    }

    return ptr_updt;
}

void advanced_search(char *stext, char *rtext, FILE *infile, FILE *outfile, char lflag, long start_line, long end_line, char wflag) {
    long line_num = 1;
    char buffer[MAX_LINE];

    while (fgets(buffer, MAX_LINE, infile)) {
        char *ptr = buffer;
        char *stop_ptr;
        struct Update ptr_updt = {NULL, 0}; 

        if (wflag){
            ptr_updt = wildcard_helpr(stext, ptr); 
            stop_ptr = ptr_updt.stop_ptr;

        } else {
            stop_ptr = strstr(buffer, stext);
        }
    
        while (*ptr != '\0') {

            if ((lflag && line_num >= start_line && line_num <= end_line) || lflag == 0) {
                if (stop_ptr != NULL && ptr == stop_ptr) {
                    fprintf(outfile, "%s", rtext);

                    if (wflag) {
                        ptr += ptr_updt.ptr_shift;
                        ptr_updt = wildcard_helpr(stext, ptr);
                        stop_ptr = ptr_updt.stop_ptr;
                    
                    } else {
                        ptr += strlen(stext);
                        stop_ptr = strstr(ptr, stext);
                    }
                    
                }
            }

            if (*ptr == '\0' || *ptr == EOF) break;

            fprintf(outfile, "%c", *ptr);

            ptr++;
        }

        line_num++; 
    }
}
