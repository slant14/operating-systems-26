#include <limits.h>
#include <stdio.h>

unsigned position(const char *s)
{
    unsigned h=0;
    while(*s){
        //to handle 18->32 bits 
        h=(h*67+(unsigned char)*s++)&0x3FFFFu;
    }
    return h;
}

void diagnostics(void)
{
    printf("== Diagnostics: ==\n");
    printf(" unsigned has %d bits\n", (int)(sizeof(unsigned) * CHAR_BIT));
    printf(" check value: %u\n", position("X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*"));
    printf("== End of Diagnostics ==\n\n");
}

void add_data(char* document, const char* data)
{
    unsigned pos = position(data);
    document[pos] = *data;
}

void decrypt_document(void)
{
    char document[69] = {0};

    add_data(document, "70wCQ");
    add_data(document, "_0GW8");
    add_data(document, "O0CDS");
    add_data(document, "W0EMg");
    add_data(document, "D0dix");
    add_data(document, "o0KiO");
    add_data(document, "l0oTk");
    add_data(document, "y1TRq");
    add_data(document, "D0djH");
    add_data(document, "_1yII");
    add_data(document, "_0GVc");
    add_data(document, "00BEe");
    add_data(document, "52Vor");
    add_data(document, "D0djD");
    add_data(document, "O0CDO");
    add_data(document, "d02sx");
    add_data(document, "_0GVv");
    add_data(document, "_0GVh");
    add_data(document, "g1A7g");
    add_data(document, "_0GVq");
    add_data(document, "l1foc");
    add_data(document, "g1A7T");
    add_data(document, "e0tqV");
    add_data(document, "10Ikp");
    add_data(document, "c0ehx");
    add_data(document, "o0KiL");
    add_data(document, "D0diz");
    add_data(document, "H1zSK");
    add_data(document, "A1EUf");
    add_data(document, "t07CX");
    add_data(document, "h2zOp");
    add_data(document, "_0GVl");
    add_data(document, "O0CDb");

    printf("== Decrypted Document: ==\n");
    printf("cctf{%s}\n", document);
    printf("== End of Decrypted Document ==\n\n");
}

int main()
{
    diagnostics();

    printf("Attempting to decrypt the document...\n");

    decrypt_document();

    return 0;
}
