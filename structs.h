#ifndef PROJECT2_STRUCTS_H
#define PROJECT2_STRUCTS_H

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


// Useful Numbers
#define NUM_OF_BLOCKS 256   // Number of blocks in block
#define BLOCK_SIZE 1024     // Block size
#define NUM_OF_INODES 16    // Number of i-nodes in block
#define INODE_SIZE 64       // Size of i-nodes
#define ADDR 11             // +11: block numbers or device numbers

// Superblock offsets
// isize starts at 0 offset
#define FSIZE_OFFSET 2
#define NFREE_OFFSET 6
#define NINODE_OFFSET 8
#define FREE_OFFSET 10
#define INODE_OFFSET 614
#define FLOCK_OFFSET 1016
#define ILOCK_OFFSET 1017
#define FMOD_OFFSET 1018
#define TIME_OFFSET 1020

// I-Node Offsets
// flags start at 0 offset
#define NLINKS_OFFSET 2
#define UID_OFFSET 4
#define GID_OFFSET 6
#define SIZE_OFFSET 8
#define ADDR_OFFSET 12
#define ACTIME_OFFSET 56
#define MODTIME_OFFSET 60

// Superblock Structure
typedef struct {
    unsigned short isize; // +0: 2 bytes - Blocks for i-list
    unsigned int fsize; // +2: 4 bytes - Number of blocks
    unsigned short nfree; // +6: 2 bytes - Pointer of free block array
    unsigned short ninode; // +8: 2 bytes - Pointer of free inodes array
    unsigned int free[151]; // +10: Array to track free blocks
    unsigned short inode[201]; // +614: Array to store free inodes
    char flock; // +1016
    char ilock; // +1017
    unsigned short fmod; // +1018
    unsigned short time[2]; // +1020: To store epoch
} superblock_type;

// I-Node Structure
typedef struct {
    unsigned short flags; // +0: Flag of a file - 2 bytes
    unsigned short nlinks; // +2: Number of links to a file - 2 bytes
    unsigned short uid; // +4: User ID of owner - 2 bytes
    unsigned short gid; // +6: Group ID of owner - 2 bytes
    unsigned int size; // +8: 4 bytes - Size of the file
    unsigned int addr[11]; // +12: Block numbers of the file location. addr[10] is used for double indirect block
    unsigned short actime[2]; // +56: Last Access time
    unsigned short modtime[2]; // +60: Last modified time
} inode_type;

// print decimal to binary
void printBits(size_t const size, void const * const ptr){
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

// print functions
void printSuperBlock(superblock_type superBlock){
    printf("Superblock:\n");
    printf("isize: %u\n", superBlock.isize);
    printf("fsize: %u\n", superBlock.fsize);
    printf("nfree: %u\n", superBlock.nfree);
    printf("ninode: %u\n", superBlock.ninode);

    printf("free number 1 is %u\n",superBlock.free[0]);
    printf("inode number 1 is %u\n",superBlock.inode[0]);
    printf("flock: %c\n", superBlock.flock);
    printf("ilock: %c\n", superBlock.ilock);
    printf("fmod: %u\n", superBlock.fmod);
    printf("time number 1 is %hi\n\n",superBlock.time[0]);
}
void printInode(inode_type inode){
    printf("I-Node:\n");
    printf("flag: ");
    printBits(sizeof(inode.flags),&inode.flags);
    printf("nlinks: %u\n", inode.nlinks);
    printf("uid: %u\n", inode.uid);
    printf("gid: %u\n", inode.gid);
    printf("size: %u\n", inode.size);
    printf("addr[0]: %u\n", inode.addr[0]);
//    printf("actime[0]: %u\n", inode.actime[0]);
//    printf("modtime[0]: %u\n", inode.modtime[0]);
}

// read in functions
superblock_type *readInSuperBlock(int file){
    superblock_type *superBlock;

    // offset to the beginning of Super-Block
    lseek(file,BLOCK_SIZE,0);
    read(file,&superBlock->isize,sizeof(superBlock->isize));

    // offset to fsize
    lseek(file,BLOCK_SIZE+FSIZE_OFFSET,0);
    read(file,&superBlock->fsize,sizeof(superBlock->fsize));

    //offset to nfree
    lseek(file,BLOCK_SIZE+NFREE_OFFSET,0);
    read(file,&superBlock->nfree,sizeof(superBlock->nfree));

    //offset to ninode
    lseek(file,BLOCK_SIZE+NINODE_OFFSET,0);
    read(file,&superBlock->ninode,sizeof(superBlock->ninode));


    // offset to free
    lseek(file,BLOCK_SIZE+FREE_OFFSET,0);
    read(file,&superBlock->free[0], sizeof(int));

    //offset to inode
    lseek(file,BLOCK_SIZE+INODE_OFFSET,0);
    read(file,&superBlock->inode[0], sizeof(short));

    // offset to flock
    lseek(file,BLOCK_SIZE+FLOCK_OFFSET,0);
    read(file,&superBlock->flock,sizeof(superBlock->flock));

    //offset to ilock
    lseek(file,BLOCK_SIZE+ILOCK_OFFSET,0);
    read(file,&superBlock->ilock,sizeof(superBlock->ilock));

    // offset to fmod
    lseek(file,BLOCK_SIZE+FMOD_OFFSET,0);
    read(file,&superBlock->fmod, sizeof(superBlock->fmod));

    // offset to time
    lseek(file,BLOCK_SIZE+TIME_OFFSET,0);
    read(file,&superBlock->time[0], sizeof(short));

    /*
    // free offset already to inode
    for (int j = 0; j < 201; i++) { // inode array size is 201
        read(file,&superBlock->inode[j], sizeof(short));
        lseek(file,offset+INODE_OFFSET+sizeof(short)*j+1,0);
    }

    // offset to time
    for (int k = 0; k < 2; k++) { // time array size is 2
        lseek(file, offset + TIME_OFFSET + sizeof(short) * k, 0);
        read(file, superBlock->time[k], sizeof(short));
    }
    */
    return superBlock;
}
inode_type *readInInode(int offset, int file){
    inode_type *inode;

    // offset to desired i-node
    lseek(file,offset,0);
    read(file,&inode->flags,sizeof(inode->flags));

    // offset to nlinks
    lseek(file,offset+NLINKS_OFFSET,0);
    read(file,&inode->nlinks,sizeof(inode->nlinks));

    // offset to uid
    lseek(file,offset+UID_OFFSET,0);
    read(file,&inode->uid,sizeof(inode->uid));

    // offset to gid
    lseek(file,offset+GID_OFFSET,0);
    read(file,&inode->gid,sizeof(inode->gid));

    // offset to size
    lseek(file,offset+SIZE_OFFSET,0);
    read(file,&inode->size,sizeof(inode->size));

    // offset to first addr
    lseek(file,offset+ADDR_OFFSET,0);
    read(file,&inode->addr,sizeof(int));

    // offset to first actime
//    lseek(file,offset+ACTIME_OFFSET,0);
//    read(file,&inode->actime,sizeof(short));

    //offset to first modtime
//    lseek(file,offset+MODTIME_OFFSET,0);
//    read(file,&inode->modtime,sizeof(short));

    return inode;
}
#endif //PROJECT2_STRUCTS_H
