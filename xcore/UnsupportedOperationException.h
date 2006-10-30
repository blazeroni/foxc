#ifndef XCORE_UNSUPPORTEDOPERATIONEXCEPTION_H
#define XCORE_UNSUPPORTEDOPERATIONEXCEPTION_H

class UnsupportedOperationException : public std::exception
{
   public:
      UnsupportedOperationException() {}
      virtual ~UnsupportedOperationException() throw() {}

      virtual const char* what() const throw()
      {
         return "Unsupported operation performed.";
      }
};

#endif
