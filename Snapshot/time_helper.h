#pragma once

#include <ctime>
#include <string>
using namespace std;

namespace time_helper
{
#define BUFFER_LENGTH	64
	struct time_split
	{
		string split_year;
		string split_month;
		string split_day;
		string split_hour;
		string split_minute;
		string split_second;

		string short_format_string() const;
		string long_format_string() const;
	};
	string format_short_time(time_t calendar_time, const time_split& sqlit);
	string format_long_time(time_t calendar_time, const time_split& sqlit);
}
