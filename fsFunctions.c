//
// Created by Jorge Perez on 4/29/20.
//

#include "fsFunctions.h"

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

// offset to first addr
lseek(file,offset+ADDR_OFFSET,0);
read(file,&inode->addr,sizeof(int));

// offset to first actime
//    lseek(file,offset+ACTIME_OFFSET,0);
//    read(file,&inode->actime,sizeof(short));

//offset to first modtime
//    lseek(file,offset+MODTIME_OFFSET,0);
//    read(file,&inode->modtime,sizeof(short));
}

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