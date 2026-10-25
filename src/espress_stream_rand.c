#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/mutex.h"
#include "espress_stream/rand.h"

#include "rand.h"

static i_t _initialize(void);
static void _finalize(void);
static void _get(b_t *buffer, u_t size);

struct espress_stream_rand_ops const espress_stream_rand[] =
{{
	_initialize,
	_finalize,
	_get
}};

static struct espress_stream_mutex _mutex[1];
static b_t _buffer[4];
static u_t _size;

static i_t _initialize(void)
{
	_size = 0;
	return espress_stream_mutex->initialize(_mutex);
}

static void _finalize(void)
{
	espress_stream_mutex->finalize(_mutex);
	_size = 0;
	memset(_buffer, 0, sizeof(_buffer));
}

static u_t _getrandom(b_t *b, u_t s)
{
	u_t ret = 0;

	if (0 < s)
	{
		if (0 == _size)
		{
			*((uint32_t *)_buffer) = get_random_u32();
			_size = 4;
		}
		*b = _buffer[4 - _size];
		_size--;
		ret = 1;
	}

	return ret;
}

static void _get(b_t *buffer, u_t size)
{
	u_t s;

	espress_stream_mutex->lock(_mutex);

	while (0 < size)
	{
		s = _getrandom(buffer, size);
		if (0 < s)
		{
			buffer += s;
			size -= s;
		}
	}

	espress_stream_mutex->unlock(_mutex);
}
