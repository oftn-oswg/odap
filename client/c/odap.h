#ifndef ODAP_H
#define ODAP_H

#include <stdlib.h>
#include <stdbool.h>

enum odap_paramtype {
	ODAP_UINT,
	ODAP_SINT,
	ODAP_DBL,
	ODAP_BUFFER
};

struct odap_param {
	unsigned int key;
	enum odap_paramtype type;
	union {
		unsigned int uint;
		signed int sint;
		double dbl;
		struct {
			size_t size;
			void *contents;
		} buffer;
	} data;
	struct odap_param *next;
};

struct odap_event {
	unsigned int event_code;
	struct odap_param *params;
	struct odap_event *next;
};

struct odap_session {
	bool tracking;
	struct odap_event *events;
	char *trackdata_file;
};

/* Functions for initializing, allocating, and deallocating ODAP structures. */
void                 odap_init (struct odap_session *dap);
struct odap_session *odap_alloc (void);
void                 odap_destroy (struct odap_session *dap);

void                 odap_set_tracking (struct odap_session *dap, bool track);
void                 odap_set_trackdata_file (struct odap_session *dap, char *name);

struct odap_param *  odap_param (unsigned int key, enum odap_paramtype type, ...);

/* Functions for registering events to be tracked */
void                 odap_track (struct odap_session *dap, unsigned int event_code, unsigned int num, ...);

#endif
