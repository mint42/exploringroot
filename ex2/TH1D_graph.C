/*	plots a histogram using the TH1D class for 1D histograms
 *	D meaning that the bins of the histogram can be type double
 * 	
 * 	"gaus" is shorthand for a gaussian function. it's recognized by the TFormula class
 *
 *	g(x) = P0e^(-((x-P1)/P2)^2)
 *
 *	where P0, P1, and P2 are set with:
 *	TF1object.SetParameter(P0, #)
 *	TF1object.SetParameters(P0#, P1#, P2#)
 *
 * 	"e" shows error bars
 *
 * 	The TF1 object isn't necessary, you can achieve the same outcome by replacing
 * 	"f" by "gaus" in the h->FillRandom() call. This is just a proof of concept.
 */

void TH1D_graph() {
	TH1D	*h = new TH1D("h", "Histogram from gaussian", 100, -3, 3);
	TF1		*f = new TF1("f", "gaus", 0, 3);

	f->SetParameters(10.0, 1.0, 0.5);

	h->FillRandom("f", 10000);
	h->Draw("e");
}
