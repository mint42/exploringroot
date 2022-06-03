#define tree_cxx 
#include "tree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <vector>

/*
 *   In a ROOT session, you can do:
 *      root> .L tree.C
 *      root> tree t
 *      root> t.GetEntry(12); // Fill t data members with entry number 12
 *      root> t.Show();       // Show values of entry 12
 *      root> t.Show(16);     // Read and show values of entry 16
 *      root> t.Loop();       // Loop on all entries and prints all multigraphs (i.e. no pulse, one pulse, two pulse)
 *      root> t.Loop(0);      // Prints no pulse multigraph
 *      root> t.Loop(1);      // Prints one pulse multigraph
 *      root> t.Loop(2);      // Prints two pulse multigraph
 *
 *
 *     This is the loop skeleton where:
 *    jentry is the global entry number in the chain
 *    ientry is the entry number in the current Tree
 *  Note that the argument to GetEntry must be:
 *    jentry for TChain::GetEntry
 *    ientry for TTree::GetEntry and TBranch::GetEntry
 *
 *       To read only selected branches, Insert statements like:
 * METHOD1:
 *    fChain->SetBranchStatus("*",0);  // disable all branches
 *    fChain->SetBranchStatus("branchname",1);  // activate branchname
 * METHOD2: replace line
 *    fChain->GetEntry(jentry);       //read all branches
 *    b_branchname->GetEntry(ientry); //read only this branch
 */

/*
 * @param: pulse_display: value taken from the loop function, determines which plots are outputted
 * @param: no_pulse_display- type bool value for the no pulse output
 * @param: one_pluse_display- type bool value for the one pulse output
 * @param: two_pulse_display- type bool value for the two pulse output
 */
void	set_display(Int_t &pulse_display, Bool_t &no_pulse_display, Bool_t &one_pulse_display, Bool_t &two_pulse_display);


void	tree::Loop(Int_t pulse_display = -1)
{
	if (fChain == 0)
		return;

	const Double_t	threshold = 50; //JR I'm adding this is but I am not quite sure what threshold is used for the pulse analysis
	const Int_t		number_entries = 128;

	// x-coordinates to draw the signal trace
	const Int_t		x[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127};

	Long64_t		nentries = fChain->GetEntriesFast();

	// counters
	const	Int_t ngraph = 9;
	Int_t	noPulse_counter = 0; // counter for no pulse
	Int_t	onePulse_counter = 0; // counter for one pulse
	Int_t	twoPulse_counter = 0; // counter for two pulses

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

	Bool_t		no_pulse_display = kFALSE; // corresponds to no signal
	Bool_t		one_pulse_display = kFALSE; // corresponds to 1 signal
	Bool_t		two_pulse_display = kFALSE; // corresponds to 2 signals

	display(pulse_display, no_pulse_display, one_pulse_display, two_pulse_display);

	// main loop. this loops over all the events in the tree.
	for (Long64_t jentry = 0; jentry < nentries; ++jentry)
	{
		fChain->GetEntry(jentry);

		if (jentry < 10)
			pulseFADC(8,4, 10, 110, 50, 10);

		if (A1 == 0 && noPulse_counter < ngraph && jentry > 1000 && no_pulse_display)
		{	 
			TString		title_nopulse = Form("Event # %lld", jentry); // title form

			multigraph_nopulse[noPulse_counter] = new TMultiGraph(title_nopulse, title_nopulse); // initialize multigraph
			graph_nopulse[noPulse_counter] = new TGraph(number_entries, x, sig); // initialize plot of no pulses (y)

			graph_nopulse[noPulse_counter]->SetLineColor(2);
			multigraph_nopulse[noPulse_counter] -> Add(graph_nopulse[noPulse_counter]); // addition of plot to multigraph

			Double_t	plotX[2] = {0.0, 128.0}; // x-coordinates to draw the offset line
			Double_t	plotY[2] = {off, off}; // y-coordinates to draw the offset line

			noPulse_offset_line[noPulse_counter] = new TGraph(2, plotX, plotY); // initialize new graph for offset line

			multigraph_nopulse[noPulse_counter]->Add(noPulse_offset_line[noPulse_counter], "L"); // addition of offset line to multigraph
			multigraph_nopulse[noPulse_counter]->SetMinimum(-100); // lower bound of plot
			multigraph_nopulse[noPulse_counter]->SetMaximum(50); // upper bound of plot

			++noPulse_counter;
		}

		if (onePulse_counter < ngraph && jentry > 1000 && one_pulse_display)
		{
			TString		title_onepulse = Form("Event # %lld",jentry); // title form

			multigraph_onepulse[onePulse_counter] = new TMultiGraph(title_onepulse, title_onepulse); // initialize multigraph

			Double_t	plotX[2] = {0.0, 128.0};
			Double_t	plotY[2] = {-threshold, -threshold}; // y-coordinates to draw the theshold line

			onePulse_threshold_line[onePulse_counter] = new TGraph(2, plotX, plotY); 

			graph_onepulse[onePulse_counter] = new TGraph(number_entries, x, sig); // initialize plot of no pulses (y)
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

					multigraph_onepulse[onePulse_counter]->SetMinimum(-2000);
					multigraph_onepulse[onePulse_counter]->SetMaximum(200);

					++onePulse_counter;
				}
			}
		}

		if (twoPulse_counter < ngraph && jentry > 1000 && two_pulse_display)
		{
			TString		title_twopulse = Form("Event # %lld", jentry); // title form

			multigraph_twopulse[twoPulse_counter] = new TMultiGraph(title_twopulse, title_twopulse); // initialize multigraph

			Double_t	plotX[2] = {0.0, 128.0};
			Double_t	plotY[2] = {-threshold, -threshold};

			twoPulse_offset_line[twoPulse_counter] = new TGraph(2, plotX, plotY); // initialize new graph for offset line

			graph_twopulse[twoPulse_counter] = new TGraph(number_entries, x, sig); // initialize plot of two pulse (y_2)
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
						multigraph_twopulse[twoPulse_counter]->Add(twoPulse_offset_line[twoPulse_counter], "L");
						multigraph_twopulse[twoPulse_counter]->SetMinimum(-1600);
						multigraph_twopulse[twoPulse_counter]->SetMaximum(200);

						++twoPulse_counter;
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

static void		set_display(Int_t &pulse_display, Bool_t &no_pulse_display, Bool_t &one_pulse_display, Bool_t &two_pulse_display)
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

/*
*	@param: NSB			number of sample before the first sample cross the threshold
*	@param: NSA			number of sample after the first sample cross the threshold
*	@param: PTWMIn/MAX	triger window in which the pulses are searched
*	@param: TET			programmable theshold
*	@param: NSAT		number of consecutive sample above thrshold for a valid signal
*/
void	tree::pulseFADC(Int_t NSA, Int_t NSB, Int_t PTW_min, Int_t PTW_max, Int_t TET, Int_t NSAT)
{
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
