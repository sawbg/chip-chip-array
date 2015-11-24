#ifndef Log_H
#define Log_H

#include <ctime>
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
			 *
			 * @param dir the directory for the newly created
			 * logfile/folder
			 *
			 * @param mode the LogMode
			 */
			Log(std::string dir, LogMode mode = LogMode::Text);

			/** Destroys the Log and closes the logfile. */
			~Log();

			/**
			 * Writes "DEBUG: " to the log file along with the
			 * message passed. Should be used for generic debugging
			 * information. If recording the value of a variable in
			 * the Log is desired, use the function Variable()
			 * instead.
			 *
			 * @param mesg the message to record in the logfile
			 */
			void Debug(auto mesg);

			/**
			 * Writes "ERROR: " to the log file. Should only be use
			 * when an exception is thrown.
			 *
			 * @param mesg the message to record in the log
			 */
			void Error(auto mesg);

			/**
			 * Creates a bitmap image in the subdirectory created by
			 * the Log during initialization. Does nothing if
			 * LogMode::Text was passed in the constructor.
			 *
			 * @param image the image to save
			 */
			//void Image(byte* image);

			/**
			 * Writes "STATUS: " to the log file. Should be used
			 * when recording the status or state of the program. It
			 * should not be used to record microalgorithmic
			 * changes. Use Verbose() for these instead.
			 *
			 * @param mesg the message to record in the logfile
			 */
			void Status(auto mesg);

			/**
			 * Writes "VARIABLE: " to the log file. Should be used
			 * whenever recording the value of a variable is
			 * desired.
			 *
			 * @param name the variable name to record
			 * @param value the variable value to record
			 */
			void Variable(auto name, auto value);

			/**
			 * Writes "VERBOSE: " to the log file. Should only be
			 * used for recording small, specific portions of code.
			 * To record a change in the more general state of the
			 * program, use Status() instead.
			 *
			 * @param mesg the message to record in the logfile
			 */
			void Verbose(auto mesg);

		private:
			// CONSTANTS

			/**
			 * The path sepearator. Can be changed if code adapted
			 * for Windows.
			 */
			const char PATH_SEP = '/';

			/**
			 * The (maximum) length of the filename character
			 * array.
			 */
			const uint8 LEN = 256;


			// OTHER VARIABLES

			/**
			 * The log file directory, including the directory
			 * created with LogMode::Multi.
			 */
			char[128] dir;

			/**
			 * The log file filename.
			 */
			char[LEN] filename;

			/**
			 * The number of images saved in the image directory.
			 * Used in naming the images in LogMode::Multi.
			 */
			uint8 imgCount = 0;

			/**
			 * The LogMode.
			 */
			LogMode mode;

			/**
			 * The logfile.
			 */
			std::ofstream file;


			// FUNCTIONS

			/**
			 * Prints an error message and exits the application.
			 * Used when errors occur within this class ONLY.
			 *
			 * @param mesg the error message to print
			 * @param f the failure object caught
			 */
			void LogError(auto mesg, std::ofstream::failure f);
	}

	void Log::Log(char * dir, LogMode mode) {
		// format date and time
		char[32] date;
		time_t sec = time(nullptr);
		struct tm * loctime = localtime(&sec);
		strftime(date, 32, "%m-%e_%H-%M-%S", loctime);

		// create temperary strings
		std::string dirstr = std::string(dir);
		std::string datestr = std::string(date);

		// add path separator if necessary
		if(dirstr[dirstr.length() - 1] != PATH_SEP) dirstr += PATH_SEP;

		// add directory for log and images if necessary
		if(mode == LogMode::Multi) dirstr += datestr + PATH_SEP;

		// set log filename
		dir = dirstr.c_str();
		dirstr += datestr + ".log";
		filename = dirstr.c_str();

		// set class mode and dir
		this.mode = mode;
		this.dir = dir;

		// Initializing file
		file.exceptions(std::ofstream::eofbit | std::ofstream::failbit
				| std::ofstream::badbit);

		try {
			file.open(filename, std::ofstream::out
					| std::ofstream::app);
		} catch(std::ofstream::failure ex) {
			LogError("Oh, no! An error has occurred opening the \
					log file.", ex);
		}
	}

	void Log::~Log() {
		try {
			file.flush();
			file.close();
		} catch (std::ofstream::failure f) {
			LogError("Gosh dang it! A fatal error has occured \
					closing the logfile.", f);
		}
	}

	void Log::Debug(auto mesg) {
		try {
			file << "DEBUG: " << mesg << endl;
			file.flush();
		} catch(std::ofstream::failure f) {
			LogError("Debug() write error", f);
		}
	}

	void Log::Error(auto mesg) {
		try {
			file << "ERROR: " << mesg << endl;
			file.flush();
		} catch(std::ofstream::failure f) {
			LogError("Error() write error", f);
		}
	}

	//	void Log::Image(...) {}

	void Log::LogError(auto mesg, failure f) {
		std::cerr << mesg << endl;
		std::cerr << "ERROR CODE: " << ex.code() << endl;
		std::cerr << "MESSAGE: " << ex.what() << endl;
		exit(ERROR);
	}

	void Log::Status(auto mesg) {
		try {
			file << "STATUS: " << mesg << endl;
			file.flush();
		} catch (std::ofstream::failure f) {
			LogError("Status() write error", f);
		}
	}

	void Log::Variable(auto name, auto value) {
		try {
			file << "VARIABLE: " << name << " = " << value << endl;
			file.flush();
		} catch(std::ofstream::failure f) {
			LogError("Variable() write error", f);
		}
	}

	void Log::Verbose(auto mesg) {
		try {
			file << "VERBOSE: " << mesg << endl;
			file.flush();
		} catch(std::ofstream::failure f) {
			LogError("Verbose() write error", f);
		}
	}
}
#endif
