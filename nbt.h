/*
   Copyright Alexander Argentakis
   Repo: https://github.com/MFDGaming/CNBT
   This file is licensed under the GPL v2.0 license
 */

#ifndef NBT_H
#define NBT_H

#include "./cbinarystream/binary_stream.h"

#define END_TAG 0
#define BYTE_TAG 1
#define SHORT_TAG 2
#define INT_TAG 3
#define LONG_TAG 4
#define FLOAT_TAG 5
#define DOUBLE_TAG 6
#define BYTE_ARRAY_TAG 7
#define STRING_TAG 8
#define LIST_TAG 9
#define COMPOUND_TAG 10
#define INT_ARRAY_TAG 11
#define LONG_ARRAY_TAG 12

#define BIG_ENDIAN 0
#define LITTLE_ENDIAN 1
#define NETWORK_ENDIAN 2

union multi;

typedef struct {
	char *data;
	long size;
} byte_array_t;

typedef struct {
        long *data;
        long size;
} int_array_t;

typedef struct {
        long long *data;
        long size;
} long_array_t;

typedef struct {
	char tag_id;
	long size;
	union multi *data;
} list_t;

typedef struct {
	int size;
	char *tag_ids;
	char **names;
	union multi *data;
} compound_t;

union multi {
	char byte_tag;
	short short_tag;
	long int_tag;
	long long long_tag;
	float float_tag;
	double double_tag;
	byte_array_t byte_array_tag;
	char *string_tag;
	list_t list_tag;
	int_array_t int_array_tag;
	long_array_t long_array_tag;
	compound_t compound_tag;
};

char get_byte_tag(binary_stream_t *stream);

short get_short_tag(int endianess, binary_stream_t *stream);

long get_int_tag(int endianess, binary_stream_t *stream);

long long get_long_tag(int endianess, binary_stream_t *stream);

float get_float_tag(int endianess, binary_stream_t *stream);

double get_double_tag(int endianess, binary_stream_t *stream);

byte_array_t get_byte_array_tag(int endianess, binary_stream_t *stream);

char *get_string_tag(int endianess, binary_stream_t *stream);

union multi get_multi_tag(char tag_id, int endianess, binary_stream_t *stream);

list_t get_list_tag(int endianess, binary_stream_t *stream);

compound_t get_compound_tag(int endianess, binary_stream_t *stream);

int_array_t get_int_array_tag(int endianess, binary_stream_t *stream);

long_array_t get_long_array_tag(int endianess, binary_stream_t *stream);

void put_byte_tag(char value, binary_stream_t *stream);

void put_short_tag(short value, int endianess, binary_stream_t *stream);

void put_int_tag(long value, int endianess, binary_stream_t *stream);

void put_long_tag(long long value, int endianess, binary_stream_t *stream);

void put_float_tag(float value, int endianess, binary_stream_t *stream);

void put_double_tag(double value, int endianess, binary_stream_t *stream);

void put_byte_array_tag(byte_array_t value, int endianess, binary_stream_t *stream);

void put_string_tag(char *value, int endianess, binary_stream_t *stream);

void put_list_tag(list_t value, int endianess, binary_stream_t *stream);

void put_compound_tag(compound_t value, int endianess, binary_stream_t *stream);

void put_int_array_tag(int_array_t value, int endianess, binary_stream_t *stream);

void put_long_array_tag(long_array_t value, int endianess, binary_stream_t *stream);

#endif
