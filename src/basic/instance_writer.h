#ifndef _BASIC_WRITER_
#define _BASIC_WRITER_

#include "writer.h"
#include <sstream>

using namespace std;

class InstanceWriter : public Writer
{
public:
	int write(const Instance *pInstance)
	{
	  if (!m_outf_.is_open()) return -1;
	//   const string &label = pInstance->m_label_;

	//   m_outf_ << label << "\t";
	  vector<string> words = pInstance->m_words_;
	  int word_size = words.size();
	  for (int idx = 0; idx < word_size; idx++)
		  m_outf_ << words.at(idx) << " ";
	  m_outf_ << endl;
	  return 0;
	}
};

#endif