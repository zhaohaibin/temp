#include "time_helper.h"


namespace time_helper
{
	std::string time_split::short_format_string() const
	{
		string str = "%Y" + split_year + "%m" + split_month + "%d" + split_day;
		return str;
	}

	std::string time_split::long_format_string() const
	{
		string str = "%Y" + split_year + "%m" + split_month + "%d" + split_day + "%H" + split_hour + "%M" + split_minute + "%S" + split_second;
		return str;
	}

	std::string format_short_time( time_t calendar_time, const time_split& split)
	{
		char timeBuffer[BUFFER_LENGTH] = {0};
		string foramt_str = split.short_format_string();
		strftime(timeBuffer, BUFFER_LENGTH, foramt_str.c_str(), localtime(&calendar_time));
		return string(timeBuffer);
	}

	std::string format_long_time( time_t calendar_time, const time_split& split )
	{
		char timeBuffer[BUFFER_LENGTH] = {0};
		string format_str = split.long_format_string();
		strftime(timeBuffer, BUFFER_LENGTH, format_str.c_str(), localtime(&calendar_time));
		return string(timeBuffer);
	}
}