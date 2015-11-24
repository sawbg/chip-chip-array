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
			 */
			Log(std::string dir, LogMode mode = Logmode::Text);
			
			/** Destroys the Log and closes the logfile. */
			~Log();
			
			//@{
			/**
			 * Writes "DEBUG: " to the log file along with the
			 * message passed. Should be used for generic debugging
			 * information. If recording the value of a variable in
			 * the Log is desired, use the function Variable()
			 * instead.
			*/
			void Debug(const char * mesg);
			void Debug(std::string mesg);
			void Debug(std::stringstream mesg);
			//@}

			//@{
			/**
			 * Writes "ERROR: " to the log file. Should only be use
			 * when an exception is thrown.
			 */
			void Error(const char * mesg);
			void Error(std::string mesg);
			void Error(std::stringstream mesg);
			//@}
			
			/**
			 * Creates a bitmap image in the subdirectory created by
			 * the Log during initialization. Does nothing if
			 * LogMode::Text was passed in the constructor.
			 */
			//void Image(byte* image);
			
			//@{
			/**
			 * Writes "STATUS: " to the log file. Should be used
			 * when recording the status or state of the program. It
			 * should not be used to record microalgorithmic
			 * changes. Use Verbose() for these instead.
			 */
			void Status(const char * mesg);
			void Status(std::string mesg);
			void Status(std::stringstream mesg);
			//@}

			//@{
			/**
			 * Writes "VARIABLE: " to the log file. Should be used
			 * whenever recording the value of a variable is
			 * desired.
			 */
			void Variable(const char * name, auto value);
			void Variable(std::string name, auto value);
			void Variable(std::stringstream name, auto value);
			//@}

			//@{
			/**
			 * Writes "VERBOSE: " to the log file. Should only be
			 * used for recording small, specific portions of code.
			 * To record a change in the more general state of the
			 * program, use Status() instead.
			 */
			void Verbose(const char * mesg);
			void Verbose(std::string mesg);
			void Verbose(std::stringstream mesg);
			//@}

		private:
			// Constants
			const char PATH_SEP = '/';
			const ubyte LEN = 256;
			
			// Other variables
			char[128] dir;
			char[LEN] filename;
			uint8 imgCount = 0;
			LogMode mode;
			std::ofstream file;
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
			std::cerr << "Oh, no! An error has occurred opening \
				the log file.";
			std::cerr << "ERROR CODE: " << ex.code();
			std::cerr << "MESSAGE: " << ex.what();
			exit(ERROR);
		}
	}

	void Log::~Log() {
		file.flush();
		file.close();
	}

	void Log::Debug(const char * mesg) {
		file << mesg;
		file.flush();
	}

	void Log::Debug(std::string mesg) {
		Debug(mesg.c_str());
	}

	void Log::Debug(std::stringstream mesg) {
		string temp = mesg.str();  // required
		Debug(mesg.str());
	}

	void Log::Error(const char * mesg) {
		file << mesg;
		file.flush();
	}

	void Log::Error(std::string mesg) {
		
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
