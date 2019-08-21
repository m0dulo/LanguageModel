#ifndef BASIC_WRITER_H_
#define BASIC_WRITER_H_

#pragma once

#include <fstream>
#include <iostream>

#include "instance.h"

class Writer
{
protected:
	std::ofstream m_outf_;
public:
	virtual ~Writer()
	{
		if (m_outf_.is_open()) m_outf_.close();
	}

	int startWriting(const char *filename) {
		m_outf_.open(filename);
		if (!m_outf_) {
			std::cout << "Writerr::startWriting() open file err: " << filename << std::endl;
			return -1;
		}
		return 0;
	}

	void finishWriting() {
		m_outf_.close();
	}

	virtual int write(const Instance *pInstance) = 0;
};

#endif // BASIC_WRITER_H_