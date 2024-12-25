#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

int main(int argc, char *argv[]) {

    FILE* filePointer;
    unsigned char byteRead;

    //Check if the filename is provided as a command line argument
    if (argc != 2) {

        printf("Missing <filename> parameter\n", argv[0]);

        exit(0);
    }

    //Open the binary file 
    filePointer = fopen(argv[1], "rb");
    if (filePointer == NULL) {

        perror("Error opening file");

        exit(0);
    }


    //Begin Printing
    printf("Ethernet Header: \n----------------\n");


    //Destination MAC
    printf("Destination MAC Address:\t\t");
    fread(&byteRead, 1, 1, filePointer);
    for (int i = 0; i < 5; i++) {

        printf("%02x:", byteRead);
        fread(&byteRead, 1, 1, filePointer);

    }
    printf("%02x\n", byteRead);


    //Source MAC
    printf("Source MAC Address:\t\t\t");
    fread(&byteRead, 1, 1, filePointer);
    for (int i = 0; i < 5; i++) {

        printf("%02x:", byteRead);
        fread(&byteRead, 1, 1, filePointer);
        
    }
    printf("%02x\n", byteRead);
    
    
    //Type
    printf("Type:\t\t\t\t\t");
    fread(&byteRead, 1, 1, filePointer);
    printf("%02x", byteRead);
    fread(&byteRead, 1, 1, filePointer);
    printf("%02x", byteRead);
    printf("\n\n");


    printf("IPv4 Header: \n----------------\n");


    //Version
    fread(&byteRead, 1, 1, filePointer);
    printf("Version:\t\t\t\t");
    printf("%02x \n", byteRead >> 4);

    //Internet Header Length
    printf("IHL:\t\t\t\t\t");
    printf("%02x \n", byteRead & 0x0F);
    char ihl = byteRead & 0x0F;


    //DSCP
    fread(&byteRead, 1, 1, filePointer);
    printf("DSCP:\t\t\t\t\t");
    printf("%02x \n", byteRead >> 2);

    //ECN
    printf("ECN:\t\t\t\t\t");
    unsigned char ecnMsg = byteRead & 0x03;
    switch(ecnMsg){

        case 0b11:
            printf("%02x %s", ecnMsg, "\tCongestion experienced\n");
            break;
        case 0b10:
            printf("%02x %s", ecnMsg, "\tECT type 0 packet\n");
            break;
        case 0b01:
            printf("%02x %s", ecnMsg, "\tECT type 1 packet\n");
            break;
        default:
            //case 0b00:
            printf("%02x %s", ecnMsg, "\tNon-ECT packet\n");
    }


    //Total length
    printf("Total length:\t\t\t\t");
    //temporary variable to store first 8 bits read, shift and add second 8 bits later 
    unsigned short temp;
    fread(&byteRead, 1, 1, filePointer);
    temp = byteRead << 8;
    fread(&byteRead, 1, 1, filePointer);
    temp += byteRead;
    printf("%d \n", temp);


    //Identification
    printf("Identification:\t\t\t\t");
    temp = 0;
    fread(&byteRead, 1, 1, filePointer);
    temp = byteRead << 8;
    fread(&byteRead, 1, 1, filePointer);
    temp += byteRead;
    printf("%d \n", temp);


    //Flags
    printf("Flags:\t\t\t\t");
    fread(&byteRead, 1, 1, filePointer);
    //temporary variable to store 8 bits and operate on last 3 digits for flags field
    unsigned char flags = byteRead >> 5;
    switch(flags){

        case 0b11:
            printf("\tFragment Error\n");
            break;
        case 0b10:
            printf("\tDon't Fragment\n");
            break;
        case 0b01:
            printf("\tMore Fragments\n");
            break;
        default:
            //case 0b00:
            printf("\tLast Fragment\n");
    }
    
    //Fragment Offset
    printf("Fragment Offset:\t\t\t");
    temp = 0;
    temp = byteRead << 8;
    fread(&byteRead, 1, 1, filePointer);
    temp += byteRead;
    printf("%x \n", temp & 0x1FFF);


    //Time to live
    fread(&byteRead, 1, 1, filePointer);
    printf("Time to live:\t\t\t\t");
    printf("%03d \n", byteRead);

    //Protocol
    fread(&byteRead, 1, 1, filePointer);
    printf("Protocol:\t\t\t\t");
    printf("%d \n", byteRead);


    //Header Checksum
    printf("IP Checksum:\t\t\t\t");
    //temporary variable to store first 8 bits read, shift and add second 8 bits later 
    temp = 0;
    fread(&byteRead, 1, 1, filePointer);
    temp = byteRead << 8;
    fread(&byteRead, 1, 1, filePointer);
    temp += byteRead;
    printf("%s%4x\n", "0x", temp);


    //Source IP Address
    printf("Source Ip Address:\t\t\t");
    fread(&byteRead, 1, 1, filePointer);
    printf("%0d", byteRead);
    for (int i = 0; i < 3; i++) {

        fread(&byteRead, 1, 1, filePointer);
        printf(".%0d", byteRead);
        
    }

    //Destination IP Address
    printf("\nDestination Ip Address:\t\t\t");
    fread(&byteRead, 1, 1, filePointer);
    printf("%0d", byteRead);
    for (int i = 0; i < 3; i++) {

        fread(&byteRead, 1, 1, filePointer);
        printf(".%0d", byteRead);
        
    }
    printf("\n");

    //options array
    unsigned char options[4];

    
    //IP Options
    for(int i = 0; i < (ihl - 5); i ++){

        fread(&options, 1, 4, filePointer);
        printf("%s %d \t\t\t0x%02x%02x%02x%02x\n", "IP Option Word#", i, options[0], options[1], options[2], options[3]);
        
    }

    if(ihl == 5){
        printf("%s\t\t\t\t%s", "Options:", "No Options\n\n");
    }

    //Payload
    printf("Payload:\n");
    fread(&byteRead, 1, 1, filePointer);
    //temp variable to determine whether to print a space or new line
    temp = 0;
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