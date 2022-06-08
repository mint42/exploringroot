#define tree_cxx 
#include "tree.h"
#include "fadc.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <vector>

#define MAX_NUM_GRAPHS 9			// number of graphs to be drawn from each category (no pulse, 1 pulse, 2 pulse)

/*
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
 * @param: pulse_display	option to pick which multigraphs to show.
 * 							-1 is sent by default
 *
 * 							=  0		only show the 0 pulse instances
 * 							=  1		only show the 1 pulse instances
 * 							=  2		only show the 2 pulse instances
 * 							!= 0,1,2	display all graphs
 *
 */

void	tree::Loop(Int_t pulse_display = -1) {
	if (fChain == 0)
		return;

	// x-coordinates to draw the signal trace
	const Int_t		x[TOTAL_NSAMPLES] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127};

	Long64_t		nentries = fChain->GetEntriesFast();

	// counters keep track of how many events of each type are found. each will
	// stop at MAX_NUM_GRAPHS (9) since the program only cares to graph those first 
	// few instances
	Int_t	noPulse_counter = 0;
	Int_t	onePulse_counter = 0;
	Int_t	twoPulse_counter = 0;

	// declaration of multigraph, no pulse plots, and offset line 
	TMultiGraph	*multigraph_nopulse[MAX_NUM_GRAPHS];
	TGraph		*graph_nopulse[MAX_NUM_GRAPHS];
	TGraph		*noPulse_offset_line[MAX_NUM_GRAPHS];

	// declaration of multigraph, one pulse plots, point of pulse, and offset line
	TMultiGraph	*multigraph_onepulse[MAX_NUM_GRAPHS];
	TGraph		*graph_onepulse[MAX_NUM_GRAPHS];
//	TGraph		*graph_onepulse_dot[MAX_NUM_GRAPHS];
	TGraph		*onePulse_threshold_line[MAX_NUM_GRAPHS];

	// declaration of multigraph, two pulse plots, points of pulses, and offset line 
	TMultiGraph	*multigraph_twopulse[MAX_NUM_GRAPHS];
	TGraph		*graph_twopulse[MAX_NUM_GRAPHS];
//	TGraph		*graph_twopulse_dot1[MAX_NUM_GRAPHS];
//	TGraph		*graph_twopulse_dot2[MAX_NUM_GRAPHS];
	TGraph		*twoPulse_offset_line[MAX_NUM_GRAPHS];

	// uses pulse_display variable to set display options
	Bool_t		display_no_pulse  = (pulse_display != 1 && pulse_display != 2) ? kTRUE : kFALSE;
	Bool_t		display_one_pulse = (pulse_display != 0 && pulse_display != 2) ? kTRUE : kFALSE;
	Bool_t		display_two_pulse = (pulse_display != 0 && pulse_display != 1) ? kTRUE : kFALSE;

	// for algorithm processing
	struct fadc	pulses;
	Int_t		n_events_pulses[MAX_NUM_PULSES + 1] = {0, 0, 0, 0, 0};

	// main loop. this loops over all the events in the tree.
	for (Long64_t jentry = 0; jentry < nentries; ++jentry)
	{
		fChain->GetEntry(jentry);

		pulseFADC(pulses);
		if (pulses.npulses == 0)
			++n_events_pulses[0];
		else if (pulses.npulses == 1)
			++n_events_pulses[1];
		else if (pulses.npulses == 2)
			++n_events_pulses[2];
		else if (pulses.npulses == 3)
			++n_events_pulses[3];
		else if (pulses.npulses == 4)
			++n_events_pulses[4];

		if (A1 == 0 && noPulse_counter < MAX_NUM_GRAPHS && jentry > 1000 && display_no_pulse)
		{	 
			cout << "No Pulse Event #: " << jentry << endl;
			TString		title_nopulse = Form("Event # %lld", jentry); // title form

			multigraph_nopulse[noPulse_counter] = new TMultiGraph(title_nopulse, title_nopulse); // initialize multigraph
			graph_nopulse[noPulse_counter] = new TGraph(TOTAL_NSAMPLES, x, sig); // initialize plot of no pulses

			graph_nopulse[noPulse_counter]->SetLineColor(2);
			multigraph_nopulse[noPulse_counter] -> Add(graph_nopulse[noPulse_counter]); // addition of plot to multigraph

			Double_t	plotX[2] = {0, TOTAL_NSAMPLES_FLOAT}; // x-coordinates to draw the offset line
			Double_t	plotY[2] = {off, off}; // y-coordinates to draw the offset line

			noPulse_offset_line[noPulse_counter] = new TGraph(2, plotX, plotY); // initialize new graph for offset line

			multigraph_nopulse[noPulse_counter]->Add(noPulse_offset_line[noPulse_counter], "L"); // addition of offset line to multigraph
			multigraph_nopulse[noPulse_counter]->SetMinimum(-100); // lower bound of plot
			multigraph_nopulse[noPulse_counter]->SetMaximum(50); // upper bound of plot

			++noPulse_counter;
		}

		Float_t		start_array_detection[MAX_NUM_PULSES] = {TOTAL_NSAMPLES, TOTAL_NSAMPLES, TOTAL_NSAMPLES, TOTAL_NSAMPLES};
		Float_t		stop_array_detection[MAX_NUM_PULSES] = {0, 0, 0, 0};

		find_start_and_stop(sig, start_array_detection, stop_array_detection);

		if (onePulse_counter < MAX_NUM_GRAPHS && jentry > 1000 && display_one_pulse)
		{
			if (start_array_detection[0] < TOTAL_NSAMPLES &&
				start_array_detection[1] == TOTAL_NSAMPLES &&
				stop_array_detection[0] > 0 &&
				stop_array_detection[1] == 0 &&
				(stop_array_detection[0] - start_array_detection[0] >= 12))
			{
				cout << "One Pulse Event #: " << jentry << endl;

				TString		title_onepulse = Form("Event # %lld",jentry); // title form

				multigraph_onepulse[onePulse_counter] = new TMultiGraph(title_onepulse, title_onepulse); // initialize multigraph

				Int_t		plotX[2] = {0, TOTAL_NSAMPLES};
				Int_t		plotY[2] = {-TET, -TET}; // y-coordinates to draw the theshold line

				onePulse_threshold_line[onePulse_counter] = new TGraph(2, plotX, plotY); 

				graph_onepulse[onePulse_counter] = new TGraph(TOTAL_NSAMPLES, x, sig);
				graph_onepulse[onePulse_counter]->SetLineColor(2);

				multigraph_onepulse[onePulse_counter]->Add(graph_onepulse[onePulse_counter]); 
				multigraph_onepulse[onePulse_counter]->Add(onePulse_threshold_line[onePulse_counter], "L");

				multigraph_onepulse[onePulse_counter]->SetMinimum(-2000);
				multigraph_onepulse[onePulse_counter]->SetMaximum(200);

				++onePulse_counter;
			}
		}

		if (twoPulse_counter < MAX_NUM_GRAPHS && jentry > 1000 && display_two_pulse)
		{
			if ((start_array_detection[1] < TOTAL_NSAMPLES && stop_array_detection[1] > 0) &&
			   ((stop_array_detection[0] - start_array_detection[0]) >= 8) &&
			   ((stop_array_detection[1] - start_array_detection[1]) >= 2))
			{
				cout << "Two Pulse Event #: " << jentry << endl;
				cout << "First Start: " << start_array_detection[0] << endl;
				cout << "Second Start: " << start_array_detection[1] << endl;
				cout << "First Stop: " << stop_array_detection[0] << endl;
				cout << "Second Stop: " << stop_array_detection[1] << endl;
				cout << endl;

				TString		title_twopulse = Form("Event # %lld", jentry);

				multigraph_twopulse[twoPulse_counter] = new TMultiGraph(title_twopulse, title_twopulse);

				Int_t		plotX[2] = {0, TOTAL_NSAMPLES};
				Int_t		plotY[2] = {-TET, -TET};

				twoPulse_offset_line[twoPulse_counter] = new TGraph(2, plotX, plotY);

				graph_twopulse[twoPulse_counter] = new TGraph(TOTAL_NSAMPLES, x, sig);
				graph_twopulse[twoPulse_counter]->SetLineColor(2);


				multigraph_twopulse[twoPulse_counter]->Add(graph_twopulse[twoPulse_counter]);
				multigraph_twopulse[twoPulse_counter]->Add(twoPulse_offset_line[twoPulse_counter], "L");
				multigraph_twopulse[twoPulse_counter]->SetMinimum(-1600);
				multigraph_twopulse[twoPulse_counter]->SetMaximum(200);

				++twoPulse_counter;
			}
		}
	}

	cout << "# zero pulse events: " << n_events_pulses[0] << endl;
	cout << "# one pulse events: " << n_events_pulses[1] << endl;
	cout << "# two pulse events: " << n_events_pulses[2] << endl;
	cout << "# three pulse events: " << n_events_pulses[3] << endl;
	cout << "# four pulse events: " << n_events_pulses[4] << endl;

	if (display_no_pulse) // if display_no_pulse is set to TRUE
	{
		auto multipleNoPulsePlots = new TCanvas("c1", "Entries for No Pulse Plots"); // initialize a new canvas
		multipleNoPulsePlots->Divide(3, 3); // divide canvas into 3 by 3 sections

		for (Int_t i = 0; i < MAX_NUM_GRAPHS; ++i)
		{
			multipleNoPulsePlots->cd(i + 1); // on the canvas of multipleNoPulsePlots...
			multigraph_nopulse[i]->Draw("ACP"); // draw the plot at index i
		}
		multipleNoPulsePlots->Print("multipleNoPulsePlots.pdf"); // prints pdf of multigraph
	}

	if (display_one_pulse) // if display_one_pulse is set to TRUE
	{
		auto multipleOnePulsePlots = new TCanvas("c2", "Entries for One Pulse Plots");
		multipleOnePulsePlots->Divide(3, 3);

		for (Int_t i = 0; i < MAX_NUM_GRAPHS; ++i)
		{
			multipleOnePulsePlots->cd(i + 1);
			multigraph_onepulse[i]->Draw("ACP");
		}
		multipleOnePulsePlots->Print("multipleOnePulsePlots.pdf");
	}

	if (display_two_pulse) // if display_two_pulse is set to TRUE
	{
		auto multipleTwoPulsePlots = new TCanvas("c3", "Entries for Two Pulse Plots");
		multipleTwoPulsePlots->Divide(3, 3);

		for (Int_t i = 0; i < MAX_NUM_GRAPHS; ++i)
		{
			multipleTwoPulsePlots->cd(i + 1);
			multigraph_twopulse[i]->Draw("ACP");
		}
		multipleTwoPulsePlots->Print("multipleTwoPulsePlots.pdf");
	}

	return;
}

// this function returns the start array
void	tree::find_start_and_stop(Int_t sig[], Float_t tc[], Float_t stop_time[])
{
	for (Int_t i = 0; i < MAX_NUM_PULSES; ++i)
	{
		tc[i] = TOTAL_NSAMPLES;
		stop_time[i] = 0;
	}

	Int_t		npulse = 0;

	for (Int_t i = 0; i < TOTAL_NSAMPLES; ++i)
	{
		if (-sig[i] >= TET && tc[npulse] == TOTAL_NSAMPLES)	     
			tc[npulse] = i;	
		if (-sig[i] <= TET && tc[npulse] != TOTAL_NSAMPLES && stop_time[npulse] == 0)
		{
			stop_time[npulse] = i;
			++npulse;
			if (npulse == MAX_NUM_PULSES)
				break;
		}
	}
}

void	tree::pulseFADC(struct fadc &pulses)
{
	Bool_t	verbose = kFALSE;

//	if (verbose)
//	{
//		for (Int_t i = 0; i < TOTAL_NSAMPLES; ++i)
//			cout << i << ":" << sig[i] << " , ";
//		cout << endl << endl;
//	}

	// finding the valid pulses
	if (verbose)
		cout << "Finding valid pulses: \n";

	Int_t	npulse = 0; // keeps track of the # of pulses detected
	pulses.tc[0] = 0;
	pulses.tc[1] = 0;
	pulses.tc[2] = 0;
	pulses.tc[3] = 0;

	for (Int_t i = PTW_MIN; i < PTW_MAX; ++i)
	{
		// when the signal goes over the threshold
		if (-sig[i] >= TET)
		{
			pulses.tc[npulse] = i;

			// loop until the signal goes back under the threshold
			++i;
			while (-sig[i] >= TET && i < PTW_MAX)
				++i;

			if (verbose)
				cout << "npulse =" << npulse << " pulse start=" << pulses.tc[npulse] << endl;

			// check if the signal was over theshold for long enough
			if (i - pulses.tc[npulse] >= NSAT)
			{
				if (verbose)
					cout << " pulse long enough\n";
				++npulse;
			}
			else
			{
				if (verbose)
					cout << " pulse too short\n";
				pulses.tc[npulse] = 0;
			}

			if (npulse == MAX_NUM_PULSES)
				break;
		}
	}
	
	pulses.npulses = npulse;

	if (npulse == 0)
		return ;

	if (verbose)
		cout << endl << endl;

	// finding a pulse pedestal
	// if the first pulse started within the window of the pedestal
	// the signal is said to be bad and FADC_pulse_pedestal_good = false
	if (verbose)
		cout << "Finding the pedestal:\n";

	pulses.pedestal_good = kTRUE;
	pulses.pedestal = 0;

	if (pulses.tc[0] < NPED) // checks if threshold is crossed in the pedestal range
	{
		cout << "bad pedestal\n";
		pulses.pedestal_good = kFALSE;
		return ;
	}

	for (Int_t i = 0; i < NPED; ++i)
		pulses.pedestal += sig[i];

	if (verbose)
	{
		cout << "good pedestal!\n";
		cout << "pedestal value =" << pulses.pedestal << "\n";
	}

	if (verbose)
		cout << endl << endl;

	// finding the pulse integral
	if (verbose)
		cout << "Pulse integral: \n";

	Int_t	pulse_duration_start;
	Int_t	pulse_duration_stop;

	pulses.integral[0] = 0;
	pulses.integral[1] = 0;
	pulses.integral[2] = 0;
	pulses.integral[3] = 0;

	for (Int_t i = 0; i < pulses.npulses; ++i)
	{
		if (verbose)
			cout << "pulse #" << i + 1<< "\n";

		pulse_duration_start = std::max(pulses.tc[i] - NSB, 1);
		pulse_duration_stop = std::min(pulses.tc[i] + NSA - 1, PTW_MAX);

		for (Int_t j = pulse_duration_start; j < pulse_duration_stop; ++j)
		{
			if (verbose)
				cout << j << ":" << sig[j] << " , ";
			pulses.integral[i] += abs(sig[j]);
		}
		if (verbose)
			cout << "integral =" << pulses.integral[i]<<"\n";
	}

	if (verbose)
		cout << endl << endl;

	// finding the average min voltage using with the first 4 signals
	Int_t	vmin = (sig[0] + sig[1] + sig[2] + sig[3]) / 4;

	// finding the pulse peaks
	if (verbose)
		cout << "Pulse Peaks: \n";

	pulses.vpeak[0] = 0;
	pulses.vpeak[1] = 0;
	pulses.vpeak[2] = 0;
	pulses.vpeak[3] = 0;
	pulses.vpeak_time[0] = 0;
	pulses.vpeak_time[1] = 0;
	pulses.vpeak_time[2] = 0;
	pulses.vpeak_time[3] = 0;
	pulses.vmid[0] = 0;
	pulses.vmid[1] = 0;
	pulses.vmid[2] = 0;
	pulses.vmid[3] = 0;

	for (Int_t i = 0; i < pulses.npulses; ++i)
	{
		for (Int_t j = pulses.tc[i]; j < PTW_MAX; ++j)
		{
			if (-sig[j] > -sig[j + 1])
			{
				pulses.vpeak[i] = sig[j];
				pulses.vpeak_time[i] = j;
				pulses.vmid[i] = (pulses.vpeak[i] - vmin) / 2.0;

				if (verbose)
					cout << "vpeak =" << sig[j] << " time=" << j << endl;		  
				break ;
			}
		}
	}

	if (verbose)
		cout << endl << endl;

	// find the pulse timings
	if (verbose)
		cout << "Pulse timing: \n";

	pulses.time_coarse[0] = 0;
	pulses.time_coarse[1] = 0;
	pulses.time_coarse[2] = 0;
	pulses.time_coarse[3] = 0;
	pulses.time_fine[0] = 0;
	pulses.time_fine[1] = 0;
	pulses.time_fine[2] = 0;
	pulses.time_fine[3] = 0;

	for (Int_t i = 0; i < pulses.npulses; ++i)
	{
		if (verbose)
			cout << "pulse #" << i + 1 << "\n";

		// finding the coarse and fine times
		for (Int_t j = pulses.tc[i]; j < pulses.vpeak_time[i]; ++j)
		{
			if (sig[j] >= pulses.vmid[i] && pulses.vmid[i] > sig[j + 1])
			{
				pulses.time_coarse[i] = j;
				pulses.time_fine[i] = 64 * ((pulses.vmid[i] - sig[pulses.time_coarse[i]]) /
										 ((sig[pulses.time_coarse[i] + 1]) - sig[pulses.time_coarse[i]]));
				if (verbose)
					cout << "coarse time is " << j << " with signal value =" << sig[j] << endl;
			}
		}
	}

	return;
}
