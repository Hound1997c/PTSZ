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
    int p,r,d,w;
    bool available=true;
    int begin;
    int end;
};

struct less_parameter
{
    char arg;
    int value;
    inline bool operator() (const Job& job1, const Job& job2)
    {
        switch (arg)
        {
        case 'w':
            return (job1.w < job2.w);
            break;
        case 'v':
            return (abs(value - job1.w) < abs(value - job2.w));
        default:
            return (job1.d < job2.d);
            break;
        } 
    }
};

void read_jobs(Job * &jobs, string filename, int inst_size)
{
    ifstream infile;
    infile.open(filename);
    int n;
    infile>>n;
    if(n!=inst_size)
    {
        printf("niepoprawna instancja!\n");
        return;
    }
    //jobs = new Job [n];
    Job * job = jobs;
    for(int i=0;i<n;i++,job++)
    {
        infile>>job->p>>job->r>>job->d>>job->w;
        job->index = i;
    }
    infile.close();
    /*for(int i=0;i<n;i++)
    {
        printf("%i %i %i %i\n",jobs[i].p,jobs[i].r,jobs[i].d,jobs[i].w);
    } */
}

void display_instance(Job * jobs, int n)
{
    Job * job = jobs;
    for(int i = 0; i<n; i++)
    {
        printf("%i %i %i %i\n",job->p,job->r,job->d,job->w);
        job++;
    }
}

int shedule_variant(Job * &jobs,Job choosen_job,int n)
{
    less_parameter lp;
    lp.value = choosen_job.w;
    lp.arg = 'v';
    //sort(jobs.begin(),jobs.end(),lp);
    sort(jobs,jobs+n,lp);

    int criterion_value = 0;
    int cmax = 0;
    //for(vector<Job>::iterator cur_job = jobs.begin();cur_job != jobs.end(); ++cur_job)
    for(int i=0;i<n;i++)
    {   
        Job * cur_job = &jobs[i];
        if (cmax < cur_job->r) cmax = cur_job->r;
        cmax += cur_job->p;
        if (cmax > cur_job->d) criterion_value += cur_job->w;
    }
    return criterion_value;
}

void shedule_instance(Job * jobs, int n, string filename, int& time, int& crit)
{   
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
    int bestSolution = INT32_MAX;
    //cout<<"best solution: "<<bestSolution<<endl;
    int best_order [n];
    for(int i =0;i<n;i++) best_order[i]=i;
    Job * jobs_to_sort = new Job [n];
    //for(vector<Job>::iterator sure_job = jobs_vector.begin();sure_job != jobs_vector.end(); ++sure_job)
    for(int i=0;i<n;i++)
    {   
        for(int j=0;j<n;j++) jobs_to_sort[j]=jobs[j];
        int criterion_value = shedule_variant(jobs_to_sort,jobs[i],n);
        if(criterion_value < bestSolution)
        {
            //cout<<i<<" better\n";
            bestSolution = criterion_value;
            for(int j=0;j<n;j++) best_order[j]=jobs_to_sort[j].index;
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    delete [] jobs_to_sort;
    //cout<<bestSolution<<endl;
    ofstream outfile;
    //string new_filename = filename + "_out.txt";
    outfile.open(filename);
    outfile<<bestSolution<<endl;
    for(int j=0;j<n;j++) outfile<<best_order[j]<<" ";
    outfile.close();

    time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    crit = bestSolution;
}

int main(int argc, char* argv[]) 
{ 
    /*Job * jobs_to_shedule;
    int instance_size = 100;
    jobs_to_shedule = new Job [instance_size];
    string instance_name = "instances/132325_" + to_string(instance_size) + ".txt";
    read_jobs(jobs_to_shedule,instance_name,instance_size);
    string output_name = "output/132325_" + to_string(instance_size) + "_out.txt";
    cout<<shedule_instance(jobs_to_shedule,instance_size,output_name);
    //display_instance(jobs_to_shedule,instance_size);
    delete [] jobs_to_shedule;*/

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
    
    int times [10];
    int criterios [10];
    int * output = new int [2];


    for(int i=0;i<index_instances.size();i++)
    {
        cout<<endl<<index_instances[i]<<endl<<endl;
        Job * jobs_to_shedule;
        for(int j=0;j<10;j++)
        {   
            int inst_size=(j+1)*50;
            jobs_to_shedule = new Job [inst_size];
            string input_name = "instances/" + index_instances[i] + "/"
                 + index_instances[i] + "_" + to_string(inst_size) + ".txt";
            read_jobs(jobs_to_shedule,input_name,inst_size);
            string output_name = "output/" +  index_instances[i] + "/"
                + index_instances[i] + "_" + to_string(inst_size)
                + "_out.txt";
            
            int time,crit;
            shedule_instance(jobs_to_shedule,inst_size,output_name,time,crit);
            //cout<<time<<endl;
            times[j]=time;
            criterios[j]=crit;
        }
        cout<<endl<<"times: "<<endl;
        for(int j=0;j<10;j++) cout<<times[j]<<endl;
        cout<<endl<<"crit: "<<endl;
        for(int j=0;j<10;j++) cout<<criterios[j]<<endl;
        delete [] jobs_to_shedule;
    }

    delete [] output;
    return 0; 
} 
