//
// Created by 10051644 on 16/05/2018.
//

#ifndef DES_DES_H
#define DES_DES_H

#define ENCRYPTION_MODE 1
#define DECRYPTION_MODE 0

typedef struct {
    unsigned char k[8];
    unsigned char c[4];
    unsigned char d[4];
} key_set;

void generate_key(unsigned char* key);
void generate_sub_keys(unsigned char* main_key, key_set* key_sets);
void process_message(unsigned char* message_piece, unsigned char* processed_piece, key_set* key_sets, int mode);

#endif //DES_DES_H
