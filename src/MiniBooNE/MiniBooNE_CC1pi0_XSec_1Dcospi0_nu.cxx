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

#include "MiniBooNE_CC1pi0_XSec_1Dcospi0_nu.h"

// The constructor
MiniBooNE_CC1pi0_XSec_1Dcospi0_nu::MiniBooNE_CC1pi0_XSec_1Dcospi0_nu(std::string inputfile, FitWeight *rw, std::string type, std::string fakeDataFile){
  
  fName = "MiniBooNE_CC1pi0_XSec_1Dcospi0_nu";
  fPlotTitles = "; cos#theta_{#pi^{0}}; d#sigma/dcos#theta_{#pi^{0}} (cm^{2}/CH_{2})";
  EnuMin = 0.5;
  EnuMax = 2.;
  fIsDiag = false;
  fNormError = 0.107;
  Measurement1D::SetupMeasurement(inputfile, type, rw, fakeDataFile);

  this->SetDataValues(GeneralUtils::GetTopLevelDir()+"/data/MiniBooNE/CC1pi0/dxsecdcospi_edit.txt");
  this->SetCovarMatrixFromCorrText(GeneralUtils::GetTopLevelDir()+"/data/MiniBooNE/CC1pi0/dxsecdcospi_covar.txt", this->fNDataPointsX);
  //this->SetCovarMatrix(FitPar::GetDataBase()+"/MiniBooNE/cc1pi0/dxsecdcospi_covar.txt", this->fNDataPointsX -1 );
  this->SetupDefaultHist();

  // Calculates a flux averaged cross-section from (Evt("width")/Flux("width")) * 14.08/6.0
  this->fScaleFactor = this->fEventHist->Integral("width")*double(1E-38)/double(fNEvents)*(14.08)/TotalIntegratedFlux("width");
};


void MiniBooNE_CC1pi0_XSec_1Dcospi0_nu::FillEventVariables(FitEvent *event) {
  
  if (event->NumFSParticle(111) == 0 ||
      event->NumFSParticle(13) == 0)
    return;

  TLorentzVector Pnu  =event->GetNeutrinoIn()->fP;
  TLorentzVector Ppi0 = event->GetHMFSParticle(111)->fP;
  TLorentzVector Pmu  = event->GetHMFSParticle(13)->fP;

  double CosPi0 = cos(FitUtils::th(Pnu, Ppi0));

  fXVar = CosPi0;

  return;
};

//******************************************************************** 
bool MiniBooNE_CC1pi0_XSec_1Dcospi0_nu::isSignal(FitEvent *event) {
//******************************************************************** 
  return SignalDef::isCC1pi(event, 14, 111, EnuMin, EnuMax);
}
