﻿#pragma once
#if _MSC_VER <= 1400
#define RetAddr() NULL
#else
#include <intrin.h>
#define RetAddr() _ReturnAddress()
#endif


SNSBEGIN

class SLogBinary {
  public:
    SLogBinary(const char *buf, int len)
    {
        _buf = buf;
        _len = len;
    }
    const char *_buf;
    int _len;
};

class SOUI_EXP SLogStream {
  public:
    SLogStream(char *buf, int len);
  public:
    SLogStream &operator<<(const void *t);
    SLogStream &operator<<(const char *t);
#if defined(WIN32) || defined(_WIN64)
    SLogStream &operator<<(const wchar_t *t);
#endif
    SLogStream &operator<<(bool t);
    SLogStream &operator<<(char t);
    SLogStream &operator<<(unsigned char t);
    SLogStream &operator<<(short t);
    SLogStream &operator<<(unsigned short t);
    SLogStream &operator<<(int t);
    SLogStream &operator<<(unsigned int t);
    SLogStream &operator<<(long t);
    SLogStream &operator<<(unsigned long t);
    SLogStream &operator<<(long long t);
    SLogStream &operator<<(unsigned long long t);
    SLogStream &operator<<(float t);
    SLogStream &operator<<(double t);
    SLogStream &operator<<(const SLogBinary &binary);
  private:
	SLogStream &writeData(const char *ft, ...);
	SLogStream &writeLongLong(long long t);
	SLogStream &writeULongLong(unsigned long long t);
	SLogStream &writePointer(const void *t);
	SLogStream &writeString(const char *t);
	SLogStream &writeWString(const wchar_t *t);
	SLogStream &writeBinary(const SLogBinary &t);

  private:
    SLogStream();
    SLogStream(SLogStream &);
    char *_begin;
    char *_end;
    char *_cur;
};


	typedef void (*LogCallback)(const char *tag, const char* pLogStr, int level, int loggerId,const char * file,int line,const char *fun,void *retAddr);

	class SOUI_EXP Log
	{
	public:
		enum{
			MAX_TAGLEN = 100,
			MAX_LOGLEN = 1024,
		};
		enum LogPriority {
			/** For internal use only.  */
			LOG_UNKNOWN = 0,
			/** The default priority, for internal use only.  */
			LOG_DEFAULT, /* only for SetMinPriority() */
			/** Verbose logging. Should typically be disabled for a release apk. */
			LOG_VERBOSE,
			/** Debug logging. Should typically be disabled for a release apk. */
			LOG_DEBUG,
			/** Informational logging. Should typically be disabled for a release apk. */
			LOG_INFO,
			/** Warning logging. For use with recoverable failures. */
			LOG_WARN,
			/** Error logging. For use with unrecoverable failures. */
			LOG_ERROR,
			/** Fatal logging. For use when aborting. */
			LOG_FATAL,
		};
	public:
		Log(int nLoggerId,const char *tag, int level,const char * filename, const char *funcname,int lineIndex,void *pAddr);

		~Log();
		SLogStream & stream();

		static void setLogLevel(int nLevel);
		static void setLogCallback(LogCallback logCallback);
		static void setLogEcho(bool bEnable);
	private:
		static void DefCallback(const char *tag, const char* pLogStr, int level, int loggerId,const char * file,int line,const char *fun,void *retAddr);

		char m_tag[100];
		int  m_level;
		int  m_loggerId;
		void * m_pAddr;
		const char * m_file,* m_func;
		int m_line;
		char m_logBuf[MAX_LOGLEN+1];
		SLogStream m_stream;

		static bool s_enableEcho;
		static int s_logLevel;
		static LogCallback gs_logCallback;
	};

SNSEND

#define SLOG(tag,level,loggerId) SOUI::Log(loggerId,tag,level,__FILE__,__FUNCTION__,__LINE__,RetAddr()).stream()

#define SLOG_FMT(tag,level, loggerId,logformat, ...)                                    \
	do                                                                                  \
	{                                                                                   \
		if(sizeof(logformat[0])==sizeof(char)) {                                        \
			char logbuf[SOUI::Log::MAX_LOGLEN]={0};                                     \
			int nLen = _snprintf(logbuf, SOUI::Log::MAX_LOGLEN,                         \
			(const char*)logformat, ##__VA_ARGS__);                                     \
			SLOG(tag,level,loggerId)<<logbuf;                                           \
		}else                                                                           \
		{                                                                               \
			wchar_t logbuf[SOUI::Log::MAX_LOGLEN]={0};                                  \
			int nLen = _snwprintf(logbuf, SOUI::Log::MAX_LOGLEN,                        \
			(const wchar_t*)logformat, ##__VA_ARGS__);                                  \
			SLOG(tag,level,loggerId)<<logbuf;                                           \
		}                                                                               \
	}while(false);


//流式输出日志，当kLogTag有效时使用，否则编译失败，kLogTag可以是当前定义的宏，也可以是当前对象的成员变量。
#define SLOGD() SLOG(kLogTag,SOUI::Log::LOG_DEBUG,0)
#define SLOGI() SLOG(kLogTag,SOUI::Log::LOG_INFO,0)
#define SLOGW() SLOG(kLogTag,SOUI::Log::LOG_WARN,0)
#define SLOGE() SLOG(kLogTag,SOUI::Log::LOG_ERROR,0)
#define SLOGF() SLOG(kLogTag,SOUI::Log::LOG_FATAL,0)

//流式输出日志，每条日志手动指定tag
#define SLOGD2(tag) SLOG(tag,SOUI::Log::LOG_DEBUG,0)
#define SLOGI2(tag) SLOG(tag,SOUI::Log::LOG_INFO,0)
#define SLOGW2(tag) SLOG(tag,SOUI::Log::LOG_WARN,0)
#define SLOGE2(tag) SLOG(tag,SOUI::Log::LOG_ERROR,0)
#define SLOGF2(tag) SLOG(tag,SOUI::Log::LOG_FATAL,0)

//格式化输出日志，当kLogTag有效时使用，否则编译失败，kLogTag可以是当前定义的宏，也可以是当前对象的成员变量。
#define SLOGFMTD(logformat, ...) SLOG_FMT(kLogTag,SOUI::Log::LOG_DEBUG,0,logformat,##__VA_ARGS__)
#define SLOGFMTI(logformat, ...) SLOG_FMT(kLogTag,SOUI::Log::LOG_INFO,0,logformat,##__VA_ARGS__)
#define SLOGFMTW(logformat, ...) SLOG_FMT(kLogTag,SOUI::Log::LOG_WARN,0,logformat,##__VA_ARGS__)
#define SLOGFMTE(logformat, ...) SLOG_FMT(kLogTag,SOUI::Log::LOG_ERROR,0,logformat,##__VA_ARGS__)
#define SLOGFMTF(logformat, ...) SLOG_FMT(kLogTag,SOUI::Log::LOG_FATAL,0,logformat,##__VA_ARGS__)

//格式化输出日志，每条日志手动指定tag
#define SLOGFMTD2(tag,logformat, ...) SLOG_FMT(tag,SOUI::Log::LOG_DEBUG,0,logformat,##__VA_ARGS__)
#define SLOGFMTI2(tag,logformat, ...) SLOG_FMT(tag,SOUI::Log::LOG_INFO,0,logformat,##__VA_ARGS__)
#define SLOGFMTW2(tag,logformat, ...) SLOG_FMT(tag,SOUI::Log::LOG_WARN,0,logformat,##__VA_ARGS__)
#define SLOGFMTE2(tag,logformat, ...) SLOG_FMT(tag,SOUI::Log::LOG_ERROR,0,logformat,##__VA_ARGS__)
#define SLOGFMTF2(tag,logformat, ...) SLOG_FMT(tag,SOUI::Log::LOG_FATAL,0,logformat,##__VA_ARGS__)

//SOUI4内部使用的日志输出，自动将TAG定义为soui4
#define kSoui4Tag "soui4"
#define SSLOGD() SLOG(kSoui4Tag,SOUI::Log::LOG_DEBUG,0)
#define SSLOGI() SLOG(kSoui4Tag,SOUI::Log::LOG_INFO,0)
#define SSLOGW() SLOG(kSoui4Tag,SOUI::Log::LOG_WARN,0)
#define SSLOGE() SLOG(kSoui4Tag,SOUI::Log::LOG_ERROR,0)
#define SSLOGF() SLOG(kSoui4Tag,SOUI::Log::LOG_FATAL,0)

#define SSLOGFMTD(logformat, ...) SLOG_FMT(kSoui4Tag,SOUI::Log::LOG_DEBUG,0,logformat,##__VA_ARGS__)
#define SSLOGFMTI(logformat, ...) SLOG_FMT(kSoui4Tag,SOUI::Log::LOG_INFO,0,logformat,##__VA_ARGS__)
#define SSLOGFMTW(logformat, ...) SLOG_FMT(kSoui4Tag,SOUI::Log::LOG_WARN,0,logformat,##__VA_ARGS__)
#define SSLOGFMTE(logformat, ...) SLOG_FMT(kSoui4Tag,SOUI::Log::LOG_ERROR,0,logformat,##__VA_ARGS__)
#define SSLOGFMTF(logformat, ...) SLOG_FMT(kSoui4Tag,SOUI::Log::LOG_FATAL,0,logformat,##__VA_ARGS__)

