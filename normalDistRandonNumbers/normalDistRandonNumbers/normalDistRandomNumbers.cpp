// normalDistRandonNumbers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <new>
#include "libxl.h"
/*#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>*/

double randn (double nMin, double nMax, double mu, double sigma)
{
	double U1, U2, W, mult;
	static double X1, X2;
	static int call = 0;
	using namespace std;  
	if (call == 1)
	{
		call = !call;
		return (mu + sigma * (double) X2);
	}
	//srand(time(NULL));
	do
	{
		/*double temp1 = rand ();
		double temp2 = rand ();
		cout << temp1<<'\t'<<temp2<<endl;
		U1 = nMin + (temp1 / RAND_MAX) * (nMax-nMin);
		U2 = nMin + (temp2 / RAND_MAX) * (nMax-nMin);*/
		U1 = nMin + ((double) rand () / RAND_MAX) * (nMax-nMin);
		U2 = nMin + ((double) rand () / RAND_MAX) * (nMax-nMin);
		W = pow (U1, 2) + pow (U2, 2);

		// cout << RAND_MAX;
	}
	while (W >= 1 || W == 0);

	mult = sqrt ((-2 * log (W)) / W);
	X1 = U1 * mult;
	X2 = U2 * mult;

	call = !call;

	return (mu + sigma * (double) X1);
}

double get_normalPDF(double x, double mean, double stdDev)
{
	using namespace std;
	static const double inv_sqrt_2pi = 0.3989422804014327;
	double a = (x - mean) / stdDev;

	return inv_sqrt_2pi / stdDev * exp(-0.5 * a * a);
}

//int main(int nMin, int nMax, int stdDev, int variance)
int main()
{
	int num;
	using namespace std;
	cout << "Enter no.of measurement you require: " << endl;
	cin >> num;
	double *result, *pdf; // for dynamic memory allocation
	double nMax, nMin, stdDev, mean; 
	int index;
	nMax=10;
	nMin=-10;
	//mean=-2;
	//stdDev=1.5;
	cout << "Enter the mean followed by standard Deviation by presing enter each time" << endl;
	cin >> mean >> stdDev;
	//initgraph(&gd,&gm,"tc\\bgi\\egavga.bgi")... 


	result = new double[num];
	pdf =    new double[num];
	if ((result == nullptr) || (pdf == nullptr))
	{
		cout << "Error: memory could not be allocated...terminating";
			return 0;
	}
	else
	{

		for(index=0;index<num;index++)
		{
			result[index] = randn(nMin, nMax, mean, stdDev);
			//cout << randn(nMin, nMax, mean, stdDev) << endl;
			cout << result[index] << endl;
			pdf[index]=get_normalPDF(result[index], mean, stdDev);
			//putpixel(result[index], pdf[index], 3);
		}

		// Copying to excell starts here
		using namespace libxl;
		Book* book = xlCreateBook(); // xlCreateXMLBook() for xlsx
		if(book)
		{
			Sheet* sheet = book->addSheet(L"Sheet1");
			if(sheet)
			{
				int r=2; // row number in excell
				int c=1; // column number in excell
				sheet->writeStr(r, c, L"X-value");
				sheet->writeStr(r, c+1, L"f(x)-value");
				//int ntemp=0;
				//int valtemp=0;
				for(index=0;index<num;index++)
				{
					sheet->writeNum(index+r+1, c, result[index]);
					sheet->writeNum(index+r+1, c+1, pdf[index]);

					/*	if(ntemp==0)
					valtemp=index+r+1;
					++ntemp;*/
				}

				/* labelling of avg and std_dev in excel is done here. 
				char str1[20], str2[20];
				strcpy(str1,"B");
				strcat(str1, valtemp);

				labelling ends here*/

				sheet->writeStr(10, 4, L"Mean");
				sheet->writeStr(10, 5, L"= MITTELWERT(B3:B103)");
				sheet->writeStr(12, 4, L"Std_Dev");
				sheet->writeStr(12, 5, L"= STABWA(B3:B103)");
			}
			book->save(L"normalDistRandomNumbers.xls");
			book->release();
		} 
		delete [] result;
		delete [] pdf;
	}
		// Copying to excell ends here
		//closegraph();
		cin.clear();
	cin.ignore(255, '\n');
	cin.get();
	return 0;
}


