
#include "exception.hh"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>

#include <execinfo.h>
#include <cxxabi.h>

using namespace std;

IKException::IKException( const char *file, int line, const string &msg ) : m_file(file), m_line(line) {

  int len = msg.length();
  if( len > IK_EXCEPTION_MAX_MESSAGE_SIZE ) len = IK_EXCEPTION_MAX_MESSAGE_SIZE - 1;
  memcpy( m_message, msg.c_str(), len );
  m_message[len] = 0;

  m_backtrace_len = backtrace( m_backtrace, IK_EXCEPTION_MAX_BACKTRACE_SIZE );
}

IKException::IKException( const char *file, int line, const char *msg ) : m_file(file), m_line(line) {

  int len = strlen(msg);
  if( len > IK_EXCEPTION_MAX_MESSAGE_SIZE ) len = IK_EXCEPTION_MAX_MESSAGE_SIZE - 1;
  memcpy( m_message, msg, len );
  m_message[len] = 0;

  m_backtrace_len = backtrace( m_backtrace, IK_EXCEPTION_MAX_BACKTRACE_SIZE );
}


void IKException::report() const {

  std::stringstream ss; 

  ss << "("   << m_file; 
  ss << " @ " << m_line; 
  ss << ") "  << m_message; 

  cerr << "E: " << ss.str() << endl; 

  /* dump de-mangled stack trace.
   * taken from: http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes */

  char ** messages = backtrace_symbols( m_backtrace, m_backtrace_len );    

  if( !messages ) {
    cerr << "no stacktrace could be found" << endl;
    return;
  }

  // skip first stack frame (points here)
  for (int i = 1; i < m_backtrace_len; ++i)
  {
    char *mangled_name = 0, *offset_begin = 0, *offset_end = 0;

    // find parantheses and +address offset surrounding mangled name
    for (char *p = messages[i]; *p; ++p)
    {
      if (*p == '(') {
        mangled_name = p; 

      } else if (*p == '+') {
        offset_begin = p;

      } else if (*p == ')') {
        offset_end = p;
        break;
      }
    }

    // if the line could be processed, attempt to demangle the symbol
    if (mangled_name && offset_begin && offset_end && mangled_name < offset_begin) {
      *mangled_name++ = '\0';
      *offset_begin++ = '\0';
      *offset_end++ = '\0';

      //cout << "mangled name: " << mangled_name << endl;

      int status;
      char * real_name = abi::__cxa_demangle(mangled_name, 0, 0, &status);

      // if demangling is successful, output the demangled function name
      if (status == 0) {    
        std::cerr << " : (" << i << ") " << messages[i] << " : " 
          << real_name << "+" << offset_begin << offset_end 
          << std::endl;

      } else {
        // otherwise, output the mangled function name
        std::cerr << " : (" << i << ") " << messages[i] << " : " 
          << mangled_name << "+" << offset_begin << offset_end 
          << std::endl;
      }
      free(real_name);

    } else {
      // otherwise, print the whole line
      std::cerr << " : (" << i << ") " << messages[i] << std::endl;
    }
  }
  std::cerr << std::endl;

  free(messages);

}

