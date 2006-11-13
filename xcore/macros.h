#ifndef XCORE_MACROS_H
#define XCORE_MACROS_H

#define SAFE_DELETE(p) { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p);    (p)=NULL; } }


#endif
