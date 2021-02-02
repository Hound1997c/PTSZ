#ifndef ANT
#define ANT

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <fstream>
#include<iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <limits>
#include <chrono>
#include <dirent.h>
#include <math.h>

#define ALFA 0.95
#define BETA 0.1
#define ITERAT 50 ///100
#define PHRMN 5
#define RO 0.75
#define HOW_MANY_ANTS 10
#define PHRM_INC 1
#define INIT_PHRM 1

using namespace std;


struct Job
{
    int index;
    int p,r;
    int available=true;

    struct ByR
    {
        inline bool operator() (const Job& struct1, const Job& struct2)
        {
            return (struct1.r < struct2.r);
        }
    };
};

struct Procesor
{
    int index;
    float slowness;
    //vector<int> proc_jobs;
    double cmax=0;
    double flow=0;
    double ** phermone_Matrix;
    double all_Jobs_Num;

    void initPhermoneMatrix(int n)
    {
        this->all_Jobs_Num=n;
        this->phermone_Matrix = new double * [n];
        for(int i=0;i<n;i++)
        {
            phermone_Matrix[i] = new double [n];
            for(int j=0;j<n;j++) phermone_Matrix[i][j]=INIT_PHRM;
        }
    }

    void showPhermoneMatrix()
    {   
        int n=this->all_Jobs_Num;
        cout<<endl<<endl;
        for(int i=0;i<n;i++,cout<<endl)
            for(int j=0;j<n;j++) cout<<phermone_Matrix[i][j]<<" ";
    }

    void evaporatePhermoneMatrix()
    {
        for(int row=0; row<this->all_Jobs_Num; row++)
            for(int col=0; col<this->all_Jobs_Num; col++)
                this->phermone_Matrix[row][col] *= RO;
    }

    void updetePhermoneMatrix(vector<int> job_solution, double ant_solution)
    {
        for(uint j=1;j<job_solution.size();j++)
        {
            int row = job_solution[j-1];
            int col = job_solution[j];
            this->phermone_Matrix[row][col]+=(PHRM_INC/ant_solution);
            //cout<<"solution: "<<ant_solution<<endl;
            //cout<<"inc pM: "<< (PHRM_INC/ant_solution);   
        }
    }

    void destroyPhermoneMatrix()
    {
        for(int i=0;i<this->all_Jobs_Num;i++) delete [] phermone_Matrix[i];
        delete [] phermone_Matrix;
    }

    void refresh()
    {
        this->cmax=0;
        this->flow=0;
        //this->proc_jobs.clear();
    }

    struct ByCmax
    {
        bool operator ()( const Procesor &a, const Procesor &b ) const
        { 
            return a.cmax < b.cmax;
        }
    };

    struct BySlow
    {
        inline bool operator() (const Procesor& struct1, const Procesor& struct2)
        {
            return (struct1.slowness > struct2.slowness);
        }
    };
};

struct Ant
{
    vector<int> procesors_solution [5];
    double mean_flow = 0;

    void clearAnt()
    {
        this->mean_flow=0;
        for(int p=0;p<5;p++) procesors_solution[p].clear();
    }

    void showSolution()
    {
        cout<<this->mean_flow<<endl;
        for(int i=0;i<5;i++)
        {
            for(uint j=0;j<this->procesors_solution[i].size();j++)
            {
                cout<<this->procesors_solution[i][j]<<" ";
            }
            cout<<endl;
        }
    }

    double saveSolution(int n, vector<Procesor> procesors)
    {
        double flow = 0;
        for(unsigned int p=0 ; p<procesors.size(); p++)
        {
            //cout<<endl<<"proc "<<p<<" slowness: "<<procesors[p].slowness<<endl;
            //for(uint j=0; j<procesors_solution[p].size(); j++) cout<<procesors_solution[p][j]<<" ";
            flow +=procesors[p].flow;
        }
        //cout<<"\nflow: ";
        this->mean_flow = round(flow/(double)n);
        return this->mean_flow;
    }
};

#endif