#ifndef ConsoleDisplay_h
#define ConsoleDisplay_h

class ConsoleDisplay{
	public:
		ConsoleDisplay(long total_entries=0, long print_step=1);
		void Reset(long total_entries, long print_step=1);
		void Update(long entry);
		void Finish();

	private:
		long totalEntries;
		long printStep;
		bool isTTY;
		bool needNewLine;

		void PrintProgress(long entry);
		int GetTerminalWidth() const;
		int GetBarWidth() const;
};

#endif
