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

Logger::Logger(const char* filename)
{
	filename_ = filename;
	cols_ = {"Left", "Right"};
}

void Logger::log(const char** input) {
	assert(sizeof input == sizeof cols_);
	std::ofstream data_(filename_);
	data_ << input[0] << "," << input[1] << std::endl;
}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

