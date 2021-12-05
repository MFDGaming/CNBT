#include "./nbt.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *f = fopen("nbt.dat", "rb");
	fseek(f, 0, SEEK_END);
	binary_stream_t stream;
	stream.offset = 0;
	stream.size = ftell(f);
	fseek(f, 0, SEEK_SET);
	printf("%d\n", stream.size);
	stream.buffer = malloc(stream.size);
	fread(stream.buffer, 1, stream.size, f);
	compound_t compound = get_compound_tag(2, &stream);
	printf("%s\n", compound.data[1110].compound_tag.data[0].string_tag);
	return 0;
}
