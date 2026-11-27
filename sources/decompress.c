#include <stdio.h>
#include <stdlib.h>

typedef struct razzhaitie_ctx razzhaitie_ctx;
typedef void (*razzhaitie_output_cb)(const unsigned char *, int);

extern void zhaitie_init(void);
extern razzhaitie_ctx *razzhaitie_new(void);
extern int razzhaitie(
    razzhaitie_ctx *,
    const unsigned char *,
    int,
    razzhaitie_output_cb
);

static FILE *output;

static void emit_output(const unsigned char *data, int length)
{
    if (length > 0 &&
        fwrite(data, 1, (size_t)length, output) != (size_t)length) {
        perror("fwrite");
        exit(1);
    }
}

int main(int argc, char **argv)
{
    const char *input_name = argc > 1 ? argv[1] : "test.zai";
    const char *output_name = argc > 2 ? argv[2] : "presentation.out";

    FILE *input = fopen(input_name, "rb");
    if (input == NULL) {
        perror(input_name);
        return 1;
    }

    output = fopen(output_name, "wb");
    if (output == NULL) {
        perror(output_name);
        fclose(input);
        return 1;
    }

    zhaitie_init();

    unsigned char *buffer = malloc(24);
    if (buffer == NULL) {
        perror("malloc");
        return 1;
    }

    if (fread(buffer, 1, 24, input) != 24) {
        fprintf(stderr, "input is shorter than 24 bytes\n");
        return 1;
    }

    razzhaitie_ctx *ctx = razzhaitie_new();
    int length = 24;

    for (;;) {
        int next_length = razzhaitie(ctx, buffer, length, emit_output);

        if (next_length == 0)
            break;

        if (next_length < 0) {
            fprintf(stderr, "decoder returned a negative length\n");
            return 1;
        }

        unsigned char *next_buffer = realloc(buffer, (size_t)next_length);
        if (next_buffer == NULL) {
            perror("realloc");
            return 1;
        }

        buffer = next_buffer;
        length = next_length;

        if (fread(buffer, 1, (size_t)length, input) != (size_t)length) {
            fprintf(stderr, "compressed stream ended early\n");
            return 1;
        }
    }

    free(buffer);
    fclose(output);
    fclose(input);
    return 0;
}
