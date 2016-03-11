/**
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief contains definitions for architecture-independant numeric variables,
 * enumerations and enumerated classes, and #define'd constants, and to_sting()
 * overloads for the enumerated classes.
 */

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
	Blue,
	Perrywinkle  // essentially, no color
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
	A = 'A',
	B = 'B',
	C = 'C'
};

namespace std {
	/**
	 * Converts a BlockPosition to a string.
	 */
	string to_string(BlockPosition pos) {
		if(pos == BlockPosition::Front) return string("Front");
		else return string("Back");
	}

	/**
	 * Converts a Color to a string.
	 */
	string to_string(Color color) {
		string ret;

		switch((ENUM)color) {
			case 0:
				ret = "Red";
				break;

			case 1:
				ret = "Yellow";
				break;

			case 2:
				ret = "Green";
				break;

			case 3:
				ret = "Blue";
				break;

			case 4:
				ret = "All";
				break;
		}

		return ret;
	}

	/**
	 * Converts a LogMode to a string.
	 */
	string to_string(LogMode mode) {
		if(mode == LogMode::Multi) return string("Text");
		else return string("Multi");
	}

	/**
	 * Converts a Result to a string.
	 */
	string to_string(Result res) {
		string ret;

		switch((ENUM)res) {
			case -1:
				ret = "No Blocks";
				break;

			case 0:
				ret = "Two whole, no halves";
				break;

			case 2:
				ret = "Two whole, two halves";
				break;

			case 4:
				ret = "No whole, four halves";
				break;
		}

		return ret;
	}

	/**
	 * Converts a Side to a string.
	 */
	string to_string(Side side) {
		if(side == Side::Left) return string("Left");
		else return string("Right");
	}

	/**
	 * Converts a Size to a string.
	 */
	string to_string(Size size) {
		if(size == Size::Long) return string("Long");
		else return string("Short");
	}

	/**
	 * Converts a Zone to a string.
	 */
	string to_string(Zone zone) {
		return string(1, (char)zone);
	}
}

#endif
