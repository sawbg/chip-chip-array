#ifndef definitions_H
#define definitions_H

#define ENUM signed char
#define ERROR -1

typedef unsigned char byte;
typedef unsigned char uint8;
typedef signed char sint8;

typedef unsigned short uint16;
typedef signed short sint16;

typedef unsigned int uint32;
typedef signed int sint32;

typedef unsigned long long uint64;
typedef signed long long sint64;

typedef float float32;
typedef double float64;

/** The position of the block relative to the arm. */
enum class BlockPosition : ENUM {
	Front,
	Back
};

/** The color of a block or train car. */
enum class Color : ENUM {
	Red,
	Yellow,
	Green,
	Blue
};

/**
 * The mode in which the Log should prepare
 * (i.e., text only or text and images).
 */
enum class LogMode : ENUM {
	Text,
	Multi
};

/**
 * The number of half blocks picked up in a stack.
 * The integer value of the 
 */
enum class Result : ENUM {
	No_Blocks = -1,
	No_Halves = 0,
	Two_Halves = 2,
	Four_Halves = 4
};

/** Represents which block to pick up when multiple blocks are visible. */
enum class Side : ENUM {
	Left,
	Right
};

/** The block size, either 2.5" or 5". */
enum class Size : ENUM {
	Short,
	Long
};

/** Zone A, B, or C */
enum class Zone : ENUM {
	A,
	B,
	C
};

#endif
