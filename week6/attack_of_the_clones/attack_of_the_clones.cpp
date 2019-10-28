#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

bool edf(pair<int, int> t1, pair<int, int> t2) {
    return t1.second < t2.second;
}

void testcase() {
    int n; cin >> n; // # tasks (jedi)
    int m; cin >> m; // # time units (intervals)
    
    vector<pair<int, int> > tasks(n);
    map<int, int> tasks_per_interval;
    for (int i = 0 ; i < n ; i++) {
        int a; cin >> a; // Starting time
        int b; cin >> b; // Ending time
        tasks[i] = make_pair(--a, --b); // a, b are given from 1 to m (included)
        
        tasks_per_interval[a]++;
        tasks_per_interval[b]--;
        if (a > b) {
            tasks_per_interval[0]++;
        }
    }
    
    // General assumption (from the description): there is an interval s where there's at most 10 tasks
    int s = -1;
    int nb_tasks = 0;
    int min_tasks = n;
    const int MAX_TASKS = 10;
    for (auto it = tasks_per_interval.begin() ; it != tasks_per_interval.end() ; it++) {
        nb_tasks += it->second;
        if (nb_tasks <= min_tasks && nb_tasks <= MAX_TASKS) {
            s = it->first;
            min_tasks = nb_tasks;
        }
    }
    
    // Now that we have this least protected segment, we can shift everything so that it starts there
    // that way, we have tasks that go over this segment, and tasks that don't
    vector<pair<int, int> > regular_tasks;
    vector<pair<int, int> > modular_tasks;
    
    for (int i = 0 ; i < n ; i++) {
        int start = tasks[i].first;
        int deadline = tasks[i].second;
        
        pair<int, int> shifted = make_pair(start - s, deadline - s);
        if (start <= s) {
            shifted.first += m;
        }
        if (deadline <= s) {
            shifted.second += m;
        }
        
        if ((start <= deadline && (start <= s && s <= deadline)) || (start > deadline && (start <= s || s <= deadline))) {
            // A task is modular if either:
            //  it was previously regular, and starts before s and ends after, or
            //  it was previously modular, and either started before s or ended after s
            modular_tasks.push_back(shifted);
        } else {
            regular_tasks.push_back(shifted);
        }
    }
    
    // Idea: schedule with EDF (Earliest Deadline First) - however, we know that there can only be one jedi covering the segment s
    // Som fir every jedi that can cover it, we get the best number of tasks we can schedule
    random_shuffle(regular_tasks.begin(), regular_tasks.end());
    sort(regular_tasks.begin(), regular_tasks.end(), edf);
    
    int scheduled = 0;
    // We first try to see how good we can do without any of those modular tasks
    // For that, we assume a modular task starting at -1 and without any deadline - so no overlap with anything
    int start = -1;
    for (auto it = regular_tasks.begin() ; it != regular_tasks.end() ; it++) {
        if (it->first > start) {
            // In other words: if *it does not overlap with the fictitious modular task
            start = it->second;
            scheduled++;
        }
    }
    
    // Now we try to schedule every one of those modular tasks
    for (auto it = modular_tasks.begin() ; it != modular_tasks.end() ; it++) {
        int start = it->first;
        int deadline = it->second % m;
        
        int schedulable = 1; // It starts at 1 because *it is already scheduled
        for (auto jt = regular_tasks.begin() ; jt != regular_tasks.end() ; jt++) {
            if (jt->first > deadline && jt->second < start) {
                // In other words: if *jt does not overlap with *it (the modular task)
                deadline = jt->second;
                schedulable++;
            }
        }
        
        scheduled = max(scheduled, schedulable);
    }
    
    cout << scheduled << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}
