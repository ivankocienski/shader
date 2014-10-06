
#pragma once

#include <exception>
#include <string>

#define IK_EXCEPTION_MAX_MESSAGE_SIZE 1024
#define IK_EXCEPTION_MAX_BACKTRACE_SIZE 256

class IKException : public std::exception {
private:


  const char *m_file;
  char  m_message[IK_EXCEPTION_MAX_MESSAGE_SIZE];

  int   m_line;
  void *m_backtrace[IK_EXCEPTION_MAX_BACKTRACE_SIZE];
  int   m_backtrace_len;


public:

  IKException( const char *, int, const char * );
  IKException( const char *, int, const std::string& );

  void report() const;
};

#define kraise(msg) { throw IKException( __FILE__, __LINE__, msg ); }
