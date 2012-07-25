#ifndef FIELDS_H
#define FIELDS_H

const int ST_ID     = 0;
const int ST_DESC   = 1;
const int ST_NOTES  = 2;
const int ST_HTD    = 3;
const int ST_IMP    = 4;
const int ST_EST    = 5;
const int ST_USER   = 6;
const int ST_TYP    = 7;
const int ST_STATUS = 8;
const int ST_SPRINT = 9;
const int ST_COMP   = 10;
const int ST_VERSION= 11;
const int ST_PARENT = 12;
const int ST_LAST   = 13;

extern const char *storyFieldNames[ST_LAST];

const int SP_NAME = 0;
const int SP_DUE  = 1;
const int SP_COMP = 2;
const int SP_DESC = 3;
const int SP_CAP  = 4;
const int SP_VEL  = 5;
const int SP_DAYS = 6;
const int SP_BURN = 7;
const int SP_LAST = 8;

extern const char *sprintFieldNames[SP_LAST];

#endif // FIELDS_H
