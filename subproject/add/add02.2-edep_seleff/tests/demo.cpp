#include <iostream>
using namespace std;
#include "../../../../third_party/splinefit3/splineFit3.h"
#include "TGraphErrors.h"

void demo()
{
    const int N = 6;
    double x[N]  = {3, 5, 10, 20, 50, 100};
    double y[N]  = {0.98, 0.99, 1.01, 1.00, 0.97, 0.96};
    double ey[N] = {0.01, 0.01, 0.01, 0.01, 0.015, 0.02};

    TGraphErrors *gr = new TGraphErrors(N, x, y, 0, ey);

    SplineFit *spFit = new SplineFit(4, "b1e1",
        SplineFit::LogX | SplineFit::ExtrapolateLB | SplineFit::ExtrapolateLE);

    spFit->SetRange(3.0, 100.0);
    spFit->BuildTF1("myspline");
    spFit->SetGraph(gr);

    spFit->SetNodesEvenLog(3.0, 100.0);
    spFit->SetXnodeLimits(0);

    spFit->doFit(3.0, 100.0, "FQ");
    spFit->CalConfInt(0.68);
    spFit->GetConfIntBand();

    gr->Draw("AP");
    spFit->gConfIntBand->Draw("PF same");
    spFit->f1SplineFit->Draw("same");
    gr->Draw("P same");
}
