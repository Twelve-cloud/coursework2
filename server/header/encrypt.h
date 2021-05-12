#ifndef ENCRYPT_H
#define ENCRYPT_H

#define KEY 17

#define ENCRYPTION_CYCLE(str, key) 									\
        for(uint32_t i = 0; i < str.size(); i++)                    \
        str[i] ^= key;                                              \

#endif // ENCRYPT_H
