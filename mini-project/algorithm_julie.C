#define tree_cxx 
#include "tree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <vector>

/**
 * @display - displays the plots given the parameter of the loop function
 * @param: pulse_display: value taken from the loop function, determines which plots are outputted
 * @param: no_pulse_display- type bool value for the no pulse output
 * @param: one_pluse_display- type bool value for the one pulse output
 * @param: two_pulse_display- type bool value for the two pulse output
 *
 */
void display(Int_t &pulse_display, Bool_t &no_pulse_display, Bool_t &one_pulse_display, Bool_t &two_pulse_display);

void tree::Loop(Int_t pulse_display = -1)
{
//   In a ROOT session, you can do:
//      root> .L tree.C
//      root> tree t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries and prints all multigraphs (i.e. no pulse, one pulse, two pulse)
//      root> t.Loop(0);      // Prints no pulse multigraph
//      root> t.Loop(1);      // Prints one pulse multigraph
//      root> t.Loop(2);      // Prints two pulse multigraph
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Double_t threshold =50; //JR I'm adding this is but I am not quite sure what threshold is used for the pulse analysis
   
   Long64_t nentries = fChain->GetEntriesFast();

   const Int_t number_entries = 128;
   Double_t x[number_entries], y[number_entries];
   for (Int_t i = 0; i < number_entries; i++)
     x[i] = i * 1.0;   
   Double_t x1,x2,x3,x4,y1,y2,y3,y4;
   
   Double_t t1_arr1[1];
   Double_t A1_arr1[1];

   Double_t t1_arr2[1];
   Double_t A1_arr2[1];
   Double_t t2_arr2[1];
   Double_t A2_arr2[1];
   
   Double_t min_A1 = 50000;
   Double_t max_A1 = 0;
   Double_t max_A2 = 0;

   Double_t tmin;
   Double_t tmax;
  
   // const Int_t cluster_entries = (number_entries / 4); 
   // Double_t sig_divide_four[cluster_entries]; // new array to hold the clustered values of array "sig" (32 length)

   // counter
   const Int_t ngraph = 9;
   Int_t noPulse_counter = 0; // counter for no pulse
   Int_t onePulse_counter = 0; // counter for one pulse
   Int_t twoPulse_counter = 0; // counter for two pulses

   // test graph and multigraph
   TGraph *graph_average_four[ngraph];

   // declaration of multigraph, no pulse plots and offset line 
   TMultiGraph *multigraph_nopulse[ngraph];
   TGraph *graph_nopulse[ngraph];
   TGraph *noPulse_offset_line[ngraph];

   // declaration of multigraph, one pulse plots, point of pulse and offset line
   TMultiGraph *multigraph_onepulse[ngraph];
   TGraph *graph_onepulse[ngraph];
   TGraph *graph_onepulse_dot[ngraph];
   TGraph *onePulse_threshold_line[ngraph];
   
   // declaration of multigraph, two pulse plots, points of pulses and offset line 
   TMultiGraph *multigraph_twopulse[ngraph];
   TGraph *graph_twopulse[ngraph];
   TGraph *graph_twopulse_dot1[ngraph];
   TGraph *graph_twopulse_dot2[ngraph];
   TGraph *twoPulse_offset_line[ngraph];
   
   Bool_t no_pulse_display=kFALSE; // corresponds to no signal
   Bool_t one_pulse_display=kFALSE; // corresponds to 1 signal
   Bool_t two_pulse_display=kFALSE; // corresponds to 2 signals

   display(pulse_display, no_pulse_display, one_pulse_display, two_pulse_display);


   Long64_t nbytes = 0, nb = 0;
   //Int_t counter = 0;
   // this loop over all the events in the tree
   for (Long64_t jentry = 0; jentry < nentries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

     
      int sum = 0;
      int index = 0;

      for (Int_t i = 0; i < number_entries; i++){
      	y[i] = sig[i];
      }

      if(jentry<10)
	pulseFADC(8,4, 10, 110, 50, 10);




     
      if (A1==0 && noPulse_counter < ngraph && jentry > 1000 && no_pulse_display)
	{	 
	  TString title_nopulse = Form("Event # %lld", jentry); // title form
	  
	  multigraph_nopulse[noPulse_counter] = new TMultiGraph(title_nopulse, title_nopulse);// initialize multigraph
	  graph_nopulse[noPulse_counter] = new TGraph(number_entries, x, y); // initialize plot of no pulses (y)
	  graph_nopulse[noPulse_counter] -> SetLineColor(2);
	  multigraph_nopulse[noPulse_counter] -> Add(graph_nopulse[noPulse_counter]); // addition of plot to multigraph

	  Double_t plotX[2] = {0.0, 128.0}; // array holding x-coordinates
	  Double_t plotY[2] = {off, off}; // array holding y-coordinates
	  noPulse_offset_line[noPulse_counter] = new TGraph(2, plotX, plotY); // initialize new graph for offset line
	  multigraph_nopulse[noPulse_counter] -> Add(noPulse_offset_line[noPulse_counter], "L"); // addition of offset line to multigraph

	  multigraph_nopulse[noPulse_counter] -> SetMinimum(-100); // lower bound of plot
	  multigraph_nopulse[noPulse_counter] -> SetMaximum(50); // upper bound of plot
	  
	  noPulse_counter += 1; // increment counter by 1 until 9 is hit
	}
      
      if (onePulse_counter < ngraph && jentry > 1000 && one_pulse_display)
	{
	  TString title_onepulse=Form("Event # %lld",jentry); // title form
	  
	  multigraph_onepulse[onePulse_counter] = new TMultiGraph(title_onepulse, title_onepulse); // initialize multigraph

	  Double_t plotX[2] = {0.0, 128.0}; // x-coordinates
	  Double_t plotY[2] = {-threshold, -threshold}; // y-coordinates
	  onePulse_threshold_line[onePulse_counter] = new TGraph(2, plotX, plotY); 
	 
	  graph_onepulse[onePulse_counter] = new TGraph(number_entries, x, y); // initialize plot of no pulses (y)
	  graph_onepulse[onePulse_counter] -> SetLineColor(2);


	  Float_t* start_array_1_detection = start(sig);
          Float_t* stop_array_1_detection = stop(sig);


	  if (start_array_1_detection[0] < 128 && start_array_1_detection[1] == 128 && stop_array_1_detection[0] > 0 && stop_array_1_detection[1] == 0) {
	    Int_t duration_above_tc = stop_array_1_detection[0] - start_array_1_detection[0];
	    if (duration_above_tc >= 12) {

	      multigraph_onepulse[onePulse_counter] -> Add(graph_onepulse[onePulse_counter]); 
	      multigraph_onepulse[onePulse_counter] -> Add(onePulse_threshold_line[onePulse_counter], "L");

	      multigraph_onepulse[onePulse_counter]->SetMinimum(-2000); // set lower bound
	      multigraph_onepulse[onePulse_counter]->SetMaximum(200); // set upper bound

	      onePulse_counter += 1; // increment by 1 until 9 
	    }

	  }

	}
      
      if(twoPulse_counter < ngraph && jentry > 1000 && two_pulse_display)
	{
	  TString title_twopulse = Form("Event # %lld", jentry); // title form
	  
	  multigraph_twopulse[twoPulse_counter] = new TMultiGraph(title_twopulse, title_twopulse); // initialize multigraph

	  Double_t plotX[2] = {0.0, 128.0}; // x-coordinates
	  Double_t plotY[2] = {-threshold, -threshold}; // y-coordinates
	  twoPulse_offset_line[twoPulse_counter] = new TGraph(2, plotX, plotY); // initialize new graph for offset line

	  graph_twopulse[twoPulse_counter] = new TGraph(number_entries, x, y); // initialize plot of two pulse (y_2)
	  graph_twopulse[twoPulse_counter] -> SetLineColor(2);


	  Float_t* start_array_2_detections = start(sig);
          Float_t* stop_array_2_detections = stop(sig);

	  if (start_array_2_detections[1] < 128 && stop_array_2_detections[1] > 0) {
	    Int_t first_duration_above_tc = stop_array_2_detections[0] - start_array_2_detections[0];
	    if (first_duration_above_tc >= 8) {
	      Int_t second_duration_above_tc = stop_array_2_detections[1] - start_array_2_detections[1];
	      if (second_duration_above_tc >= 2) {
		cout << "First Start: " << start_array_2_detections[0] << endl;
		cout << "Second Start: " << start_array_2_detections[1] << endl;
		cout << "First Stop: " << stop_array_2_detections[0] << endl;
		cout << "Second Stop: " << stop_array_2_detections[1] << endl;
		cout << endl;

		multigraph_twopulse[twoPulse_counter] -> Add(graph_twopulse[twoPulse_counter]);
		multigraph_twopulse[twoPulse_counter] -> Add(twoPulse_offset_line[twoPulse_counter], "L"); // addition of offset line
		multigraph_twopulse[twoPulse_counter] -> SetMinimum(-1600); // set lower bound
		multigraph_twopulse[twoPulse_counter] -> SetMaximum(200); // set upper bound
	  
	      twoPulse_counter += 1; // increment by 1 until 9
	      }
	    }
	  }
	}
      
        
   }

   cout<<" I'm out\n"; // to output out of loop


   if (no_pulse_display) { // if no_pulse_display is set to TRUE
     auto multipleNoPulsePlots = new TCanvas("c1", "Entries for No Pulse Plots"); // initialize a new canvas
     multipleNoPulsePlots -> Divide(3, 3); // divide canvas into 3 by 3 sections
     
     for (int i = 0; i < ngraph; i++) { // as long at i is less than ngraph(9)
       multipleNoPulsePlots -> cd(i + 1); // on the canvas of multipleNoPulsePlots...
       multigraph_nopulse[i] -> Draw("ACP"); // draw the plot at index i
     }
     multipleNoPulsePlots -> Print("multipleNoPulsePlots.pdf"); // prints pdf of multigraph
   }

   if (one_pulse_display){ // if one_signal_display is set to TRUE
     auto multipleOnePulsePlots = new TCanvas("c2", "Entries for One Pulse Plots");
     multipleOnePulsePlots->Divide(3, 3);

     for (int i = 0; i < ngraph; i++) {
     multipleOnePulsePlots -> cd(i + 1);
     multigraph_onepulse[i] -> Draw("ACP");
     }
     multipleOnePulsePlots -> Print("multipleOnePulsePlots.pdf");

   }

   if (two_pulse_display) { // if two_signal_display is set to TRUE
     auto multipleTwoPulsePlots = new TCanvas("c3", "Entries for Two Pulse Plots");
     multipleTwoPulsePlots -> Divide(3, 3);
     
     for (int i = 0; i < ngraph; i++) {
       multipleTwoPulsePlots -> cd(i + 1);
       multigraph_twopulse[i] -> Draw("ACP");
     }
     multipleTwoPulsePlots -> Print("multipleTwoPulsePlots.pdf");
   }


   return;
}


void display(Int_t &pulse_display, Bool_t &no_pulse_display, Bool_t &one_pulse_display, Bool_t &two_pulse_display){
   
   // control flow for param selection
   if (pulse_display == 0) { // if t.Loop(0), only multigraph for no pulse is displayed
     no_pulse_display = kTRUE;
   }
   else if (pulse_display == 1) { // if t.Loop(1), only multigraph for one pulse is displayed
     one_pulse_display = kTRUE;
   }
   else if (pulse_display == 2) { // if t.Loop(2), only multigraph for two pulse is displayed
     two_pulse_display = kTRUE;
   }
   else { // any other number or no number as the parameter of t.Loop(), all multigraphs are displayed (i.e. no pulse, one pulse, two pulse)
     no_pulse_display = kTRUE;
     one_pulse_display = kTRUE;
     two_pulse_display = kTRUE;
   }
}




