/*
 * Logger.cpp
 *
 *  Created on: Nov 10, 2015
 *      Author: Paly Robotics
 */

#include "Logger.h"
#include <cassert>
#include <iostream>
#include <fstream>

Logger::Logger(std::string filename, std::string *cols)
{
	filename_ = filename;
	cols_ = cols;
}

void Logger::log(std::string *input) {
	assert(sizeof input == sizeof cols_);

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

