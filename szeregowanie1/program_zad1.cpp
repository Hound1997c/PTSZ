#include<iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <fstream>
#include <string>

using namespace std;

struct Job
{
    int index;
    int p,r,d,w;
};

Job * jobs;
int * order_of_jobs;

void generate_instance_past (string filename, int n, int max_p, int max_w, int max_r, int max_chill_rpd)
{
    srand (time(NULL));

    filename+=("_n_"+to_string(n)+"_p_"+
            to_string(max_chill_rpd)+ "_w_"+to_string(max_w)+"_r_"+
            to_string(max_p)+"_chill_"+to_string(max_r)+".txt");

    cout<<"filename: "<<filename<<endl;

    Job * jobs_array = new Job [n+1];

    jobs_array[1].index = 1;
    jobs_array[1].r=rand() % max_r + 1;
    jobs_array[1].p=rand() % max_p + 1;
    jobs_array[1].d= jobs_array[1].r + jobs_array[1].p + (rand() % (max_chill_rpd+1));
    jobs_array[1].w = rand() % max_w + 1;

    for(int i=2;i<=n;i++)
    {
        jobs_array[i].index=i;
        jobs_array[i].r=jobs_array[i-1].r+rand() % max_r + 1;
        jobs_array[i].p=rand() % max_p + 1;
        jobs_array[i].d =
            jobs_array[i-1].r+jobs_array[i-1].p
            +jobs_array[i].r + jobs_array[i].p +
            + (rand() % (max_chill_rpd+1));
        jobs_array[i].w = rand() % max_w + 1;
    }

    for(int i=n;i>0;i--)
    {
        int index_to_swap = rand() % i + 1;
        Job temp_job = jobs_array[i];
        jobs_array[i] = jobs_array[index_to_swap];
        jobs_array[index_to_swap] = temp_job;
    }

    ofstream outfile;

    outfile.open(filename);

    outfile<<n<<endl;


    for(int i=1; i<=n; i++)
    {
        outfile<<jobs_array[i].p <<" ";
        outfile<<jobs_array[i].r <<" ";
        outfile<<jobs_array[i].d <<" ";
        outfile<<jobs_array[i].w <<endl;
    }

    outfile.close();

    delete [] jobs_array;
    //delete [] jobs_array_mixed;

}

void generate_instance (string filename, int n, int max_p, int max_w, int max_r, int max_chill_rpd)
{
    srand (time(NULL));

    /*filename+=("_n_"+to_string(n)+"_p_"+
            to_string(max_chill_rpd)+ "_w_"+to_string(max_w)+"_r_"+
            to_string(max_p)+"_chill_"+to_string(max_r)+".txt");*/

    cout<<"filename: "<<filename<<endl;

    Job * jobs_array = new Job [n+1];

    jobs_array[1].index = 1;
    jobs_array[1].r=rand() % max_r + 1;
    jobs_array[1].p=rand() % max_p + 1;
    jobs_array[1].d= jobs_array[1].r + jobs_array[1].p + (rand() % (max_chill_rpd+1));
    jobs_array[1].w = rand() % max_w + 1;

    for(int i=2;i<=n;i++)
    {
        jobs_array[i].index=i;
        jobs_array[i].r=(rand() % max_r + 1);
        jobs_array[i].p=rand() % max_p + 1;
        jobs_array[i].d =
            +jobs_array[i].r + jobs_array[i].p +
            + (rand() % (max_chill_rpd+1));
        jobs_array[i].w = rand() % max_w + 1;
    }

    for(int i=n;i>0;i--)
    {
        int index_to_swap = rand() % i + 1;
        Job temp_job = jobs_array[i];
        jobs_array[i] = jobs_array[index_to_swap];
        jobs_array[index_to_swap] = temp_job;
    }

    ofstream outfile;

    outfile.open(filename);

    outfile<<n<<endl;


    for(int i=1; i<=n; i++)
    {
        outfile<<jobs_array[i].p <<" ";
        outfile<<jobs_array[i].r <<" ";
        outfile<<jobs_array[i].d <<" ";
        outfile<<jobs_array[i].w <<endl;
    }

    outfile.close();

    delete [] jobs_array;
    //delete [] jobs_array_mixed;

}

void generate_output_file(int kryt, int n)
{
    ofstream outfile;
    string name = "132325_out";
    name = name + "_" + to_string(n) + ".txt";
    outfile.open(name);
    outfile<<kryt<<endl;
    for(int i = 1 ;i <=n ;i++)
    {
        outfile<<i<<" ";
    }
    outfile.close();
}

void zad1()
{
    int p=30;
    int w=20;
    int r=30;
    int chill=40;
    for(int i=1; i<=10; i++)
    {
        int n = i*50;
        string insta_name = "132325_" + to_string(n) + ".txt";
        generate_instance(insta_name,n,p,w,r,chill);
        //generate_output_file(0,n);
    }
}

void zad1_gen_outfile()
{
    for(int i=1; i<=10; i++)
    {
        generate_output_file(0,i*50);
    }
}

void zad2(string student_index)
{
    ifstream instance, outfile;

    int criterion_values_array [11];

    for(int i=1;i<=10;i++)
    {
        //cout<<i<<endl;
        string instance_name = student_index + "_" + to_string(i*50) + ".txt";
        string output_name = "132325_out_" + to_string(i*50) + ".txt";
        cout<<instance_name<<" "<<output_name<<endl;
        instance.open(instance_name);
        outfile.open(output_name);
        cout<<"open"<<endl;
        //if (!instance.is_open()) cout<<"blad! "<<instance_name<<endl;
        //if (!outfile.is_open()) cout<<"blad! "<<output_name<<endl;

        int n, criterion_outfile;
        instance>>n;
        outfile>>criterion_outfile;
        cout<<n<<" "<<criterion_outfile<<endl;
        jobs = new Job [n+1];
        order_of_jobs = new int [n+1];
        for(int j=1;j<=n;j++)
        {
            cout<<"j:"<<j<<endl;
            instance>>jobs[j].p>>jobs[j].r>>jobs[j].d>>jobs[j].w;
            //cout<<jobs[j].p<<" "<<jobs[j].r<<" "<<jobs[j].d<<" "<<jobs[j].w<<endl;
            outfile>>order_of_jobs[j];
        }

        int criterion_value = 0;
        int cmax = 0;
        for(int j=1; j<=n ;j++)
        {   
            Job * job = &jobs[order_of_jobs[j]];
            if (cmax < job->r) cmax = job->r;
            cmax += job->p;
            if (cmax > job->d) criterion_value += job->w;
        }
        criterion_values_array[i]=criterion_value;
        //cout<<"after 4"<<endl;
        delete [] jobs;
        delete [] order_of_jobs;
        instance.close();
        outfile.close();
    }

    cout<<"criterion values: "<<student_index<<endl;
    for(int i=1 ; i<=10; i++)
    {
        cout<<criterion_values_array[i]<<endl;
    }
    //jobs = new Job []
}



int main()
{
    //zad1();
    zad2("132325");
    return 0;
}