// Thomas Neyman
// April 1st, 2021

///<summary>
/// This tool simply takes a file path from the user input and produces all
/// of the path's inode information for the user to read. This is usable only
/// for ext2 file systems using the Linux kernel.
///<summary>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>
#include <time.h>

// Block size of 1024 bytes
#define BLKSIZE 1024

typedef struct ext2_group_desc GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode INODE;
typedef struct ext2_dir_entry_2 DIR;

/// Many naming conventions and variables used from chapter 7 in the text Systems 
/// Programming in Unix/Linux by K. C. Wang
/// SUPER refers to the Superblock that contains info on the entire file system
/// GD refers to the Group Descriptor that describes the block
/// INODE is the file's unique structure of 128 bytes
/// DIR refers to the directory entry structure
/// This github page was helpful for seeing what was contained in <ext2fs/ext2_fs.h>
/// https://github.com/mmeeks/ext2tools/blob/master/ext2fs/ext2_fs.h

/// super pointer
SUPER *sp;
/// group pointer
GD *gp;
/// inode pointer
INODE *ip;
/// directory pointer
DIR *dp;

char buf[BLKSIZE];
int fd, iblock;
char *pathname;

/// <summary>
/// Gets the group Descriptor of the file
/// <summary>
int get_block(int fd, int blk, char *buf)
{
    lseek(fd, blk * BLKSIZE, SEEK_SET);
    return read(fd, buf, BLKSIZE);
}

/// <summary>
/// lists the inode information of the file specified from the path
/// Also lists the root inode information
/// <summary>
int inode(char *pathname)
{
    char *cp, temp[256];
    int blk;

    fd = open(pathname, O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening file");
    }

    get_block(fd, 2, buf);
    gp = (GD *)buf;

    printf("Inode Group Descriptors:\n");
    // print out the various inode group descriptors
    printf("Block bitmap block = %d\n", gp -> bg_block_bitmap);
    printf("Inode bitmap block = %d\n", gp -> bg_inode_bitmap);
    printf("Inodes table block = %d\n\n", gp -> bg_inode_table);

    // Begin listing root inode info
    iblock = gp -> bg_inode_table;
    get_block(fd, iblock, buf);
    ip = (INODE *)buf;
    // We want to point to the second INODE now
    ip++;

    printf("mode = %4x \n", ip -> i_mode);  // https://stackoverflow.com/questions/6701332/printf-precision/6701377
    printf("uid = %d\n", ip -> i_uid);
    printf("gid = %d\n", ip -> i_gid);
    printf("size = %d\n", ip -> i_size);
    printf("ctime = %s", ctime(&ip -> i_ctime));
    printf("links = %d\n\n", ip -> i_links_count);

    // Now print the block numbers of the disk
    printf("Block Numbers:\n");
    for (int i=0; i<15; i++)
    {
        if (ip -> i_block[i]) {
            printf("i_block[%d] = %d\n", i, ip -> i_block[i]);
        }
    }

    // Now print the Directory contents
    blk = ip -> i_block[0];
    get_block(fd, blk, buf);
    dp = (DIR *)buf;
    cp = buf;

    printf("\nDirectory Contents:\n");
    while  (cp < buf + BLKSIZE)
    {
        strncpy(temp, dp -> name, dp -> name_len);
        temp[dp -> name_len] = 0;
        printf("inode = %d | rec_length = %d | name_length = %d | %s\n", dp -> inode, dp -> rec_len, dp -> name_len, temp);
        cp += dp -> rec_len;
        dp = (DIR *)cp;
    }
}

/// <summary>
/// Takes the input from the user as an argument. The input should be
/// a file path. The file path is sent to the inode function to print
/// all of the file's information.
/// <summary>
int main(int argc, char *argv[])
{
    if (argc > 1) {
        if (argv[1] > 0) {
            pathname = argv[1];
            inode(pathname);
        }
    }
}