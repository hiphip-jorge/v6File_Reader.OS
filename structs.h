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
#pragma pack(1)
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
#pragma pack(1)
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

// print functions
void printSuperBlock(superblock_type superBlock){
    printf("isize: %hi\n", superBlock.isize);
    printf("fsize: %i\n", superBlock.fsize);
    printf("nfree: %hi\n", superBlock.nfree);
    printf("ninode: %i\n", superBlock.ninode);

    printf("free number 1 is %i\n",superBlock.free[0]);
    printf("inode number 1 is %hi\n",superBlock.inode[0]);
    printf("flock: %c\n", superBlock.flock);
    printf("ilock: %c\n", superBlock.ilock);
    printf("fmod: %hi\n", superBlock.fmod);
    printf("time number 1 is %hi\n\n",superBlock.time[0]);
}
void printInode(inode_type inode){
    printf("flag: %hi\n", inode.flags);
    printf("size: %i\n\n", inode.size);
    /*
    printf("addr number 1 is ", inode.addr[0]);
    printf("actime number 1 is ", inode.actime[0]);
    printf("modtime number 1 is\n", inode.modtime[0]);
     */
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
    read(file,&superBlock->free, sizeof(int));

    //offset to inode
    lseek(file,BLOCK_SIZE+INODE_OFFSET,0);
    read(file,&superBlock->inode, sizeof(short));

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
    read(file,&superBlock->time, sizeof(short));

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

    /*
    // offset to nlinks
    lseek(file,offset+NLINKS_OFFSET,0);
    read(file,&inode->nlinks,sizeof(inode->nlinks));

    // offset to uid
    lseek(file,offset+UID_OFFSET,0);
    read(file,&inode->uid,sizeof(inode->uid));

    // offset to gid
    lseek(file,offset+GID_OFFSET,0);
    read(file,&inode->gid,sizeof(inode->gid));
    */
    // offset to size
    lseek(file,offset+SIZE_OFFSET,0);
    read(file,&inode->size,sizeof(inode->size));
    /*
    // offset to first addr
    lseek(file,offset+ADDR_OFFSET,0);
    read(file,&inode->addr,sizeof(int));

    // offset to first actime
    lseek(file,offset+ACTIME_OFFSET,0);
    read(file,&inode->actime,sizeof(short));

    //offset to first modtime
    lseek(file,offset+MODTIME_OFFSET,0);
    read(file,&inode->modtime,sizeof(short));
*/

    return inode;
}
#endif //PROJECT2_STRUCTS_H
