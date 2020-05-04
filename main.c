#include "fsFunctions.h"

int main(int argc, char** argv){
    int fd;     // file directory
    int ft;     // file type
    int i = 0;
    int dirInode;
    superblock_type superblock;
    inode_type inode;
    dir_type dir[DIR_NUM];
    char blockBuf[BLOCK_SIZE];
    char input[BLOCK_SIZE];
    char fs[] ="v6fs";
    char testPath[] = "/dir2/file1.txt";
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

    // look for path element in directory
    if ((dirInode = findElem(dir,pathElem))<0)
        fprintf(stderr, "Cannot open %s. Try again later.\n",testPath);

    // move to element inode
    readInInode(2*BLOCK_SIZE+(dirInode-1)*64,fd,&inode);
    printInode(inode);

    // check file type
    ft = fileType(&inode);

    if (ft == 1){
        printf("File is a directory\n");
        pathElem = strtok(NULL,"/");
        printf(pathElem);
        /*
        while (pathElem != NULL){
            // go to block
            readInDir(BLOCK_SIZE*inode.addr[0], fd,dir,inode.size/DIR_SIZE);
            printDir(dirInode, inode.size/DIR_SIZE);

            // look for directory in block
            printf(pathElem);
            if ((dirInode = findElem(dir,pathElem))<0)
                fprintf(stderr, "Cannot open %s. Try again later.\n",testPath);

            // go to file inode
            readInInode(2*BLOCK_SIZE+(dirInode-1)*64,fd,&inode);
            printInode(inode);

            // get next element
            strtok(NULL,"/");

        }*/
    }else{
        printf("File is not a directory");
    }




    close(fd);
    return 0;
}