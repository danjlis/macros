
#include "TLatex.h"

void myText(const double x, const double y, const int color, const char* text, const double size = 0.035) {
	TLatex latex;
	latex.SetTextSize(size); //0.0350);
	latex.SetTextAlign(13);  //align at top
	latex.SetTextColor(color);
	latex.DrawLatexNDC(x, y, text);
}
