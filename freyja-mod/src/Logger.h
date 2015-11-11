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


class Logger {
public:
	Logger(std::string filename, std::string *cols);
	void log(std::string *input);
	virtual ~Logger();
private:
	std::string filename_;
	std::string *cols_;
	std::ofstream stream_;
};

#endif /* LOGGER_H_ */
