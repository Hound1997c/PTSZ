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

using namespace std;

struct Job
{
    int index;
    int p,r;
};


void generate_instance (string filename, int n, int p_range, int r_range)
{
    srand (time(NULL));

    cout<<"filename: "<<filename<<endl;


    Job * jobs_array = new Job [n];
    int max_r = 0;
    for(int i=0;i<n;i++)
    {
        if(i%5==0)
        {
            max_r+=r_range;
        }
        jobs_array[i].r = (rand() % max_r);
        jobs_array[i].p = rand() % p_range + 1;
    }

    for(int i=n-1;i>=0;i--)
    {
        int index_to_swap = rand() % (i+1);
        Job temp_job = jobs_array[i];
        jobs_array[i] = jobs_array[index_to_swap];
        jobs_array[index_to_swap] = temp_job;
    }


    ofstream outfile;
    outfile.open(filename);
    outfile<<n<<endl;
    cout<<"zapisanie maszyn\n";

    /*for(int i=0;i<5;i++)
    {
        outfile<<1.0 + i*0.25<<" ";
    }*/
    outfile<<1.0<<" "<<0.75<<" "<<0.5<<" "<<0.4<<" "<<0.3;
    outfile<<endl;

    cout<<"losowanie zadan\n";

    
    for(int i=0;i<n;i++)
    {
        if(i%5==0)
        {
            max_r+=r_range;
        }
        outfile<<jobs_array[i].p<<" ";
        outfile<<jobs_array[i].r<<endl;
    }
    delete [] jobs_array;
    /*cout<<"pomieszanie zadan\n";

     */

    outfile.close();
    //delete [] jobs_array_mixed;

} 

float count_simple_sheduling(vector<Job> jobs,vector<int> procesors)
{
   int Flow = 0;
   int jobs_per_procesor=jobs.size()/procesors.size();
   for(int i=0;i<procesors.size();i++)
   {
       cout<<"\nproc: "<<i<<endl;
       int cmax=0;
       for(int j=i*jobs_per_procesor;j<i*jobs_per_procesor+jobs_per_procesor;j++ )
       {
           Job * job = &jobs[j];
           cout<<"job: "<<job->p<<" "<<job->r<<" ";
           if(cmax<job->r) cmax+=job->r;
           cmax+=procesors[i]*job->p;
           Flow+= cmax-job->r;
       }
   }
   cout<<endl;
   int n=jobs.size();
   if(n == 0) return -1;
   else return Flow/n;
}

void read_jobs_and_procesors(string filename,vector<Job> &jobs, vector<int> &procesors)
{
    ifstream infile;
    infile.open(filename);

    int n;
    infile>>n;
    string line;
    cout<<"pobieramy linie\n";
    getline(infile,line);
    for(int i=0;i<5;i++)
    {
        int fastness;
        infile>>fastness;
        procesors.push_back(fastness);
    }
    for(int i=0;i<n;i++)
    {
        Job job;
        infile>>job.p>>job.r;
        jobs.push_back(job);
    }
    //for(int i=0;i<5;i++) cout<<"procesor: "<<procesors[i]<<endl;
    infile.close();
}

void count_flow_from_instances()
{

    const char* PATH = "./instances";
    DIR *dir = opendir(PATH);
    struct dirent *entry = readdir(dir);
    vector<string> index_instances;
    while (entry != NULL)
    {
        if (entry->d_type == DT_DIR)
            //printf("%s\n", entry->d_name);
            if(entry->d_name[0] == '1')
            {
                //printf("%s\n", entry->d_name);
                index_instances.push_back(entry->d_name);
            }

        entry = readdir(dir);
    }

    closedir(dir);
    

    
    for(int i=0;i<index_instances.size();i++)
    {
        cout<<endl<<index_instances[i]<<endl<<endl;
        for(int j=0;j<10;j++)
        {
            cout<<"j: "<<j<<endl;   
            vector<Job> jobs;
            vector<int> procesors;
            int inst_size=(j+1)*50;
            string input_name = "instances/" + index_instances[i] + "/"
                 + index_instances[i] + "_" + to_string(inst_size) + ".txt";
            read_jobs_and_procesors(input_name,jobs,procesors);
            cout<<count_simple_sheduling(jobs,procesors)<<endl;
            /*string output_name = "output/" +  index_instances[i] + "/"
                + index_instances[i] + "_" + to_string(inst_size)
                + "_out.txt"; */
            jobs.clear();
            procesors.clear();
            
        }
        
    }

}

void write_all_instances_132325()
{
    string dirfilename = "instances/132325/132325_";

    for(int num = 50;num<=500;num+=50)
    {
        string filename = dirfilename + to_string(num)+".txt";
        generate_instance(filename,num,10,20);
    }
}

int main()
{
    //zad1();
    //zad2("132325");
    write_all_instances_132325();
    //count_flow_from_instances();
    return 0;
}