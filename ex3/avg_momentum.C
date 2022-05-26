/*
 *
 * A replacement for Analyze.h and Analyze.C that uses TTreeReader
 * instead of a TSelector created by MakeSelector.
 
 * To compare using the TTreeReader approach versus the TSelector
 * approach, I've implemented Exercise 6 of my ROOT tutorial.
 
*/

#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

void avg_momentum()
{
	TFile			*myFile = TFile::Open("experiment.root");
	TTreeReader		myReader("tree1", myFile);

	double			sum_momentum = 0;
	double			n_momentum = 0;

// TTreeReaderValue<T> is defined for each branch being used

	TTreeReaderValue<Float_t> px = {myReader, "px"};
	TTreeReaderValue<Float_t> py = {myReader, "py"};
	TTreeReaderValue<Float_t> pz = {myReader, "pz"};
	
	while (myReader.Next())
	{
		sum_momentum = sum_momentum + TMath::Sqrt((*px)*(*px) + (*py)*(*py) + (*pz)*(*pz));
		++n_momentum;
	}

	cout << "Avg Momemtum:" << sum_momentum/n_momentum << endl;
}
