#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "des.h"

#define DES_KEY_SIZE 8

unsigned char* keyFile="des.key";
unsigned char* sourceFile="des_source.txt";
unsigned char* encFile="des_enc_c.bin";
unsigned char* decFile="des_dec_c.txt";

FILE *key_file, *input_file, *output_file;

int process(short int process_mode, key_set* key_sets, unsigned char* inFile,unsigned char* outFile);

int main() {
    printf("DES test\n");

    //Allocate space for DES key
    unsigned char* des_key=(unsigned char*)malloc(8*sizeof(char));

    if(access(keyFile,F_OK)==-1){
        key_file=fopen(keyFile,"wb");
        if(!key_file){
            printf("Could not open key file");
            return 1;
        }

        unsigned int iseed=(unsigned int)time(NULL);
        srand(iseed);

        short int bytes_written;

        generate_key(des_key);
        bytes_written=fwrite(des_key,1,DES_KEY_SIZE,key_file);
        fclose(key_file);
        printf("DES key generated");
    } else {
        key_file=fopen(keyFile,"rb");
        if(!key_file){
            printf("Could not open key file\n");
            return 1;
        }

        short int bytes_read;
        bytes_read=fread(des_key,sizeof(unsigned char),DES_KEY_SIZE,key_file);
        fclose(key_file);
        printf("DES key loaded from file\n");
    }

    key_set* key_sets=(key_set*)malloc(17*sizeof(key_set));
    generate_sub_keys(des_key,key_sets);

    process(ENCRYPTION_MODE,key_sets,sourceFile,encFile);
    process(DECRYPTION_MODE,key_sets,encFile,decFile);

    free(des_key);
    free(key_sets);
    return 0;
}

int process(short int process_mode, key_set* key_sets, unsigned char* inFile,unsigned char* outFile){
    clock_t start, finish;
    double time_taken;
    unsigned long file_size;
    unsigned short int padding;
    short int bytes_written;
    unsigned long block_count = 0, number_of_blocks;
    unsigned char* data_block = (unsigned char*) malloc(8*sizeof(char));
    unsigned char* processed_block = (unsigned char*) malloc(8*sizeof(char));

    start = clock();

    input_file=fopen(inFile,"rb");
    if(!input_file){
        printf("Could not read input file");
        return -1;
    }

    output_file=fopen(outFile,"wb");
    if(!output_file){
        printf("Could not write to output file.");
        return -1;
    }

    fseek(input_file,0L,SEEK_END);
    file_size=ftell(input_file);

    fseek(input_file,0L,SEEK_SET);
    number_of_blocks=file_size/8+((file_size%8)?1:0);

    while(fread(data_block,1,8,input_file)){
        block_count++;
        if(block_count==number_of_blocks){
            if(process_mode==ENCRYPTION_MODE){
                padding=8-file_size%8;
                if(padding<8){
                    memset((data_block+8-padding),(unsigned char)padding,padding);
                }

                process_message(data_block, processed_block, key_sets, process_mode);
                bytes_written = fwrite(processed_block, 1, 8, output_file);

                if(padding==8){
                    memset(data_block,(unsigned char)padding,8);
                    process_message(data_block,processed_block,key_sets,process_mode);
                    bytes_written=fwrite(processed_block,1,8,output_file);
                }
            } else {
                process_message(data_block, processed_block, key_sets, process_mode);
                padding = processed_block[7];
                if (padding < 8) {
                    bytes_written = fwrite(processed_block, 1, 8 - padding, output_file);
                }
            }
        } else {
            process_message(data_block, processed_block, key_sets, process_mode);
            bytes_written = fwrite(processed_block, 1, 8, output_file);
        }
        memset(data_block, 0, 8);

        finish = clock();
    }

    free(data_block);
    free(processed_block);
    fclose(input_file);
    fclose(output_file);

    time_taken = (double)(finish - start)/(double)CLOCKS_PER_SEC;
    printf("Finished processing %s. Time taken: %lf seconds.\n", inFile, time_taken);

    return 0;
}