/*
   Copyright Alexander Argentakis
   Repo: https://github.com/MFDGaming/CNBT
   This file is licensed under the GPL v2.0 license
 */

#include "./nbt.h"
#include <stdlib.h>
#include <string.h>

char get_byte_tag(binary_stream_t *stream)
{
	return get_byte(stream);
}

short get_short_tag(int endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case BIG_ENDIAN:
			return get_short_be(stream);
		case LITTLE_ENDIAN:
		case NETWORK_ENDIAN:
			return get_short_le(stream);
		default:
			return 0;
	}
}

long get_int_tag(int endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case BIG_ENDIAN:
			return get_int_be(stream);
		case LITTLE_ENDIAN:
			return get_int_le(stream);
		case NETWORK_ENDIAN:
			return get_signed_var_int(stream);
		default:
			return 0;
	}
}

long long get_long_tag(int endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case BIG_ENDIAN:
			return get_long_be(stream);
		case LITTLE_ENDIAN:
			return get_long_le(stream);
		case NETWORK_ENDIAN:
			return get_signed_var_long(stream);
		default:
			return 0;
	}
}

float get_float_tag(int endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case BIG_ENDIAN:
			return get_float_be(stream);
		case LITTLE_ENDIAN:
		case NETWORK_ENDIAN:
			return get_float_le(stream);
		default:
			return 0;
	}
}

double get_double_tag(int endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case BIG_ENDIAN:
			return get_double_be(stream);
		case LITTLE_ENDIAN:
		case NETWORK_ENDIAN:
			return get_double_le(stream);
		default:
			return 0;
	}
}

byte_array_t get_byte_array_tag(int endianess, binary_stream_t *stream)
{
	byte_array_t byte_array;
	byte_array.size = get_int_tag(endianess, stream);
	byte_array.data = malloc(byte_array.size * sizeof(char));
	int i;
	for (i = 0; i < byte_array.size; ++i) {
		byte_array.data[i] = get_byte_tag(stream);
	}
	return byte_array;
}

char *get_string_tag(int endianess, binary_stream_t *stream)
{
	long length;
	switch (endianess) {
                case BIG_ENDIAN:
                        length = get_unsigned_short_be(stream);
			break;
                case LITTLE_ENDIAN:
			length = get_unsigned_short_le(stream);
			break;
                case NETWORK_ENDIAN:
                        length = get_var_int(stream);
			break;
                default:
                        length = 0;
			break;
        }
	char *result = malloc((length + 1) * sizeof(char));
	int i;
	for (i = 0; i < length; ++i) {
		result[i] = get_byte_tag(stream);
	}
	result[i] = 0;
	return result;
}

union multi get_multi_tag(char tag_id, int endianess, binary_stream_t *stream)
{
	union multi tag;
	switch (tag_id) {
		case BYTE_TAG:
			tag.byte_tag = get_byte_tag(stream);
			break;
		case SHORT_TAG:
			tag.byte_tag = get_short_tag(endianess, stream);
			break;
		case INT_TAG:
			tag.int_tag = get_int_tag(endianess, stream);
			break;
		case LONG_TAG:
			tag.long_tag = get_long_tag(endianess, stream);
			break;
		case FLOAT_TAG:
			tag.float_tag = get_float_tag(endianess, stream);
			break;
		case DOUBLE_TAG:
			tag.double_tag = get_double_tag(endianess, stream);
			break;
		case BYTE_ARRAY_TAG:
			tag.byte_array_tag = get_byte_array_tag(endianess, stream);
			break;
		case STRING_TAG:
			tag.string_tag = get_string_tag(endianess, stream);
			break;
		case LIST_TAG:
			tag.list_tag = get_list_tag(endianess, stream);
			break;
		case COMPOUND_TAG:
			tag.compound_tag = get_compound_tag(endianess, stream);
			break;
		case INT_ARRAY_TAG:
			tag.int_array_tag = get_int_array_tag(endianess, stream);
			break;
		case LONG_ARRAY_TAG:
			tag.long_array_tag = get_long_array_tag(endianess, stream);
			break;
	}
	return tag;
}

list_t get_list_tag(int endianess, binary_stream_t *stream)
{
	list_t list;
	list.tag_id = get_byte_tag(stream);
	list.size = get_int_tag(endianess, stream);
	list.data = malloc(list.size * sizeof(union multi));
	int i;
	for (i = 0; i < list.size; ++i) {
		list.data[i] = get_multi_tag(list.tag_id, endianess, stream);
	}
	return list;
}

compound_t get_compound_tag(int endianess, binary_stream_t *stream)
{
	compound_t compound;
	compound.size = 0;
	compound.tag_ids = malloc(0);
	compound.names = malloc(0);
	compound.data = malloc(0);
	while (stream->offset < stream->size) {
		char tag_id = get_byte_tag(stream);
		if (tag_id == 0) {
			break;
		}
		++compound.size;
		compound.tag_ids = realloc(compound.tag_ids, compound.size);
		compound.names = realloc(compound.names, compound.size * sizeof(char *));
		compound.data = realloc(compound.data, compound.size * sizeof(union multi));
		compound.tag_ids[compound.size - 1] = tag_id;
		compound.names[compound.size - 1] = get_string_tag(endianess, stream);
		compound.data[compound.size - 1] = get_multi_tag(tag_id, endianess, stream);
	}
	return compound;
}

int_array_t get_int_array_tag(int endianess, binary_stream_t *stream)
{
	int_array_t int_array;
	int_array.size = get_int_tag(endianess, stream);
	int_array.data = malloc(int_array.size * sizeof(char));
	int i;
	for (i = 0; i < int_array.size; ++i) {
		int_array.data[i] = get_int_tag(endianess, stream);
	}
	return int_array;
}

long_array_t get_long_array_tag(int endianess, binary_stream_t *stream)
{
	long_array_t long_array;
	long_array.size = get_int_tag(endianess, stream);
	long_array.data = malloc(long_array.size * sizeof(char));
	int i;
	for (i = 0; i < long_array.size; ++i) {
		long_array.data[i] = get_int_tag(endianess, stream);
	}
	return long_array;
}

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
