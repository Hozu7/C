#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS

int main(int argc, char *argv[]) {

    FILE* filePointer;
    unsigned char byteRead;

    //Check if the filename is provided as a command line argument
    if (argc != 2) {

        printf("Missing <filename> parameter\n", argv[0]);

        return 1;
    }

    //Open the binary file 
    filePointer = fopen(argv[1], "rb");
    if (filePointer == NULL) {

        perror("Error opening file");

        return 2;
    }

    //Begin Printing
    printf("Ethernet Header: \n");
    printf("---------------- \n");


    //Destination MAC
    printf("Destination MAC Address:          ");
    fread(&byteRead, 1, 1, filePointer);
    for (int i = 0; i < 5; i++) {

        printf("%02x:", byteRead);
        fread(&byteRead, 1, 1, filePointer);

    }
    printf("%02x\n", byteRead);


    //Source MAC
    printf("Source MAC Address:               ");
    fread(&byteRead, 1, 1, filePointer);
    for (int i = 0; i < 5; i++) {

        printf("%02x:", byteRead);
        fread(&byteRead, 1, 1, filePointer);
        
    }
    printf("%02x\n", byteRead);
    
    
    //Type
    printf("Type:                             ");
    fread(&byteRead, 1, 1, filePointer);
    printf("%02x", byteRead);
    fread(&byteRead, 1, 1, filePointer);
    printf("\n\n");

    //Payload
    printf("Payload:\n");
    fread(&byteRead, 1, 1, filePointer);
    //temp variable to determine whether to print a space or new line
    int temp = 0;
    while(!feof(filePointer)){
        printf("%02x ", byteRead);
        temp++;
        if((temp % 32) == 0){
            printf("\n");
        }else if((temp % 8) == 0){
            printf("  ");
        }
        fread(&byteRead, 1, 1, filePointer);
    }
    
    // Close the file
    fclose(filePointer);

    return 0;
}