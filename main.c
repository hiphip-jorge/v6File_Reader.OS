#include "structs.h"

int main(int argc, char** argv){
    int fd;
    inode_type* inode;
    superblock_type *sBlock;

    // open disk
    if((fd = open("../v6fs",2)) == -1){
        fprintf(stderr, "Cannot open v6fs drive. Try again later.\n");
        exit(1);
    }

//    inode = readInInode(2*BLOCK_SIZE,fd);
    sBlock = readInSuperBlock(fd);

    printSuperBlock(*sBlock);
//    printInode(*inode);
    close(fd);
    return 0;
}

// flag and isize are reading the same content, size is modified as a result.
// depending which one is executed first, they alternate. See why they are linking
// to the same address