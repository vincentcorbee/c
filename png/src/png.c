#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

void reverse_bytes(void *source, size_t count)
{
  uint8_t *temp = source;

  for (int i = 0; i < count / 2; i++)
  {
    uint8_t t = temp[i];

    temp[i] = temp[count - i - 1];
    temp[count - i - 1] = t;
  }
}

void read_bytes(FILE *file, void *buf, size_t count)
{
  fread(buf, count, 1, file);

  if (count > 1)
    reverse_bytes(buf, count);
}

int main(int argc, char *argv[])
{
  char *file_path = argv[1];

  if (file_path == NULL)
  {
    fprintf(stderr, "Error: No file specified\n");
    exit(1);
  }

  FILE *file = fopen(file_path, "rb");

  if (file == NULL)
  {
    perror("Error");
    exit(1);
  }

  u_int8_t header_buff[8];

  fread(header_buff, sizeof(header_buff), 1, file);

  for (int i = 0; i < sizeof(header_buff); i++)
  {
    printf("\%u ", header_buff[i]);
  }

  printf("\n");

  u_int32_t chunk_length;
  u_int32_t chunk_type;
  u_int32_t crc;

  read_bytes(file, &chunk_length, sizeof(chunk_length));

  read_bytes(file, &chunk_type, sizeof(chunk_type));

  printf("chunk data length: %u\n", chunk_length);

  printf("chunk type: %u\n", chunk_type);

  uint8_t chunk_name[4] = {
      chunk_type & 0xFF,
      chunk_type >> 8 & 0xFF,
      chunk_type >> 16 & 0xFF,
      chunk_type >> 24 & 0xFF,
  };

  int ancillary = chunk_name[0] >> 5 & 1;
  int private = chunk_name[0] >> 5 & 1;
  int reserved = chunk_name[0] >> 5 & 1;
  int safe_to_copy = chunk_name[0] >> 5 & 1;

  printf("chunk name: %.4s", chunk_name);

  printf("\n");

  printf("Ancillary: %d\n", ancillary);
  printf("Private: %d\n", private);
  printf("Reserved: %d\n", reserved);
  printf("Safe to copy: %d\n", safe_to_copy);

  u_int32_t width;
  u_int32_t height;
  u_int8_t bit_depth;
  u_int8_t color_type;
  u_int8_t compression_method;
  u_int8_t filter_method;
  u_int8_t interlace_method;

  read_bytes(file, &width, sizeof(width));
  read_bytes(file, &height, sizeof(width));
  read_bytes(file, &bit_depth, sizeof(bit_depth));
  read_bytes(file, &color_type, sizeof(color_type));
  read_bytes(file, &compression_method, sizeof(compression_method));
  read_bytes(file, &filter_method, sizeof(filter_method));
  read_bytes(file, &interlace_method, sizeof(interlace_method));

  read_bytes(file, &crc, sizeof(crc));

  printf("width: %u\n", width);
  printf("height: %u\n", height);
  printf("bit depth: %u\n", bit_depth);
  printf("color type: %u\n", color_type);
  printf("compression method: %u\n", compression_method);
  printf("filter method: %u\n", filter_method);
  printf("interlace method: %u\n", interlace_method);

  return 0;
}