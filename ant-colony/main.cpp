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
#include "Ant.cpp"
#include <limits.h>
#include <float.h>

int getLaziestProcIndex(vector<Procesor> procesors)
{
    int minCmax = 0;
    for(uint i = 1; i<procesors.size();i++) if(procesors[i].cmax<procesors[minCmax].cmax) minCmax = i;
    return minCmax;
}

int chooseRandomJob(int jobs_number, Procesor * procesor, int last_job_index, int * jobs_indexes)
{
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        //cout<<"r: "<<r<<endl;
        double max_prob = 0;
        double max_prob_index=0;
        double down = 0;
        for (int ji=0;ji<jobs_number;ji++)
            down+=pow(procesor->phermone_Matrix[last_job_index][jobs_indexes[ji]],ALFA);
        //cout<<"down: "<<down<<endl;
        for(int j=0;j<jobs_number;j++)
        {
            double up = pow(procesor->phermone_Matrix[last_job_index][jobs_indexes[j]],ALFA);
            double choose_probablity = up/down;
            //cout<<"prob: "<<choose_probablity<<endl;
            if(r < choose_probablity) 
            {
                //cout<<"prob chooen job: "<<j<<endl;
                return j;
            }
            /*if(choose_probablity > max_prob) 
            {
                max_prob=choose_probablity;
                max_prob_index=j;
            }*/
        }
        //cout<<"choosen job: "<<max_prob_index<<endl;
        //return max_prob_index;
        //cout<<"rand choose: \n";
        return rand() % jobs_number;
}


void read_jobs_and_procesors(string filename,vector<Job> &jobs, vector<Procesor> &procesors)
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
        Procesor proc;
        float slowness;
        infile>>slowness;
        proc.slowness=slowness;
        proc.index=i;
        procesors.push_back(proc);
    }
    for(int i=0;i<n;i++)
    {
        Job job;
        job.index=i;
        int p,r;
        infile>>p>>r;
        job.p=p;
        job.r=r;
        jobs.push_back(job);
    }
    //for(int i=0;i<5;i++) cout<<"procesor: "<<procesors[i]<<endl;
    infile.close();
}


void show_jobs(vector<Job> jobs)
{
    cout<<"show jobs\n";
    for(uint i=0;i<jobs.size();i++)
    {
        cout<<jobs[i].r<<" ";
    }
    cout<<endl;
}

double * get_min_instance_flow(string filename, string outname)
{
    //string filename = "instances/132325/132325_50.txt";
    vector<Job> oryginal_jobs;
    vector<Procesor> processors;
    read_jobs_and_procesors(filename,oryginal_jobs,processors);
    std::sort(processors.begin(), processors.end(), Procesor::BySlow());
    cout<<"procesory\n";
    for(uint p=0;p<processors.size();cout<<processors[p].slowness<<" ", p++) processors[p].initPhermoneMatrix(oryginal_jobs.size());
    cout<<endl;
    
    Ant ant_array[HOW_MANY_ANTS];
    Ant best_ant;
    double min_Total_Flow = DBL_MAX;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for(int iterat = 0;iterat < ITERAT ; iterat++)
    {
        //cout<<"iteracja: "<<iterat<<endl;
        for(int ant=0; ant < HOW_MANY_ANTS; ant++)
        {
            //cout<<"mrówka: "<<ant<<endl;
            int * jobs_indexes = new int [oryginal_jobs.size()];
            for(uint j=0;j<oryginal_jobs.size();j++) jobs_indexes[j]=oryginal_jobs[j].index;
            
            //cout<<"przedwhile\n";
            int choosen_Job_Index;
            for(uint jobs_to_shedule = oryginal_jobs.size(); jobs_to_shedule>0; jobs_to_shedule--)
            {
                //cout<<jobs_to_shedule<<endl;
                int laziest_Proc_Index = getLaziestProcIndex(processors);
                Procesor * proc = &processors[laziest_Proc_Index];
                
                if(jobs_to_shedule==oryginal_jobs.size()) choosen_Job_Index=rand () % jobs_to_shedule;
                else choosen_Job_Index=chooseRandomJob(jobs_to_shedule,proc,choosen_Job_Index,jobs_indexes);
                
                //cout<<"choosen job: "<<choosen_Job_Index<<endl;
                Job * job = &oryginal_jobs[jobs_indexes[choosen_Job_Index]];

                //proc->proc_jobs.push_back(jobs_indexes[choosen_Job_Index]);
                ant_array[ant].procesors_solution[laziest_Proc_Index].push_back(jobs_indexes[choosen_Job_Index]);


                if(proc->cmax < job->r) proc->cmax = job->r;
                proc->cmax+= (double)job->p/(double)proc->slowness;
                proc->flow+= proc->cmax-job->r;
                jobs_indexes[choosen_Job_Index]=jobs_indexes[jobs_to_shedule-1];
            }
            //cout<<"powhile\n";
            double flow = ant_array[ant].saveSolution(oryginal_jobs.size(),processors);
            //cout<<"mean flow: "<<ant_array[ant].mean_flow<<endl;
            if(flow < min_Total_Flow) 
            {
                min_Total_Flow = flow;
                best_ant = ant_array[ant];
                cout<<"newflow: "<<flow<<endl;
                //ant_array[ant].showSolution();
            }
            
            delete [] jobs_indexes;
            for(uint p=0 ; p<processors.size(); p++) processors[p].refresh();
            
        }
        //if(min_Iteration_Flow < min_Total_Flow) min_Total_Flow = min_Iteration_Flow;

        for(uint p=0; p<processors.size();p++)
        {
            processors[p].evaporatePhermoneMatrix();
            //cout<<"proc: "<<p<<endl;
            for(uint a=0; a<HOW_MANY_ANTS; a++)
            {
                /*if(iterat%10==0)
                {
                    cout<<"ant "<<a<<"flow: "<<ant_array[a].mean_flow<<endl;
                    for(uint ap=0; ap<ant_array[a].procesors_solution[p].size() ; ap++)
                        cout<<ant_array[a].procesors_solution[p][ap]<<" ";
                    cout<<endl;
                }*/
                processors[p].updetePhermoneMatrix(ant_array[a].procesors_solution[p],ant_array[a].mean_flow);
            }
        }
        for(int a=0;a<HOW_MANY_ANTS;a++) ant_array[a].clearAnt();
        //if(iterat==50) for(uint p=0;p<processors.size();p++) processors[p].showPhermoneMatrix();
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    double algo_time = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    cout<<"minimum flow: "<<min_Total_Flow<<endl;
    ofstream outfile;
    outfile.open(outname);
    outfile<<best_ant.mean_flow<<endl;
    for(uint p=0;p<5;p++)
    {
        for(uint j=0;j<best_ant.procesors_solution[p].size();j++)
            outfile<<best_ant.procesors_solution[p][j]<<" ";
        cout<<endl;
    }
    outfile.close();
    //for(uint p=0;p<processors.size();p++) processors[p].showPhermoneMatrix();
    for(uint p=0;p<processors.size();p++) processors[p].destroyPhermoneMatrix();

    

    processors.clear();
    oryginal_jobs.clear();

    double * out_array = new double [2];
    out_array[0]= min_Total_Flow;
    out_array[1]= algo_time;
    return out_array;
}

int main()
{
    srand(time(NULL));

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
    
    ofstream simple_outputs, algo_times_file;
    simple_outputs.open("solutions.txt");
    algo_times_file.open("algoTimes.txt");
    
    for(uint i=0;i<index_instances.size();i++)
    {
        cout<<endl<<index_instances[i]<<endl<<endl;
        simple_outputs<<endl<<index_instances[i]<<endl<<endl;
        algo_times_file<<endl<<index_instances[i]<<endl<<endl;
        for(int j=0;j<10;j++)
        {   
            //vector<Job> jobs;
            //vector<float> procesors;
            int inst_size=(j+1)*50;
            cout<<"inst: "<<inst_size<<endl;
            string input_name = "instances/" + index_instances[i] + "/"
                 + index_instances[i] + "_" + to_string(inst_size) + ".txt";
            string output_name = "out/132325/" + index_instances[i] + "_"
                + to_string(inst_size) + ".txt";
            //read_jobs_and_procesors(input_name,jobs,procesors);
            //cout<<"procesors: "<<procesors.size()<<endl;
            //cout<<"jobs: "<<jobs.size()<<endl;
            double * solution = get_min_instance_flow(input_name,output_name);
            long double css = *solution;
            long double alg_time = *(solution+1);
            delete [] solution;
            //cout<<css<<endl;
            simple_outputs<<css<<endl;
            algo_times_file<<alg_time<<endl;
            /*string output_name = "output/" +  index_instances[i] + "/"
                + index_instances[i] + "_" + to_string(inst_size)
                + "_out.txt"; */
            //jobs.clear();
            //cout<<"jobs: "<<jobs.size()<<endl;
            //procesors.clear();
            //cout<<"procesors: "<<procesors.size()<<endl;
            
        }
        
    }
    simple_outputs.close();
    algo_times_file.close();




    
}