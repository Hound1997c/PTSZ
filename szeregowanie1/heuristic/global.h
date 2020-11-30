#ifndef PTSZ_GLOBAL_H
#define PTSZ_GLOBAL_H

#include <cstdlib>


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
        case 'vw':
            return (abs(value - job1.w) < abs(value - job2.w));
        default:
            return (job1.d < job2.d);
            break;
        } 
    }
};


/*bool less_parameter(char arg)
{
    switch (arg)
        {
        case 'w':
            return (job1.w < job2.w);
            break;
        
        default:
            return (job1.d < job2.d);
            break;
        } 
}*/

#endif