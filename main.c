#include "fsFunctions.h"

int main(int argc, char** argv){
    int fd,ft;     // file directory, file type
    unsigned int dirInode, leftover = 0;
    superblock_type superblock;
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
    if((fd = open(fs,2)) == -1){
        fprintf(stderr, "Cannot open v6fs drive. Try again later.\n");
        exit(1);
    }

    // clear input
//    memset(input,'\0',sizeof(input));

    // prompt for file and open input
    printf("Enter the file you would like to copy and output as "
           "myoutputfile.txt:\n");
//    gets(input);
    printf("%s\n",testPath);

    // collect the first path element
    pathElem = strtok(testPath, "/");
    printf("%s\n",pathElem);

    // look at root directory content in block 9 (addr[0] of inode 1)
    readInInode(2*BLOCK_SIZE,fd,&inode);
    readInDir(BLOCK_SIZE*9,fd,dir,inode.size/DIR_SIZE);
    printDir(dir,inode.size/DIR_SIZE);

    // look for path element in directory
    if ((dirInode = findElem(dir,pathElem,inode.size/DIR_SIZE))<0)
        fprintf(stderr, "Cannot open %s. Try again later.\n",testPath);

    puts("\n");
    // move to element inode
    readInInode(2*BLOCK_SIZE+(dir[dirInode].inode-1)*64,fd,&inode);
    printInode(inode);

    // check file type
    ft = fileType(&inode);

    if (ft == 1){
        pathElem = strtok(NULL,"/");
        printf("\n%s\n",pathElem);

        while (pathElem != NULL) {
            // go to block
            readInDir(BLOCK_SIZE*inode.addr[0], fd, dir, inode.size/DIR_SIZE);
            printDir(dir, inode.size/DIR_SIZE);
            printf("/////////////////////////////");

            // look for directory in block
            if ((dirInode = findElem(dir, pathElem, inode.size/DIR_SIZE)) < 0)
                fprintf(stderr, "Cannot open %s. Try again later.\n", testPath);

            puts("\n\n");
            // go to file inode
            readInInode(2 * BLOCK_SIZE + (dir[dirInode].inode - 1) * 64, fd, &inode);
            printInode(inode);

            if((ft=fileType(&inode)) != 1)
                break;

            // get next element
            pathElem = strtok(NULL, "/");
            printf("pathElem: %s\n\n",pathElem);
        }
    }else{
        printf("File is not a directory");
    }
    if (ft == 2){
        leftover = inode.size;
        for (int i = 0; i < 10, i < (inode.size/BLOCK_SIZE)+1; i++){
            // go to block of first inode addr
            printf("\n\nThis is a block #%u!\n\n",inode.addr[i]);
            lseek(fd,BLOCK_SIZE*(inode.addr[i]-1), 0);
            if (leftover/BLOCK_SIZE == 0)
                read(fd,blockBuf,leftover);
            else
                read(fd,blockBuf, BLOCK_SIZE);
            printf("%s",blockBuf);

            leftover = leftover - BLOCK_SIZE;
        }
    }
    close(fd);
    return 0;
}