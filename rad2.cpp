
/*
Author: Vineet Pandey
Human Centered Robotics
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <dirent.h>

using namespace std;

void InputFile();

vector<string>file;
#define PI 3.141592

int M = 15;
int N = 15;

/////////      NaN Realization
/////////      Test, Train dataset auto read




int main()
{
  InputFile();

  for (int count = 0; count < 72; count++)
  {
	ifstream inFile;
	//inFile.open("dataset_full/Train/a01_s06_e01_skeleton_proj.txt");
	inFile.open(file[count].c_str());
	cout<<"Reading File: "<<file[count]<<endl;

	if (!inFile.is_open())
	{
	  cout<<"Unable to open file"<<endl;
	}

	int line = 0;
	string s;
	while(getline(inFile, s))
	{
	 line++;
	}

	float values[line][5];

	inFile.clear();
	inFile.seekg(0);
	string str;
	int n = 0;
	int m = 0;
  	float num = 0;

	while(getline(inFile, str))
	{
	  if (m>4)
	  {
	  n = n+1;
	  m = 0;
	  }
	  	   stringstream stream(str);
			while (stream >> num)
			{
			  values[n][m] = num;
			 m = m+1;
			}
		//n++;
	}

int frames = (n+1)/20;
inFile.close();


//----------------------------------------------------------------------------------------------


//Calculating Joint Distances...
float dist[frames][5];
for (int fd=0; fd<frames; fd++)
{
  int r0 = (2 + 20*(fd))-1;
  int r1 = (4 + 20*(fd))-1;
  int r2 = (8 + 20*(fd))-1;
  int r3 = (12 + 20*(fd))-1;
  int r4 = (16 + 20*(fd))-1;
  int r5 = (20 + 20*(fd))-1;

	dist[fd][0] = pow((pow((values[r1][2]-values[r0][2]),2) + pow((values[r1][3]-values[r0][3]),2) + pow((values[r1][4]-values[r0][4]),2)),0.5);

	dist[fd][1] = pow((pow((values[r2][2]-values[r0][2]),2) + pow((values[r2][3]-values[r0][3]),2) + pow((values[r2][4]-values[r0][4]),2)),0.5);

	dist[fd][2] = pow((pow((values[r3][2]-values[r0][2]),2) + pow((values[r3][3]-values[r0][3]),2) + pow((values[r3][4]-values[r0][4]),2)),0.5);

	dist[fd][3] = pow((pow((values[r4][2]-values[r0][2]),2) + pow((values[r4][3]-values[r0][3]),2) + pow((values[r4][4]-values[r0][4]),2)),0.5);

	dist[fd][4] = pow((pow((values[r5][2]-values[r0][2]),2) + pow((values[r5][3]-values[r0][3]),2) + pow((values[r5][4]-values[r0][4]),2)),0.5);

   //cout<<dist[fd][0]<<" "<<dist[fd][1]<<" "<<dist[fd][2]<<" "<<dist[fd][3]<<" "<<dist[fd][4]<<endl;
   //cout<<fd<<endl;
}



//----------------------------------------------------------------------------------------------


//Calculating Angles...
float theta[frames][5];
float a = 0;
float b = 0;
float c = 0;
float cos_t = 0;

for (int ft=0; ft<frames; ft++)
{
 int r0 = (2 + 20*(ft))-1;
 int r1 = (4 + 20*(ft))-1;
 int r2 = (8 + 20*(ft))-1;
 int r3 = (12 + 20*(ft))-1;
 int r4 = (16 + 20*(ft))-1;
 int r5 = (20 + 20*(ft))-1;

  a = pow((values[r1][2]-values[r0][2]),2) + pow((values[r1][3]-values[r0][3]),2);
  a = pow(a,0.5);
  b = pow((values[r3][2]-values[r0][2]),2) + pow((values[r3][3]-values[r0][3]),2);
  b = pow(b,0.5);
  c = pow((values[r1][2]-values[r3][2]),2) + pow((values[r1][3]-values[r3][3]),2);
  c = pow(c,0.5);
  cos_t = (pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b);
  if (cos_t < -1.0) cos_t = -1.0 ;
  if (cos_t > 1.0) cos_t = 1.0 ;
  theta[ft][0] = acos(cos_t)*180/PI;

  a = pow((values[r1][2]-values[r0][2]),2) + pow((values[r1][3]-values[r0][3]),2);
  a = pow(a,0.5);
  b = pow((values[r2][2]-values[r0][2]),2) + pow((values[r2][3]-values[r0][3]),2);
  b = pow(b,0.5);
  c = pow((values[r1][2]-values[r2][2]),2) + pow((values[r1][3]-values[r2][3]),2);
  c = pow(c,0.5);
  cos_t = (pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b);
  if (cos_t < -1.0) cos_t = -1.0 ;
  if (cos_t > 1.0) cos_t = 1.0 ;
  if ((isnan(acos(cos_t)*180/PI))>0)
  theta[ft][1] = theta[ft-1][1];
  else theta[ft][1] = acos(cos_t)*180/PI;

  a = pow((values[r2][2]-values[r0][2]),2) + pow((values[r2][3]-values[r0][3]),2);
  a = pow(a,0.5);
  b = pow((values[r4][2]-values[r0][2]),2) + pow((values[r4][3]-values[r0][3]),2);
  b = pow(b,0.5);
  c = pow((values[r2][2]-values[r4][2]),2) + pow((values[r2][3]-values[r4][3]),2);
  c = pow(c,0.5);
  cos_t = (pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b);
  if (cos_t < -1.0) cos_t = -1.0 ;
  if (cos_t > 1.0) cos_t = 1.0 ;
  if ((isnan(acos(cos_t)*180/PI))>0)
  theta[ft][2] = theta[ft-1][2];
  else theta[ft][2] = acos(cos_t)*180/PI;

  a = pow((values[r5][2]-values[r0][2]),2) + pow((values[r5][3]-values[r0][3]),2);
  a = pow(a,0.5);
  b = pow((values[r4][2]-values[r0][2]),2) + pow((values[r4][3]-values[r0][3]),2);
  b = pow(b,0.5);
  c = pow((values[r5][2]-values[r4][2]),2) + pow((values[r5][3]-values[r4][3]),2);
  c = pow(c,0.5);
  cos_t = (pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b);
  if (cos_t < -1.0) cos_t = -1.0 ;
  if (cos_t > 1.0) cos_t = 1.0 ;
  theta[ft][3] = acos(cos_t)*180/PI;

  a = pow((values[r5][2]-values[r0][2]),2) + pow((values[r5][3]-values[r0][3]),2);
  a = pow(a,0.5);
  b = pow((values[r3][2]-values[r0][2]),2) + pow((values[r3][3]-values[r0][3]),2);
  b = pow(b,0.5);
  c = pow((values[r5][2]-values[r3][2]),2) + pow((values[r5][3]-values[r3][3]),2);
  c = pow(c,0.5);
  cos_t = (pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b);
  if (cos_t < -1.0) cos_t = -1.0 ;
  if (cos_t > 1.0) cos_t = 1.0 ;
  theta[ft][4] = acos(cos_t)*180/PI;
//cout<<theta[ft][0]<<" "<<theta[ft][1]<<" "<<theta[ft][2]<<" "<<theta[ft][3]<<" "<<theta[ft][4]<<endl;
//cout<<ft<<endl;
}


//----------------------------------------------------------------------------------------------


//Histograms...
float d_max[5] = {0};
float t_max[5] = {0};
float d_min[5] = {0};
float t_min[5] = {0};
float d_range[5] = {0};
float t_range[5] = {0};
float d_binsize[5] = {0};
float t_binsize[5] = {0};

for (int w=0; w < 5; w++)
{
    for (int q = 0; q < frames; q++)
    {
      if (dist[q][w] > d_max[w])
      {
      d_max[w] = dist[q][w];
      }
      if (theta[q][w] > t_max[w])
      {
      t_max[w] = theta[q][w];
      }
	  if (dist[q][w] < d_min[w])
      {
      d_min[w] = dist[q][w];
      }
	  if (theta[q][w] < t_min[w])
      {
      t_min[w] = theta[q][w];
      }
    }
   d_range[w] = d_max[w] - d_min[w];
   t_range[w] = t_max[w] - t_min[w];
   d_binsize[w] = d_range[w]/N;
   t_binsize[w] = t_range[w]/M;

}

float ref_dist[frames][5];
float ref_theta[frames][5];


	for (int w = 0; w < 5; w++)
    {
	  for (int q = 0; q < frames; q++)
      {
	  ref_dist[q][w] = dist[q][w] - d_min[w];
	  ref_theta[q][w] = theta[q][w] - t_min[w];
	  }
	}


vector<float> histogram((5*(M+N)),0);

	for (int q = 0; q < frames; q++)
    {
	 int bin_number[10] = {0};
	 for (int w = 0; w < 5; w++)
	 {
	  bin_number[w] = int (ceil(ref_dist[q][w]/d_binsize[w]));
	  bin_number[w+N] = int (ceil(ref_theta[q][w]/t_binsize[w]));
		if(bin_number[w] == 0)
		{
          bin_number[w] = 1;
        }
        if(bin_number[w+N] == 0)
		{
          bin_number[w+N] = 1;
        }
	 }
	  for(int w=0;w<5;w++)
	  {
	   histogram[(w*N)+((bin_number[w])-1)] = histogram[(w*N)+((bin_number[w])-1)] + 1;
	   histogram[((5*N)+(w*5))+((bin_number[5+w])-1)] = histogram[((5*N)+(w*5))+((bin_number[5+w])-1)] + 1;
	  }
    }


//------------------------------------------------------------------------------------------------


//Output File...
	ofstream outFile;
	//outFile.open("rad_d1",ofstream::app);
	outFile.open("RAD_TRAIN",ofstream::app);
	//outFile.open("rad_d1.t",ofstream::app);
	//outFile.open("RAD_TEST",ofstream::app);
	for (int i=0;i<5*(M+N);i++)
	{
    if(i==0)
    {
    	if(count/12+1==1)
    		outFile<<"8"<<" ";
    	else if(count/12+1==2)
    		outFile<<"10"<<" ";
    	else if(count/12+1==3)
    		outFile<<"12"<<" ";
    	else if(count/12+1==4)
    		outFile<<"13"<<" ";
    	else if(count/12+1==5)
    		outFile<<"15"<<" ";
    	else if(count/12+1==6)
    		outFile<<"16"<<" ";

    	/*if(count/8+1==1)
    		outFile<<"8"<<" ";
    	else if(count/8+1==2)
    		outFile<<"10"<<" ";
    	else if(count/8+1==3)
    		outFile<<"12"<<" ";
    	else if(count/8+1==4)
    		outFile<<"13"<<" ";
    	else if(count/8+1==5)
    		outFile<<"15"<<" ";
    	else if(count/8+1==6)
    		outFile<<"16"<<" ";*/
    	
    }
    outFile <<i+1<<":"<< histogram[i]/frames<<" ";
	}

	outFile << "\n";
	outFile.close();

}

return 0;
}


//-------------------------------------------------------------------------------------------------


//File Names..
void InputFile()
{
ofstream files("filenames.txt");
int a = 8;

  switch (a)
  {
  	case (8) : for (int s=1; s<7; s++)
	{
	  for (int e=1; e<3; e++)
	  {
		if(a<10)
		files << "dataset/train/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else
		files << "dataset/train/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
	  }
	}
	a=a+2;

  	case (10) : for (int s=1; s<7; s++)
	{
	  for (int e=1; e<3; e++)
	  {
		if(a<10)
		files << "dataset/train/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else
		files << "dataset/train/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
	  }

	}
	a=a+2;
  	case (12) : for (int s=1; s<7; s++)
	{
	  for (int e=1; e<3; e++)
	  {
		if(a<10)
		files << "dataset/train/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else
		files << "dataset/train/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
	  }
	}
	a=a+1;
  	case (13) : for (int s=1; s<7; s++)
	{
	  for (int e=1; e<3; e++)
	  {
		if(a<10)
		files << "dataset/train/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else
		files << "dataset/train/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
	  }
	}
	a = a+2;
	case (15) : for (int s=1; s<7; s++)
	{
	  for (int e=1; e<3; e++)
	  {
		if(a<10)
		files << "dataset/train/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else
		files << "dataset/train/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
	  }
	}
	a = a+1;
	case (16) : for (int s=1; s<7; s++)
	{
	  for (int e=1; e<3; e++)
	  {
		if(a<10)
		files << "dataset/train/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else
		files << "dataset/train/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
	  }
	}
	break;
  }
  /*switch (a)
  {
  	case (8) : for (int s=7; s<11; s++)
	{
	  for (int e=1; e<3; e++)
	  {
		if(a>9 && s<10)
		files << "dataset_full/Test/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else if(a<10 && s>9)
		files << "dataset_full/Test/a0"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else if(a>9 && s>9)
		files << "dataset_full/Test/a"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else
		files << "dataset_full/Test/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
	  }
	}
	a = a+2;

  	case (10) : for (int s=7; s<11; s++)
	{
	  for (int e=1; e<3; e++)
	  {
		if(a>9 && s<10)
		files << "dataset_full/Test/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else if(a<10 && s>9)
		files << "dataset_full/Test/a0"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else if(a>9 && s>9)
		files << "dataset_full/Test/a"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else
		files << "dataset_full/Test/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
	  }
	}
	a = a+2;
  	case (12) : for (int s=7; s<11; s++)
	{
	  for (int e=1; e<3; e++)
	  {
		if(a>9 && s<10)
		files << "dataset_full/Test/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else if(a<10 && s>9)
		files << "dataset_full/Test/a0"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else if(a>9 && s>9)
		files << "dataset_full/Test/a"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else
		files << "dataset_full/Test/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
	  }
	}
	a = a+1;
  	case (13) : for (int s=7; s<11; s++)
	{
	  for (int e=1; e<3; e++)
	  {
		if(a>9 && s<10)
		files << "dataset_full/Test/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else if(a<10 && s>9)
		files << "dataset_full/Test/a0"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else if(a>9 && s>9)
		files << "dataset_full/Test/a"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else
		files << "dataset_full/Test/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
	  }
	}
	a = a+2;
	case (15) : for (int s=7; s<11; s++)
	{
	  for (int e=1; e<3; e++)
	  {
		if(a>9 && s<10)
		files << "dataset_full/Test/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else if(a<10 && s>9)
		files << "dataset_full/Test/a0"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else if(a>9 && s>9)
		files << "dataset_full/Test/a"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else
		files << "dataset_full/Test/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
	  }
	}
	a = a+1;
	case (16) : for (int s=7; s<11; s++)
	{
	  for (int e=1; e<3; e++)
	  {
		if(a>9 && s<10)
		files << "dataset_full/Test/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else if(a<10 && s>9)
		files << "dataset_full/Test/a0"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else if(a>9 && s>9)
		files << "dataset_full/Test/a"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
		else
		files << "dataset_full/Test/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
	  }
	}
	break;
  }
*/


 files.close();

	ifstream readFile;
	readFile.open("filenames.txt");
	string str;
	string temp;
	while(getline(readFile, str))
	{
	  	stringstream stream(str);
			while (stream >> temp)
			{
			 file.push_back(temp);
			}
	}
 readFile.close();
}
