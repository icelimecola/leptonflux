#include "ConsoleDisplay.h"

#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>

#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;

ConsoleDisplay::ConsoleDisplay(long total_entries, long print_step){
	//====init
	totalEntries = 0;
	printStep = 1;
	isTTY = isatty(STDOUT_FILENO);
	needNewLine = 0;
	//====reset
	Reset(total_entries, print_step);
}

void ConsoleDisplay::Reset(long total_entries, long print_step){
	totalEntries = total_entries;
	printStep = print_step;
	if( printStep<1 ) printStep = 1;
	needNewLine = 0;
}

void ConsoleDisplay::Update(long entry){
	if( totalEntries<=0 ) return;
	if( entry<0 || entry>=totalEntries ) return;
	if( entry%printStep!=0 && entry!=totalEntries-1 ) return;
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

	time_t now = time(0);
	struct tm *t = localtime(&now);
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
