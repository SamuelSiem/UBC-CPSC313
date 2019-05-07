#include <stdio.h>
#include "mask.h"
#include <stdlib.h>

// When completed this file will contain several versions of the
// mask() function and conditional compilation will be used to
// determine which version runs.  Each version will be named mask1,
// mask2, and so on. To select the version of the mask function to
// use, change the following declaration to the version of the mask()
// to be used.  All versions of mask() must produce a correct result.

// The provided Makefile also includes the possibility of creating an
// executable called "timemask-maskN" (replacing maskN with any
// version you'd like to test). You may compile it with "make
// timemask-maskN" then run "./timemask-maskN".

#ifndef MASK_VERSION
#define MASK_VERSION mask9
#endif

// You are only allowed to change the contents of this file with
// respect to improving the performance of this program. You may not
// change the data structure for each image or the parameters for the
// function mask. You may, however, change the structure of the code
// and the types/implementation of local variables (col, row, weight,
// etc.).


// You are required to document the changes you make in the README.txt
// file. For each entry in the README.txt file there is to be a
// version of the matching mask() function here such that the markers
// can see, and run if needed, the actual code that you used to
// produce your results for the logged change to the mask function.

static inline long mask0(long oldImage[N][N], long newImage[N][N], int rows, int cols) {
  return baseMask(oldImage, newImage, rows, cols);
}

// For each version of mask you are testing duplicate the function
// below, rename it to a different name, and make your changes. To use
// the new version, change the #define above to use the current version.

static inline long mask1(long oldImage[N][N], long newImage[N][N], int rows, int cols) {

  // TODO This function should contain code that produces the same
  // output as baseMask, but is expected to run faster than mask0 by
  // making better use of caching.
  int i, j;
  int col, row;
  long check = 0;

  long (*weight)[N] = calloc(N * N, sizeof(long));

  //initialize the new image
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++) {
      newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j];
      weight[i][j] = WEIGHT_CENTRE;
    }

  // Count the cells to the top left
  for (i = 1; i < cols; i++) {
    col = i - 1;
    for (j = 1; j < rows; j++) {
      row = j - 1;
      newImage[j][i] += WEIGHT_CORNER * oldImage[row][col];
      weight[j][i] += WEIGHT_CORNER;
    }
  }

  // Count the cells immediately above
  for (i = 0; i < cols; i++) {
    for (j = 1; j < rows; j++) {
      row = j - 1;
      newImage[j][i] += WEIGHT_SIDE * oldImage[row][i];
      weight[j][i] += WEIGHT_SIDE;
    }
  }

  // Count the cells to the top right
  for (i = 0; i < cols - 1; i++) {
    col = i + 1;
    for (j = 1; j < rows; j++) {
      row = j - 1;
      newImage[j][i] += WEIGHT_CORNER * oldImage[row][col];
      weight[j][i] += WEIGHT_CORNER;
    }
  }

  // Count the cells to the immediate left
  for (i = 1; i < cols; i++) {
    col = i - 1;
    for (j = 0; j < rows; j++) {
      newImage[j][i] += WEIGHT_SIDE * oldImage[j][col];
      weight[j][i] += WEIGHT_SIDE;
    }
  }

  // Count the cells to the immediate right
  for (i = 0; i < cols - 1; i++) {
    col = i + 1;
    for (j = 0; j < rows; j++) {
      newImage[j][i] += WEIGHT_SIDE * oldImage[j][col];
      weight[j][i] += WEIGHT_SIDE;
    }
  }

  // Count the cells to the bottom left
  for (i = 1; i < cols; i++) {
    col = i - 1;
    for (j = 0; j < rows - 1; j++) {
      row = j + 1;
      newImage[j][i] += WEIGHT_CORNER * oldImage[row][col];
      weight[j][i] += WEIGHT_CORNER;
    }
  }

  // Count the cells immediately below
  for (i = 0; i < cols; i++) {
    for (j = 0; j < rows - 1; j++) {
      row = j + 1;
      newImage[j][i] += WEIGHT_SIDE * oldImage[row][i];
      weight[j][i] += WEIGHT_SIDE;
    }
  }

  // Count the cells to the bottom right
  for (i = 0; i < cols - 1; i++) {
    col = i + 1;
    for (j = 0; j < rows - 1; j++) {
      row = j + 1;
      newImage[j][i] += WEIGHT_CORNER * oldImage[row][col];
      weight[j][i] += WEIGHT_CORNER;
    }
  }

  // Produce the final result
  for (i = 0; i < cols; i++)
    for (j = 0; j < rows; j++) {
      newImage[j][i] = newImage[j][i] / weight[j][i];
      check += newImage[j][i];
    }

  return check;
}

static inline long mask2(long oldImage[N][N], long newImage[N][N], int rows, int cols) {

  // TODO This function should contain code that produces the same
  // output as baseMask, but is expected to run faster than mask1 (or
  // mask0) by making better use of caching.

  int i, j;
  //int col, row;
  long check = 0;

  long (*weight)[N] = calloc(N * N, sizeof(long));

  //initialize the new image
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++) {
      newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j];
      weight[i][j] = WEIGHT_CENTRE;
    }

  // Count the cells to the top left
  for (i = 1; i < rows; i++) {
    for (j = 1; j < cols; j++) {
      newImage[i][j] += WEIGHT_CORNER * oldImage[i-1][j-1];
      weight[i][j] += WEIGHT_CORNER;
    }
  }

  // Count the cells immediately above
  for (i = 1; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      newImage[i][j] += WEIGHT_SIDE * oldImage[i-1][j];
      weight[i][j] += WEIGHT_SIDE;
    }
  }

  // Count the cells to the top right
  for (i = 1; i < rows; i++) {
    for (j = 0; j < cols - 1; j++) {
      newImage[i][j] += WEIGHT_CORNER * oldImage[i-1][j+1];
      weight[i][j] += WEIGHT_CORNER;
    }
  }

  // Count the cells to the immediate left
  for (i = 0; i < rows; i++) {
    for (j = 1; j < cols; j++) {
      newImage[i][j] += WEIGHT_SIDE * oldImage[i][j-1];
      weight[i][j] += WEIGHT_SIDE;
    }
  }

  // Count the cells to the immediate right
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols-1; j++) {
      newImage[i][j] += WEIGHT_SIDE * oldImage[i][j+1];
      weight[i][j] += WEIGHT_SIDE;
    }
  }

  // Count the cells to the bottom left
  for (i = 0; i < rows-1; i++) {
    for (j = 1; j < cols; j++) {
      newImage[i][j] += WEIGHT_CORNER * oldImage[i+1][j-1];
      weight[i][j] += WEIGHT_CORNER;
    }
  }

  // Count the cells immediately below
  for (i = 0; i < rows-1; i++) {
    for (j = 0; j < cols; j++) {
      newImage[i][j] += WEIGHT_SIDE * oldImage[i+1][j];
      weight[i][j] += WEIGHT_SIDE;
    }
  }

  // Count the cells to the bottom right
  for (i = 0; i < rows - 1; i++) {
    for (j = 0; j < cols - 1; j++) {
      newImage[i][j] += WEIGHT_CORNER * oldImage[i+1][j+1];
      weight[i][j] += WEIGHT_CORNER;
    }
  }

  // Produce the final result
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++) {
      newImage[i][j] = newImage[i][j] / weight[i][j];
      check += newImage[i][j];
    }

  return check;
}

static inline long mask3(long oldImage[N][N], long newImage[N][N], int rows, int cols) {

  // TODO This function should contain code that produces the same
  // output as baseMask, but is expected to run faster than mask1 (or
  // mask0) by making better use of caching.

//reason: there is 4 loops instead of many loops, so tht we only need to access cache fewer times.

  int i, j;
  long check = 0;

  long (*weight)[N] = calloc(N * N, sizeof(long));

  //Initialize the new image and
  //Count the cells to the top left and
  //Count the cells immediately above and
  //Count the cells to the immediate left
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++) {
      if(i==0 && j==0){
        newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] ;
        weight[i][j] = WEIGHT_CENTRE;
      }else if(i>=1 && j==0){
        newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i-1][j];
        weight[i][j] = WEIGHT_CENTRE + WEIGHT_SIDE;
      }else if(i==0 && j>=1){
        newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i][j-1];
        weight[i][j] = WEIGHT_CENTRE + WEIGHT_SIDE;
      }
      else{//j>1 and i>1
        newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1];
        weight[i][j] = WEIGHT_CENTRE + WEIGHT_CORNER + 2*WEIGHT_SIDE;
      }
    }
  }

  //Count the cells to the immediate right and
  //Count the cells to the top right
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols-1; j++) {
      //if(j!=cols-1)
      if(i==0){
        newImage[i][j] += WEIGHT_SIDE * oldImage[i][j+1];
        weight[i][j] += WEIGHT_SIDE;
      }else{
        newImage[i][j] += WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1];
        weight[i][j] += WEIGHT_SIDE + WEIGHT_CORNER;
      }
    }
  }

  // Count the cells immediately below and
  // Count the cells to the bottom left
  for (i = 0; i < rows-1; i++) {
    for (j = 0; j < cols; j++) {
      if(j==0){
        newImage[i][j] += WEIGHT_SIDE * oldImage[i+1][j];
        weight[i][j] += WEIGHT_SIDE;
      }else{
        newImage[i][j] += WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1];
        weight[i][j] += WEIGHT_SIDE + WEIGHT_CORNER;
      }
    }
  }

  // Count the cells to the bottom right
  for (i = 0; i < rows - 1; i++) {
    for (j = 0; j < cols - 1; j++) {
      newImage[i][j] += WEIGHT_CORNER * oldImage[i+1][j+1];
      weight[i][j] += WEIGHT_CORNER;
    }
  }

  // Produce the final result
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++) {
      newImage[i][j] = newImage[i][j] / weight[i][j];
      check += newImage[i][j];
    }
  }

  return check;
}

static inline long mask4(long oldImage[N][N], long newImage[N][N], int rows, int cols) {

  // TODO This function should contain code that produces the same
  // output as baseMask, but is expected to run faster than mask1 (or
  // mask0) by making better use of caching.

//reason: there is 4 loops instead of many loops, so tht we only need to access cache fewer times.

  int i, j;
  long check = 0;

  long (*weight)[N] = calloc(N * N, sizeof(long));

  for (i = 0; i < rows; i++){
      for (j = 0; j < cols; j++) {
        if(i==0 && j==0){
          newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] +  WEIGHT_SIDE * oldImage[i][j+1] ;
          weight[i][j] = WEIGHT_CENTRE + WEIGHT_SIDE;
        }else if(i>=1 && j==0){
          newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1];
          weight[i][j] = WEIGHT_CENTRE + 2*WEIGHT_SIDE + WEIGHT_CORNER;
        }else if(i==0 && j>=1){
          if(j < cols-1){
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i][j-1] +  WEIGHT_SIDE * oldImage[i][j+1];
            weight[i][j] = WEIGHT_CENTRE + WEIGHT_SIDE + WEIGHT_SIDE;
          }else{
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i][j-1];
      			weight[i][j] = WEIGHT_CENTRE + WEIGHT_SIDE;
          }
        }
        else{//j>1 and i>1
          if(j < cols-1){
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1];
            weight[i][j] = WEIGHT_CENTRE + 2*WEIGHT_CORNER + 3*WEIGHT_SIDE;
          }else{
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1];
            weight[i][j] = WEIGHT_CENTRE + WEIGHT_CORNER + 2*WEIGHT_SIDE;
            }
          }
        }
      }

  for (i = 0; i < rows-1; i++) {
    for (j = 0; j < cols; j++) {
      if(j==0){
        newImage[i][j] += WEIGHT_SIDE * oldImage[i+1][j];
        weight[i][j] += WEIGHT_SIDE;
      }else{
        newImage[i][j] += WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1];
        weight[i][j] += WEIGHT_SIDE + WEIGHT_CORNER;
      }
    }
  }

  // Count the cells to the bottom right
  for (i = 0; i < rows - 1; i++) {
    for (j = 0; j < cols - 1; j++) {
      newImage[i][j] += WEIGHT_CORNER * oldImage[i+1][j+1];
      weight[i][j] += WEIGHT_CORNER;
    }
  }

  // Produce the final result
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++) {
      newImage[i][j] = newImage[i][j] / weight[i][j];
      check += newImage[i][j];
    }
  }

  return check;
}

static inline long mask5(long oldImage[N][N], long newImage[N][N], int rows, int cols) {

  // TODO This function should contain code that produces the same
  // output as baseMask, but is expected to run faster than mask1 (or
  // mask0) by making better use of caching.

//reason: there is 4 loops instead of many loops, so tht we only need to access cache fewer times.

  int i, j;
  long check = 0;

  long (*weight)[N] = calloc(N * N, sizeof(long));

  for (i = 0; i < rows; i++){
      for (j = 0; j < cols; j++) {
        if(i==0 && j==0){
          newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] +  WEIGHT_SIDE * oldImage[i][j+1] +  WEIGHT_SIDE * oldImage[i+1][j];
          weight[i][j] = WEIGHT_CENTRE + WEIGHT_SIDE + WEIGHT_SIDE;
        }else if(i>=1 && j==0){
          if(i < rows-1){
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1] + WEIGHT_SIDE * oldImage[i+1][j];
            weight[i][j] = WEIGHT_CENTRE + 2*WEIGHT_SIDE + WEIGHT_CORNER + WEIGHT_SIDE;
          }else{
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1];
    				weight[i][j] = WEIGHT_CENTRE + 2*WEIGHT_SIDE + WEIGHT_CORNER;
          }
        }else if(i==0 && j>=1){
          if(j < cols-1){
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i][j-1] +  WEIGHT_SIDE * oldImage[i][j+1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1];
            weight[i][j] = WEIGHT_CENTRE + WEIGHT_SIDE + WEIGHT_SIDE + WEIGHT_SIDE + WEIGHT_CORNER;
          }else{
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i][j-1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1];
      			weight[i][j] = WEIGHT_CENTRE + WEIGHT_SIDE + WEIGHT_SIDE + WEIGHT_CORNER;
          }
        }
        else{//j>1 and i>1
          if(j < cols-1){
            if(i < rows-1){
              newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1];
              weight[i][j] = WEIGHT_CENTRE + 2*WEIGHT_CORNER + 3*WEIGHT_SIDE + WEIGHT_SIDE + WEIGHT_CORNER;
            }else{
              newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1];
    					weight[i][j] = WEIGHT_CENTRE + 2*WEIGHT_CORNER + 3*WEIGHT_SIDE;
            }
          }else{
            if(i < rows-1){
                newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1];
                weight[i][j] = WEIGHT_CENTRE + WEIGHT_CORNER + 2*WEIGHT_SIDE + WEIGHT_SIDE + WEIGHT_CORNER;
              }else{
                newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1];
      					weight[i][j] = WEIGHT_CENTRE + WEIGHT_CORNER + 2*WEIGHT_SIDE;
              }
            }
          }
        }
      }

  // Count the cells to the bottom right
  for (i = 0; i < rows - 1; i++) {
    for (j = 0; j < cols - 1; j++) {
      newImage[i][j] += WEIGHT_CORNER * oldImage[i+1][j+1];
      weight[i][j] += WEIGHT_CORNER;
    }
  }

  // Produce the final result
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++) {
      newImage[i][j] = newImage[i][j] / weight[i][j];
      check += newImage[i][j];
    }
  }

  return check;
}

static inline long mask6(long oldImage[N][N], long newImage[N][N], int rows, int cols) {

  // TODO This function should contain code that produces the same
  // output as baseMask, but is expected to run faster than mask1 (or
  // mask0) by making better use of caching.

//reason: there is 4 loops instead of many loops, so tht we only need to access cache fewer times.

  int i, j;
  long check = 0;

  long (*weight)[N] = calloc(N * N, sizeof(long));

  for (i = 0; i < rows; i++){
      for (j = 0; j < cols; j++) {
        if(i==0 && j==0){
          newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] +  WEIGHT_SIDE * oldImage[i][j+1] +  WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j+1];
          weight[i][j] = WEIGHT_CENTRE + 2*WEIGHT_SIDE + WEIGHT_CORNER;
        }else if(i>=1 && j==0){
          if(i < rows-1){
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1] + WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j+1];
            weight[i][j] = WEIGHT_CENTRE + 2*WEIGHT_SIDE + 2*WEIGHT_CORNER + WEIGHT_SIDE;
          }else{
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1];
    				weight[i][j] = WEIGHT_CENTRE + 2*WEIGHT_SIDE + WEIGHT_CORNER;
          }
        }else if(i==0 && j>=1){
          if(j < cols-1){
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i][j-1] +  WEIGHT_SIDE * oldImage[i][j+1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1] + WEIGHT_CORNER * oldImage[i+1][j+1];
            weight[i][j] = WEIGHT_CENTRE + 3*WEIGHT_SIDE + 2*WEIGHT_CORNER;
          }else{
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i][j-1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1];
      			weight[i][j] = WEIGHT_CENTRE + 2*WEIGHT_SIDE + WEIGHT_CORNER;
          }
        }
        else{//j>1 and i>1
          if(j < cols-1){
            if(i < rows-1){
              newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1] + WEIGHT_CORNER * oldImage[i+1][j+1];
              weight[i][j] = WEIGHT_CENTRE + 4*WEIGHT_SIDE + 4*WEIGHT_CORNER;
            }else{
              newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1];
    					weight[i][j] = WEIGHT_CENTRE + 2*WEIGHT_CORNER + 3*WEIGHT_SIDE;
            }
          }else{
            if(i < rows-1){
                newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1];
                weight[i][j] = WEIGHT_CENTRE + 2*WEIGHT_CORNER + 3*WEIGHT_SIDE;
              }else{
                newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1];
      					weight[i][j] = WEIGHT_CENTRE + WEIGHT_CORNER + 2*WEIGHT_SIDE;
              }
            }
          }
          newImage[i][j] = newImage[i][j] / weight[i][j];
          check += newImage[i][j];
        }
      }

  return check;
}

static inline long mask7(long oldImage[N][N], long newImage[N][N], int rows, int cols) {

  // TODO This function should contain code that produces the same
  // output as baseMask, but is expected to run faster than mask1 (or
  // mask0) by making better use of caching.

//reason: there is 4 loops instead of many loops, so tht we only need to access cache fewer times.

  int i, j;
  long check = 0;

  long (*weight)[N] = calloc(N * N, sizeof(long));

  register int w_centre = WEIGHT_CENTRE;
  register int w_side = WEIGHT_SIDE;
  register int w_corner = WEIGHT_CORNER;

  for (i = 0; i < rows; i++){
      for (j = 0; j < cols; j++) {
        register int nImage = newImage[i][j];
        if(i==0 && j==0){
          nImage = w_centre * oldImage[i][j] +  w_side * oldImage[i][j+1] +  w_side * oldImage[i+1][j] + w_corner * oldImage[i+1][j+1];
          weight[i][j] = w_centre + 2*w_side + w_corner;
        }else if(i>=1 && j==0){
          if(i < rows-1){
            nImage = w_centre * oldImage[i][j] + w_side * oldImage[i-1][j] + w_side * oldImage[i][j+1] + w_corner * oldImage[i-1][j+1] + w_side * oldImage[i+1][j] + w_corner * oldImage[i+1][j+1];
            weight[i][j] = w_centre + 2*w_side + 2*w_corner + w_side;
          }else{
            nImage = w_centre * oldImage[i][j] + w_side * oldImage[i-1][j] + w_side * oldImage[i][j+1] + w_corner * oldImage[i-1][j+1];
            weight[i][j] = w_centre + 2*w_side + w_corner;
          }
        }else if(i==0 && j>=1){
          if(j < cols-1){
            nImage = w_centre * oldImage[i][j] + w_side * oldImage[i][j-1] +  w_side * oldImage[i][j+1]+w_side * oldImage[i+1][j] + w_corner * oldImage[i+1][j-1] + w_corner * oldImage[i+1][j+1];
            weight[i][j] = w_centre + 3*w_side + 2*w_corner;
          }else{
            nImage = w_centre * oldImage[i][j] + w_side * oldImage[i][j-1]+w_side * oldImage[i+1][j] + w_corner * oldImage[i+1][j-1];
            weight[i][j] = w_centre + 2*w_side + w_corner;
          }
        }
        else{//j>1 and i>1
          if(j < cols-1){
            if(i < rows-1){
              nImage = w_centre * oldImage[i][j] + w_corner * oldImage[i-1][j-1] + w_side * oldImage[i-1][j] + w_side * oldImage[i][j-1]+ w_side * oldImage[i][j+1] + w_corner * oldImage[i-1][j+1]+w_side * oldImage[i+1][j] + w_corner * oldImage[i+1][j-1] + w_corner * oldImage[i+1][j+1];
              weight[i][j] = w_centre + 4*w_side + 4*w_corner;
            }else{
              nImage = w_centre * oldImage[i][j] + w_corner * oldImage[i-1][j-1] + w_side * oldImage[i-1][j] + w_side * oldImage[i][j-1]+ w_side * oldImage[i][j+1] + w_corner * oldImage[i-1][j+1];
              weight[i][j] = w_centre + 2*w_corner + 3*w_side;
            }
          }else{
            if(i < rows-1){
                nImage = w_centre * oldImage[i][j] + w_corner * oldImage[i-1][j-1] + w_side * oldImage[i-1][j] + w_side * oldImage[i][j-1]+ w_side * oldImage[i][j+1]+w_side * oldImage[i+1][j] + w_corner * oldImage[i+1][j-1];
                weight[i][j] = w_centre + 2*w_corner + 3*w_side;
              }else{
                nImage = w_centre * oldImage[i][j] + w_corner * oldImage[i-1][j-1] + w_side * oldImage[i-1][j] + w_side * oldImage[i][j-1]+ w_side * oldImage[i][j+1];
                weight[i][j] = w_centre + w_corner + 2*w_side;
              }
            }
          }
          newImage[i][j] = nImage / weight[i][j];
          check += newImage[i][j];
        }
      }

  return check;
}

static inline long mask8(long oldImage[N][N], long newImage[N][N], int rows, int cols) {

  // TODO This function should contain code that produces the same
  // output as baseMask, but is expected to run faster than mask1 (or
  // mask0) by making better use of caching.

//reason: combined all the loops so that we dont access the cache too many times (1 loop instead of 10 loops)
//reason: using register in local variable i and j so that we dont have to write i and j to the memory/cache (temporal locality)

  register int i, j;
  long check = 0;

  long (*weight)[N] = calloc(N * N, sizeof(long));
  register long* wt = &(*weight)[N];

  for (i = 0; i < rows; i++){
      for (j = 0; j < cols; j++) {
        if(i==0 && j==0){
          newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] +  WEIGHT_SIDE * oldImage[i][j+1] +  WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j+1];
          *wt = WEIGHT_CENTRE + 2*WEIGHT_SIDE + WEIGHT_CORNER;
        }else if(i>=1 && j==0){
          if(i < rows-1){
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1] + WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j+1];
            *wt = WEIGHT_CENTRE + 2*WEIGHT_SIDE + 2*WEIGHT_CORNER + WEIGHT_SIDE;
          }else{
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1];
    				*wt = WEIGHT_CENTRE + 2*WEIGHT_SIDE + WEIGHT_CORNER;
          }
        }else if(i==0 && j>=1){
          if(j < cols-1){
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i][j-1] +  WEIGHT_SIDE * oldImage[i][j+1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1] + WEIGHT_CORNER * oldImage[i+1][j+1];
            *wt = WEIGHT_CENTRE + 3*WEIGHT_SIDE + 2*WEIGHT_CORNER;
          }else{
            newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i][j-1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1];
      			*wt = WEIGHT_CENTRE + 2*WEIGHT_SIDE + WEIGHT_CORNER;
          }
        }
        else{//j>1 and i>1
          if(j < cols-1){
            if(i < rows-1){
              newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1] + WEIGHT_CORNER * oldImage[i+1][j+1];
              *wt = WEIGHT_CENTRE + 4*WEIGHT_SIDE + 4*WEIGHT_CORNER;
            }else{
              newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1];
    					*wt = WEIGHT_CENTRE + 2*WEIGHT_CORNER + 3*WEIGHT_SIDE;
            }
          }else{
            if(i < rows-1){
                newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1];
                *wt = WEIGHT_CENTRE + 2*WEIGHT_CORNER + 3*WEIGHT_SIDE;
              }else{
                newImage[i][j] = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1];
      					*wt = WEIGHT_CENTRE + WEIGHT_CORNER + 2*WEIGHT_SIDE;
              }
            }
          }
          newImage[i][j] = newImage[i][j] / *wt;
          check += newImage[i][j];
        }
      }

  return check;
}

static inline long mask9(long oldImage[N][N], long newImage[N][N], int rows, int cols) {

  // TODO This function should contain code that produces the same
  // output as baseMask, but is expected to run faster than mask1 (or
  // mask0) by making better use of caching.

//reason: combined all the loops so that we dont access the cache too many times (1 loop instead of 10 loops)
//reason: using register in local variable i and j so that we dont have to write i and j to the memory/cache (temporal locality)

  register int i, j;
  register long check = 0;
  register long wt;
  register long nImage;

  for (i = 0; i < rows; i++){
      for (j = 0; j < cols; j++) {
        if(i==0 && j==0){
          nImage = WEIGHT_CENTRE * oldImage[i][j] +  WEIGHT_SIDE * oldImage[i][j+1] +  WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j+1];
          wt = WEIGHT_CENTRE + 2*WEIGHT_SIDE + WEIGHT_CORNER;
        }else if(i>=1 && j==0){
          if(i < rows-1){
            nImage = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1] + WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j+1];
            wt = WEIGHT_CENTRE + 2*WEIGHT_SIDE + 2*WEIGHT_CORNER + WEIGHT_SIDE;
          }else{
            nImage = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1];
    				wt = WEIGHT_CENTRE + 2*WEIGHT_SIDE + WEIGHT_CORNER;
          }
        }else if(i==0 && j>=1){
          if(j < cols-1){
            nImage = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i][j-1] +  WEIGHT_SIDE * oldImage[i][j+1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1] + WEIGHT_CORNER * oldImage[i+1][j+1];
            wt = WEIGHT_CENTRE + 3*WEIGHT_SIDE + 2*WEIGHT_CORNER;
          }else{
            nImage = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_SIDE * oldImage[i][j-1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1];
      			wt = WEIGHT_CENTRE + 2*WEIGHT_SIDE + WEIGHT_CORNER;
          }
        }
        else{//j>1 and i>1
          if(j < cols-1){
            if(i < rows-1){
              nImage = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1] + WEIGHT_CORNER * oldImage[i+1][j+1];
              wt = WEIGHT_CENTRE + 4*WEIGHT_SIDE + 4*WEIGHT_CORNER;
            }else{
              nImage = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1] + WEIGHT_CORNER * oldImage[i-1][j+1];
    					wt = WEIGHT_CENTRE + 2*WEIGHT_CORNER + 3*WEIGHT_SIDE;
            }
          }else{
            if(i < rows-1){
                nImage = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1]+WEIGHT_SIDE * oldImage[i+1][j] + WEIGHT_CORNER * oldImage[i+1][j-1];
                wt = WEIGHT_CENTRE + 2*WEIGHT_CORNER + 3*WEIGHT_SIDE;
              }else{
                nImage = WEIGHT_CENTRE * oldImage[i][j] + WEIGHT_CORNER * oldImage[i-1][j-1] + WEIGHT_SIDE * oldImage[i-1][j] + WEIGHT_SIDE * oldImage[i][j-1]+ WEIGHT_SIDE * oldImage[i][j+1];
      					wt = WEIGHT_CENTRE + WEIGHT_CORNER + 2*WEIGHT_SIDE;
              }
            }
          }
          check += nImage / wt;
        }
      }

  return check;
}

long mask(long oldImage[N][N], long newImage[N][N], int rows, int cols) {
  return MASK_VERSION(oldImage, newImage, rows, cols);
}
