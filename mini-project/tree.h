//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jul 30 17:04:40 2021 by ROOT version 6.18/05
// from TTree tree/a Tree with data from the csv file
// found on file: tree2.root
//////////////////////////////////////////////////////////

#ifndef tree_h
#define tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class tree
{
	public :
		TTree	*fChain;   //!pointer to the analyzed TTree or TChain
		Int_t	fCurrent; //!current Tree number in a TChain

		// Fixed size dimensions of array or collections stored in the TTree if any.

		// Declaration of leaf types
		Int_t		sig[128];
		Int_t		sig2[32];
		Float_t		off;
		Float_t		A1;
		Float_t		A2;
		Float_t		t1;
		Float_t		t2;
		// outputs from signal processing
		Float_t		start_time[4];
		Float_t		stop_time[4];
		Float_t		threshold = 400;

		Int_t	FADC_pulse_sum[4];
//		Int_t	FADC_pulse_pedestal;
//		Int_t	FADC_pulse_pedestal_good;
		Int_t	FADC_time_coarse[4];
		Int_t	FADC_time_fine[4];

		// List of branches
		TBranch		*b_si;   //!
		TBranch		*b_off;   //!
		TBranch		*b_A1;   //!
		TBranch		*b_A2;   //!
		TBranch		*b_t1;   //!
		TBranch		*b_t2;   //!

		tree(TTree *tree=0);
		virtual ~tree();
		virtual Int_t		Cut(Long64_t entry);
		virtual Int_t		GetEntry(Long64_t entry);
		virtual Long64_t	LoadTree(Long64_t entry);
		virtual void		Init(TTree *tree);
		virtual void		Loop(Int_t pulse_display);
		virtual Bool_t		Notify();
		virtual void		Show(Long64_t entry = -1);
		virtual Float_t		*start(Int_t sig[]);
		virtual Float_t		*stop(Int_t sig[]);
		virtual void		pulseFADC();
};

#endif

#ifdef tree_cxx
tree::tree(TTree *tree) : fChain(0) 
{
	// if parameter tree is not specified (or zero), connect the file
	// used to generate this class and read the Tree.
	if (tree == 0)
	{
		TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("tree2.root");
		if (!f || !f->IsOpen())
			f = new TFile("tree2.root");
		f->GetObject("tree",tree);
	}
	Init(tree);
}

tree::~tree()
{
	if (!fChain)
		return;
	delete fChain->GetCurrentFile();
}

Int_t tree::GetEntry(Long64_t entry)
{
	// Read contents of entry.
	if (!fChain)
		return 0;
	return fChain->GetEntry(entry);
}

Long64_t tree::LoadTree(Long64_t entry)
{
	// Set the environment to read one entry
	if (!fChain)
		return -5;
	Long64_t centry = fChain->LoadTree(entry);
	if (centry < 0)
		return centry;
	if (fChain->GetTreeNumber() != fCurrent)
	{
		fCurrent = fChain->GetTreeNumber();
		Notify();
	}
	return centry;
}

void tree::Init(TTree *tree)
{
	// The Init() function is called when the selector needs to initialize
	// a new tree or chain. Typically here the branch addresses and branch
	// pointers of the tree will be set.
	// It is normally not necessary to make changes to the generated
	// code, but the routine can be extended by the user if needed.
	// Init() will be called many times when running on PROOF
	// (once per file to be processed).

	// Set branch addresses and branch pointers
	if (!tree)
		return;
	fChain = tree;
	fCurrent = -1;
	fChain->SetMakeClass(1);

	fChain->SetBranchAddress("sig", sig, &b_si);
	fChain->SetBranchAddress("off", &off, &b_off);
	fChain->SetBranchAddress("A1", &A1, &b_A1);
	fChain->SetBranchAddress("A2", &A2, &b_A2);
	fChain->SetBranchAddress("t1", &t1, &b_t1);
	fChain->SetBranchAddress("t2", &t2, &b_t2);
	Notify();
}

Bool_t tree::Notify()
{
	// The Notify() function is called when a new file is opened. This
	// can be either for a new TTree in a TChain or when when a new TTree
	// is started when using PROOF. It is normally not necessary to make changes
	// to the generated code, but the routine can be extended by the
	// user if needed. The return value is currently not used.
	return kTRUE;
}

void tree::Show(Long64_t entry)
{
	// Print contents of entry.
	// If entry is not specified, print current entry
	if (!fChain)
		return;
	fChain->Show(entry);
}

Int_t tree::Cut(Long64_t entry)
{
	// This function may be called from Loop.
	// returns  1 if entry is accepted.
	// returns -1 otherwise.
	return 1;
}

#endif // #ifdef tree_cxx
