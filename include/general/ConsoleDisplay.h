#ifndef ConsoleDisplay_h
#define ConsoleDisplay_h

class ConsoleDisplay{
	public:
		ConsoleDisplay(long total_entries=0);
		void Reset(long total_entries=0);
		void Update(long entry);
		void Finish();

	private:
		long totalEntries;
		bool isTTY;
		bool needNewLine;
		int lastLogBucket;
		double refreshInterval;
		double startTime;
		double lastRefreshTime;

		void PrintProgress(long entry);
		void FormatDuration(char *buf, int nbuf, double seconds) const;
		int GetTerminalWidth() const;
		int GetBarWidth(int nfixed=0) const;
		double GetNowTime() const;
};

#endif
