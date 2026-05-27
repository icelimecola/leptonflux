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
	lastFillCount = -1;
	refreshInterval = 0.10;
	lastRefreshTime = 0;
	//====reset
	Reset(total_entries);
}

void ConsoleDisplay::Reset(long total_entries){
	totalEntries = total_entries;
	needNewLine = 0;
	lastFillCount = -1;
	lastRefreshTime = 0;
}

void ConsoleDisplay::Update(long entry){
	if( totalEntries<=0 ) return;
	if( entry<0 || entry>=totalEntries ) return;

	int nbar = GetBarWidth();
	double progress = 100.0*(entry+1)/totalEntries;
	int nfill = int(progress/100.0*nbar);
	if( nfill<0 ) nfill=0;
	if( nfill>nbar ) nfill=nbar;

	double now = GetNowTime();
	bool is_last_entry = (entry==totalEntries-1);
	bool time_ready = (lastRefreshTime<=0 || now-lastRefreshTime>=refreshInterval);
	bool bar_changed = (nfill!=lastFillCount);

	if( !is_last_entry && !bar_changed && !time_ready ) return;
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
	int nbar = GetBarWidth();
	int nfill = int(progress/100.0*nbar);
	if( nfill<0 ) nfill=0;
	if( nfill>nbar ) nfill=nbar;

	string bar(nfill, '=');
	bar += string(nbar-nfill, ' ');

	time_t now_time = time(0);
	struct tm *t = localtime(&now_time);
	char timebuf[16] = {0};
	char progressbuf[16] = {0};
	if( t!=0 )
		snprintf(timebuf, sizeof(timebuf), "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
	else
		snprintf(timebuf, sizeof(timebuf), "--:--:--");
	snprintf(progressbuf, sizeof(progressbuf), "%6.2f", progress);

	if( isTTY )
		cout << "\r";
	cout << timebuf
		 << " Processing [" << bar << "] "
		 << progressbuf << "% "
		 << entry+1 << " / " << totalEntries << flush;

	needNewLine = 1;
	lastFillCount = nfill;
	lastRefreshTime = GetNowTime();
	if( entry==totalEntries-1 ) Finish();
}

int ConsoleDisplay::GetTerminalWidth() const{
	struct winsize ws;
	if( isTTY && ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws)==0 && ws.ws_col>0 )
		return ws.ws_col;
	return 120;
}

int ConsoleDisplay::GetBarWidth() const{
	int width = GetTerminalWidth();
	int nbar = width - 36;
	if( nbar<10 ) nbar=10;
	if( nbar>80 ) nbar=80;
	return nbar;
}

double ConsoleDisplay::GetNowTime() const{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec + 1.0e-6*tv.tv_usec;
}
