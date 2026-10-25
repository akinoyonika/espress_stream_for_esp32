#ifndef __ESPRESS_STREAM_API_DEPEND_H__
#define __ESPRESS_STREAM_API_DEPEND_H__

#ifndef __ESPRESS_STREAM_DEPEND_H__
	struct sockaddr_storage {
		char address[32];
		uint16_t port;
	};
#endif

#if defined(ESP8266)
#	define sched_yield() yield()
#endif

#endif /* __ESPRESS_STREAM_API_DEPEND_H__ */