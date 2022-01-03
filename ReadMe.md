# CONVERT FILE PATHNAME TO INODE

## DESCRIPTION
This simple code takes a filepath in an EXT2 file system as an input argument and prints its
information. It prints information such as the file type, owner id, size, date of creation,
and block numbers. The EXT2 file system is exclusively used by the Linux kernel namely for 
flash-based storage like SD cards and USB flash drives.

## SPECIFICATIONS
This tool uses standard C libraries and requires a GNU compiler in order to function. It also 
requires the use of the linuc/ext2_fs.h header files driver. These files will need to be installed 
onto your system if they are not there already. Running the command:

```bash
$ sudo apt-get install e2fslibs-dev
```

should solve this. A simple makefile is included in the folder to make compiling quicker. 

## INSTALLATION
1. After unzipping the contents of the folder, navigate to the directory in the terminal.
2. type `make` into the command line and press enter.
3. An associated binary file called `ext2_Inode_Tool` will be compiled and added to the directory,
4. type `./ext2_Inode_Tool ` and then a file path to a file or directory that you want run. For example:

```bash
$ ./ext2_Inode_Tool /home/pi/Pictures/cat.jpg
```

5. The file's information should be listed appropriately.
