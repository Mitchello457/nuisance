// Copyright 2016 L. Pickering, P Stowell, R. Terri, C. Wilkinson, C. Wret

/*******************************************************************************
*    This file is part of NUISANCE.
*
*    NUISANCE is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    NUISANCE is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with NUISANCE.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#include "MiniBooNE_CC1pip_XSec_1DTpi_nu.h"

// The constructor
MiniBooNE_CC1pip_XSec_1DTpi_nu::MiniBooNE_CC1pip_XSec_1DTpi_nu(std::string inputfile, FitWeight *rw, std::string type, std::string fakeDataFile){

  fName = "MiniBooNE_CC1pip_XSec_1DTpi_nu";
  fPlotTitles = "; T_{#pi} (MeV); d#sigma/dT_{#pi^{+}}} (cm^{2}/MeV/CH_{2})";
  EnuMin = 0.5;
  EnuMax = 2.;
  fIsDiag = true;
  fNormError = 0.107;
  Measurement1D::SetupMeasurement(inputfile, type, rw, fakeDataFile);
  
  this->SetDataValues(GeneralUtils::GetTopLevelDir()+"/data/MiniBooNE/CC1pip/ccpipXSec_KEpi.txt");
  this->SetupDefaultHist();

  fFullCovar = StatUtils::MakeDiagonalCovarMatrix(fDataHist);
  covar = StatUtils::GetInvert(fFullCovar);
  //StatUtils::ForceNormIntoCovar(this->covar, this->fDataHist, this->fNormError);

  this->fScaleFactor = this->fEventHist->Integral("width")*double(1E-38)/double(fNEvents)*(14.08)/TotalIntegratedFlux("width");
};


//********************************************************************   
void MiniBooNE_CC1pip_XSec_1DTpi_nu::FillEventVariables(FitEvent *event) {
//********************************************************************   

  if (event->NumFSParticle(211) == 0 ||
      event->NumFSParticle(13) == 0)
    return;

  TLorentzVector Pnu  = event->GetNeutrinoIn()->fP;
  TLorentzVector Ppip = event->GetHMFSParticle(211)->fP;
  TLorentzVector Pmu  = event->GetHMFSParticle(13)->fP;

  // No W cut on MiniBooNE data from publication
  // WARNING: DRAGONS LAY HERE! Mike Wilking's thesis might not have this. Beware that the publication says W < 1.35 GeV, but this is "efficiency corrected", i.e. FILLED WITH MONTE-CARLO!!!!!!!! AAAAAH
  //double hadMass = FitUtils::Wrec(Pnu, Pmu, Ppip);

  double Tpi = FitUtils::T(Ppip)*1000.;

  fXVar = Tpi;

  return;
};


bool MiniBooNE_CC1pip_XSec_1DTpi_nu::isSignal(FitEvent *event) {
  return SignalDef::isCC1pi(event, 14, 211, EnuMin, EnuMax);
}
