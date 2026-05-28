#include "ConsoleDisplay.h"

#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>

#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

ConsoleDisplay::ConsoleDisplay(long total_entries){
	//====init
	totalEntries = 0;
	isTTY = isatty(STDOUT_FILENO);
	needNewLine = 0;
	lastLogBucket = -1;
	refreshInterval = 0.10;
	startTime = 0;
	lastRefreshTime = 0;
	//====reset
	Reset(total_entries);
}

void ConsoleDisplay::Reset(long total_entries){
	totalEntries = total_entries;
	needNewLine = 0;
	lastLogBucket = -1;
	startTime = GetNowTime();
	lastRefreshTime = 0;
}

void ConsoleDisplay::Update(long entry){
	if( totalEntries<=0 ) return;
	if( entry<0 || entry>=totalEntries ) return;

	double progress = 100.0*(entry+1)/totalEntries;
	bool is_last_entry = (entry==totalEntries-1);

	if( !isTTY ){
		int log_bucket = int(progress)/10;
		if( !is_last_entry && log_bucket==lastLogBucket ) return;
		PrintProgress(entry);
		return;
	}

	double now = GetNowTime();
	if( !is_last_entry && lastRefreshTime>0 && now-lastRefreshTime<refreshInterval ) return;
	PrintProgress(entry);
}

void ConsoleDisplay::Finish(){
	if( needNewLine ){
		cout << endl;
		needNewLine = 0;
	}
}

void ConsoleDisplay::PrintProgress(long entry){
	double progress = 100.0*(entry+1)/totalEntries;
	double now = GetNowTime();
	time_t now_time = time(0);
	struct tm *t = localtime(&now_time);
	char timebuf[16] = {0};
	char progressbuf[16] = {0};
	char etabuf[32] = {0};
	if( t!=0 )
		snprintf(timebuf, sizeof(timebuf), "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
	else
		snprintf(timebuf, sizeof(timebuf), "--:--:--");
	snprintf(progressbuf, sizeof(progressbuf), "%6.2f", progress);

	long ndone = entry+1;
	double eta = -1;
	if( ndone>=totalEntries )
		eta = 0;
	else if( startTime>0 && now>startTime && ndone>0 )
		eta = (totalEntries-ndone)*(now-startTime)/ndone;
	FormatDuration(etabuf, sizeof(etabuf), eta);

	if( !isTTY ){
		cout << timebuf
			 << " Processing "
			 << progressbuf << "% "
			 << entry+1 << " / " << totalEntries
			 << " ETA " << etabuf << endl;
		lastLogBucket = int(progress)/10;
		lastRefreshTime = now;
		return;
	}

	string prefix = string(timebuf) + " Processing [";
	string suffix = "] ";
	suffix += progressbuf;
	suffix += "% ";
	suffix += to_string(entry+1);
	suffix += " / ";
	suffix += to_string(totalEntries);
	suffix += " ETA ";
	suffix += etabuf;

	int nbar = GetBarWidth(int(prefix.size()+suffix.size()));
	int nfill = int(progress/100.0*nbar);
	if( nfill<0 ) nfill=0;
	if( nfill>nbar ) nfill=nbar;
	string bar(nfill, '=');
	bar += string(nbar-nfill, ' ');

	cout << "\r"
		 << prefix << bar << suffix
		 << "\033[K" << flush;

	needNewLine = 1;
	lastRefreshTime = now;
	if( entry==totalEntries-1 ) Finish();
}

void ConsoleDisplay::FormatDuration(char *buf, int nbuf, double seconds) const{
	if( nbuf<=0 ) return;
	if( seconds<0 ){
		snprintf(buf, nbuf, "--:--:--");
		return;
	}

	long sec = long(seconds+0.5);
	long hour = sec/3600;
	long min = (sec%3600)/60;
	sec %= 60;

	if( hour<100 )
		snprintf(buf, nbuf, "%02ld:%02ld:%02ld", hour, min, sec);
	else
		snprintf(buf, nbuf, "%ld:%02ld:%02ld", hour, min, sec);
}

int ConsoleDisplay::GetTerminalWidth() const{
	struct winsize ws;
	if( isTTY && ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws)==0 && ws.ws_col>0 )
		return ws.ws_col;
	return 120;
}

int ConsoleDisplay::GetBarWidth(int nfixed) const{
	int width = GetTerminalWidth();
	int nbar = width - nfixed;
	if( nbar<5 ) nbar=5;
	if( nbar>80 ) nbar=80;
	return nbar;
}

double ConsoleDisplay::GetNowTime() const{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec + 1.0e-6*tv.tv_usec;
}
