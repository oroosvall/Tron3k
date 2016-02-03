#include "TimeQuery.h"

#include <Windows.h>

#define PREC 1000000.0

struct TimeQuery
{
	bool terminated;
	std::string name;
	double PCFreq = 0.0;
	__int64 startTime;
	__int64 stopTime;
};

std::vector<TimeQuery> querry;

void resetQuery()
{
	querry.clear();
}


int startTimer(std::string name)
{
	if (querry.size() > 2000)
		querry.clear();
	TimeQuery t;
	t.terminated = false;
	t.name = name;
	//t.startTime = std::chrono::high_resolution_clock::now();

	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		printf("QueryPerformanceFrequency failed!\n");

	t.PCFreq = double(li.QuadPart) / PREC;

	QueryPerformanceCounter(&li);
	t.startTime= li.QuadPart;

	
	int index = querry.size();
	
	querry.push_back(t);

	return index;
}

void stopTimer(int index)
{
	
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);

	querry[index].stopTime = li.QuadPart;

	querry[index].terminated = true;
}

std::string getQueryResult()
{
	std::string result = "TimeQuery\n";

	for (unsigned int i = 0; i < querry.size(); i++)
	{
		TimeQuery t = querry[i];
		
		double time = (t.stopTime - t.startTime) / t.PCFreq;

		result += t.name + ": " + std::to_string(time) + " microsecond\n";
	}

	return result;
}

void terminateQuery()
{
	for (unsigned int i = 0; i < querry.size(); i++)
	{
		if (!querry[i].terminated)
		{
			LARGE_INTEGER li;
			QueryPerformanceCounter(&li);

			querry[i].stopTime = li.QuadPart;
			querry[i].terminated = true;
		}
	}
}
