/*
Author: Vineet Pandey
Human Centered Robotics
*/

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <math.h>

using namespace std;

vector<float> file_names(72);

#define pi 3.14

float distance(float x1, float y1, float x2, float y2);
float angle(float x0, float y0, float x1, float y1);

int theta_bins=8;
float theta_range=360/theta_bins;

void readfile();
vector<string> file;

int training_dataset=192;
int testing_dataset=128;

int main()
{
    ofstream outFile;

    readfile();
    for(int file_number=0; file_number<training_dataset; file_number++)
    {
        string line;
        string str;

        ifstream files;
        files.open(file[file_number].c_str());

        int i=0;
        int y=0;
        while(getline(files,line))
        {
            i++;
        }
        int x = i-1;
        int number_of_lines=x;
        vector<vector<float> > v(number_of_lines, vector<float>(5,0));

        i=0;
        files.clear();
        files.seekg(0);
        while(getline(files,str))
        {
            stringstream stream(str);
            float num;
            y=0;
            while(stream>>num)
            {
                if(num==0)
                {
                    if(i>=40)
                    {
                        v[i][2]=2*(v[i-20][2])-(v[i-40][2]);
                        v[i][3]=2*(v[i-20][3])-(v[i-40][3]);
                        v[i][4]=2*(v[i-20][4])-(v[i-40][4]);
                    }
                    else if(i>=20 && i<40)
                    {
                        v[i][2]=(v[i-20][2]);
                        v[i][3]=(v[i-20][3]);
                        v[i][4]=(v[i-20][4]);
                    }
                    else
                    {
                        v[i][2]=0;
                        v[i][3]=0;
                        v[i][4]=0;
                    }
                }
                else
                {
                    v[i][y] = num;
                }
                y = y+1;
            }
            i++;
        }

        files.close();
        int nframes=x/20;
        vector<vector<vector<float> > > p_xy (nframes,vector<vector<float> >(20,vector <float>(2,0)));
        vector<vector<vector<float> > > p_yz (nframes,vector<vector<float> >(20,vector <float>(2,0)));
        vector<vector<vector<float> > > p_xz (nframes,vector<vector<float> >(20,vector <float>(2,0)));

        //-------------------------------main program begins
        int ninst = nframes-1;
        int level_2_1_instances = ninst/2;
        int level_2_2_instances = ninst-level_2_1_instances;
        int level_2_1_1_instances = level_2_1_instances/2;
        int level_2_1_2_instances = level_2_1_instances - level_2_1_1_instances;
        int level_2_2_1_instances = level_2_2_instances/2;
        int level_2_2_2_instances = level_2_2_instances - level_2_2_1_instances;




        vector<vector<float> > d_xy(20, vector<float>(ninst,0));
        vector<vector<float> > d_yz(20, vector<float>(ninst,0));
        vector<vector<float> > d_xz(20, vector<float>(ninst,0));

        vector<vector<float> > t_xy(20, vector<float>(ninst,0));
        vector<vector<float> > t_yz(20, vector<float>(ninst,0));
        vector<vector<float> > t_xz(20, vector<float>(ninst,0));

        vector<vector<float> > q_xy(20, vector<float>(ninst,0));
        vector<vector<float> > q_yz(20, vector<float>(ninst,0));
        vector<vector<float> > q_xz(20, vector<float>(ninst,0));

        vector<vector<float> > h_xy(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_yz(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_xz(20,vector <float>(theta_bins,0));

        vector<vector<float> > h_xy_level_2_1(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_yz_level_2_1(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_xz_level_2_1(20,vector <float>(theta_bins,0));

        vector<vector<float> > h_xy_level_2_2(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_yz_level_2_2(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_xz_level_2_2(20,vector <float>(theta_bins,0));

        vector<vector<float> > h_xy_level_2_1_1(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_yz_level_2_1_1(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_xz_level_2_1_1(20,vector <float>(theta_bins,0));

        vector<vector<float> > h_xy_level_2_1_2(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_yz_level_2_1_2(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_xz_level_2_1_2(20,vector <float>(theta_bins,0));

        vector<vector<float> > h_xy_level_2_2_1(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_yz_level_2_2_1(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_xz_level_2_2_1(20,vector <float>(theta_bins,0));

        vector<vector<float> > h_xy_level_2_2_2(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_yz_level_2_2_2(20,vector <float>(theta_bins,0));
        vector<vector<float> > h_xz_level_2_2_2(20,vector <float>(theta_bins,0));

        vector<vector<float> > h_3D(20,vector <float>(21*theta_bins,0));
        vector<float> h_3D_final((20*21*theta_bins),0);
        //using training instance for our hogram vector computation
        for(int u=0; u<nframes; u++)
        {
            //frame instances
            for(int i=0; i<20; i++)
            {
                for(int n=0; n<2; n++)
                {
                    p_xy[u][i][n]=v[(20*u)+i][2+n];
                    p_yz[u][i][n]=v[(20*u)+i][3+n];
                    p_xz[u][i][n]=v[(20*u)+i][2+(2*n)];
                }
            }
        }

        for(int m=0; m<ninst; m++)
        {
            for(int i=0; i<20; i++)
            {
                d_xy[i][m]=distance(p_xy[m][i][0],p_xy[m][i][1],p_xy[m+1][i][0],p_xy[m+1][i][1]);
                d_yz[i][m]=distance(p_yz[m][i][0],p_yz[m][i][1],p_yz[m+1][i][0],p_yz[m+1][i][1]);
                d_xz[i][m]=distance(p_xz[m][i][0],p_xz[m][i][1],p_xz[m+1][i][0],p_xz[m+1][i][1]);

                t_xy[i][m]=floor(angle(p_xy[m][i][0],p_xy[m][i][1],p_xy[m+1][i][0],p_xy[m+1][i][1])*100)/100;   //we use this to rund off all the values to the 2 decimal digits
                t_yz[i][m]=floor(angle(p_yz[m][i][0],p_yz[m][i][1],p_yz[m+1][i][0],p_yz[m+1][i][1])*100)/100;
                t_xz[i][m]=floor(angle(p_xz[m][i][0],p_xz[m][i][1],p_xz[m+1][i][0],p_xz[m+1][i][1])*100)/100;
            }
        }

        for(int m=0; m<ninst; m++)
        {
            for(int i=0; i<20; i++)
            {
                if(t_xy[i][m]<0)
                {
                    t_xy[i][m]=(t_xy[i][m])+360;
                }
                if(t_yz[i][m]<0)
                {
                    t_yz[i][m]=(t_yz[i][m])+360;
                }
                if(t_xz[i][m]<0)
                {
                    t_xz[i][m]=(t_xz[i][m])+360;
                }
            }
        }
        //compute the quadtrant for each of theta angles
        //we divide all the angles into 8 bins
        for(int m=0; m<ninst; m++)
        {
            for(int i=0; i<20; i++)
            {
                q_xy[i][m]=ceil(t_xy[i][m]/theta_range);
                q_yz[i][m]=ceil(t_yz[i][m]/theta_range);
                q_xz[i][m]=ceil(t_xz[i][m]/theta_range);

                if(q_xy[i][m]==0)
                {
                    q_xy[i][m]=1;                            //making sure angle zero is also in quadtrant 1
                }
                if(q_yz[i][m]==0)
                {
                    q_yz[i][m]=1;
                }
                if(q_xz[i][m]==0)
                {
                    q_xz[i][m]=1;
                }
            }
        }
        //printing all the quadrant values

        for(int m=0; m<ninst; m++)
        {
            for(i=0; i<20; i++)
            {
                h_xy[i][q_xy[i][m]-1] = h_xy[i][q_xy[i][m]-1] + d_xy[i][m];
                h_yz[i][q_yz[i][m]-1] = h_yz[i][q_xy[i][m]-1] + d_yz[i][m];
                h_xz[i][q_xz[i][m]-1] = h_xz[i][q_xy[i][m]-1] + d_xz[i][m];
            }
        }
        for(int m=0; m< level_2_1_instances ; m++)
        {
            for(i=0; i<20; i++)
            {
                h_xy_level_2_1[i][q_xy[i][m]-1]  =  h_xy_level_2_1[i][q_xy[i][m]-1]  +  d_xy[i][m];
                h_yz_level_2_1[i][q_yz[i][m]-1]  =  h_yz_level_2_1[i][q_xy[i][m]-1]  +  d_yz[i][m];
                h_xz_level_2_1[i][q_xz[i][m]-1]  =  h_xz_level_2_1[i][q_xy[i][m]-1]  + d_xz[i][m];
            }
        }
        for(int m=0; m< level_2_2_instances ; m++)
        {
            for(i=0; i<20; i++)
            {
                h_xy_level_2_2[i][q_xy[i][m]-1] = h_xy_level_2_2[i][q_xy[i][m]-1] + d_xy[i][m + level_2_1_instances];
                h_yz_level_2_2[i][q_yz[i][m]-1] = h_yz_level_2_2[i][q_yz[i][m]-1] + d_yz[i][m + level_2_1_instances];
                h_xz_level_2_2[i][q_xz[i][m]-1] = h_xz_level_2_2[i][q_xz[i][m]-1] + d_xz[i][m + level_2_1_instances];
            }
        }
        for(int m=0; m< level_2_1_1_instances ; m++)
        {
            for(i=0; i<20; i++)
            {
                h_xy_level_2_1_1[i][q_xy[i][m]-1] = h_xy_level_2_1_1[i][q_xz[i][m]-1] + d_xy[i][m];
                h_yz_level_2_1_1[i][q_yz[i][m]-1] = h_yz_level_2_1_1[i][q_xz[i][m]-1] + d_yz[i][m];
                h_xz_level_2_1_1[i][q_xz[i][m]-1] = h_xz_level_2_1_1[i][q_xz[i][m]-1] + d_xz[i][m];
            }
        }
        for(int m=0; m< level_2_1_2_instances ; m++)
        {
            for(i=0; i<20; i++)
            {
                h_xy_level_2_1_2[i][q_xy[i][m]-1] = h_xy_level_2_1_2[i][q_xy[i][m]-1] + d_xy[i][m + level_2_1_1_instances];
                h_yz_level_2_1_2[i][q_yz[i][m]-1] = h_yz_level_2_1_2[i][q_xy[i][m]-1] + d_yz[i][m + level_2_1_1_instances];
                h_xz_level_2_1_2[i][q_xz[i][m]-1] = h_xz_level_2_1_2[i][q_xy[i][m]-1] + d_xz[i][m + level_2_1_1_instances];
            }
        }
        for(int m=0; m< level_2_2_1_instances ; m++)
        {
            for(i=0; i<20; i++)
            {
                h_xy_level_2_2_1[i][q_xy[i][m]-1] = h_xy_level_2_2_1[i][q_xy[i][m]-1] + d_xy[i][m + level_2_1_instances];
                h_yz_level_2_2_1[i][q_yz[i][m]-1] = h_yz_level_2_2_1[i][q_xy[i][m]-1] + d_yz[i][m + level_2_1_instances];
                h_xz_level_2_2_1[i][q_xz[i][m]-1] = h_xz_level_2_2_1[i][q_xy[i][m]-1] + d_xz[i][m + level_2_1_instances];
            }
        }
        for(int m=0; m< level_2_2_2_instances ; m++)
        {
            for(i=0; i<20; i++)
            {
                h_xy_level_2_2_2[i][q_xy[i][m]-1] = h_xy_level_2_2_2[i][q_xy[i][m]-1] + d_xy[i][m + level_2_1_instances + level_2_2_1_instances];
                h_yz_level_2_2_2[i][q_yz[i][m]-1] = h_yz_level_2_2_2[i][q_xy[i][m]-1] + d_yz[i][m + level_2_1_instances + level_2_2_1_instances];
                h_xz_level_2_2_2[i][q_xz[i][m]-1] = h_xz_level_2_2_2[i][q_xy[i][m]-1] + d_xz[i][m + level_2_1_instances + level_2_2_1_instances];
            }
        }

        for(int i=0; i<20; i++)
        {
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][j] = h_xy[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][j+theta_bins] = h_xy_level_2_1[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(2*theta_bins) + j] = h_xy_level_2_2[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(3*theta_bins)+j] = h_xy_level_2_1_1[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(4*theta_bins)+j] = h_xy_level_2_1_2[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(5*theta_bins)+j] = h_xy_level_2_2_1[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(6*theta_bins)+j] = h_xy_level_2_2_2[i][j];
            }

            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(7*theta_bins)+j] = h_yz[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(8*theta_bins)+j] = h_yz_level_2_1[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(9*theta_bins)+j] = h_yz_level_2_2[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(10*theta_bins)+j] = h_yz_level_2_1_1[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(11*theta_bins)+j] = h_yz_level_2_1_2[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(12*theta_bins)+j] = h_yz_level_2_2_1[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(13*theta_bins)+j] = h_yz_level_2_2_2[i][j];
            }


            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(14*theta_bins)+j] = h_xz[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(15*theta_bins)+j] = h_xz_level_2_1[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(16*theta_bins)+j] = h_xz_level_2_2[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(17*theta_bins)+j] = h_xz_level_2_1_1[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(18*theta_bins)+j] = h_xz_level_2_1_2[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(19*theta_bins)+j] = h_xz_level_2_2_1[i][j];
            }
            for(int j=0; j < theta_bins; j++)
            {
                h_3D[i][(20*theta_bins)+j] = h_xz_level_2_2_2[i][j];
            }

        }
        //3D hogram;
        for(i=0; i<20; i++)
        {
            for(int j=0; j<(21*theta_bins); j++)
            {
                h_3D_final[(i*(21*theta_bins))+j]=h_3D[i][j];
            }
        }


        //outFile.open("hod_d1",ofstream::app);//train dataset
        outFile.open("hod_d1.t",ofstream::app);//test dataset

        for (int i=0; i<(21*20*theta_bins); i++)
        {
            if (i<((21*20*theta_bins)-1))
            {
                outFile << h_3D_final[i];
                outFile << " ";
            }
            else
            {
                outFile << h_3D_final[i];
                outFile << "\n";
            }
        }
        cout<<file_number<<endl;
        outFile.close();
    }
    return 0;

}

float distance(float x1, float y1, float x2, float y2)
{
    float x = (x1 - x2)*(x1 - x2);
    float y = (y1 - y2)*(y1 - y2);

    float dist_sqr = x+y;
    dist_sqr = sqrt(dist_sqr);
    return dist_sqr;
}

float angle(float x0, float y0, float x1, float y1)
{
    float a1= atan2((y1-y0),(x1-x0))*180/pi;

    if(a1<0)
    {
        a1=a1+360;
    }


    return abs(a1);
}

void readfile()
{
    ofstream files("files.txt");


    for (int a=1; a<17; a++)//test data set
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
    }
    //Training dataset
    /*
      for (int a=1; a<17; a++)
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
    */
    files.close();
    ifstream myFile;
    myFile.open("files.txt");
    string str;
    string temp;
    while(getline(myFile, str))
    {
        stringstream stream(str);
        while (stream >> temp)
        {
            file.push_back(temp);
        }
    }
    myFile.close();
}
