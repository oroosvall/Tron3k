#ifndef TIMEQUERY_H
#define TIMEQUERY_H

#include <vector>
#include <string>

extern void resetQuery();

extern int startTimer(std::string name);

extern void stopTimer(int index);

extern std::string getQueryResult();

extern void terminateQuery();

#endif