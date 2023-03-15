#include <iostream>
#include <string>

#define DAYS_IN_400_YEARS ((24 * 366 + 76 * 365) * 4 + 1)

inline bool check_leap(int year)
{
	if ((year & 3) != 0)
		return false;
	if (year % 100 == 0 && year % 400 != 0)
		return false;
	std::cout << year << " is a leap year" << std:: endl;
	return true;
}

inline int num_days_in_month(int month, int year)
{
	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		return 28 + check_leap(year);
	}
	return -1; // false value
}

class Date_Ty
{
public:
	uint32_t year, month, day;
	Date_Ty(uint32_t year, uint32_t month, uint32_t day) : year(year), month(month), day(day){};
	bool operator==(const Date_Ty &d)
	{
		return year == d.year && month == d.month && day == d.day;
	}

	bool operator<(const Date_Ty &d)
	{
		if (year < d.year)
			return true;
		else if (year > d.year)
			return false;

		if (month < d.month)
			return true;
		else if (month > d.month)
			return false;

		if (day < d.day)
			return true;
		// else if (day > d.day)
		return false;
	}

	bool operator>(const Date_Ty &d)
	{
		if (year > d.year)
			return true;
		else if (year < d.year)
			return false;

		if (month > d.month)
			return true;
		else if (month < d.month)
			return false;

		if (day > d.day)
			return true;
		/// else if (day < d.day)
		return false;
	}

	void operator++()
	{
		this->day++;
		if (this->day == num_days_in_month(this->month, this->year) + 1)
		{
			this->day = 1;
			this->month++;
		}
		if (this->month == 13)
		{
			this->month = 1;
			this->year++;
		}
	}
};

Date_Ty parse_date_string(std::string date)
{
	uint32_t year, month, day;
	std::string tok;
	uint32_t pos = 0;

	tok = date.substr(pos, 4); // year string
	pos += 4;
	year = stoi(tok);
	pos++; // eat the seperator

	tok = date.substr(pos, 2);
	pos += 2;
	month = stoi(tok);
	pos++;

	tok = date.substr(pos, 2);
	// pos+=2;
	day = stoi(tok);
	// pos++;
	return Date_Ty{year, month, day};
}

inline bool check_leap(Date_Ty date)
{
	return check_leap(date.year);
}

int advance_one_year(Date_Ty &date)
{
	bool add_one = false;
	if (date == Date_Ty(date.year, 02, 29) || date < Date_Ty(date.year, 02, 29))
		add_one = check_leap(date.year);
	else
		add_one = check_leap(date.year + 1);

	date.year++;
	return 365 + add_one; // returns 366 if 29th feb lies between the dates
}


int advance_one_day(Date_Ty &date)
{
	date.day++;
	if (date.day == num_days_in_month(date.month, date.year) + 1)
	{
		date.day = 1;
		date.month++;
	}
	if (date.month == 13)
	{
		date.month = 1;
		date.year++;
	}
	return 1;
}

int diff_num_days(Date_Ty beginDate, Date_Ty endDate)
{
	int diffNumDays = 0;

	diffNumDays += DAYS_IN_400_YEARS * ((endDate.year - beginDate.year) / 400);
	beginDate.year = endDate.year - ((endDate.year - beginDate.year) % 400);

	while (beginDate.year < endDate.year - 1)
		diffNumDays += advance_one_year(beginDate);
	while (!(beginDate == endDate))
	{
		++beginDate;
		++diffNumDays;
	}

	return diffNumDays;
}

int diff_num_days(std::string begin, std::string end)
{
	Date_Ty beginDate = parse_date_string(begin);
	Date_Ty endDate = parse_date_string(end);
	return diff_num_days(beginDate, endDate);
}

int main()
{
	Date_Ty df = parse_date_string("2003-03-14");
	Date_Ty de = parse_date_string("2125-04-13");

	std::cout << diff_num_days(df, de) << std::endl;
	return 0;
}
