//////////////////////////////////////////////////
//// Name: Jorge Perez
//// Due Date: May 7, 2020
//// Course: cs4348 - Operating Systems
//// file: fsFunctions.c
////////////////////////////////////////////////////

#include "fsFunctions.h"

// Read in Functions ////////////////////////////////////////////
void readInSuperBlock(int file, superblock_type* superBlock){

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
    read(file,superBlock->time, sizeof(short));
}
void readInInode(int offset, int file, inode_type* inode){
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

    // offset to addr and read necessary blocks
    for (int i = 0; i < 11; ++i) {
        if (i < (inode->size/BLOCK_SIZE)+1) {
            lseek(file, offset + ADDR_OFFSET + i * sizeof(int), 0);
            read(file, &inode->addr[i], sizeof(int));
        }
    }
}
void readInDir(int offset, int file, dir_type* dir, int size){
    for (int i = 0; i < size;i++) {
        // read i-node
        lseek(file,offset+i*16,0);
        read(file,&dir[i].inode,sizeof(dir[i].inode));
        // read filename
        lseek(file,offset+i*16+2,0);
        read(file,&dir[i].filename,sizeof(dir[i].filename));
    }
}
////////////////////////////////////////////////////////////////

// Print Functions /////////////////////////////////////////
void printSuperBlock(const superblock_type superBlock){
    printf("Superblock:\n");
    printf("isize: %u\n", superBlock.isize);
    printf("fsize: %u\n", superBlock.fsize);
    printf("nfree: %u\n", superBlock.nfree);
    printf("ninode: %u\n", superBlock.ninode);

    printf("free[0]: %u\n",superBlock.free[0]);
    printf("inode[0]: %u\n",superBlock.inode[0]);
    printf("flock: %c\n", superBlock.flock);
    printf("ilock: %c\n", superBlock.ilock);
    printf("fmod: %u\n", superBlock.fmod);
    printf("time[0]: %u\n\n",superBlock.time[0]);
}
void printInode(const inode_type inode){
    printf("I-Node:\n");
    printf("flag: %lu\n",decimalToBinary(inode.flags));
    printf("nlinks: %u\n", inode.nlinks);
    printf("uid: %u\n", inode.uid);
    printf("gid: %u\n", inode.gid);
    printf("size: %u\n", inode.size);
    for (int i = 0; i < 11; ++i) {
        if (i < (inode.size/BLOCK_SIZE)+1)
            printf("addr[%i]: %u\n",i,inode.addr[i]);
    }
}
void printDir(const dir_type* dir, int size){
    for (int i = 0; i < 10, i < size; i++) {
        printf("inode [%i]: %u\n",i,dir[i].inode);
        printf("filename [%i]: %s\n",i,dir[i].filename);
    }
}
///////////////////////////////////////////////////////////

// misc ///////////////////////////////////////////////////
// find path element in directory
int findElem(dir_type* dir, const char* elem, const int size){
    for (int i = 0; i < size; i++) {
        if (strcmp(dir[i].filename,elem) == 0){
            return i;
        }
        memset(dir[i].filename,'\0', sizeof(dir[i].filename));
    }
    return -1;
}

// determine type of file
// 0 - plain file, 1 - directory
// 2 - char-type, 3 - block-type
int fileType(const inode_type* inode){
    unsigned long binFlag = decimalToBinary(inode->flags);
    int type = 0;
    if ((binFlag/10000000000000) % 100 == 10)
        type = 1;
    if ((binFlag/10000000000000) % 100 == 01)
        type = 2;
    if ((binFlag/10000000000000) % 100 == 11)
        type = 3;
    return type;

}

// decimal to binary for flags
long decimalToBinary(unsigned short n){
    long bin = 0, temp = 1;
    unsigned short rem = 0;

    while (n!=0){
        rem = n%2;
        n = n / 2;
        bin = bin + rem*temp;
        temp = temp * 10;
    }

    return bin;
}
///////////////////////////////////////////////////////////
