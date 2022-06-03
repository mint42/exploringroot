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
 */

void	display (Int_t &pulse_display, Bool_t &no_pulse_display, Bool_t &one_pulse_display, Bool_t &two_pulse_display);

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

void	tree::Loop(Int_t pulse_display = -1)
{
	if (fChain == 0)
		return;

	const Double_t	threshold = 50; //JR I'm adding this is but I am not quite sure what threshold is used for the pulse analysis
	const Int_t		number_entries = 128;
	const Double_t		x[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0, 46.0, 47.0, 48.0, 49.0, 50.0, 51.0, 52.0, 53.0, 54.0, 55.0, 56.0, 57.0, 58.0, 59.0, 60.0, 61.0, 62.0, 63.0, 64.0, 65.0, 66.0, 67.0, 68.0, 69.0, 70.0, 71.0, 72.0, 73.0, 74.0, 75.0, 76.0, 77.0, 78.0, 79.0, 80.0, 81.0, 82.0, 83.0, 84.0, 85.0, 86.0, 87.0, 88.0, 89.0, 90.0, 91.0, 92.0, 93.0, 94.0, 95.0, 96.0, 97.0, 98.0, 99.0, 100.0, 101.0, 102.0, 103.0, 104.0, 105.0, 106.0, 107.0, 108.0, 109.0, 110.0, 111.0, 112.0, 113.0, 114.0, 115.0, 116.0, 117.0, 118.0, 119.0, 120.0, 121.0, 122.0, 123.0, 124.0, 125.0, 126.0, 127.0};
	Double_t		y[number_entries];
	Long64_t		nentries = fChain->GetEntriesFast();

//	Double_t x1,x2,x3,x4,y1,y2,y3,y4;
//
//	Double_t t1_arr1[1];
//	Double_t A1_arr1[1];
//
//	Double_t t1_arr2[1];
//	Double_t A1_arr2[1];
//	Double_t t2_arr2[1];
//	Double_t A2_arr2[1];
//
//	Double_t min_A1 = 50000;
//	Double_t max_A1 = 0;
//	Double_t max_A2 = 0;
//
//	Double_t tmin;
//	Double_t tmax;

//	const Int_t cluster_entries = (number_entries / 4); 
//	Double_t sig_divide_four[cluster_entries]; // new array to hold the clustered values of array "sig" (32 length)

	// counters
	const	Int_t ngraph = 9;
	Int_t	noPulse_counter = 0; // counter for no pulse
	Int_t	onePulse_counter = 0; // counter for one pulse
	Int_t	twoPulse_counter = 0; // counter for two pulses

	// test graph and multigraph
	TGraph	*graph_average_four[ngraph];

	// declaration of multigraph, no pulse plots and offset line 
	TMultiGraph	*multigraph_nopulse[ngraph];
	TGraph		*graph_nopulse[ngraph];
	TGraph		*noPulse_offset_line[ngraph];

	// declaration of multigraph, one pulse plots, point of pulse and offset line
	TMultiGraph	*multigraph_onepulse[ngraph];
	TGraph		*graph_onepulse[ngraph];
	TGraph		*graph_onepulse_dot[ngraph];
	TGraph		*onePulse_threshold_line[ngraph];

	// declaration of multigraph, two pulse plots, points of pulses and offset line 
	TMultiGraph	*multigraph_twopulse[ngraph];
	TGraph		*graph_twopulse[ngraph];
	TGraph		*graph_twopulse_dot1[ngraph];
	TGraph		*graph_twopulse_dot2[ngraph];
	TGraph		*twoPulse_offset_line[ngraph];

	Bool_t		no_pulse_display=kFALSE; // corresponds to no signal
	Bool_t		one_pulse_display=kFALSE; // corresponds to 1 signal
	Bool_t		two_pulse_display=kFALSE; // corresponds to 2 signals

	display(pulse_display, no_pulse_display, one_pulse_display, two_pulse_display);

//	Long64_t nbytes = 0, nb = 0;
//	Int_t counter = 0;

	// this loops over all the events in the tree
	for (Long64_t jentry = 0; jentry < nentries; ++jentry)
	{
//		Long64_t ientry = LoadTree(jentry);
//		if (ientry < 0)
//			break;

		fChain->GetEntry(jentry);

//	int sum = 0;
//	int index = 0;

		for (Int_t i = 0; i < number_entries; ++i)
			y[i] = sig[i];

		if (jentry < 10)
			pulseFADC(8,4, 10, 110, 50, 10);

		if (A1 == 0 && noPulse_counter < ngraph && jentry > 1000 && no_pulse_display)
		{	 
			TString		title_nopulse = Form("Event # %lld", jentry); // title form

			multigraph_nopulse[noPulse_counter] = new TMultiGraph(title_nopulse, title_nopulse); // initialize multigraph
			graph_nopulse[noPulse_counter] = new TGraph(number_entries, x, y); // initialize plot of no pulses (y)

			graph_nopulse[noPulse_counter]->SetLineColor(2);
			multigraph_nopulse[noPulse_counter] -> Add(graph_nopulse[noPulse_counter]); // addition of plot to multigraph

			Double_t	plotX[2] = {0.0, 128.0}; // array holding x-coordinates
			Double_t	plotY[2] = {off, off}; // array holding y-coordinates

			noPulse_offset_line[noPulse_counter] = new TGraph(2, plotX, plotY); // initialize new graph for offset line
			multigraph_nopulse[noPulse_counter]->Add(noPulse_offset_line[noPulse_counter], "L"); // addition of offset line to multigraph
			multigraph_nopulse[noPulse_counter]->SetMinimum(-100); // lower bound of plot
			multigraph_nopulse[noPulse_counter]->SetMaximum(50); // upper bound of plot

			++noPulse_counter; // increment counter by 1 until 9 is hit
		}

		if (onePulse_counter < ngraph && jentry > 1000 && one_pulse_display)
		{
			TString		title_onepulse = Form("Event # %lld",jentry); // title form

			multigraph_onepulse[onePulse_counter] = new TMultiGraph(title_onepulse, title_onepulse); // initialize multigraph

			Double_t	plotX[2] = {0.0, 128.0}; // x-coordinates
			Double_t	plotY[2] = {-threshold, -threshold}; // y-coordinates

			onePulse_threshold_line[onePulse_counter] = new TGraph(2, plotX, plotY); 

			graph_onepulse[onePulse_counter] = new TGraph(number_entries, x, y); // initialize plot of no pulses (y)
			graph_onepulse[onePulse_counter]->SetLineColor(2);

			Float_t		*start_array_1_detection = start(sig);
			Float_t		*stop_array_1_detection = stop(sig);

			if (start_array_1_detection[0] < 128 &&
				start_array_1_detection[1] == 128 &&
				stop_array_1_detection[0] > 0 &&
				stop_array_1_detection[1] == 0)
			{
				Int_t	duration_above_tc = stop_array_1_detection[0] - start_array_1_detection[0];

				if (duration_above_tc >= 12)
				{
					multigraph_onepulse[onePulse_counter]->Add(graph_onepulse[onePulse_counter]); 
					multigraph_onepulse[onePulse_counter]->Add(onePulse_threshold_line[onePulse_counter], "L");

					multigraph_onepulse[onePulse_counter]->SetMinimum(-2000); // set lower bound
					multigraph_onepulse[onePulse_counter]->SetMaximum(200); // set upper bound

					++onePulse_counter; // increment by 1 until 9 
				}
			}
		}

		if (twoPulse_counter < ngraph && jentry > 1000 && two_pulse_display)
		{
			TString		title_twopulse = Form("Event # %lld", jentry); // title form

			multigraph_twopulse[twoPulse_counter] = new TMultiGraph(title_twopulse, title_twopulse); // initialize multigraph

			Double_t	plotX[2] = {0.0, 128.0}; // x-coordinates
			Double_t	plotY[2] = {-threshold, -threshold}; // y-coordinates

			twoPulse_offset_line[twoPulse_counter] = new TGraph(2, plotX, plotY); // initialize new graph for offset line

			graph_twopulse[twoPulse_counter] = new TGraph(number_entries, x, y); // initialize plot of two pulse (y_2)
			graph_twopulse[twoPulse_counter]->SetLineColor(2);

			Float_t		*start_array_2_detections = start(sig);
			Float_t		*stop_array_2_detections = stop(sig);

			if (start_array_2_detections[1] < 128 && stop_array_2_detections[1] > 0)
			{
				Int_t	first_duration_above_tc = stop_array_2_detections[0] - start_array_2_detections[0];

				if (first_duration_above_tc >= 8)
				{
					Int_t	second_duration_above_tc = stop_array_2_detections[1] - start_array_2_detections[1];

					if (second_duration_above_tc >= 2)
					{
						cout << "First Start: " << start_array_2_detections[0] << endl;
						cout << "Second Start: " << start_array_2_detections[1] << endl;
						cout << "First Stop: " << stop_array_2_detections[0] << endl;
						cout << "Second Stop: " << stop_array_2_detections[1] << endl;
						cout << endl;

						multigraph_twopulse[twoPulse_counter]->Add(graph_twopulse[twoPulse_counter]);
						multigraph_twopulse[twoPulse_counter]->Add(twoPulse_offset_line[twoPulse_counter], "L"); // addition of offset line
						multigraph_twopulse[twoPulse_counter]->SetMinimum(-1600); // set lower bound
						multigraph_twopulse[twoPulse_counter]->SetMaximum(200); // set upper bound

						++twoPulse_counter; // increment by 1 until 9
					}
				}
			}
		}
	}

	cout << " I'm out\n"; // to output out of loop

	if (no_pulse_display) // if no_pulse_display is set to TRUE
	{
		auto multipleNoPulsePlots = new TCanvas("c1", "Entries for No Pulse Plots"); // initialize a new canvas

		multipleNoPulsePlots->Divide(3, 3); // divide canvas into 3 by 3 sections

		for (Int_t i = 0; i < ngraph; ++i) // as long at i is less than ngraph(9)
		{
			multipleNoPulsePlots->cd(i + 1); // on the canvas of multipleNoPulsePlots...
			multigraph_nopulse[i]->Draw("ACP"); // draw the plot at index i
		}

		multipleNoPulsePlots->Print("multipleNoPulsePlots.pdf"); // prints pdf of multigraph
	}

	if (one_pulse_display) // if one_signal_display is set to TRUE
	{
		auto multipleOnePulsePlots = new TCanvas("c2", "Entries for One Pulse Plots");

		multipleOnePulsePlots->Divide(3, 3);

		for (Int_t i = 0; i < ngraph; ++i)
		{
			multipleOnePulsePlots->cd(i + 1);
			multigraph_onepulse[i]->Draw("ACP");
		}

		multipleOnePulsePlots->Print("multipleOnePulsePlots.pdf");
	}

	if (two_pulse_display) // if two_signal_display is set to TRUE
	{
		auto multipleTwoPulsePlots = new TCanvas("c3", "Entries for Two Pulse Plots");
		multipleTwoPulsePlots->Divide(3, 3);

		for (Int_t i = 0; i < ngraph; ++i)
		{
			multipleTwoPulsePlots->cd(i + 1);
			multigraph_twopulse[i]->Draw("ACP");
		}
		multipleTwoPulsePlots->Print("multipleTwoPulsePlots.pdf");
	}

	return;
}


void	display(Int_t &pulse_display, Bool_t &no_pulse_display, Bool_t &one_pulse_display, Bool_t &two_pulse_display)
{
	// control flow for param selection
	if (pulse_display == 0) // if t.Loop(0), only multigraph for no pulse is displayed
		no_pulse_display = kTRUE;
	else if (pulse_display == 1) // if t.Loop(1), only multigraph for one pulse is displayed
		one_pulse_display = kTRUE;
	else if (pulse_display == 2) // if t.Loop(2), only multigraph for two pulse is displayed
		two_pulse_display = kTRUE;
	else // any other number or no number as the parameter of t.Loop(), all multigraphs are displayed (i.e. no pulse, one pulse, two pulse)
	{
		no_pulse_display = kTRUE;
		one_pulse_display = kTRUE;
		two_pulse_display = kTRUE;
	}
}

// this function returns the start array
Float_t		*tree::start(Int_t sig[])
{
	//cout<<" jentry= "<<jentry<<endl;
	//cout<<" A1="<<A1<<"  t1="<<t1<<" A2="<<A2<<" t2="<<t2<<endl;

	const Int_t	narray = 128;
	const Int_t	maxpulse = 4; //looking for up to 4 pulses

	for (Int_t i = 0; i < maxpulse; ++i)
	{
		start_time[i] = narray;
		stop_time[i] = 0;
	}

	Int_t		npulse = 0;

	for (Int_t i = 0; i < narray; ++i)
	{

		if (-sig[i] >= threshold && start_time[npulse] == narray)	     
			start_time[npulse] = i;	
		if (-sig[i] <= threshold && start_time[npulse] != narray && stop_time[npulse] == 0)
		{
			stop_time[npulse] = i;
			++npulse;
			if (npulse == maxpulse)
				i = narray + 1;
		}
	}

	return start_time;
}


// this function returns the stop array
Float_t* tree::stop(Int_t sig[])
{
	//cout<<" jentry= "<<jentry<<endl;
	//cout<<" A1="<<A1<<"  t1="<<t1<<" A2="<<A2<<" t2="<<t2<<endl;

	const Int_t	narray = 128;
	const Int_t	maxpulse = 4; //looking for up to 4 pulses

	for (Int_t i = 0; i < maxpulse; ++i)
	{
		start_time[i] = narray;
		stop_time[i] = 0;
	}
	Int_t npulse = 0;

	for (Int_t i = 0; i < narray; ++i)
	{
		if (-sig[i] >= threshold && start_time[npulse] == narray)	     
			start_time[npulse] = i;	
		if(-sig[i] <= threshold && start_time[npulse] != narray && stop_time[npulse] == 0)
		{
			stop_time[npulse] = i;
			++npulse;
			if (npulse == maxpulse)
				i = narray + 1;
		}
	}

	return stop_time;
}

void	tree::pulseFADC(Int_t NSA, Int_t NSB, Int_t PTW_min, Int_t PTW_max, Int_t TET, Int_t NSAT)
{
	// NSB number of sample before the first sample cross the threshold
	// NSA number of sample after the first sample cross the threshold
	// PTWMIn and MAX triger window in which the pulses are searched
	//TET programmable theshold
	// NSAT number of consecutive sample above thrshold for a valid signal

	Bool_t	bad_input = kFALSE;
	Bool_t	verbose = kTRUE;

	Int_t	narray = 128;
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
	if (!bad_input)
	{
		if (verbose)
		{
			for (Int_t i = 0; i < narray; ++i)
				cout<<i<<":"<<sig[i]<<" , ";
			cout << endl;
		}

		//looking for up to 4 pulses
		const Int_t	maxpulse=4;
		Int_t		npulsefound=0;
		Int_t		start_time[4];
		Int_t		stop_time[4];

		for (Int_t i = 0; i < maxpulse; ++i)
		{
			start_time[i] = narray;
			stop_time[i] = 0;
		}
		Int_t npulse = 0;

		for (Int_t i = PTW_min; i < PTW_max; ++i)
		{
			// find when the signal goes over threshold
			if (-sig[i] >= TET && start_time[npulse] == narray)	     
				start_time[npulse] = i;
			// find the the signal goes back under threshold
			if (-sig[i] <= TET && start_time[npulse] != narray && stop_time[npulse] == 0)
			{
				stop_time[npulse] = i;
				if (verbose)
					cout << "npulse =" << npulse << " pulse start=" << start_time[npulse] << " stop time =" << stop_time[npulse];
				// check if the signal was over theshold for long enough
				Int_t	duration = stop_time[npulse] - start_time[npulse] + 1;

				if (duration > NSAT)
				{
					if (verbose)
						cout << " pulse long enough\n";
					//this is pulse was above threshold for long enough
					++npulse;
					++npulsefound;
				}
				else
				{
					if (verbose)
						cout << " pulse too short\n";
					start_time[npulse] = narray;
					stop_time[npulse] = 0;
				}
				if (npulse == maxpulse)
					i = narray + 1;
			}
		}

		// finding a pedestal at the begining of the trace
		// if the first pulse started whitin the window of the pedestal, signal pedestal is bad with FADC_pedestal_good.
		FADC_pulse_pedestal_good = kTRUE;
		FADC_pulse_pedestal = 0;

		for (Int_t i = 0; i < 4; ++i)
			FADC_pulse_pedestal += sig[i];

		if (start_time[0] < NSA + NSB)
			FADC_pulse_pedestal_good = kFALSE;

		if (verbose)
		{
			cout << "Pedestal evaluation \n";
			cout << "pedestal =" << FADC_pulse_pedestal << "\n";

			if (FADC_pulse_pedestal_good)
				cout << "good pedestal\n";
			else
				cout<<"bad pedestal\n";
		}

		/// pulse integral
		if (verbose)
			cout << "Pulse integral \n";

		for (Int_t i = 0; i < npulsefound; ++i)
		{
			if (verbose)
				cout << "pulse #" << i << "\n counting bins";
			FADC_pulse_sum[i] = 0;

			Int_t	countstart = std::min(start_time[i] - NSA, 0);
			Int_t	countstop = std::min(start_time[i] + NSB, narray);

			for (Int_t j = countstart; j < countstop; ++j)
			{
				if (verbose)
					cout << j << ":" << sig[j] <<" , ";
				FADC_pulse_sum[i] += sig[j];
			}
			if (verbose)
				cout << "integral =" << FADC_pulse_sum[i]<<"\n";
		}

		///pulse timing
		if (verbose)
			cout << "Pulse integral \n";
		for (Int_t i = 0; i < maxpulse; ++i)
			FADC_time_coarse[i] = -1;

		Int_t	vpeak = 0;
		Int_t	ipeak = 0;

		for (Int_t i = 0; i < npulsefound; ++i)
		{
			if (verbose)
				cout << "pulse #" << i << "\n counting bins";

			for (Int_t j = start_time[i]; j < stop_time[i] - 1; ++j)
			{
				if (sig[j] > sig[j + 1])
				{
					vpeak = (sig[j] + FADC_pulse_pedestal) / 2.;
					ipeak = j;

					if (verbose)
						cout << "vpeak =" << vpeak << " bin=" << j << endl;		  
				}
			}
			// now going backward to find where the signal goes halfway to the peak
			for (Int_t j = ipeak; j > start_time[i] - NSA; --j)
			{
				if (sig[j] < vpeak)
				{
					FADC_time_coarse[i] = j;
					//	      fADC_time_fine[i]= to compute
					j = start_time[i] - NSA - 1;
					if (verbose)
						cout << "coarse time is " << j << " with signal value =" << sig[j] << endl;
				}
			}
		}
	}

	return;
}
