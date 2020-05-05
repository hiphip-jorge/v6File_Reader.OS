#include "fsFunctions.h"

int main(int argc, char** argv){
    int fd1,fd2,ft,leftover = 0;     // file directory, file type
    unsigned int dirInode;
    unsigned int indirBlock[BLOCK_SIZE/4];  // 256 indirect blocks in inode addr
    inode_type inode;
    dir_type dir[DIR_NUM];
    char blockBuf[BLOCK_SIZE];
    char input[BLOCK_SIZE];
    char fs[] ="v6fs";
    char testPath[] = "/dir3/subdir1/subdir2/subdir3/file4.txt";
    char *pathElem;

    // prompt for file system and open input
    printf("Enter the file system name: ");
//    gets(input);

    // open disk
    if((fd1 = open(fs,2)) == -1){
        fprintf(stderr, "Cannot open v6fs drive. Try again later.\n");
        exit(1);
    }

    // prompt for file and open input
    printf("Enter the file you would like to copy and output as "
           "myoutputfile.txt:\n");
//    gets(input);
    printf("%s\n",testPath);

    // create output file
    fd2 = creat("myoutputfile.txt",0777);

    if((open("myoutputfile.txt",2)) == -1){
        fprintf(stderr, "Cannot open output file. Try again later.\n");
        exit(1);
    }

    // collect the first path element
    pathElem = strtok(testPath, "/");
    printf("%s\n",pathElem);

    // look at root directory content in block 9 (addr[0] of inode 1)
    readInInode(2*BLOCK_SIZE,fd1,&inode);
    readInDir(BLOCK_SIZE*9,fd1,dir,inode.size/DIR_SIZE);
    printDir(dir,inode.size/DIR_SIZE);

    // look for path element in directory
    if ((dirInode = findElem(dir,pathElem,inode.size/DIR_SIZE))<0)
        fprintf(stderr, "Cannot open %s. Try again later.\n",testPath);

    puts("\n");
    // move to element inode
    readInInode(2*BLOCK_SIZE+(dir[dirInode].inode-1)*64,fd1,&inode);
    printInode(inode);

    // check file type
    ft = fileType(&inode);

    // Iterate through directories
    if (ft == 1){
        pathElem = strtok(NULL,"/");
        printf("\n%s\n",pathElem);

        while (pathElem != NULL) {
            // go to block
            readInDir(BLOCK_SIZE*inode.addr[0], fd1, dir, inode.size/DIR_SIZE);
            printDir(dir, inode.size/DIR_SIZE);
            printf("/////////////////////////////");

            // look for directory in block
            if ((dirInode = findElem(dir, pathElem, inode.size/DIR_SIZE)) < 0)
                fprintf(stderr, "Cannot open %s. Try again later.\n", testPath);

            puts("\n\n");
            // go to file inode
            readInInode(2 * BLOCK_SIZE + (dir[dirInode].inode - 1) * 64, fd1, &inode);
            printInode(inode);

            if((ft=fileType(&inode)) != 1)
                break;

            // get next element
            pathElem = strtok(NULL, "/");
            printf("pathElem: %s\n\n",pathElem);
        }
    }else{
        printf("\nFile is not a directory\n");
    }

    // check file size: small or large
    // small implementation
    if (inode.size < 4*BLOCK_SIZE){
        printf("\nSmall File!\n\n");
        leftover = inode.size;
        for (int i = 0; i < 11; i++){
            // go to blocks in inode addr
            printf("\n\nThis is a block #%u!\n\n", inode.addr[i]);
            lseek(fd1, BLOCK_SIZE * inode.addr[i], 0);
            if (leftover / BLOCK_SIZE == 0) {
                read(fd1,blockBuf,leftover);
                write(fd2,blockBuf,leftover);
            }else {
                read(fd1, blockBuf, BLOCK_SIZE);
                write(fd2,blockBuf,sizeof(blockBuf));
            }

            // reset blockBuf
            memset(blockBuf,'\0',sizeof(blockBuf));
            if(0 <= leftover - BLOCK_SIZE)
                leftover = leftover - BLOCK_SIZE;
            else
                break;
        }
    } else{ // large implementation
        leftover = inode.size / BLOCK_SIZE;
        for (int i = 0; i < ((inode.size/BLOCK_SIZE)/256)+1; i++) {
            printf("\n//////Iter I: %i\n////////addr[%i]: %u////////////////////////////////////////////\n", i,i,inode.addr[i]);
            if (i < leftover) {
                // go to blocks in inode addr
                lseek(fd1, BLOCK_SIZE*(inode.addr[i]), 0);
                read(fd1,indirBlock, sizeof(indirBlock));
                // go to each block and pull content
                for (int j = 0; j < 256; j++) {
                    printf("\n////////Iter %i:\tThis is indirect block #%u////////\n", j, indirBlock[j]);
                    lseek(fd1,BLOCK_SIZE*(indirBlock[j]-1), 0);
                    if (leftover / (BLOCK_SIZE/4) == 0) {
                        read(fd1, blockBuf, leftover);
                        leftover--;
                    }
                    else
                        read(fd1, blockBuf, BLOCK_SIZE);
                    //printf("%s", blockBuf);
                    write(fd2,blockBuf,sizeof(blockBuf));
                    if (leftover == 0)
                        break;
                }
            }
            if(0 <= leftover - 256)
                leftover = leftover - 256;
            printf("Leftover:%i\n",leftover);
        }
    }
    close(fd1);
    close(fd2);
    return 0;
}