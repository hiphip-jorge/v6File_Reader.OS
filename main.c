#include "fsFunctions.h"

int main(int argc, char** argv){
    int fd;
    superblock_type superblock;
    inode_type inode;
    dir_type dir;

    // open disk
    if((fd = open("../v6fs",2)) == -1){
        fprintf(stderr, "Cannot open v6fs drive. Try again later.\n");
        exit(1);
    }
/*
    readInSuperBlock(fd,&superblock);
    readInInode(2*BLOCK_SIZE,fd,&inode);
    lseek(fd,BLOCK_SIZE,0);
    printSuperBlock(superblock);
    printf("\n");
    printInode(inode);
    printf("\n");
*/

    readInDir(BLOCK_SIZE*(9-1),fd,&dir);
    printDir(dir);


    close(fd);
    return 0;
}