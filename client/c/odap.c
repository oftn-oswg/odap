#include <stdarg.h>
#include <string.h>

#include "odap.h"

void odap_init (struct odap_session *dap) {
	dap->events = NULL;
	dap->tracking = true;
	dap->trackdata_file = NULL;
}

struct odap_session *odap_alloc () {
	struct odap_session *dap = malloc (sizeof *dap);
	if (dap)
		odap_init (dap);

	return dap;
}

void odap_destroy (struct odap_session *dap) {
	if (!dap)
		return;

	/* Free events too */
	if (dap->events) {

		struct odap_event *event = dap->events;
		while (event) {

			/* Free event parameters too */
			struct odap_param *param = event->params;
			while (param) {

				/* Free parameter buffers too */
				if (param->type == ODAP_BUFFER) {
					if (param->data.buffer.contents) {
						free (param->data.buffer.contents);
					}
				}

				struct odap_param *next = param->next;
				free(param);
				param = next;
			}


			struct odap_event *next = event->next;
			free (event);
			event = next;
		}

	}

	free (dap);
}

void odap_set_tracking (struct odap_session *dap, bool tracking) {
	if (dap)
		dap->tracking = tracking;
}

void odap_set_trackdata_file (struct odap_session *dap, char *name) {
	if (dap)
		dap->trackdata_file = name;
}

struct odap_param *odap_param (unsigned int key, enum odap_paramtype type, ...) {
	size_t size;
	void *buffer;
	va_list args;
	struct odap_param *param = malloc (sizeof *param);

	if (!param)
		return NULL;

	param->key = key;
	param->type = type;
	param->next = NULL;

	va_start (args, type);

	switch (type) {
	case ODAP_UINT: param->data.uint = va_arg (args, unsigned int); break;
	case ODAP_SINT: param->data.sint = va_arg (args, signed int); break;
	case ODAP_DBL:  param->data.dbl  = va_arg (args, double); break;
	case ODAP_BUFFER:
		/* We need to allocate a new copy */
		size = va_arg (args, size_t);
		buffer = malloc (size);
		memcpy (buffer, va_arg (args, void *), size);

		param->data.buffer.size = size;
		param->data.buffer.contents = buffer;
		break;
	}

	va_end (args);

	return param;
}

void odap_track (struct odap_session *dap, unsigned int event_code, unsigned int num, ...) {
	va_list params;
	struct odap_event *event = malloc (sizeof *event);

	if (!event || !dap)
		return;

	event->event_code = event_code;
	event->params = NULL;
	event->next = NULL;

	va_start (params, num);
	while (num--) {
		struct odap_param *param = va_arg (params, struct odap_param *);
		if (param) {
			/* Add param to event */
			param->next = event->params;
			event->params = param;
		}
	}
	va_end (params);

	/* Add new event to session */
	event->next = dap->events;
	dap->events = event;
}
