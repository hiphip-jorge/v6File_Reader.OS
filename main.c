//////////////////////////////////////////////////
// Name: Jorge Perez
// Due Date: May 7, 2020
// Course: cs4348 - Operating Systems
// file: main.c
//////////////////////////////////////////////////

#include "fsFunctions.h"

int main(int argc, char** argv){
    int fd1,fd2,ft,dirInode,leftover = 0;     // file directory, file type
    unsigned int indirBlock[BLOCK_SIZE/4], indirBlock2[BLOCK_SIZE/4],
                    indirBlock3[BLOCK_SIZE/4];  // 256 indirect blocks in inode addr
    inode_type inode;
    dir_type dir[DIR_NUM];
    char blockBuf[BLOCK_SIZE];
    char input[BLOCK_SIZE];
    char *pathElem;

    // prompt for file system and open input
    printf("Enter the file system name: ");
    gets(input);

    // open disk
    if((fd1 = open(input,2)) == -1){
        fprintf(stderr, "Cannot open v6fs drive. Try again later.\n");
        exit(1);
    }

    // prep input for file path
    memset(input,'\0',sizeof(input));

    // prompt for file and open input
    printf("Enter the file you would like to copy and output to "
           "myoutputfile.txt:\n");
    gets(input);

    // collect the first path element
    pathElem = strtok(input, "/");

    // look at root directory content in block 9 (addr[0] of inode 1)
    readInInode(2*BLOCK_SIZE,fd1,&inode);
    readInDir(BLOCK_SIZE*9,fd1,dir,inode.size/DIR_SIZE);

    // look for path element in directory
    if ((dirInode = findElem(dir,pathElem,inode.size/DIR_SIZE))<0) {
        fprintf(stderr, "'%s' does not exist in this directory. Try again later.\n", pathElem);
        exit(1);
    }

    // move to element inode
    readInInode(2*BLOCK_SIZE+(dir[dirInode].inode-1)*64,fd1,&inode);

    // check file type
    ft = fileType(&inode);

    // Iterate through directories
    if (ft == 1){
        pathElem = strtok(NULL,"/");

        while (pathElem != NULL) {
            // go to block
            readInDir(BLOCK_SIZE*inode.addr[0], fd1, dir, inode.size/DIR_SIZE);

            // look for directory in block
            if ((dirInode = findElem(dir, pathElem, inode.size/DIR_SIZE)) < 0) {
                printf("Cannot open '%s'. Try again later.\n", pathElem);
                exit(0);
            }

            // go to file inode
            readInInode(2 * BLOCK_SIZE + (dir[dirInode].inode - 1) * 64, fd1, &inode);

            // break if no longer directory
            if((ft=fileType(&inode)) != 1)
                break;

            // get next element
            pathElem = strtok(NULL, "/");
        }
    }

    if (inode.size == 0){
        printf("This file is empty. Nothing is output.\n");
        exit(0);
    }

    // if a plain file
    if ((ft=fileType(&inode)) == 0) {
        // create output file
        fd2 = creat("myoutputfile.txt",0666);
        // open output file
        if((open("myoutputfile.txt",2)) == -1){
            fprintf(stderr, "Cannot open output file. Try again later.\n");
            exit(1);
        }

        // if small file, else large file
        if (inode.size < 11 * BLOCK_SIZE) {
            printf("\nFile size: %u\n"
                   "Hmm..this is a small file. Easy peasy!\n", inode.size);
            leftover = inode.size;
            for (int i = 0; i < 11; i++) {
                // go to blocks in inode addr
                lseek(fd1, BLOCK_SIZE * inode.addr[i], 0);
                if (leftover / BLOCK_SIZE == 0) {
                    read(fd1, blockBuf, leftover);
                    write(fd2, blockBuf, leftover);
                } else {
                    read(fd1, blockBuf, BLOCK_SIZE);
                    write(fd2, blockBuf, sizeof(blockBuf));
                }

                // reset blockBuf
                memset(blockBuf, '\0', sizeof(blockBuf));
                if (0 < leftover - BLOCK_SIZE)
                    leftover = leftover - BLOCK_SIZE;
                else
                    break;
            }
        } else { // large file implementation up to 10th addr. Last one is separate
            printf("\nFile size: %u\n"
                   "Uh-Oh..we got a large file!\n", inode.size);
            // go through adder[0-9]
            for (int i = 0; i < inode.size / (256 * BLOCK_SIZE) + 1; i++) {
                // do while singly direct
                if (i < 10) {
                    // move to addr[i]
                    lseek(fd1, BLOCK_SIZE * inode.addr[i], 0);
                    // if reading full addr blocks, else read leftover contents of addr block
                    if (i < inode.size / (256 * BLOCK_SIZE)) {
                        // read indirect block
                        read(fd1, indirBlock, sizeof(indirBlock));
                        // copy content of each block in indirect block
                        for (int j = 0; j < 256; j++) {
                            // move to block indirBlock[j]
                            lseek(fd1, BLOCK_SIZE * indirBlock[j], 0);
                            // read block indirBlock[j]
                            read(fd1, blockBuf, sizeof(blockBuf));
                            // write to output file
                            write(fd2, blockBuf, sizeof(blockBuf));
                            // reset buffer
                            memset(blockBuf, '\0', sizeof(blockBuf));
                        }
                    } else {
                        // read leftover number of blocks
                        read(fd1, indirBlock, (inode.size % leftover) / BLOCK_SIZE);
                        // copy context of each block into indirect block
                        for (int j = 0; j < (inode.size % leftover) / BLOCK_SIZE; j++) {
                            // if reading full block sizes, else read left over bytes
                            if (j < ((inode.size % leftover) / BLOCK_SIZE) - 1) {
                                // move to block indirBlock[j]
                                lseek(fd1, BLOCK_SIZE * indirBlock[j], 0);
                                // read block indirect
                                read(fd1, blockBuf, sizeof(blockBuf));
                                // write to output file
                                write(fd2, blockBuf, sizeof(blockBuf));
                                // reset buffer
                                memset(blockBuf, '\0', sizeof(blockBuf));
                            } else {
                                // move to last indirBlock
                                lseek(fd1, BLOCK_SIZE * indirBlock[j], 0);
                                // read remaining bytes of block
                                read(fd1, blockBuf, (inode.size % leftover) -
                                                    ((((inode.size % leftover) / BLOCK_SIZE) - 1) * BLOCK_SIZE));
                                // write to output file
                                write(fd2, blockBuf, (inode.size % leftover) -
                                                     ((((inode.size % leftover) / BLOCK_SIZE) - 1) * BLOCK_SIZE));
                                // reset buffer
                                memset(blockBuf, '\0', sizeof(blockBuf));
                            }
                        }
                    }
                } else {
                    break;
                }
                leftover = leftover + (256 * BLOCK_SIZE);
            }
        }

        // read from triple indirect address block
        if (10 * 256 * BLOCK_SIZE < inode.size) {
            printf("Call in back-up. We're going triple indirect!\n");
            // determine how many bytes we have left to read
            //leftover = inode.size - BLOCK_SIZE * 256 * 10;
            for (int i = 0; i < 256; i++) {
                // go through levels of indirection
                lseek(fd1, BLOCK_SIZE * (inode.addr[10]), 0);
                // read in the first indirect blocks
                read(fd1, indirBlock, sizeof(indirBlock));
                for (int j = 0; j < 256; j++) {
                    // go to the ith inderBlock
                    lseek(fd1, BLOCK_SIZE * indirBlock[i], 0);
                    // read in indirect blocks
                    read(fd1, indirBlock2, sizeof(indirBlock2));
                    // go to indirBlock[j]
                    for (int k = 0; k < 256; k++) {
                        lseek(fd1, BLOCK_SIZE * indirBlock2[j], 0);
                        // read in indirect blocks
                        read(fd1, indirBlock3, sizeof(indirBlock3));
                        if (BLOCK_SIZE - 1 < (inode.size - leftover)) {
                            // go to block indirBlock[k] for reading
                            lseek(fd1, BLOCK_SIZE * indirBlock3[k], 0);
                            // read into buffer
                            read(fd1, blockBuf, sizeof(blockBuf));
                            // write to output file
                            write(fd2, blockBuf, sizeof(blockBuf));
                            // reset buffer
                            memset(blockBuf, '\0', sizeof(blockBuf));
                            // add another block of bytes read to leftover
                            leftover = leftover + BLOCK_SIZE;
                        } else {
                            // go to block indirBlock[k] for reading
                            lseek(fd1, BLOCK_SIZE * indirBlock3[k], 0);
                            // read remaining bytes into buffer
                            read(fd1, blockBuf, (inode.size - leftover) % BLOCK_SIZE);
                            // write to output file
                            write(fd2, blockBuf, (inode.size - leftover) % BLOCK_SIZE);
                            // reset buffer
                            memset(blockBuf, '\0', sizeof(blockBuf));
                            // add # of bytes read to leftover
                            leftover = leftover + (inode.size - leftover) % BLOCK_SIZE;
                        }
                        if (leftover >= inode.size)
                            break;
                    }
                    if (leftover >= inode.size)
                        break;
                }
                if (leftover >= inode.size)
                    break;
            }
        }
        printf("\nThe contents of %s have successfully "
               "been output to 'myoutputfile.txt'.\n"
               "Enjoy!",pathElem);
    }

    close(fd1);
    close(fd2);
    return 0;
}
