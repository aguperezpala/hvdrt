/*
 * Timestamp.h
 *
 *  Created on: 12/09/2011
 *      Author: agustin
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

// TODO: this will work only in linux
#include <sys/time.h>

#include <list>
#include <string>


class Timestamp {
public:
	typedef std::pair<double, std::string>	TimestampEntry;

	static double getNowTimeStamp(void) const
	{
		struct timeval now;
		if(gettimeofday(&now, 0) < 0)
			return 0.0;

		return nowDouble = static_cast<double>(now.tv_sec + (
									static_cast<double>(now.tv_usec)/1000000.0));
	}


public:
	// Creates the timestamp now
	Timestamp()
	{
		gettimeofday(&mTimestamp, 0);
	}

	double getFirstTimestamp(void) const
	{
		return static_cast<double>(mTimestamp.tv_sec + (
				static_cast<double>(mTimestamp.tv_usec)/1000000.0));
	}

	// Returns the difference from the first timestamp and now
	double getDiffTimestamp(void) const
	{
		struct timeval now;

		if(gettimeofday(&now, 0) < 0)
			return 0.0;

		double nowDouble = static_cast<double>(now.tv_sec + (
							static_cast<double>(now.tv_usec)/1000000.0));

		double first = getFirstTimestamp();

		return now - first;
	}

	// Returns the timetamp entries
	const std::list<TimestampEntry> &getEntries(void) const {return mEntries;}

	// Add a new Timestamp
	void addTimestamp(const std::string &info)
	{
		TimestampEntry entry;

		entry.first = Timestamp::getNowTimeStamp();
		entry.second = info;

		mEntries.push_back(entry);
	}


private:
	struct timeval 				mTimestamp;
	std::list<TimestampEntry>	mEntries;
};


#endif /* TIMESTAMP_H_ */
