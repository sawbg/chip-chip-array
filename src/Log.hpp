/**
 * @file
 * @author Samuel Andrew Wisner, awisner94@gmail.com
 * @brief Contains Log class.
 */

#ifndef Log_H
#define Log_H

#include <ctime>
#include <fstream>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <sys/stat.h>

#include "definitions.hpp"

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
	 *
	 * This class DOES NOT WORK without compiling without a "LOG"
	 * definition (#define LOG or -DLOG).
	 */
	class Log {
		public:
			/**
			 * Initializes Log object but does not open log. Open()
			 * must be called.
			 */
			Log() {};

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
			Log(auto dir, LogMode mode = LogMode::Text);

			/**
			 * Destroys the Log and closes the logfile.
			 */
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
			 * @param filename the filename for the saved image
			 */
			void Image(cv::Mat image, auto filename);

			/**
			 *
			 */
			void Open(auto dir, LogMode mode = LogMode::Text);

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
			static const uint8 LEN = 255;


			// OTHER VARIABLES

			/**
			 * The log file directory, including the directory
			 * created with LogMode::Multi.
			 */
			std::string dir;

			/**
			 * The log file filename.
			 */
			std::string filename;

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
	};

	Log::Log(auto dir, LogMode mode) {
#ifdef LOG
		Open(dir, mode);
#endif
	}

	Log::~Log() {
#ifdef LOG
		try {
			file.flush();
			file.close();
		} catch (std::ofstream::failure f) {
			LogError("Gosh dang it! A fatal error has occured " 
					"closing the logfile.", f);
		}
#endif
	}

	void Log::Debug(auto mesg) {
#ifdef LOG
		try {
			file << "DEBUG: " << mesg << std::endl;
			file.flush();
		} catch(std::ofstream::failure f) {
			LogError("Debug() write error", f);
		}
#endif
	}

	void Log::Error(auto mesg) {
#ifdef LOG
		try {
			file << "ERROR: " << mesg << std::endl;
			file.flush();
		} catch(std::ofstream::failure f) {
			LogError("Error() write error", f);
		}
#endif
	}

	void Log::Image(cv::Mat image, auto filename) {
#ifdef LOG
		try {
			cv::imwrite(dir + std::string(filename), image);
		} catch(std::ofstream::failure f) {
			LogError("Image() write error", f);
		} catch(std::exception ex) {
			Error("Error writing image " + std::string(filename));
		}
#endif
	}

	void Log::LogError(auto mesg, std::ofstream::failure f) {
#ifdef LOG
		std::cerr << mesg << std::endl;
		std::cerr << "MESSAGE: " << f.what() << std::endl;
		exit(ERROR);
#endif
	}

	void Log::Open(auto dir, LogMode mode) {
#ifdef LOG
		// format date and time
		char date[32];
		time_t sec = time(nullptr);
		struct tm * loctime = localtime(&sec);
		strftime(date, 32, "%m-%d_%H-%M-%S", loctime);

		// create temperary strings
		this->dir = std::string(dir);
		std::string datestr = std::string(date);

		// add path separator if necessary
		if(this->dir[this->dir.length() - 1] != PATH_SEP) {
			this->dir += PATH_SEP;
		}

		// add directory for log and images if necessary
		if(mode == LogMode::Multi) this->dir += datestr + PATH_SEP;

		int ret = mkdir(this->dir.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP
				| S_IROTH | S_IWOTH | S_IXUSR | S_IXGRP
				| S_IXOTH);

		filename = this->dir + datestr + ".log";

		// set class mode
		this->mode = mode;

		// Initializing file
		file.exceptions(std::ofstream::failbit
				| std::ofstream::badbit);

		try {
			file.open(filename, std::ofstream::out
					| std::ofstream::app);
		} catch(std::ofstream::failure ex) {
			LogError("Oh, no! An error has occurred opening the "
					"log file.", ex);
		}
#endif
	}

	void Log::Status(auto mesg) {
#ifdef LOG
		try {
			file << "STATUS: " << mesg << std::endl;
			file.flush();
		} catch (std::ofstream::failure f) {
			LogError("Status() write error", f);
		}
#endif
	}

	void Log::Variable(auto name, auto value) {
#ifdef LOG
		try {
			file << "VARIABLE: " << name << " = " << value
				<< std::endl;
			file.flush();
		} catch(std::ofstream::failure f) {
			LogError("Variable() write error", f);
		}
#endif
	}

	void Log::Verbose(auto mesg) {
#ifdef LOG
		try {
			file << "VERBOSE: " << mesg << std::endl;
			file.flush();
		} catch(std::ofstream::failure f) {
			LogError("Verbose() write error", f);
		}
#endif
	}
}
#endif
