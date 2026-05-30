_Pragma("once")

#include <vector>

#include "TDirectory.h"
#include "TString.h"

class RootSaveDir{
    public:
        static void Enter(TString dirname){
            TDirectory *save_dir = gDirectory;
            DirStack().push_back(save_dir);
            if(save_dir==0) return;

            TDirectory *work_dir = save_dir->GetDirectory(dirname);
            if(work_dir==0) work_dir = save_dir->mkdir(dirname);
            if(work_dir!=0) work_dir->cd();
        }

        static void Exit(){
            if(DirStack().empty()) return;
            TDirectory *save_dir = DirStack().back();
            DirStack().pop_back();
            if(save_dir!=0) save_dir->cd();
        }

    private:
        static std::vector<TDirectory*>& DirStack(){
            static std::vector<TDirectory*> dir_stack;
            return dir_stack;
        }
};
