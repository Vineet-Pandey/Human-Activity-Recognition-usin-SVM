
/* 
  Author:Vineet Pandey
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
#define pi 3.14

int N = 10;

int main()
{
    InputFile();

    for (int count = 0; count < 128; count++)
    {
        ifstream inFile;
        //inFile.open("../dataset_full/Train/a01_s01_e01_skeleton_proj.txt");
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
                //cout<<values[n][m]<<" ";
                m = m+1;
            }//cout<<endl;
        }

        int frames = (n+1)/20;
        inFile.close();



        float dx[frames][20];
        float dy[frames][20];
        float dz[frames][20];

        for (int n=0; n<frames; n++)
        {
            for (int j=0; j<20; j++)
            {
                dx[n][j] = values[(20*n)+j][2] - values[(20*n)+1][2];
                dy[n][j] = values[(20*n)+j][3] - values[(20*n)+1][3];
                dz[n][j] = values[(20*n)+j][4] - values[(20*n)+1][4];
            }
        }

        float dx_min[frames];
        float dx_max[frames];
        float dy_min[frames];
        float dy_max[frames];
        float dz_min[frames];
        float dz_max[frames];
        float dx_bin_num_size[frames];
        float dy_bin_num_size[frames];
        float dz_bin_num_size[frames];
        float dx_abs[frames][20];
        float dy_abs[frames][20];
        float dz_abs[frames][20];

        for(n=0; n<frames; n++)
        {
            dx_min[n] = dx[n][0];
            dy_min[n] = dy[n][0];
            dz_min[n] = dz[n][0];
            dx_max[n] = dx[n][0];
            dy_max[n] = dy[n][0];
            dz_max[n] = dz[n][0];

            for(int j=0; j<19; j++)
            {
                if(dx_min[n]>dx[n][j])
                {
                    dx_min[n]=dx[n][j];
                }
                if(dy_min[n]>dy[n][j])
                {
                    dy_min[n]=dy[n][j];
                }
                if(dz_min[n]>dz[n][j])
                {
                    dz_min[n]=dz[n][j];
                }
                if(dx_max[n]<dx[n][j])
                {
                    dx_max[n]=dx[n][j];
                }
                if(dy_max[n]<dy[n][j])
                {
                    dy_max[n]=dy[n][j];
                }
                if(dz_max[n]<dz[n][j])
                {
                    dz_max[n]=dz[n][j];
                }
            }
            dx_bin_num_size[n] = (dx_max[n] - dx_min[n])/N;
            dy_bin_num_size[n] = (dy_max[n] - dy_min[n])/N;
            dz_bin_num_size[n] = (dz_max[n] - dz_min[n])/N;
        }

        for(n=0; n<frames; n++)
        {
            for(int j=0; j<19; j++)
            {
                dx_abs[n][j] = dx[n][j] - dx_min[n];
                dy_abs[n][j] = dy[n][j] - dy_min[n];
                dz_abs[n][j] = dz[n][j] - dz_min[n];
            }
        }

        vector<float> hist(57*N,0);

        for(n=0; n<frames; n++)
        {
            int bin_num[3*19]= {0};
            for(int j=0; j<19; j++)
            {
                bin_num[j] = int(ceil(dx_abs[n][j]/dx_bin_num_size[n]));
                bin_num[19+j] = int(ceil(dy_abs[n][j]/dy_bin_num_size[n]));
                bin_num[38+j] = int(ceil(dz_abs[n][j]/dz_bin_num_size[n]));
                if(bin_num[j]==0)
                {
                    bin_num[j]=1;
                }
                if(bin_num[19+j]==0)
                {
                    bin_num[19+j]=1;
                }
                if(bin_num[38+j]==0)
                {
                    bin_num[38+j]=1;
                }
            }

            for(int k=0; k<19; k++)
            {
                hist[(k*N)+((bin_num[k])-1)]= hist[(k*N)+((bin_num[k])-1)] + 1;
                hist[((19*N)+(k*N))+((bin_num[19+k])-1)] = hist[((19*N)+(k*N))+((bin_num[19+k])-1)] + 1;
                hist[((38*N)+(k*N))+((bin_num[38+k])-1)] = hist[((38*N)+(k*N))+((bin_num[38+k])-1)] + 1;
            }
            //cout<<endl;
        }

        /*for(int i=0;i<3*(N)*19;i++)
        {
          cout<<(hist[i]/frames)<<endl;
        }*/




//Output File...
        ofstream outFile;
        //outFile.open("hjpd_d1",ofstream::app);//training dataset
        outFile.open("hjpd_d1.t",ofstream::app);//testing dataset
        for (int i=0; i<(57*N); i++)
        {
            if (i < 1)
            {
                outFile << endl;
            }
            outFile << hist[i]/frames<<" ";
        }
        outFile.close();

    }

    return 0;
}


//File Names..
void InputFile()
{
    ofstream files("files.txt");

    /*for (int a=1; a<17; a++)//training dataset
    {
    for (int s=1; s<7; s++)
    {
      for (int e=1; e<3; e++)
      {
    	if(a<10)
    	files << "/dataset_full/Train/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
    	else
    	files << "/dataset_full/Train/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
      }
    }
    }*/

    for (int a=1; a<17; a++)//testing dataset
    {
        for (int s=7; s<11; s++)
        {
            for (int e=1; e<3; e++)
            {
                if(a>9 && s<10)
                    files << "/dataset_full/Test/a"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
                else if(a<10 && s>9)
                    files << "/dataset_full/Test/a0"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
                else if(a>9 && s>9)
                    files << "/dataset_full/Test/a"<<a<<"_s"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
                else
                    files << "/dataset_full/Test/a0"<<a<<"_s0"<<s<<"_e0"<<e<<"_skeleton_proj.txt"<<endl;
            }
        }
    }


    files.close();

    ifstream readFile;
    readFile.open("files.txt");
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
