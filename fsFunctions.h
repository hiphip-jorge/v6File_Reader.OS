
#ifndef OS4348_P2_FSFUNCTIONS_H
#define OS4348_P2_FSFUNCTIONS_H

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Constants ////////////////////////////
#define BLOCK_SIZE 1024     // Block size
#define DIR_NUM 64
#define DIR_SIZE 16

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
/////////////////////////////////////////

// I-Node Offsets //////////
// flags start at 0 offset
#define NLINKS_OFFSET 2
#define UID_OFFSET 4
#define GID_OFFSET 6
#define SIZE_OFFSET 8
#define ADDR_OFFSET 12
#define ACTIME_OFFSET 56
#define MODTIME_OFFSET 60
///////////////////////////

// Structs ///////////////
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

// Directory Type
typedef struct {
    unsigned short  inode;
    char filename[14];
} dir_type;
////////////////////////////////////

// Prototypes /////////////////////
long decimalToBinary(unsigned short n);
// find path element in directory
int findElem(dir_type* dir, const char* elem, const int size);
// determine type of file
int fileType(const inode_type* inode);
// print functions
void printSuperBlock(const superblock_type superBlock);
void printInode(const inode_type inode);
void printDir(const dir_type* dir, int size);
// read in functions
void readInSuperBlock(int file, superblock_type* superBlock);
void readInInode(int offset, int file, inode_type* inode);
void readInDir(int offset, int file, dir_type* dir, int size);
///////////////////////////////////


#endif //OS4348_P2_FSFUNCTIONS_H
