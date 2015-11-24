#ifndef Log_H
#define Log_H

#include <fstream>
#include <iostream>
#include <string>

#include "definitions"

namespace ChipChipArray {

	/**
	 * This class logs the text and images passed to it to specificed directory.
	 *
	 * A "container" directory to which the class can write is passed in the
	 * constructor. When the Log is initialized with LogMode::Text, a new log
	 * file is created with a filename based on the time of initialization in
	 * the given directory. When initialized in LogMode::Multi, it will create
	 * a subdirectory in the given directory with a name based on time. In this
	 * new directory, a log file will be created. Images may later be stored in
	 * this directory with names based on the order in which they were saved.
	 */
	public class Log {
		public:
			/**
			 * Initializes the Log.
			 *
			 * A new log file is created in dir if LogMode::Text is
			 * given. The file will have a name based on the current
			 * date and time. If LogMode::Multi is given, a new
			 * directory is created, and a log file with a name
			 * based on the current date and time is created inside
			 * it.
			 */
			Log(string dir, LogMode mode);
			
			/** Destroys the Log */
			~Log();
			
			/**
			 * Writes "DEBUG: " to the log file along with the
			 * message passed. Should be used for generic debugging
			 * information. If recording the value of a variable in
			 * the Log is desired, use the function Variable()
			 * instead.
			*/
			void Debug(string mesg);

			/**
			 * Writes "ERROR: " to the log file. Should only be use
			 * when an exception is thrown.
			 */
			void Error(string mesg);
			
			/**
			 * Creates a bitmap image in the subdirectory created by
			 * the Log during initialization. Does nothing if
			 * LogMode::Text was passed in the constructor.
			 */
			//void Image(byte* image);
			
			/**
			 * Writes "STATUS: " to the log file. Should be used
			 * when recording the status or state of the program. It
			 * should not be used to record microalgorithmic
			 * changes. Use Verbose() for these instead.
			 */
			void Status(string mesg);

			/**
			 * Writes "VARIABLE: " to the log file. Should be used
			 * whenever recording the value of a variable is
			 * desired.
			 */
			void Variable(string name, auto value);

			/**
			 * Writes "VERBOSE: " to the log file. Should only be
			 * used for recording small, specific portions of code.
			 * To record a change in the more general state of the
			 * program, use Status() instead.
			 */
			void Verbose(string mesg);

		private:
			LogMode mode;
			ofstream file;
			uint8 imgCount = 0;

	}

	void Log::Log(string dir, LogMode mode) {
		this.mode = mode;

		// open file here, appending only
	}

	void Log::~Log() {
		// close file here
	}

	void Log::Debug(string mesg) {

	}

	void Log::Error(string mesg) {

	}

//	void Log::Image(...) {}

	void Log::Status {

	}

	void Log::Variable(string name, auto value) {

	}

	void Log::Verbose(string mesg) {

	}

}
#endif
