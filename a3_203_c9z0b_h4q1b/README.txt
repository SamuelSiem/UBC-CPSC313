
Put your answers to non-coding questions in this file as described in
the assignment description.

All answers are to be in plain text ASCII.

Problem 2
a) The miss rate: 0.125
   Explanation: The size of a block (being 64) directly coincides with the size of 8 longs (8 elements from the row).
   The for loops access the arrays in row order making use of the spacial locality in the cache. Therefore, a miss
   only occurs in two cases:
			1) cold miss (the first item in the row)
			2) when using a block that was previously used (the first item in the row)
				- By the time a new set of elements start to use a block previously used, the previous set would
				  have been done using it. Therefore, it is okay to replace them.
   This leaves us with a 1/8 miss rate == 0.125

b) The miss rate: 1
   Explanation: The for loops access the arrays in column order which doesnt make us of the spacial locality in any
   way. Each time the array is accessed, 8 elements from that row is loaded into the block. However, instead of accessing
   using all the loaded elements, the function requires that a new row be loaded. Because multiple addresses map to the
   same block, the addresses are contuinuously being replaced at each array access point. Therefore, a new row
   set of elements is loaded at each array access point. 8/8 miss rate == 1


c) The miss rate: 0.5
	Explanation: Similar to the sumB function, sumC access the items in the array using column order. However, in this
	sum implementation, there is an increment of 2 and within each iteration, 2 array accesses are made for each block
	(a total of 4 accesses). Therefore, before a new set of elements is brought in, there is a hit on the current data in
	the cache. This happens twice for every iteration; ie, there are 2 hits and 2 misses in each iteration.

d) The miss rate: 1
	Explanation: Each two consecutive set of 8 longs in the array row will, be matched to a set in the cache. Because
	the cache utilises LRU, each block will eventually be replaced in the set resulting in a cache miss each time.
	For example, the following arrayelements will utilise the set 1:
		- addresses of elements 0 - 7 and 8 - 15 (row 0)
		- addresses of elements 0 - 7 and 8 - 15 (row 2)
		- addresses of elements 0 - 7 and 8 - 15 (row 4)
		- addresses of elements 0 - 7 and 8 - 15 (row 6)
	When accessing row 0 column 0, there will be a cold miss. Similarly with row 0 column 1. However, when we get
	to row 2 column 0, [row 0 column 0] will be replaced as it is the LRU.[Row 0 column 1] will be replaced by
	[row 2 column 1] as it is now the new LRU and the cycle continues . When we get back to row 0 column 0, there
	will be a miss again as it was replaced. Therefore, for every array access, a miss will be experienced.

e)  The miss rate: 1
	Explanation: Each four consecutive set of 8 longs in the array row will, be matched to a set in the cache. Because
	the cache utilises LRU, each block will eventually be replaced in the set resulting in a cache miss each time.
	For example, the following arrayelements will utilise the set 1:
		- addresses of elements 0 - 7, 8 - 15, 16 - 23 amnd 24 - 31 (row 0)
		- addresses of elements 0 - 7, 8 - 15, 16 - 23 amnd 24 - 31 (row 2)
		- addresses of elements 0 - 7, 8 - 15, 16 - 23 amnd 24 - 31 (row 4)
		- addresses of elements 0 - 7, 8 - 15, 16 - 23 amnd 24 - 31 (row 6)
	When accessing row 0 column 0, there will be a cold miss. Similarly with row 0 column 1. However, when we get
	to row 2 column 0, [row 0 column 0] will be replaced as it is the LRU.[Row 0 column 1] will be replaced by
	[row 2 column 1] as it is now the new LRU and the cycle continues . When we get back to row 0 column 0, there
	will be a miss again as it was replaced. Therefore, for every array access, a miss will be experienced.
	(Same explanation as before but with 4 blocks and less sets).

f) miss rate = [0.836 - 1]
	Explanation: In the best case, the "random" algorithm could consistently choose the same block to replace each
	time. That would mean, all the accesses for the other three blocks (apart from the cold miss) will result in a
	hitv(in each set). That is:
		8 items * 3 blocks = 24
		24 - 3 cold misses  = 21
		21 hits * 8 sets/ 128 * 8 (total) = 0.164 hit rate
		1 - 0.164 = 0.836 miss rate
	On the other hand (the worst case), the algorithm could always choose a new block to replace resulting in the
	miss rate of 1.
	The algorithm could also produce something within the range of 0.836 and 1.

g)miss rate = 0.125
	Explanation: With only 120 columns, the blocks are aligned in such a way
	that in a full iteration of j, different blocks are accessed. Therefore,
	The only time there is a miss is when replacing the whole block for the second
	iteration or cold misses. That is 1/8 miss rate.







PART 3

mask1 : changed the first 4 loops to row-based access instead of column based to
	ensure that we are utilising the spacial locality of our cache. Before
	we change all the loops, we are trying with only a few.
      : 0.92

mask2 : changes all the loops to row-based access to ensure that we are utilising
	spacial locality of our cache. This was done as a second version since
	the first changes resulted in an improved optimisation. Also, we deleted
	the column and row variables.
      : 0.27 (yay!)
mask3 : the loops that have the same conditions were combined as we wanted to reduce
	the number of times the cache would be accessed.
      : 0.16
mask4 - mask8 : combine many for loops with different constraints into one for
		loop, we used our own picture (smaller picture) and it works fine. But apparently,
    after I tried running my code using the picture provided. It didn't work well. However,
    our last version (mask 9) works perfectly
    ~0.11

mask 9: on top of everything, in this last version we used registers for every variable so That
  instead of asking the main memory everytime, we stored it in registers. Also, we removed newImage[i][j]
  and just changed it with regular variable, because that value doesn't necessarily to be an arrays
  0.05 (FINALLY!!)
