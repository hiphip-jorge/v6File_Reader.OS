#include "fsFunctions.h"

int main(int argc, char** argv){
    int fd;
    superblock_type superblock;
    inode_type inode;


    // open disk
    if((fd = open("../v6fs",2)) == -1){
        fprintf(stderr, "Cannot open v6fs drive. Try again later.\n");
        exit(1);
    }


    readInSuperBlock(fd,&superblock);
    readInInode(2*BLOCK_SIZE,fd,&inode);
    lseek(fd,BLOCK_SIZE,0);

    printSuperBlock(superblock);
    printf("\n");
    printInode(inode);
    printf("\n");



    close(fd);
    return 0;
}