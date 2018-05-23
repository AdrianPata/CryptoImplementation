#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "sha256.h"

int main() {
    int pass;

    unsigned char* shaSrc="sha_src.txt";
    unsigned char* shaDest="sha_dest.txt";
    FILE *shaSrcFile,*shaDstFile;

    if(access(shaSrc,F_OK)==0){
        shaSrcFile=fopen(shaSrc,"rb");
        if(!shaSrcFile){
            printf("Could not open file '%s'. (%s)",shaSrc,strerror(errno));
            return 1;
        }

        fseek(shaSrcFile,0,SEEK_END);
        long shaSrcSize=ftell(shaSrcFile);
        fseek(shaSrcFile,0,SEEK_SET);
        char *shaSrcText=malloc(shaSrcSize);
        fread(shaSrcText,shaSrcSize,1,shaSrcFile);
        fclose(shaSrcFile);

        BYTE buf[SHA256_BLOCK_SIZE];
        SHA256_CTX ctx;

        sha256_init(&ctx);
        sha256_update(&ctx, shaSrcText, shaSrcSize);
        sha256_final(&ctx, buf);

        for(long i=0;i<SHA256_BLOCK_SIZE;i++){
            printf("%x ",buf[i]);
        }
        printf("\n");

        shaDstFile=fopen(shaDest,"wb");
        fwrite(buf,1,SHA256_BLOCK_SIZE,shaDstFile);
        fclose(shaDstFile);

        free(shaSrcText);
    } else {
        printf("File '%s' not found.");
        return 1;
    }

    return 0;
}