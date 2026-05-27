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
		int lastFillCount;
		int lastLogBucket;
		double refreshInterval;
		double lastRefreshTime;

		void PrintProgress(long entry);
		int GetTerminalWidth() const;
		int GetBarWidth() const;
		double GetNowTime() const;
};

#endif
