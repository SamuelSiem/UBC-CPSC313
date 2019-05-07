#include "ext2.h"

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fsuid.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>

#define EXT2_OFFSET_SUPERBLOCK 1024
#define EXT2_INVALID_BLOCK_NUMBER ((uint32_t) -1)

/* open_volume_file: Opens the specified file and reads the initial
   EXT2 data contained in the file, including the boot sector, file
   allocation table and root directory.

   Parameters:
     filename: Name of the file containing the volume data.
   Returns:
     A pointer to a newly allocated volume_t data structure with all
     fields initialized according to the data in the volume file
     (including superblock and group descriptor table), or NULL if the
     file is invalid or data is missing.
 */
volume_t *open_volume_file(const char *filename) {

  /* TO BE COMPLETED BY THE STUDENT */
  volume_t* file = malloc(sizeof(volume_t));
	file->fd = open(filename, O_RDONLY);

	if(file->fd < 0)
    return NULL;

  //Getting the file size
  char buffer[1024];
  lseek(file->fd, EXT2_OFFSET_SUPERBLOCK, SEEK_SET);
  read(file->fd, buffer, 1024);
  superblock_t* superblock = (superblock_t*) buffer;
  file->super = *superblock;
  file->block_size = 1024 << file->super.s_log_block_size;
  file->volume_size = file->block_size * file->super.s_blocks_count;
  file->num_groups = (file->super.s_blocks_count + file->super.s_blocks_per_group - 1 )/ file->super.s_blocks_per_group;

  group_desc_t* groupbuffer = malloc(sizeof(group_desc_t)*file->num_groups);
  lseek(file->fd, (file->super.s_first_data_block+1)*file->block_size, SEEK_SET);
  read(file->fd, groupbuffer, file->num_groups*sizeof(group_desc_t));
  file->groups = groupbuffer;

  return file;
}

/* close_volume_file: Frees and closes all resources used by a EXT2 volume.

   Parameters:
     volume: pointer to volume to be freed.
 */
void close_volume_file(volume_t *volume) {

  /* TO BE COMPLETED BY THE STUDENT */
  close(volume->fd);
  free(volume->groups);
  free(volume);
}

/* read_block: Reads data from one or more blocks. Saves the resulting
   data in buffer 'buffer'. This function also supports sparse data,
   where a block number equal to 0 sets the value of the corresponding
   buffer to all zeros without reading a block from the volume.

   Parameters:
     volume: pointer to volume.
     block_no: Block number where start of data is located.
     offset: Offset from beginning of the block to start reading
             from. May be larger than a block size.
     size: Number of bytes to read. May be larger than a block size.
     buffer: Pointer to location where data is to be stored.

   Returns:
     In case of success, returns the number of bytes read from the
     disk. In case of error, returns -1.
 */
ssize_t read_block(volume_t *volume, uint32_t block_no, uint32_t offset, uint32_t size, void *buffer) {

  /* TO BE COMPLETED BY THE STUDENT */
  if(block_no < 0){ //in case of error
    return -1;
  }

  if(block_no == 0){ //sparse file
    memset(buffer, 0 , size);
    return size;
  }
  //success
  return pread(volume->fd, buffer, size, (block_no*volume->block_size)+offset);
}

/* read_inode: Fills an inode data structure with the data from one
   inode in disk. Determines the block group number and index within
   the group from the inode number, then reads the inode from the
   inode table in the corresponding group. Saves the inode data in
   buffer 'buffer'.

   Parameters:
     volume: pointer to volume.
     inode_no: Number of the inode to read from disk.
     buffer: Pointer to location where data is to be stored.

   Returns:
     In case of success, returns a positive value. In case of error,
     returns -1.
 */
  ssize_t read_inode(volume_t *volume, uint32_t inode_no, inode_t *buffer) {

  /* TO BE COMPLETED BY THE STUDENT */
  if(inode_no == 0){
    int size = sizeof(buffer);
    memset(buffer, 0 , size);
    return -1;
  }

  int block_group = (inode_no - 1) / volume->super.s_inodes_per_group;
  int index = (inode_no - 1) % volume->super.s_inodes_per_group;
  int offset = index*volume->super.s_inode_size;
  int group_table =  volume->groups[block_group].bg_inode_table + block_group*volume->super.s_blocks_per_group;

  return read_block(volume, group_table, offset , sizeof(inode_t), buffer);
}

/* read_ind_block_entry: Reads one entry from an indirect
   block. Returns the block number found in the corresponding entry.

   Parameters:
     volume: pointer to volume.
     ind_block_no: Block number for indirect block.
     index: Index of the entry to read from indirect block.

   Returns:
     In case of success, returns the block number found at the
     corresponding entry. In case of error, returns
     EXT2_INVALID_BLOCK_NUMBER.
 */
static uint32_t read_ind_block_entry(volume_t *volume, uint32_t ind_block_no,
				     uint32_t index) {

  /* TO BE COMPLETED BY THE STUDENT */
  uint32_t *number = malloc(sizeof(uint32_t));

  ssize_t ret = read_block(volume, ind_block_no, sizeof(uint32_t)*index, sizeof(uint32_t), number);
  if(ret >= 0){
    return *number;
  }else{
    return EXT2_INVALID_BLOCK_NUMBER;
  }
}

/* read_inode_block_no: Returns the block number containing the data
   associated to a particular index. For indices 0-11, returns the
   direct block number; for larger indices, returns the block number
   at the corresponding indirect block.

   Parameters:
     volume: Pointer to volume.
     inode: Pointer to inode structure where data is to be sourced.
     index: Index to the block number to be searched.

   Returns:
     In case of success, returns the block number to be used for the
     corresponding entry. This block number may be 0 (zero) in case of
     sparse files. In case of error, returns
     EXT2_INVALID_BLOCK_NUMBER.
 */
static uint32_t get_inode_block_no(volume_t *volume, inode_t *inode, uint32_t block_idx) {

  /* TO BE COMPLETED BY THE STUDENT */
  if(block_idx >= 0 && block_idx <= 11){
    return inode->i_block[block_idx];
  }

  int size =volume->block_size/4;

  if(block_idx >= 12 && block_idx <= (12+size-1)){
    return read_ind_block_entry(volume,inode->i_block_1ind, (block_idx-12));
  }

  if(block_idx >= (12+size) && block_idx <= (12+size+(size*size)-1)){
    int offset = ((block_idx-12)/size)-1 ;
    int block_no = read_ind_block_entry(volume,inode->i_block_2ind, offset);
    return read_ind_block_entry(volume, block_no, ((block_idx-12)%size));
  }

  if(block_idx >= (12+size+(size*size)) && block_idx <= (12+size+(size*size)+(size*size*size)-1)){
    int offset = ((block_idx-12/size))-1;
    int block_no = read_ind_block_entry(volume,inode->i_block_3ind, offset);
    int another_offset = offset = (offset/size)-1;
    block_no = read_ind_block_entry(volume,block_no, another_offset);
    another_offset = offset%size;
    return read_ind_block_entry(volume,block_no, another_offset);
  }

  return EXT2_INVALID_BLOCK_NUMBER;
}

/* read_file_block: Returns the content of a specific file, limited to
   a single block.

   Parameters:
     volume: Pointer to volume.
     inode: Pointer to inode structure for the file.
     offset: Offset, in bytes from the start of the file, of the data
             to be read.
     max_size: Maximum number of bytes to read from the block.
     buffer: Pointer to location where data is to be stored.

   Returns:
     In case of success, returns the number of bytes read from the
     disk. In case of error, returns -1.
 */
ssize_t read_file_block(volume_t *volume, inode_t *inode, uint32_t offset, uint32_t max_size, void *buffer) {

  /* TO BE COMPLETED BY THE STUDENT */

  int block_index = offset / volume->block_size;
  int newOffset = offset % volume->block_size;
  int block_no = get_inode_block_no(volume, inode, block_index);

  int newSize = max_size;
  int inode_size = inode_file_size(volume,inode);
  if(max_size > volume->block_size - newOffset){
    newSize = volume->block_size - newOffset;
  }

  if(max_size > inode_size - newOffset){
    newSize = inode_size - newOffset;
  }

  return read_block(volume, block_no, newOffset, newSize, buffer);
}

/* read_file_content: Returns the content of a specific file, limited
   to the size of the file only. May need to read more than one block,
   with data not necessarily stored in contiguous blocks.

   Parameters:
     volume: Pointer to volume.
     inode: Pointer to inode structure for the file.
     offset: Offset, in bytes from the start of the file, of the data
             to be read.
     max_size: Maximum number of bytes to read from the file.
     buffer: Pointer to location where data is to be stored.

   Returns:
     In case of success, returns the number of bytes read from the
     disk. In case of error, returns -1.
 */
ssize_t read_file_content(volume_t *volume, inode_t *inode, uint32_t offset, uint32_t max_size, void *buffer) {

  uint32_t read_so_far = 0;

  if (offset + max_size > inode_file_size(volume, inode))
    max_size = inode_file_size(volume, inode) - offset;

  while (read_so_far < max_size) {
    int rv = read_file_block(volume, inode, offset + read_so_far,
			     max_size - read_so_far, buffer + read_so_far);
    if (rv <= 0) return rv;
    read_so_far += rv;
  }
  return read_so_far;
}

/* follow_directory_entries: Reads all entries in a directory, calling
   function 'f' for each entry in the directory. Stops when the
   function returns a non-zero value, or when all entries have been
   traversed.

   Parameters:
     volume: Pointer to volume.
     inode: Pointer to inode structure for the directory.
     context: This pointer is passed as an argument to function 'f'
              unmodified.
     buffer: If function 'f' returns non-zero for any file, and this
             pointer is set to a non-NULL value, this buffer is set to
             the directory entry for which the function returned a
             non-zero value. If the pointer is NULL, nothing is
             saved. If none of the existing entries returns non-zero
             for 'f', the value of this buffer is unspecified.
     f: Function to be called for each directory entry. Receives three
        arguments: the file name as a NULL-terminated string, the
        inode number, and the context argument above.

   Returns:
     If the function 'f' returns non-zero for any directory entry,
     returns the inode number for the corresponding entry. If the
     function returns zero for all entries, or the inode is not a
     directory, or there is an error reading the directory data,
     returns 0 (zero).
 */
uint32_t follow_directory_entries(volume_t *volume, inode_t *inode, void *context,
				  dir_entry_t *buffer,
				  int (*f)(const char *name, uint32_t inode_no, void *context)) {

  /* TO BE COMPLETED BY THE STUDENT */
  if((inode->i_mode & S_IFMT) != S_IFDIR){ //if inode is not in directory
    return 0;
  }

  int max_size = inode_file_size(volume, inode);
  int curr_size = 0;
  int number;
  dir_entry_t* buffer_help = malloc(sizeof(dir_entry_t));

  while(curr_size < max_size){
    read_file_content(volume, inode, curr_size, sizeof(dir_entry_t), buffer_help);

    char name[sizeof(buffer_help->de_name)+1];
    strncpy(name, buffer_help->de_name, sizeof(buffer_help->de_name));
    name[buffer_help->de_name_len] = '\0';

    number = f(name, buffer_help->de_inode_no ,context);
    curr_size += buffer_help->de_rec_len;

    if(number && buffer){
      //update buffer;
      memcpy(buffer, buffer_help, sizeof(dir_entry_t));
      free(buffer_help);
      return buffer->de_inode_no;
    }
  }
  return 0;
}

/* Simple comparing function to be used as argument in find_file_in_directory function */
static int compare_file_name(const char *name, uint32_t inode_no, void *context) {
  return !strcmp(name, (char *) context);
}

/* find_file_in_directory: Searches for a file in a directory.

   Parameters:
     volume: Pointer to volume.
     inode: Pointer to inode structure for the directory.
     name: NULL-terminated string for the name of the file. The file
           name must match this name exactly, including case.
     buffer: If the file is found, and this pointer is set to a
             non-NULL value, this buffer is set to the directory entry
             of the file. If the pointer is NULL, nothing is saved. If
             the file is not found, the value of this buffer is
             unspecified.

   Returns:
     If the file exists in the directory, returns the inode number
     associated to the file. If the file does not exist, or the inode
     is not a directory, or there is an error reading the directory
     data, returns 0 (zero).
 */
uint32_t find_file_in_directory(volume_t *volume, inode_t *inode, const char *name, dir_entry_t *buffer) {
  return follow_directory_entries(volume, inode, (char *) name, buffer, compare_file_name);
}

/* find_file_from_path: Searches for a file based on its full path.

   Parameters:
     volume: Pointer to volume.
     path: NULL-terminated string for the full absolute path of the
           file. Must start with '/' character. Path components
           (subdirectories) must be delimited by '/'. The root
           directory can be obtained with the string "/".
     dest_inode: If the file is found, and this pointer is set to a
                 non-NULL value, this buffer is set to the inode of
                 the file. If the pointer is NULL, nothing is
                 saved. If the file is not found, the value of this
                 buffer is unspecified.

   Returns:
     If the file exists, returns the inode number associated to the
     file. If the file does not exist, or there is an error reading
     any directory or inode in the path, returns 0 (zero).
 */
 uint32_t find_file_from_path(volume_t *volume, const char *path, inode_t *dest_inode) {

   /* TO BE COMPLETED BY THE STUDENT */
   if(path[0] != 47){
     return 0;
   }

   int i = 0;
   char *path_copy = malloc(strlen(path));
   strcpy(path_copy, path);

   inode_t *buffer_help = malloc(sizeof(inode_t));
   read_inode(volume, EXT2_ROOT_INO, buffer_help);

   if(path[1]=='\0'){//just root
       memcpy(dest_inode, buffer_help, sizeof(inode_t));
       free(buffer_help);
       return EXT2_ROOT_INO;
   }

   //then it's not just the root
   char *ch[strlen(path)];
   char *token = strtok(path_copy, "/");
   while(token != NULL){
     ch[i++] = strdup(token);
     token = strtok(NULL, "/");
   }
   uint32_t new_file_inode_no;
   dir_entry_t *buffer = malloc(sizeof(dir_entry_t));

   for(int j = 0; j < i; j++){ //for every directory
     new_file_inode_no = find_file_in_directory(volume, buffer_help, ch[j], buffer);
     read_inode(volume, new_file_inode_no, buffer_help);
   }

   if(new_file_inode_no){
     memcpy(dest_inode, buffer_help, sizeof(inode_t));
     free(buffer);
     free(buffer_help);
     return new_file_inode_no;
   }else{
     return 0;
   }
 }
