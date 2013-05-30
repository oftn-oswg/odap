ODAP (Open Data Analytics Project)
==================================

ODAP is planned to be a lightweight library for desktop tracking and analytics.

Example
-------

```c
bool accurate = checkbox->ischecked;
odap_track (dap, EVENT_ACCURACY, 1, odap_param (0, ODAP_UINT, (unsigned int) accurate));
```
