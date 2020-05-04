#include "fsFunctions.h"

int main(int argc, char** argv){
    int fd;
    superblock_type superblock;
    inode_type inode;
    dir_type dir[DIR_NUM];
    char blockBuf[BLOCK_SIZE];

    // open disk
    if((fd = open("../v6fs",2)) == -1){
        fprintf(stderr, "Cannot open v6fs drive. Try again later.\n");
        exit(1);
    }

    printf("Read I-node 1\n");
    readInInode(2*BLOCK_SIZE,fd,&inode);
    printInode(inode);
    printf("\nGo to block #9\n");
    readInDir(BLOCK_SIZE*9,fd,dir,inode.size/DIR_SIZE);
    printDir(dir,inode.size/DIR_SIZE);
    printf("Read I-node 4\n");
    readInInode(2*BLOCK_SIZE+(4-1)*64,fd,&inode);
    printInode(inode);
    printf("\nGo to block #61693\n");
    readInDir(BLOCK_SIZE*61693,fd,dir,inode.size/DIR_SIZE);
    printDir(dir,inode.size/DIR_SIZE);
    printf("Read I-node 10\n");
    readInInode(2*BLOCK_SIZE+(10-1)*64,fd,&inode);
    printInode(inode);
    lseek(fd,BLOCK_SIZE*61747,0);
    read(fd,blockBuf,sizeof(blockBuf)-1);
    printf(blockBuf);
    lseek(fd,BLOCK_SIZE*61748,0);
    memset(blockBuf, '\0', sizeof(blockBuf));
    read(fd,blockBuf,502);
    printf(blockBuf);



    close(fd);
    return 0;
}