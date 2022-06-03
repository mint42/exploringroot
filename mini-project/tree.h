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

class tree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           sig[128];
   Int_t           sig2[32];
   Float_t         off;
   Float_t         A1;
   Float_t         A2;
   Float_t         t1;
   Float_t         t2;
   // outputs from signal processing
   Float_t         start_time[4];
   Float_t         stop_time[4];
   Float_t         threshold = 400;

   Int_t         FADC_pulse_sum[4];
   Int_t         FADC_pulse_pedestal;
   Int_t         FADC_pulse_pedestal_good;
   Int_t         FADC_time_coarse[4];
   Int_t         FADC_time_fine[4];

   // List of branches
   TBranch        *b_si;   //!
   TBranch        *b_off;   //!
   TBranch        *b_A1;   //!
   TBranch        *b_A2;   //!
   TBranch        *b_t1;   //!
   TBranch        *b_t2;   //!

   tree(TTree *tree=0);
   virtual ~tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(Int_t pulse_display);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual Float_t* start(Int_t sig[]);
   virtual Float_t* stop(Int_t sig[]);
   virtual void     pulseFADC(Int_t NSA, Int_t NSB, Int_t PTW_min, Int_t PTW_max, Int_t TET, Int_t NSAT);
};

#endif

#ifdef tree_cxx
tree::tree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("tree2.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("tree2.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

tree::~tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
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
   if (!tree) return;
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
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}


// this function returns the start array
Float_t* tree::start(Int_t sig[]) {
  //cout<<" jentry= "<<jentry<<endl;
  //cout<<" A1="<<A1<<"  t1="<<t1<<" A2="<<A2<<" t2="<<t2<<endl;
  Int_t narray = 128;
  //looking for up to 4 pulses
  const Int_t maxpulse=4;

  for(Int_t i = 0; i < maxpulse; i++) {
    start_time[i] = narray;
    stop_time[i] = 0;
  }
  Int_t npulse = 0;
	
  for (Int_t i = 0; i < narray; i++) {

    if (-sig[i] >= threshold && start_time[npulse] == narray)	     
      start_time[npulse] = i;	
    if(-sig[i] <= threshold && start_time[npulse] != narray && stop_time[npulse] == 0) {
      stop_time[npulse] = i;
      npulse += 1;
      if(npulse == maxpulse) i = narray + 1;
    }

  }

  return start_time;
}


// this function returns the stop array
Float_t* tree::stop(Int_t sig[]) {
  //cout<<" jentry= "<<jentry<<endl;
  //cout<<" A1="<<A1<<"  t1="<<t1<<" A2="<<A2<<" t2="<<t2<<endl;
  Int_t narray = 128;
  //looking for up to 4 pulses
  const Int_t maxpulse=4;

  for(Int_t i = 0; i < maxpulse; i++) {
    start_time[i] = narray;
    stop_time[i] = 0;
  }
  Int_t npulse = 0;
	
  for (Int_t i = 0; i < narray; i++) {

    if (-sig[i] >= threshold && start_time[npulse] == narray)	     
      start_time[npulse] = i;	
    if(-sig[i] <= threshold && start_time[npulse] != narray && stop_time[npulse] == 0) {
      stop_time[npulse] = i;
      npulse += 1;
      if(npulse == maxpulse) i = narray + 1;
    }

  }

  return stop_time;
}



void tree::pulseFADC(Int_t NSA, Int_t NSB, Int_t PTW_min, Int_t PTW_max, Int_t TET, Int_t NSAT)
{
  // NSB number of sample before the first sample cross the threshold
  // NSA number of sample after the first sample cross the threshold
  // PTWMIn and MAX triger window in which the pulses are searched
  //TET programmable theshold
  // NSAT number of consecutive sample above thrshold for a valid signal

  Bool_t bad_input=kFALSE;
  Bool_t verbose=kTRUE;
 
  Int_t narray = 128;
  /*
  if(PTW_min<NSB) {
    cout<<"in tree::pulseFADC PTW_min="<<PTW_min<<" is msaller than NSB="<<NSB<<"this is bad \n";
    bad_input=kTRUE;
  }
  if(PTW_max>(narray-NSA))
    {
      cout<<"in tree::pulseFADC PTW_max="<<PTW_min<<" is larger than narray-NSA="<<narray-NSA<<"  this is bad \n";
      bad_input=kTRUE;
  }
  if(NSA<NSB)
    {
      cout<<"in tree::pulseFADC NSA="<<NSA<<" is smaller than NSB="<<NSB<<"  this is bad \n";
      bad_input=kTRUE;
      // this is due to the comment on page 5 a- iv: not sure it applies to us
  }
  */
  if(!bad_input)
    {
      if(verbose)
	{
	  for(int i=0;i<narray;i++)
	    cout<<i<<":"<<sig[i]<<" , ";
	  cout<<endl;
	}
      
      //looking for up to 4 pulses
      const Int_t maxpulse=4;
      Int_t npulsefound=0;
      Int_t start_time[4];
      Int_t stop_time[4];
      for(Int_t i = 0; i < maxpulse; i++) {
	start_time[i] = narray;
	stop_time[i] = 0;
      }
      Int_t npulse = 0;
      
      for (Int_t i = PTW_min; i < PTW_max; i++)
	{
	  // find when the signal goes over threshold
	  if (-sig[i] >= TET && start_time[npulse] == narray)	     
	    start_time[npulse] = i;
	  // find the the signal goes back under threshold
	  if(-sig[i] <= TET && start_time[npulse] != narray && stop_time[npulse] == 0)
	    {
	      stop_time[npulse] = i;
	      if(verbose)
		cout<<"npulse ="<<npulse<<" pulse start="<<start_time[npulse]<<" stop time ="<<stop_time[npulse];
	      // check if the signal was over theshold for long enough
	      Int_t duration=stop_time[npulse]-start_time[npulse]+1;
	      if(duration>NSAT)
		{
		  if(verbose) cout<<" pulse long enough\n";
		  //this is pulse was above threshold for long enough
		  npulse += 1;
		  npulsefound+=1;
		}
	      else
		{
		  if(verbose) cout<<" pulse too short\n";
		  start_time[npulse]= narray;
		  stop_time[npulse]=0;
		}
	      if(npulse == maxpulse) i = narray + 1;
	    }
	}
      

      
      // finding a pedestal at the begining of the trace
      // if the first pulse started whitin the window of the pedestal, signal pedestal is bad with FADC_pedestal_good.
      FADC_pulse_pedestal_good=kTRUE;
      FADC_pulse_pedestal=0;
      for (int i=0;i<4;i++)
	FADC_pulse_pedestal+=sig[i];
      if(start_time[0]<NSA+NSB)
	FADC_pulse_pedestal_good=kFALSE;
      if(verbose)
	{
	  cout<< "Pedestal evaluation \n";
	  cout<<"pedestal ="<<FADC_pulse_pedestal<<"\n";
	  if(	FADC_pulse_pedestal_good) cout<<"good pedestal\n";
	  else cout<<"bad pedestal\n";
	}
      
	
      /// pulse integral
      if(verbose)
	cout<< "Pulse integral \n";
      for(int i=0;i<npulsefound;i++)
	{
	  if(verbose) cout<<"pulse #"<<i<<"\n counting bins";
	  FADC_pulse_sum[i]=0;
	  Int_t countstart=std::min(start_time[i]-NSA,0);
	  Int_t countstop=std::min(start_time[i]+NSB,narray);
	  for(int j=countstart;j<countstop;j++)
	    {
	      if(verbose) cout<<j<<":"<< sig[j]<<" , ";
	      FADC_pulse_sum[i]+=sig[j];
	    }
	  if(verbose) cout<<"integral ="<<	FADC_pulse_sum[i]<<"\n";
	}
      
      
      
      ///pulse timing
      if(verbose)
	cout<< "Pulse integral \n";
      for(int i=0;i<maxpulse;i++)
	FADC_time_coarse[i]=-1;
      Int_t vpeak=0;
      Int_t ipeak=0;
      for(int i=0;i<npulsefound;i++)
	{
	  if(verbose) cout<<"pulse #"<<i<<"\n counting bins";
	  for (int j=start_time[i];j<stop_time[i]-1;j++)
	    {
	      if(sig[j]>sig[j+1])
		{
		  vpeak=(sig[j]+FADC_pulse_pedestal)/2.;
		  ipeak=j;
		  if(verbose) cout<<"vpeak ="<<vpeak<<" bin="<<j<<endl;		  
		}
	    }
	  // now going backward to find where the signal goes halfway to the peak
	  for (int j=ipeak; j>start_time[i]-NSA;j--)
	    {
	      if(sig[j]<vpeak)
		{
		  FADC_time_coarse[i]=j;
		  //	      fADC_time_fine[i]= to compute
		  j=start_time[i]-NSA-1;
		  if(verbose) cout<<"coarse time is "<<j<<" with signal value ="<<sig[j]<<endl;
		}
	    }
	  
	}	  
	  
	  
    }
      
      return;
}





#endif // #ifdef tree_cxx
