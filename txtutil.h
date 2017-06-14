

#ifndef TXTUTIL_H
#define TXTUTIL_H

// internal APIs
const char * endLine(const char *str);
const char* formatLine(int byteIndex, const char* data, int dataLen);

// public APIs
FILE *setStdout(FILE *newFp);
void dump(const char* data, size_t count);
int getLogPri(void);
int setLogPri(int newPri);
int logmsg(int pri, const char *format, ...);
FILE *setLogout(FILE *newFp);


#endif // TXTUTIL_H