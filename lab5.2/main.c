#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define BUFFER_SIZE 10
#define TLB_SIZE 16       
#define SIZE_TLB_ARRAY 256

typedef int32_t i32;
typedef int8_t i8;

i32 PAGE_TABLE_NUMBER[256];  
i32 PAGE_TABLE_FRAMES[256];   
i32 PAGE_NUMBER_TLB[TLB_SIZE];  
i32 FRAME_NUMBER_TLBF[TLB_SIZE]; 
i32 MEMORY_SIZE[256][256]; 
i32 Page_Counter = 0;   
i32 TLB_HITS = 0;      
i32 firstAvailableFrame = 0;  
i32 TableNumber = 0;  
i32 TLB_Entries = 0;   
i32 logicalAddress;

FILE    *addresses_file;
FILE    *backing_store;
FILE 	*fp;

i8    addresses[BUFFER_SIZE];
i8     buffer[SIZE_TLB_ARRAY];
i8     value;


void getPage(i32 addresses);
void readStore(i32 pageNumber);
void insertTLB(i32 pageNumber, i32 frameNumber);


void getPage(i32 logicalAddress){
    
    i32 pageNumber = ((logicalAddress & 0xFFFF)>>8);
    i32 offset = (logicalAddress & 0xFF);
    
    i32 frameNumber = -1; 
    
    i32 i;  
    for(i = 0; i < TLB_SIZE; i++){
        if(PAGE_NUMBER_TLB[i] == pageNumber){  
            frameNumber = FRAME_NUMBER_TLBF[i]; 
                TLB_HITS++;               
        }
    }
    

    if(frameNumber == -1){
        i32 i;  
        for(i = 0; i < TableNumber; i++){
            if(PAGE_TABLE_NUMBER[i] == pageNumber){        
                frameNumber = PAGE_TABLE_FRAMES[i];        
            }
        }
        if(frameNumber == -1){                   
            readStore(pageNumber);             
            Page_Counter++;                    
            frameNumber = firstAvailableFrame - 1;  
        }
    }
    
    insertTLB(pageNumber, frameNumber);  
    value = MEMORY_SIZE[frameNumber][offset];  
	fprintf(fp, "Virtual addresses: %d Physical addresses: %d Value: %d\n", logicalAddress, (frameNumber << 8) | offset, value);
}

void insertTLB(i32 pageNumber, i32 frameNumber){
    
    i32 i; 
    for(i = 0; i < TLB_Entries; i++){
        if(PAGE_NUMBER_TLB[i] == pageNumber){
            return;
        }
    }
    

    if(i == TLB_Entries){
        if(TLB_Entries < TLB_SIZE){  
            PAGE_NUMBER_TLB[TLB_Entries] = pageNumber;    
            FRAME_NUMBER_TLBF[TLB_Entries] = frameNumber;
        }
        else{                                            
            for(i = 0; i < TLB_SIZE - 1; i++){
                PAGE_NUMBER_TLB[i] = PAGE_NUMBER_TLB[i + 1];
                FRAME_NUMBER_TLBF[i] = FRAME_NUMBER_TLBF[i + 1];
            }
            PAGE_NUMBER_TLB[TLB_Entries-1] = pageNumber; 
            FRAME_NUMBER_TLBF[TLB_Entries-1] = frameNumber;
        }        
    }
    

    else{
        for(i = i; i < TLB_Entries - 1; i++){      
            PAGE_NUMBER_TLB[i] = PAGE_NUMBER_TLB[i + 1];    
            FRAME_NUMBER_TLBF[i] = FRAME_NUMBER_TLBF[i + 1];
        }
        if(TLB_Entries < TLB_SIZE){                
            PAGE_NUMBER_TLB[TLB_Entries] = pageNumber;
            FRAME_NUMBER_TLBF[TLB_Entries] = frameNumber;
        }
        else{                                          
            PAGE_NUMBER_TLB[TLB_Entries-1] = pageNumber;
            FRAME_NUMBER_TLBF[TLB_Entries-1] = frameNumber;
        }
    }
    if(TLB_Entries < TLB_SIZE){                    
        TLB_Entries++;
    }    
}


void readStore(i32 pageNumber){
    if (fseek(backing_store, pageNumber * SIZE_TLB_ARRAY, SEEK_SET) != 0) {
        fprintf(stderr, "Error\n");
    }
    
    if (fread(buffer, sizeof(i8), SIZE_TLB_ARRAY, backing_store) == 0) {
        fprintf(stderr, "Error\n");        
    }
    
    i32 i;
    for(i = 0; i < SIZE_TLB_ARRAY; i++){
        MEMORY_SIZE[firstAvailableFrame][i] = buffer[i];
    }
    
    PAGE_TABLE_NUMBER[TableNumber] = pageNumber;
    PAGE_TABLE_FRAMES[TableNumber] = firstAvailableFrame;
    
    firstAvailableFrame++;
    TableNumber++;
}

i32 main(i32 argc, i8 *argv[])
{
	fp = fopen("resp.txt", "w");
    
    
    backing_store = fopen("BACKING_STORE.bin", "rb");
    
    addresses_file = fopen(argv[1], "r");
    
    if (addresses_file == NULL) {
        fprintf(stderr, "Error opening addresses.txt %s\n",argv[1]);
        return -1;
    }
    
    i32 numberOfTranslatedAddresses = 0;
    while ( fgets(addresses, BUFFER_SIZE, addresses_file) != NULL) {
        logicalAddress = atoi(addresses);
        
        getPage(logicalAddress);
        numberOfTranslatedAddresses++;  
    }
    
    double pfRate = Page_Counter / (double)numberOfTranslatedAddresses;
    double TLBRate = TLB_HITS / (double)numberOfTranslatedAddresses;
    
    printf("Number of translated addresses = %d\n", numberOfTranslatedAddresses);
    printf("Page Faults = %d\n", Page_Counter);
    printf("Page Fault Rate = %.3f\n",pfRate);
    printf("TLB Hits = %d\n", TLB_HITS);
    printf("TLB Hit Rate = %.3f\n", TLBRate);
    
    
    fclose(addresses_file);
    fclose(backing_store);
    fclose(fp);
    
    return 0;
}
