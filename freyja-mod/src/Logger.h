/*
 * Logger.h
 *
 *  Created on: Nov 10, 2015
 *      Author: Paly Robotics
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include <iostream>
#include <fstream>
#include <string>


class Logger {
public:
	Logger(const char* filename);
	void log(const char** input);
	virtual ~Logger();
private:
	const char* filename_;
	const char** cols_;
};

#endif /* LOGGER_H_ */
