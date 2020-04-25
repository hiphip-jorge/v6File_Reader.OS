# Project 2
Operating Systems Concepts Spring 2020 


## Description
With this modification, you have access to a file that has been formatted as the modified v6file
system (with these modifications).

As part of this programming project, you will read the name of the v6 file name form the user as
the first input data. This will be a string ending with the user typing the enter key on the keyboard.
- You will open this file (which has the modified v6file system).
- You will then read the name of v6 file name from the user on the keyboard (again this is a string
your program will read from the user and the string ends with the user typing the enter key).
- You will then fetch the contents of this v6 file from the v6disk, create a new file on your current
working directory with name myoutputfile.txt.

Your program can then terminate.

## Given

The format of Superblock (which is 1024 Bytes) is as follows:</br>
//	superblock	Structure</br>
typedef	struct	</br>
unsigned	short	isize;	//	2	bytes	- Blocks	for	i-list</br>
unsigned	short	fsize;	//	2	bytes	- Number	of	blocks</br>
unsigned	short	nfree;	//	2	bytes	- Pointer	of	free	block	array</br>
unsigned	short	ninode;	//	2	bytes	- Pointer	of	free	inodes	array</br>
unsigned	int	free[152];	//	Array	to	track	free	blocks</br>
unsigned	short	inode[200];	//	Array	to	store	free	inodes</br>
char	flock;</br>
char	ilock;</br>
unsigned	short	fmod;</br>
unsigned	short	time[2];	//	To	store	epoch</br>
}	superblock_type;</br>

The format of i-node (which is 64 Bytes) is as follows:</br>
//	i-node	Structure</br>
typedef	struct	{</br>
unsigned	short	flags;	//	Flag	of	a	file</br>
unsigned	short	nlinks;	//	Number	of	links	to	a	file</br>
unsigned	short	uid;	//	User	ID	of	owner</br>
unsigned	short	gid;	//	Group	ID	of	owner</br>
unsigned	int	size;	//	4	bytes	- Size	of	the	file</br>
unsigned	int	addr[11];	//	Block	numbers	of	the	file	location.	addr[10]	is	used	for	double	indirect
block </br>
unsigned	short	actime[2];	//	Last	Access	time </br>
unsigned	short	modtime[2];	//	Last	modified	time </br>
}	inode_type; </br>


Some useful system calls:
- open()
- lseek()
- read()
- write()
