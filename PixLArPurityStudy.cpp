//some standard C++ includes
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <fstream>

//some ROOT includes
#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TFile.h"
#include "TMath.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TCanvas.h"

//"art" includes (canvas, and gallery)
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindOne.h"

//"larsoft" object includes
#include "lardataobj/RecoBase/OpFlash.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/AnalysisBase/T0.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h"

using namespace art;
using namespace std;
using namespace std::chrono;

const int maxEvents = 1000000;
const int maxEventsPerRun = 1000000;
const int maxTicks = 3072;

int main(int argc, char **argv) {

  //gErrorIgnoreLevel = kError;

  InputTag rawdigit_tag { "daq" };

  vector<string> filenames;
  ifstream myfile(argv[1]);
  copy(istream_iterator<string>(myfile),istream_iterator<string>(),back_inserter(filenames));

  gStyle->SetOptStat(0);

  TFile f_output("output.root","RECREATE");
  
  TH1F *ChargeHistCol = new TH1F("ChargeHistCol",";Time Tick;Charge",maxTicks,-0.5,maxTicks-0.5);

  int totalEventNum = 0;
  int prevRun = -99999;
  int runEventCount = 0;
  int thisRunNum = -1;
  for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {

    auto const& rawdigit_handle = ev.getValidHandle<vector<raw::RawDigit>>(rawdigit_tag);
    auto const& allrawdigits_vec(*rawdigit_handle);

    if(rawdigit_handle->size() == 0) continue;

    thisRunNum = ev.eventAuxiliary().run();

    if ((thisRunNum != prevRun) && (runEventCount > 0)){
      prevRun = thisRunNum;
      runEventCount = 1;
    }
    else if ((thisRunNum == prevRun) && (runEventCount >= maxEventsPerRun)){
      continue;
    }
    else {
      prevRun = thisRunNum;
      runEventCount++;
    }

    totalEventNum++;
    if (totalEventNum > maxEvents) {continue;}

    for (size_t i_ar = 0, size_allrawdigits = rawdigit_handle->size(); i_ar != size_allrawdigits; ++i_ar) {
      int chanNum = allrawdigits_vec.at(i_ar).Channel();

      for (int k = 0; k < allrawdigits_vec.at(i_ar).Samples(); k++) {
        double ADCval = allrawdigits_vec.at(i_ar).ADC(k);
      
        if((chanNum <= 31) || ((chanNum >= 160) && (chanNum <= 327)) || (chanNum >= 440)) {
          ChargeHistCol->Fill(k,ADCval);
        }
      }
    }
  }

  ChargeHistCol->Scale(1.0/((double) TMath::Min(maxEvents,totalEventNum)));

  TCanvas *canv = new TCanvas("","");
  canv->cd();

  TF1 *TheMooneyFit = new TF1("TheMooneyFit","[0]*exp(-1*0.128*x/[1])+[2]");
  TheMooneyFit->SetParameter(0,40000.0);
  TheMooneyFit->SetParameter(1,0.128*500.0);
  TheMooneyFit->SetParameter(2,25000.0);

  TheMooneyFit->SetParLimits(1,10,5000.0);
  cout << endl;
  ChargeHistCol->Fit("TheMooneyFit","","",150,3000);

  if(thisRunNum == -1) {
    cout << std::fixed << std::setprecision(1) << endl << endl << "  Measured Electron Lifetime for Run -1 is 0.0 += 0.0 microseconds" << endl << endl;
  }
  else {
    cout << std::fixed << std::setprecision(1) << endl << endl << "  Measured Electron Lifetime for Run " << thisRunNum << " is " << TheMooneyFit->GetParameter(1) << " += " << TheMooneyFit->GetParError(1) << " microseconds" << endl << endl;
  }

  if(thisRunNum != -1) {
    canv->SaveAs(Form("fitResult_run%d.png",thisRunNum));
  }

  f_output.Write();
  f_output.Close();
}
